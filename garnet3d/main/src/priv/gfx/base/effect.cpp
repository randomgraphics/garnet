#include "pch.h"
#include "garnet/gfx/effect.h"
#if !GN_ENABLE_INLINE
#include "garnet/gfx/effect.inl"
#endif

static GN::Logger * sLogger = GN::getLogger("GN.gfx.base.Effect");
GN::Logger * GN::gfx::Effect::sLogger = GN::getLogger("GN.gfx.base.Effect");

// *****************************************************************************
// local functions
// *****************************************************************************

enum FfpUniformType
{
    FFP_TRANSFORM_WORLD,
    FFP_TRANSFORM_VIEW,
    FFP_TRANSFORM_PROJ,
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
static bool sIsFfpUniformType( const GN::StrA & name, SInt32 * type )
{
    GN_GUARD;

    static const char * sTable[] =
    {
        "FFP_TRANSFORM_WORLD",
        "FFP_TRANSFORM_VIEW",
        "FFP_TRANSFORM_PROJ",
        "FFP_LIGHT0_POS",
        "FFP_LIGHT0_DIFFUSE",
        "FFP_MATERIAL_DIFFUSE",
        "FFP_MATERIAL_SPECULAR",
    };
    static const SInt32 n = (SInt32)GN_ARRAY_COUNT( sTable );

    for( SInt32 i = 0; i < n; ++i )
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

struct OpCodeDesc
{
    GN::gfx::EffectDesc::OpCode    op;
    GN::gfx::EffectDesc::TokenType dst;
    GN::gfx::EffectDesc::TokenType src0;
    GN::gfx::EffectDesc::TokenType src1;
    int                            numArgs;
};

static const OpCodeDesc sOpCodeTable[GN::gfx::EffectDesc::NUM_OPCODES] =
{
    { GN::gfx::EffectDesc::CMP_LT               , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::CMP_LE               , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::CMP_EQ               , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::CMP_NE               , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::CMP_GE               , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::CMP_GT               , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::ALU_ADD              , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::ALU_DEC              , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::ALU_NEG              , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::BIT_AND              , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::BIT_OR               , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::BIT_XOR              , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::BIT_NOT              , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::REL_AND              , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::REL_OR               , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::REL_NOT              , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::VALUEI, 2 },
    { GN::gfx::EffectDesc::CHECK_RENDERER_CAPS  , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUEI, GN::gfx::EffectDesc::OPCODE, 1 },
    { GN::gfx::EffectDesc::CHECK_SHADER_PROFILE , GN::gfx::EffectDesc::VALUEI , GN::gfx::EffectDesc::VALUES, GN::gfx::EffectDesc::OPCODE, 1 },
};

const GN::gfx::EffectDesc::CondExp GN::gfx::EffectDesc::CondExp::DUMMY;

//
//
// -----------------------------------------------------------------------------
GN::gfx::EffectDesc::OpCode GN::gfx::EffectDesc::sStr2OpCode( const StrA & s )
{
    static const char * sTable[] =
    {
        "CMP_LT",
        "CMP_LE",
        "CMP_EQ",
        "CMP_NE",
        "CMP_GE",
        "CMP_GT",
        "ALU_ADD",
        "ALU_DEC",
        "ALU_NEG",
        "BIT_AND",
        "BIT_OR",
        "BIT_XOR",
        "BIT_NOT",
        "REL_AND",
        "REL_OR",
        "REL_NOT",
        "CHECK_RENDERER_CAPS",
        "CHECK_SHADER_PROFILE",
    };

    for( size_t i = 0; i < GN_ARRAY_COUNT(sTable); ++i )
    {
        if( sTable[i] == s ) return (OpCode)i;
    }
    return OPCODE_INVALID;
}

//
//
// -----------------------------------------------------------------------------
static bool sCalc(
    GN::gfx::EffectDesc::Token & result,
    SInt32 op,
    const GN::gfx::EffectDesc::Token * s0,
    const GN::gfx::EffectDesc::Token * s1 )
{
    GN_ASSERT( 0 <= op && op <= GN::gfx::EffectDesc::NUM_OPCODES );

    // check argument type
    const OpCodeDesc & ocd = sOpCodeTable[op];
    if( ocd.numArgs > 0 )
    {
        if( !s0 )
        {
            GN_ERROR(sLogger)( "missing argument 0" );
            return false;
        }
        if( s0->type != ocd.src0 )
        {
            GN_ERROR(sLogger)( "incompatible argument type: expecting %d, but %d", ocd.src0, s0->type );
            return false;
        }
    }
    if( ocd.numArgs > 1 )
    {
        if( !s1 )
        {
            GN_ERROR(sLogger)( "missing argument 1" );
            return false;
        }
        if( s1->type != ocd.src1 )
        {
            GN_ERROR(sLogger)( "incompatible argument type: expecting %d, but %d", ocd.src1, s1->type );
            return false;
        }
    }

    result.type = GN::gfx::EffectDesc::VALUEI;

    switch( op )
    {
        case GN::gfx::EffectDesc::CMP_LT  : result.valueI = s0->valueI < s1->valueI; break;
        case GN::gfx::EffectDesc::CMP_LE  : result.valueI = s0->valueI <= s1->valueI; break;
        case GN::gfx::EffectDesc::CMP_EQ  : result.valueI = s0->valueI == s1->valueI; break;
        case GN::gfx::EffectDesc::CMP_NE  : result.valueI = s0->valueI != s1->valueI; break;
        case GN::gfx::EffectDesc::CMP_GE  : result.valueI = s0->valueI >= s1->valueI; break;
        case GN::gfx::EffectDesc::CMP_GT  : result.valueI = s0->valueI > s1->valueI; break;

        case GN::gfx::EffectDesc::ALU_ADD : result.valueI = s0->valueI + s1->valueI; break;
        case GN::gfx::EffectDesc::ALU_DEC : result.valueI = s0->valueI - s1->valueI; break;
        case GN::gfx::EffectDesc::ALU_NEG : result.valueI = -s0->valueI; break;

        case GN::gfx::EffectDesc::BIT_AND : result.valueI = s0->valueI & s1->valueI; break;
        case GN::gfx::EffectDesc::BIT_OR  : result.valueI = s0->valueI | s1->valueI; break;
        case GN::gfx::EffectDesc::BIT_XOR : result.valueI = s0->valueI ^ s1->valueI; break;
        case GN::gfx::EffectDesc::BIT_NOT : result.valueI = ~s0->valueI; break;

        case GN::gfx::EffectDesc::REL_AND : result.valueI = s0->valueI && s1->valueI; break;
        case GN::gfx::EffectDesc::REL_OR  : result.valueI = s0->valueI || s1->valueI; break;
        case GN::gfx::EffectDesc::REL_NOT : result.valueI = !s0->valueI; break;

        case GN::gfx::EffectDesc::CHECK_RENDERER_CAPS :
            result.valueI = gRenderer.getCaps( s0->valueI ); break;

        case GN::gfx::EffectDesc::CHECK_SHADER_PROFILE :
            result.valueI = gRenderer.supportShader( s0->valueS ); break;

        default : GN_UNEXPECTED(); return false; // program should not reach here.
    }

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool sDoEval(
    GN::gfx::EffectDesc::Token & result,
    const GN::gfx::EffectDesc::Token * & p,
    const GN::gfx::EffectDesc::Token * e )
{
    if( p >= e )
    {
        GN_ERROR(sLogger)( "incomplete expression" );
        return false;
    }

    if( GN::gfx::EffectDesc::OPCODE == p->type )
    {
        // store opcode
        SInt32 op = p->opcode;
        if( op < 0 || op >= GN::gfx::EffectDesc::NUM_OPCODES )
        {
            GN_ERROR(sLogger)( "invalid opcode : %d", op );
            return false;
        }
        ++p;

        const OpCodeDesc & ocd = sOpCodeTable[op];

        // get value of arguments
        GN::gfx::EffectDesc::Token s0, s1;
        if( ocd.numArgs > 0 && !sDoEval( s0, p, e ) ) return false;
        if( ocd.numArgs > 1 && !sDoEval( s1, p, e ) ) return false;

        // do operation
        if( !sCalc( result, op, &s0, &s1 ) ) return false;
    }
    else
    {
        result = *p;
        ++p;
    }

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::EffectDesc::CondExp::evaluate() const
{
    if( tokens.empty() ) return true;

    Token result;
    const Token * p = &tokens[0];
    if( !sDoEval( result, p, p + tokens.size() ) ) return false;

    switch( result.type )
    {
        case VALUEI : return 0 != result.valueI;
        case VALUES : return 0 != result.valueS[0];
        default : GN_UNEXPECTED(); return false; // program should not reach here.
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::EffectDesc::CondExp::compose(
    OpCode op, const CondExp & c1, const CondExp & c2 )
{
    static Token sEmptyToken = { VALUEI, { 1 } };

    const Token * t1, * t2;
    size_t n1, n2;

    if( c1.tokens.empty() )
    {
        t1 = &sEmptyToken;
        n1 = 1;
    }
    else
    {
        t1 = &c1.tokens[0];
        n1 = c1.tokens.size();
    }

    if( c2.tokens.empty() )
    {
        t2 = &sEmptyToken;
        n2 = 1;
    }
    else
    {
        t2 = &c2.tokens[0];
        n2 = c2.tokens.size();
    }

    GN_ASSERT( n1 > 0 && n2 > 0 );

    if( this == &c1 || this == &c2 )
    {
        GN_UNIMPL();
    }
    else
    {
        Token t = { OPCODE, { op } };
        tokens.clear();
        tokens.push_back( t );
        tokens.insert( tokens.end(), t1, t1+n1 );
        tokens.insert( tokens.end(), t2, t2+n2 );
    }
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
            GN_ERROR(sLogger)( "Shader(%s)的类型非法: %d。", shaderName.cptr(), shader.type );
            return false;
        }

        if( !shader.code.empty() )
        {
            if( shader.lang < 0 || shader.lang >= NUM_SHADING_LANGUAGES )
            {
                GN_ERROR(sLogger)( "Shader(%s)的语言类型非法: %d。", shaderName.cptr(), shader.lang );
                return false;
            }
        }

        // check texture referencing list
        for( std::map<UInt32,StrA>::const_iterator i = shader.textures.begin(); i != shader.textures.end(); ++i )
        {
            UInt32 stage = i->first;
            const StrA & name = i->second;

            if( !sExist( textures, name ) )
            {
                GN_ERROR(sLogger)( "Shader(%s)中含有无效的贴图引用：%s。", shaderName.cptr(), name.cptr() );
                return false;
            }

            if( stage >= MAX_TEXTURE_STAGES )
            {
                GN_ERROR(sLogger)( "Shader(%s)的贴图引用(%s)的stage超过允许上限(%d): %d.",
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
                GN_ERROR(sLogger)( "Shader(%s)中含有无效的变量引用：%s。", shaderName.cptr(), name.cptr() );
                return false;
            }

            // FFP shader can only bind uniform to FFP parameter"
            if( shader.code.empty() && !sIsFfpUniformType( binding, NULL ) )
            {
                GN_ERROR(sLogger)( "FFP shader(%s)对Uniform(%s)使用了非FFP的绑定: %s.",
                    shaderName.cptr(), name.cptr(), binding.cptr() );
                return false;
            }
        }
    }

    // at least one technique
    if( techniques.empty() )
    {
        GN_ERROR(sLogger)( "Technique列表不能为空!" );
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
                GN_ERROR(sLogger)( "Render state block of technique('%s')::pass(%d) is invalid.!", techName.cptr(), i );
                return false;
            }

            for( size_t i = 0; i < NUM_SHADER_TYPES; ++i )
            {
                const StrA & shaderName = pass.shaders[i];

                if( !sExist( shaders, shaderName ) )
                {
                    GN_ERROR(sLogger)( "Technique(%s)引用了无效的Shader: %s.", techName.cptr(), shaderName.cptr() );
                    return false;
                }

                const ShaderDesc & shader = getShader( shaderName );

                if( (ShaderType)i != shader.type )
                {
                    GN_ERROR(sLogger)( "Shader(%s)的类型(%s)与Technique(%s)所期望的类型(%s)不一致.",
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
            GN_ERROR(sLogger)( "Technique(%s)含有无效的render state block.!", techName.cptr() );
            return false;
        }
    }

    // check global render state block
    if( !rsb.valid() )
    {
        GN_ERROR(sLogger)( "Global render state block is invalid.!" );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::EffectDesc::toXml( File & )
{
}

// *****************************************************************************
// Effect class
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
        GN_ERROR(sLogger)( "effect descriptor is invalid!" );
        return failure();
    }

    mDesc = d;

    if( !createEffect() ) return failure();

    GN_ASSERT( mTextures.items.size() == mTextures.names.size() );
    GN_ASSERT( mTextures.items.size() == mDesc.textures.size() );

    GN_ASSERT( mUniforms.items.size() == mUniforms.names.size() );
    GN_ASSERT( mUniforms.items.size() == mDesc.uniforms.size() );

    GN_ASSERT( mShaders.items.size() == mShaders.names.size() );

    GN_ASSERT( mTechniques.items.size() == mTechniques.names.size() );

    // success
    return success();

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
        GN_ERROR(sLogger)( "Renderer is not ready." );
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
    for( UInt32 hUniform = mUniforms.items.first(); hUniform != 0; hUniform = mUniforms.items.next(hUniform) )
    {
        UniformData & ud = mUniforms.items[hUniform];
        for( UInt32 hShader = mShaders.items.first(); hShader != 0; hShader = mShaders.items.next(hShader) )
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
            GN_WARN(sLogger)( "Ignore redundant technique named '%s'. There's already a technique with same name in this effect.", desc.name.cptr() );
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
        GN_ERROR(sLogger)( "No valid technique found." );
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

    // check shader prerequisites
    if( !desc.prerequisites.evaluate() )
    {
        GN_TRACE(sLogger)( "Shader named '%s' does not pass prerequisites check. Ignored", name.cptr() );
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
            GN_ERROR(sLogger)( "Fail to create shader '%s'.", name.cptr() );
            return false;
        }
    }

    // build texture referencing list
    data.textures.reserve( desc.textures.size() );
    for( std::map<UInt32,StrA>::const_iterator i = desc.textures.begin(); i != desc.textures.end(); ++i )
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
            GN_ERROR(sLogger)( "Invalid uniform referencing to uniform named '%s', in shader '%s'.", uniName.cptr(), name.cptr() );
            return false;
        }

        urd.ffp = sIsFfpUniformType( uniBinding, &urd.ffpType );
        if( !urd.ffp )
        {
            // Make sure no programmable uniforms binding to FFP shader.
            if( !data.value )
            {
                GN_ERROR(sLogger)( "Can't use non-FFP uniform named '%s', in FFP shader named '%s'.", uniName.cptr(), name.cptr() );
                return false;
            }

            urd.shaderUniformHandle = data.value->getUniformHandle( uniBinding );

            if( 0 == urd.shaderUniformHandle )
            {
                GN_ERROR(sLogger)( "Uniform(%s)到Shader(%s)的绑定(%s)无效.",
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
                GN_TRACE(sLogger)( "Technique '%s' is ignored, because shader '%s' is not found.", desc.name.cptr(), shaderName.cptr() );
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
void GN::gfx::Effect::sSetFfpUniform( SInt32 type, const UniformData & data )
{
    GN_GUARD;

    Renderer & r = gRenderer;

    const UniformValue & value = data.value;

    switch( type )
    {
        case FFP_TRANSFORM_WORLD :
            if( UVT_MATRIX44 == type )
            {
                GN_ASSERT( !value.matrix44s.empty() );
                r.setWorld( value.matrix44s[0] );
            }
            else
            {
                GN_ERROR(sLogger)( "FFP_TRANSFORM_WORLD only accepts matrix type." );
            }
            break;

        case FFP_TRANSFORM_VIEW :
            if( UVT_MATRIX44 == type )
            {
                GN_ASSERT( !value.matrix44s.empty() );
                r.setView( value.matrix44s[0] );
            }
            else
            {
                GN_ERROR(sLogger)( "FFP_TRANSFORM_VIEW only accepts matrix type." );
            }
            break;

        case FFP_TRANSFORM_PROJ :
            if( UVT_MATRIX44 == type )
            {
                GN_ASSERT( !value.matrix44s.empty() );
                r.setProj( value.matrix44s[0] );
            }
            else
            {
                GN_ERROR(sLogger)( "FFP_TRANSFORM_PROJ only accepts matrix type." );
            }

        case FFP_LIGHT0_POS :
        case FFP_LIGHT0_DIFFUSE :
        case FFP_MATERIAL_DIFFUSE :
        case FFP_MATERIAL_SPECULAR :
            GN_UNIMPL_WARNING();
            break;

        default:
            GN_UNEXPECTED();
    }

    GN_UNGUARD;
}
