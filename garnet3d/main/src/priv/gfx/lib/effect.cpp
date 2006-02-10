#include "pch.h"
#include "garnet/gfx/effect.h"
#if !GN_ENABLE_INLINE
#include "garnet/gfx/effect.inl"
#endif

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
template<class T>
static inline bool sExist( const std::map<GN::StrA,T> & theMap, const GN::StrA & theName )
{
    return theMap.end() != theMap.find( theName );
}

//
//
// -----------------------------------------------------------------------------
template<typename T>
static inline bool sCheckFfpParameterType( const GN::StrA & name, T * type )
{
    GN_GUARD;

    static const char * sTable[] =
    {
        "FFP_TRANSFORM_WORLD",
        "FFP_TRANSFORM_VIEW",
        "FFP_TRANSFORM_PROJ",
        "FFP_VIEWPORT",
        "FFP_LIGHT0_POS",
        "FFP_LIGHT0_DIFFUSE",
        "FFP_MATERIAL_DIFFUSE",
        "FFP_MATERIAL_SPECULAR",
    };

    for( size_t i = 0; i < sizeof(sTable)/sizeof(sTable[0]); ++i )
    {
        if( sTable[i] == name )
        {
            if( type ) *type = (T)i;
            return true;
        }
    }
    return false;

    GN_UNGUARD;
}

// *****************************************************************************
// Effect descriptor methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::effect::EffectDesc::valid() const
{
    GN_GUARD;

    // check shaders
    for( std::map<StrA,ShaderDesc>::const_iterator i = shaders.begin(); i != shaders.end(); ++i )
    {
        const StrA & shaderName = i->first;
        const ShaderDesc & shader = i->second;

        if( shader.type < 0 || shader.type >= NUM_SHADER_TYPES )
        {
            GN_ERROR( "Shader(%s)的类型非法: %d。", shaderName.cstr(), shader.type );
            return false;
        }

        if( !shader.code.empty() )
        {
            if( shader.lang < 0 || shader.lang >= NUM_SHADING_LANGUAGES )
            {
                GN_ERROR( "Shader(%s)的语言类型非法: %d。", shaderName.cstr(), shader.lang );
                return false;
            }
        }

        // check texture referencing list
        for( std::map<uint32_t,StrA>::const_iterator i = shader.textures.begin(); i != shader.textures.end(); ++i )
        {
            uint32_t stage = i->first;
            const StrA & name = i->second;

            if( !sExist( textures, name ) )
            {
                GN_ERROR( "Shader(%s)中含有无效的贴图引用：%s。", shaderName.cstr(), name.cstr() );
                return false;
            }

            if( stage >= MAX_TEXTURE_STAGES )
            {
                GN_ERROR( "Shader(%s)的贴图引用(%s)的stage超过允许上限(%d): %d.",
                    shaderName.cstr(), name.cstr(), MAX_TEXTURE_STAGES, stage );
                return false;
            }
        }

        // check uniform referencing list
        for( std::map<StrA,StrA>::const_iterator i = shader.uniforms.begin(); i != shader.uniforms.end(); ++i )
        //for( size_t i = 0; i < shader.uniforms.size(); ++i )
        {
            const StrA & binding = i->first;
            const StrA & name = i->second;
            
            if( !sExist( uniforms, name ) )
            {
                GN_ERROR( "Shader(%s)中含有无效的变量引用：%s。", shaderName.cstr(), name.cstr() );
                return false;
            }

            // FFP shader can only bind uniform to FFP parameter"
            if( shader.code.empty() && !sCheckFfpParameterType( binding, (size_t*)NULL ) )
            {
                GN_ERROR( "FFP shader(%s)对Uniform(%s)使用了非FFP的绑定: %s.",
                    shaderName.cstr(), name.cstr(), binding.cstr() );
                return false;
            }
        }
    }

    // at least one technique
    if( techniques.empty() )
    {
        GN_ERROR( "Technique列表不能为空!" );
        return false;
    }

    // check techniques
    for( std::map<StrA,TechniqueDesc>::const_iterator i = techniques.begin(); i != techniques.end(); ++i )
    {
        const StrA & techName = i->first;
        const TechniqueDesc & tech = i->second;

        for( size_t i = 0; i < tech.passes.size(); ++i )
        {
            const PassDesc & pass = tech.passes[i];

            if( !pass.rsb.valid() )
            {
                GN_ERROR( "Render state block of technique('%s')::pass(%d) is invalid.!", techName.cstr(), i );
                return false;
            }

            for( size_t i = 0; i < NUM_SHADER_TYPES; ++i )
            {
                const StrA & shaderName = pass.shaders[i];

                if( !sExist( shaders, shaderName ) )
                {
                    GN_ERROR( "Technique(%s)引用了无效的Shader: %s.", techName.cstr(), shaderName.cstr() );
                    return false;
                }

                const ShaderDesc & shader = getShader( shaderName );

                if( (ShaderType)i != shader.type )
                {
                    GN_ERROR( "Shader(%s)的类型(%s)与Technique(%s)所期望的类型(%s)不一致.",
                        shaderName.cstr(),
                        shaderType2Str( shader.type ),
                        techName.cstr(),
                        shaderType2Str( (ShaderType)i ) );
                    return false;
                }
            }
        }

        if( !tech.rsb.valid() )
        {
            GN_ERROR( "Technique(%s)含有无效的render state block.!", techName.cstr() );
            return false;
        }
    }

    // check global render state block
    if( !rsb.valid() )
    {
        GN_ERROR( "Global render state block is invalid.!" );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::effect::Effect::init( Renderer & r, const EffectDesc & d )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( Effect, () );

    if( !d.valid() )
    {
        GN_ERROR( "effect descriptor is invalid!" );
        quit(); return selfOK();
    }

    mRenderer = &r;
    mDesc = d;

    if( !createEffect() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::effect::Effect::init( const Effect & e )
{
    GN_GUARD;

    GN_ASSERT( e.mRenderer && e.mDesc.valid() );
    return init( *e.mRenderer, e.mDesc );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::effect::Effect::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::effect::Effect::draw( const GeometryData * geometryDataArray, size_t count ) const
{
    GN_GUARD_SLOW;

    // make sure effect is initialized.
    GN_ASSERT( ok() );

    if( 0 == geometryDataArray || count > 0 )
    {
        GN_ERROR( "invalid geometry data array (null data pointer)!" );
        return;
    }

    size_t numPasses = drawBegin();
    for( size_t i = 0; i < numPasses; ++i )
    {
        passBegin( i );

        // render the geometries
        for( size_t i = 0; i < count; ++i )
        {
            const GeometryData & g = geometryDataArray[i];

            for( std::map<StrA,UniformValue>::const_iterator i = g.uniforms.begin(); i != g.uniforms.end(); ++i )
            {
                setUniformByName( i->first, i->second );
            }
            for( std::map<StrA,uint32_t>::const_iterator i = g.textures.begin(); i != g.textures.end(); ++i )
            {
                setTextureByName( i->first, i->second );
            }

            commitChanges();

            mRenderer->bindVtxBinding( g.vtxBinding );
            mRenderer->bindVtxBufs( g.vtxBufs[0].addr(), 0, g.numVtxBufs );
            mRenderer->bindIdxBuf( g.idxBuf.get() );

            if( g.idxBuf.empty() )
            {
                mRenderer->draw( g.prim, g.numPrim, g.startVtx );
            }
            else
            {
                mRenderer->drawIndexed( g.prim, g.numPrim, g.startVtx, g.minVtxIdx, g.numVtx, g.startIdx );
            }
        }

        passEnd();
    }
    drawEnd();

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::effect::Effect::createEffect()
{
    GN_GUARD;

    GN_ASSERT( mRenderer && mDesc.valid() && mUniforms.empty() && mTextures.empty() );

    // create texture list
    for( std::map<StrA,TextureDesc>::const_iterator i = mDesc.textures.begin();
         i != mDesc.textures.end(); ++i )
    {
        TextureData td;
        td.name = i->first;
        mTextures.add( i->first, td );

        // set default texture value
        if( !i->second.defaultValue.empty() )
        {
            uint32_t id = gTexDict.getResourceHandle( i->second.defaultValue );
            if( 0 == id )
            {
                GN_WARN( "Default texture value '%s' of texture '%s' is not a valid texture resource.",
                    i->second.defaultValue.cstr(), td.name.cstr() );
            }
            else
            {
                setTextureByName( td.name, id );
            }
        }
    }

    // create uniform list
    for( std::map<StrA,UniformDesc>::const_iterator i = mDesc.uniforms.begin();
         i != mDesc.uniforms.end(); ++i )
    {
        UniformData ud;
        ud.name = i->first;
        mUniforms.add( i->first, ud );
    }

    // create shader list
    for( std::map<StrA,ShaderDesc>::const_iterator i = mDesc.shaders.begin();
         i != mDesc.shaders.end(); ++i )
    {
        const ShaderDesc & s = i->second;

        ShaderData sd;

        sd.name = i->first;

        // build texture referencing list
        sd.textures.reserve( s.textures.size() );
        for( std::map<uint32_t,StrA>::const_iterator i = s.textures.begin(); i != s.textures.end(); ++i )
        {
            TextureRefData td;
            td.stage = i->first;
            td.handle = mTextures.find( i->second );
            GN_ASSERT( td.handle ); // check is already done in EffectDesc::valid()
            sd.textures.push_back( td );
        }

        // build uniform referencing list
        sd.uniforms.reserve( s.uniforms.size() );
        for( std::map<StrA,StrA>::const_iterator i = s.uniforms.begin(); i != s.uniforms.end(); ++i )
        {
            UniformRefData ud;
            ud.ffp = sCheckFfpParameterType( i->first, &ud.ffpParameterType );
            ud.handle = mUniforms.find( i->second );
            GN_ASSERT( ud.handle ); // check is already done in EffectDesc::valid()
            sd.uniforms.push_back( ud );
        }

        mShaders.add( sd.name, sd );
    }

    // build shader referencing list for all uniforms
    for( uint32_t hUniform = mUniforms.items.first(); hUniform != 0; hUniform = mUniforms.items.next(hUniform) )
    {
        UniformData & ud = mUniforms.items[hUniform];

        for( uint32_t hShader = mShaders.items.first(); hShader != 0; hShader = mShaders.items.next(hShader) )
        {
            const ShaderData & sd = mShaders.items[hShader];
            for( size_t i = 0; i < sd.uniforms.size(); ++i )
            {
                const UniformRefData & ur = sd.uniforms[i];
                GN_ASSERT( mUniforms.items.validHandle(ur.handle) );
                if( mUniforms.items[ur.handle].name == ud.name )
                {
                    ShaderRefData urd = { hShader, i };
                    ud.shaders.push_back( urd );
                }
            }
        }

        // set default uniform value.
        const UniformDesc & u = mDesc.getUniform(ud.name);
        if( u.hasDefaultValue ) setUniform( hUniform, u.defaultValue );
    }

    // create technique list
    GN_ASSERT( !mDesc.techniques.empty() );
    for( std::map<StrA,TechniqueDesc>::const_iterator iTech = mDesc.techniques.begin();
         iTech != mDesc.techniques.end(); ++iTech )
    {
        const TechniqueDesc & t = iTech->second;
        TechniqueData td;
        td.name = iTech->first;
        td.ready = false;

        // create pass list
        td.passes.resize( t.passes.size() );
        for( size_t iPass = 0; iPass < t.passes.size(); ++iPass )
        {
            const PassDesc & p = t.passes[iPass];
            PassData & pd = td.passes[iPass];

            pd.rsb = mRenderer->createRenderStateBlock( mDesc.rsb + t.rsb + p.rsb );
            if( 0 == pd.rsb )
            {
                GN_ERROR( "Fail to create render state block for pass(%d) of technique named '%s'.",
                    iPass, iTech->first.cstr() );
                return false;
            }
            for( size_t iShader = 0; iShader < NUM_SHADER_TYPES; ++iShader )
            {
                const StrA & s = p.shaders[iShader];
                pd.shaders[iShader] = mShaders.find( s );
                GN_ASSERT( pd.shaders[iShader] &&
                           mDesc.getShader(s).type == (ShaderType)iShader ); // check is already done in EffectDesc::valid()
            }
        }

        // add technique data to technique list
        mTechniques.add( iTech->first, td );
    }
    mActiveTechnique = mTechniques.items.first();

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::effect::Effect::initTechnique( uint32_t handle ) const
{
    GN_GUARD;

    GN_ASSERT( mTechniques.items.validHandle( handle ) );

    TechniqueData & td = mTechniques.items[handle];

    for( size_t i = 0; i < td.passes.size(); ++i )
    {
        PassData & pd = td.passes[i];

        for( size_t i = 0; i < NUM_SHADER_TYPES; ++i )
        {
            GN_ASSERT( mShaders.items.validHandle( pd.shaders[i] ) );
            ShaderData & sd = mShaders.items[pd.shaders[i]];
            const ShaderDesc & s = mDesc.getShader(sd.name);

            // create shaders instance
            if( !sd.value.empty() && !s.code.empty() )
            {
                sd.value.attach( mRenderer->createShader( s.type, s.lang, s.code, s.entry ) );
                if( sd.value.empty() )
                {
                    GN_ERROR( "Fail to create shader '%s' for technique '%s'.", sd.name.cstr(), td.name.cstr() );
                    return false;
                }
            }

            // realize uniform handles.
            for( size_t i = 0; i < sd.uniforms.size(); ++i )
            {
                UniformRefData & ur = sd.uniforms[i];
                if( !ur.ffp )
                {
                    // Make non-FFP binding belongs to non-FFP shader (already checked in EffectDesc::valid() ).
                    GN_ASSERT( sd.value );

                    std::map<StrA,StrA>::const_iterator u = s.uniforms.find( mUniforms.items[ur.handle].name );

                    GN_ASSERT( u != s.uniforms.end() &&
                               u->second == mUniforms.items[ur.handle].name );

                    const StrA & binding = u->first;
                    const StrA & name = u->second;

                    ur.shaderUniformHandle = sd.value->getUniformHandle( binding );

                    if( 0 == ur.shaderUniformHandle )
                    {
                        GN_ERROR( "Uniform(%s)到Shader(%s)的绑定(%s)无效.",
                            name.cstr(), sd.name.cstr(), binding.cstr() );
                        return false;
                    }
                }
            }
        }
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::effect::Effect::sSetFfpParameter(
    Renderer &, FfpParameterType, const UniformData & )
{
    GN_GUARD;

    GN_UNIMPL_WARNING();

    GN_UNGUARD;
}
