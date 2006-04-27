#include "pch.h"
#include "garnet/gfx/effect.h"
#if !GN_ENABLE_INLINE
#include "garnet/gfx/effect.inl"
#endif

// *****************************************************************************
// local functions
// *****************************************************************************

enum FfpUniformType
{
    FFP_TRANSFORM_WORLD,
    FFP_TRANSFORM_VIEW,
    FFP_TRANSFORM_PROJ,
    FFP_VIEWPORT,
    FFP_LIGHT0_POS,
    FFP_LIGHT0_DIFFUSE,
    FFP_MATERIAL_DIFFUSE,
    FFP_MATERIAL_SPECULAR,
    NUM_FFP_UNIFORM_TYPES,
};

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
static bool sIsFfpUniformType( const GN::StrA & name, int32_t * type )
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
    static const int32_t n = (int32_t)( sizeof(sTable)/sizeof(sTable[0]) );

    for( int32_t i = 0; i < n; ++i )
    {
        if( sTable[i] == name )
        {
            if( type ) *type = i;
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
bool GN::gfx::EffectDesc::CondExp::sDoEval( uint32_t & value, const Token * & p, const Token * e )
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
bool GN::gfx::EffectDesc::CondExp::evaluate( uint32_t & value ) const
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
void GN::gfx::EffectDesc::CondExp::fromStr( const char *, size_t )
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
bool GN::gfx::EffectDesc::valid() const
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
            if( shader.code.empty() && !sIsFfpUniformType( binding, NULL ) )
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
    for( size_t i = 0; i < techniques.size(); ++i )
    {
        const TechniqueDesc & tech = techniques[i];
        const StrA & techName = tech.name;

        // TODO: check for non-unqiuetechnique name

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
bool GN::gfx::Effect::init( const EffectDesc & d )
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

    GN_ASSERT( mTechniques.items.size() == mTechniques.names.size() );

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Effect::init( const Effect & e )
{
    GN_GUARD;

    GN_ASSERT( e.mDesc.valid() );
    return init( e.mDesc );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::Effect::quit()
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
bool GN::gfx::Effect::createEffect()
{
    GN_GUARD;

    if( !gRendererPtr )
    {
        GN_ERROR( "Renderer is not ready." );
        return false;
    }

    GN_ASSERT( mDesc.valid() && mUniforms.empty() && mTextures.empty() );

    // create texture list
    for( std::map<StrA,EffectDesc::TextureDesc>::const_iterator i = mDesc.textures.begin();
         i != mDesc.textures.end(); ++i )
    {
        TextureData td;
        td.name = i->first;
        td.value = 0;
        mTextures.add( i->first, td );
    }

    // create uniform list
    for( std::map<StrA,EffectDesc::UniformDesc>::const_iterator i = mDesc.uniforms.begin();
         i != mDesc.uniforms.end(); ++i )
    {
        UniformData ud;
        ud.name = i->first;
        mUniforms.add( i->first, ud );
    }

    // create shader list
    for( std::map<StrA,EffectDesc::ShaderDesc>::const_iterator i = mDesc.shaders.begin();
         i != mDesc.shaders.end(); ++i )
    {
        ShaderData data;
        if( createShader( data, i->first, i->second ) )
        {
            GN_ASSERT( data.name == i->first );
            mShaders.add( data.name, data );
        }
    }

    // build shader-referencing list for uniforms:
    //  for each uniforms:
    //    for each shaders:
    //      if the shader is referencing the uniform:
    //        add this shader to the uniform's shader-referencing list
    for( uint32_t hUniform = mUniforms.items.first(); hUniform != 0; hUniform = mUniforms.items.next(hUniform) )
    {
        UniformData & ud = mUniforms.items[hUniform];
        for( uint32_t hShader = mShaders.items.first(); hShader != 0; hShader = mShaders.items.next(hShader) )
        {
            const ShaderData & sd = mShaders.items[hShader];
            for( size_t i = 0; i < sd.uniforms.size(); ++i )
            {
                const UniformRefData & urd = sd.uniforms[i];
                GN_ASSERT( mUniforms.items.validHandle(urd.id) );
                if( mUniforms.items[urd.id].name == ud.name )
                {
                    ShaderRefData srd = { hShader, i };
                    ud.shaders.push_back( srd );
                }
            }
        }

        // Set default uniform value.
        // Note that setUniform() will update shader's uniform-dirty-list as well.
        const EffectDesc::UniformDesc & u = mDesc.getUniform(ud.name);
        if( u.hasDefaultValue ) setUniform( hUniform, u.defaultValue );
    }

    // create technique list
    for( size_t i = 0; i < mDesc.techniques.size(); ++i )
    {
        const EffectDesc::TechniqueDesc & desc = mDesc.techniques[i];

        if( mTechniques.find( desc.name ) )
        {
            GN_WARN( "Ignore redundant technique named '%s'. There's already a technique with same name in this effect.", desc.name.cptr() );
            continue;
        }
        
        TechniqueData data;
        if( createTechnique( data, desc ) )
        {
            GN_ASSERT( data.name == desc.name );
            mTechniques.add( data.name, data );
        }
    }
    if( mTechniques.empty() )
    {
        GN_ERROR( "No valid technique found." );
        return false;
    }

    // setup active technique
    mActiveTechnique = mTechniques.items.first();

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Effect::createShader( ShaderData & data, const StrA & name, const EffectDesc::ShaderDesc & desc )
{
    GN_GUARD;

    // check shader condition
    if( !desc.conditions.evaluate() )
    {
        GN_TRACE( "Shader named '%s' does not pass condition check. Ignored", name.cptr() );
        return false;
    }

    // store shader name
    data.name = name;

    // create shaders instance
    if( data.value.empty() && !desc.code.empty() )
    {
        data.value.attach( gRenderer.createShader( desc.type, desc.lang, desc.code, desc.hints ) );
        if( data.value.empty() )
        {
            GN_ERROR( "Fail to create shader '%s'.", name.cptr() );
            return false;
        }
    }

    // build texture referencing list
    data.textures.reserve( desc.textures.size() );
    for( std::map<uint32_t,StrA>::const_iterator i = desc.textures.begin(); i != desc.textures.end(); ++i )
    {
        TextureRefData trd;
        trd.stage = i->first;
        trd.id = mTextures.find( i->second );
        GN_ASSERT( trd.id ); // must be true for valid shader descriptor
        data.textures.push_back( trd );
    }

    // build uniform referencing list
    data.uniforms.reserve( desc.uniforms.size() );
    for( std::map<StrA,StrA>::const_iterator i = desc.uniforms.begin(); i != desc.uniforms.end(); ++i )
    {
        UniformRefData urd;

        const StrA & uniBinding = i->first;
        const StrA & uniName = i->second;

        urd.id = mUniforms.find( uniName );
        if( 0 == urd.id )
        {
            GN_ERROR( "Invalid uniform referencing to uniform named '%s', in shader '%s'.", uniName.cptr(), name.cptr() );
            return false;
        }

        urd.ffp = sIsFfpUniformType( uniBinding, &urd.ffpType );
        if( !urd.ffp )
        {
            // Make sure no programmable uniforms binding to FFP shader.
            if( !data.value )
            {
                GN_ERROR( "Can't use non-FFP uniform named '%s', in FFP shader named '%s'.", uniName.cptr(), name.cptr() );
                return false;
            }

            urd.shaderUniformHandle = data.value->getUniformHandle( uniBinding );

            if( 0 == urd.shaderUniformHandle )
            {
                GN_ERROR( "Uniform(%s)到Shader(%s)的绑定(%s)无效.",
                    uniName.cptr(),
                    name.cptr(),
                    uniBinding.cptr() );
                return false;
            }
        }

        data.uniforms.push_back( urd );
    }

    // pre-allocate memory for dirty uniform array
    data.dirtyUniforms.reserve( data.uniforms.size() );

    // success
    GN_ASSERT( data.dirtyUniforms.empty() );
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Effect::createTechnique( TechniqueData & data, const EffectDesc::TechniqueDesc & desc )
{
    GN_GUARD;

    data.name = desc.name;

    data.passes.resize( desc.passes.size() );

    for( size_t iPass = 0; iPass < desc.passes.size(); ++iPass )
    {
        const EffectDesc::PassDesc & passDesc = desc.passes[iPass];

        PassData & passData = data.passes[iPass];

        for( size_t iShader = 0; iShader < NUM_SHADER_TYPES; ++iShader )
        {
            const StrA & shaderName = passDesc.shaders[iShader];

            passData.shaders[iShader] = mShaders.find( shaderName );

            if( 0 == passData.shaders[iShader] )
            {
                GN_TRACE( "Technique '%s' is ignored, because shader '%s' is not found.", desc.name.cptr(), shaderName.cptr() );
                return false;
            }
 
            GN_ASSERT( mDesc.getShader(shaderName).type == (ShaderType)iShader );
        }
        passData.rsb = mDesc.rsb + desc.rsb + passDesc.rsb;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::Effect::sSetFfpUniform( int32_t, const UniformData & )
{
    GN_GUARD;

    GN_UNIMPL_WARNING();

    GN_UNGUARD;
}
