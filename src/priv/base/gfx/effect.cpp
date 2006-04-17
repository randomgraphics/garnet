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
// CondExp methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::effect::CondExp::sDoEval( uint32_t & value, const Token * & p, const Token * e )
{
    if( p >= e )
    {
        GN_ERROR( "incomplete expression!" );
        return false;
    }

    if( OPCODE == p->type )
    {
        int32_t op = p->opcode;

        if( op < 0 || op >= NUM_OPCODES )
        {
            GN_ERROR( "invalid opcode : %d", op );
            return false;
        }

        ++p;

        uint32_t a0, a1;
        if( !sDoEval( a0, p, e ) ) return false;
        if( !sDoEval( a1, p, e ) ) return false;

        value = sCalc( op, a0, a1 );
    }
    else if( GFXCAPS == p->type )
    {
        if( p->gfxcaps < 0 || p->gfxcaps >= NUM_RENDERER_CAPS )
        {
            GN_ERROR( "invalid renderer caps!" );
            return false;
        }
        value = gRenderer.getCaps( p->gfxcaps );
        ++p;
    }
    else if( VALUE == p->type )
    {
        value = p->value;
        ++p;
    }
    else
    {
        GN_ERROR( "invalid item type: %d", p->type );
        return false;
    }

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::effect::CondExp::evaluate( uint32_t & value ) const
{
    if( mTokens.empty() )
    {
        value = 1;
        return true;
    }

    const Token * p = &mTokens[0];
    return sDoEval( value, p, p + mTokens.size() );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::effect::CondExp::fromStr( const char *, size_t )
{
    mTokens.clear();
    GN_UNIMPL_WARNING();
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
            GN_ERROR( "Shader(%s)的类型非法: %d。", shaderName.cptr(), shader.type );
            return false;
        }

        if( !shader.code.empty() )
        {
            if( shader.lang < 0 || shader.lang >= NUM_SHADING_LANGUAGES )
            {
                GN_ERROR( "Shader(%s)的语言类型非法: %d。", shaderName.cptr(), shader.lang );
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
                GN_ERROR( "Shader(%s)中含有无效的贴图引用：%s。", shaderName.cptr(), name.cptr() );
                return false;
            }

            if( stage >= MAX_TEXTURE_STAGES )
            {
                GN_ERROR( "Shader(%s)的贴图引用(%s)的stage超过允许上限(%d): %d.",
                    shaderName.cptr(), name.cptr(), MAX_TEXTURE_STAGES, stage );
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
                GN_ERROR( "Shader(%s)中含有无效的变量引用：%s。", shaderName.cptr(), name.cptr() );
                return false;
            }

            // FFP shader can only bind uniform to FFP parameter"
            if( shader.code.empty() && !sCheckFfpParameterType( binding, (size_t*)NULL ) )
            {
                GN_ERROR( "FFP shader(%s)对Uniform(%s)使用了非FFP的绑定: %s.",
                    shaderName.cptr(), name.cptr(), binding.cptr() );
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
                GN_ERROR( "Render state block of technique('%s')::pass(%d) is invalid.!", techName.cptr(), i );
                return false;
            }

            for( size_t i = 0; i < NUM_SHADER_TYPES; ++i )
            {
                const StrA & shaderName = pass.shaders[i];

                if( !sExist( shaders, shaderName ) )
                {
                    GN_ERROR( "Technique(%s)引用了无效的Shader: %s.", techName.cptr(), shaderName.cptr() );
                    return false;
                }

                const ShaderDesc & shader = getShader( shaderName );

                if( (ShaderType)i != shader.type )
                {
                    GN_ERROR( "Shader(%s)的类型(%s)与Technique(%s)所期望的类型(%s)不一致.",
                        shaderName.cptr(),
                        shaderType2Str( shader.type ),
                        techName.cptr(),
                        shaderType2Str( (ShaderType)i ) );
                    return false;
                }
            }
        }

        if( !tech.rsb.valid() )
        {
            GN_ERROR( "Technique(%s)含有无效的render state block.!", techName.cptr() );
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
bool GN::gfx::effect::Effect::init( const EffectDesc & d )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( Effect, () );

    if( !d.valid() )
    {
        GN_ERROR( "effect descriptor is invalid!" );
        quit(); return selfOK();
    }

    mDesc = d;

    if( !createEffect() ) { quit(); return selfOK(); }

    GN_ASSERT( mTextures.items.size() == mTextures.names.size() );
    GN_ASSERT( mTextures.items.size() == mDesc.textures.size() );

    GN_ASSERT( mUniforms.items.size() == mUniforms.names.size() );
    GN_ASSERT( mUniforms.items.size() == mDesc.uniforms.size() );

    GN_ASSERT( mShaders.items.size() == mShaders.names.size() );
    GN_ASSERT( mShaders.items.size() == mDesc.shaders.size() );

    GN_ASSERT( mTechniques.items.size() == mTechniques.names.size() );
    GN_ASSERT( mTechniques.items.size() == mDesc.techniques.size() );

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

    GN_ASSERT( e.mDesc.valid() );
    return init( e.mDesc );

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

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::effect::Effect::createEffect()
{
    GN_GUARD;

    if( !gRendererPtr )
    {
        GN_ERROR( "Renderer is not ready." );
        return false;
    }

    GN_ASSERT( mDesc.valid() && mUniforms.empty() && mTextures.empty() );

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
                    i->second.defaultValue.cptr(), td.name.cptr() );
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

        // check shader condition
        if( !s.conditions.evaluate() ) continue;

        // create shaders instance
        if( sd.value.empty() && !s.code.empty() )
        {
            sd.value.attach( gRenderer.createShader( s.type, s.lang, s.code, s.hints ) );
            if( sd.value.empty() )
            {
                GN_ERROR( "Fail to create shader '%s'.", sd.name.cptr() );
                continue;
            }
        }

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
        bool urlok = true;
        sd.uniforms.reserve( s.uniforms.size() );
        for( std::map<StrA,StrA>::const_iterator i = s.uniforms.begin(); i != s.uniforms.end(); ++i )
        {
            UniformRefData urd;

            urd.binding = i->first;
            urd.ffp = sCheckFfpParameterType( i->first, &urd.ffpParameterType );
            urd.handle = mUniforms.find( i->second );
            GN_ASSERT( urd.handle ); // check is already done in EffectDesc::valid()
            sd.uniforms.push_back( urd );

            // get shader-specific handle of the uniform
            if( !urd.ffp )
            {
                // Make sure non-FFP binding belongs to non-FFP shader
                GN_ASSERT( sd.value );

                urd.shaderUniformHandle = sd.value->getUniformHandle( urd.binding );

                if( 0 == urd.shaderUniformHandle )
                {
                    const StrA & name = mUniforms.items[urd.handle].name;
                    GN_ERROR( "Uniform(%s)到Shader(%s)的绑定(%s)无效.",
                        name.cptr(),
                        sd.name.cptr(),
                        urd.binding.cptr() );
                    urlok = false;
                    continue;
                }
            }
        }
        if( !urlok ) continue;

        mShaders.add( sd.name, sd );
    }

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

        td.passes.resize( t.passes.size() );

        bool techok = true;
        for( size_t iPass = 0; iPass < t.passes.size(); ++iPass )
        {
            const PassDesc & p = t.passes[iPass];
            PassData & pd = td.passes[iPass];

            for( size_t iShader = 0; iShader < NUM_SHADER_TYPES; ++iShader )
            {
                const StrA & s = p.shaders[iShader];
                pd.shaders[iShader] = mShaders.find( s );
                techok = 0 != pd.shaders[iShader];
                GN_ASSERT( mDesc.getShader(s).type == (ShaderType)iShader );
            }
            pd.rsb = mDesc.rsb + t.rsb + p.rsb;
        }
        if( !techok ) continue;

        // add technique data to technique list
        mTechniques.add( iTech->first, td );
    }
    if( mTechniques.empty() )
    {
        GN_ERROR( "No valid technique found." );
        return false;
    }
    mActiveTechnique = mTechniques.items.first();

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::effect::Effect::sSetFfpParameter( FfpParameterType, const UniformData & )
{
    GN_GUARD;

    GN_UNIMPL_WARNING();

    GN_UNGUARD;
}
