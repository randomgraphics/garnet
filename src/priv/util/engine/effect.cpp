#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.Effect");
GN::Logger * GN::engine::Effect::sLogger = GN::getLogger("GN.engine.Effect");

using namespace GN::engine;

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
    GN::engine::EffectDesc::OpCode    op;
    GN::engine::EffectDesc::TokenType dst;
    GN::engine::EffectDesc::TokenType src0;
    GN::engine::EffectDesc::TokenType src1;
    int                            numArgs;
};

static const OpCodeDesc sOpCodeTable[GN::engine::EffectDesc::NUM_OPCODES] =
{
    { GN::engine::EffectDesc::CMP_LT               , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::CMP_LE               , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::CMP_EQ               , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::CMP_NE               , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::CMP_GE               , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::CMP_GT               , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::ALU_ADD              , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::ALU_DEC              , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::ALU_NEG              , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::BIT_AND              , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::BIT_OR               , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::BIT_XOR              , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::BIT_NOT              , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::REL_AND              , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::REL_OR               , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::REL_NOT              , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::VALUEI, 2 },
    { GN::engine::EffectDesc::CHECK_RENDERER_CAPS  , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUEI, GN::engine::EffectDesc::OPCODE, 1 },
    { GN::engine::EffectDesc::CHECK_SHADER_PROFILE , GN::engine::EffectDesc::VALUEI , GN::engine::EffectDesc::VALUES, GN::engine::EffectDesc::OPCODE, 1 },
};

const GN::engine::EffectDesc::CondExp GN::engine::EffectDesc::CondExp::DUMMY;

//
//
// -----------------------------------------------------------------------------
GN::engine::EffectDesc::OpCode GN::engine::EffectDesc::sStr2OpCode( const StrA & s )
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
    GN::engine::EffectDesc::Token & result,
    SInt32 op,
    const GN::engine::EffectDesc::Token * s0,
    const GN::engine::EffectDesc::Token * s1 )
{
    GN_ASSERT( 0 <= op && op <= GN::engine::EffectDesc::NUM_OPCODES );

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

    result.type = GN::engine::EffectDesc::VALUEI;

    switch( op )
    {
        case GN::engine::EffectDesc::CMP_LT  : result.valueI = s0->valueI < s1->valueI; break;
        case GN::engine::EffectDesc::CMP_LE  : result.valueI = s0->valueI <= s1->valueI; break;
        case GN::engine::EffectDesc::CMP_EQ  : result.valueI = s0->valueI == s1->valueI; break;
        case GN::engine::EffectDesc::CMP_NE  : result.valueI = s0->valueI != s1->valueI; break;
        case GN::engine::EffectDesc::CMP_GE  : result.valueI = s0->valueI >= s1->valueI; break;
        case GN::engine::EffectDesc::CMP_GT  : result.valueI = s0->valueI > s1->valueI; break;

        case GN::engine::EffectDesc::ALU_ADD : result.valueI = s0->valueI + s1->valueI; break;
        case GN::engine::EffectDesc::ALU_DEC : result.valueI = s0->valueI - s1->valueI; break;
        case GN::engine::EffectDesc::ALU_NEG : result.valueI = -s0->valueI; break;

        case GN::engine::EffectDesc::BIT_AND : result.valueI = s0->valueI & s1->valueI; break;
        case GN::engine::EffectDesc::BIT_OR  : result.valueI = s0->valueI | s1->valueI; break;
        case GN::engine::EffectDesc::BIT_XOR : result.valueI = s0->valueI ^ s1->valueI; break;
        case GN::engine::EffectDesc::BIT_NOT : result.valueI = ~s0->valueI; break;

        case GN::engine::EffectDesc::REL_AND : result.valueI = s0->valueI && s1->valueI; break;
        case GN::engine::EffectDesc::REL_OR  : result.valueI = s0->valueI || s1->valueI; break;
        case GN::engine::EffectDesc::REL_NOT : result.valueI = !s0->valueI; break;

        case GN::engine::EffectDesc::CHECK_RENDERER_CAPS :
            result.valueI = gRenderer.getCaps( s0->valueI ); break;

        case GN::engine::EffectDesc::CHECK_SHADER_PROFILE :
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
    GN::engine::EffectDesc::Token & result,
    const GN::engine::EffectDesc::Token * & p,
    const GN::engine::EffectDesc::Token * e )
{
    if( p >= e )
    {
        GN_ERROR(sLogger)( "incomplete expression" );
        return false;
    }

    if( GN::engine::EffectDesc::OPCODE == p->type )
    {
        // store opcode
        SInt32 op = p->opcode;
        if( op < 0 || op >= GN::engine::EffectDesc::NUM_OPCODES )
        {
            GN_ERROR(sLogger)( "invalid opcode : %d", op );
            return false;
        }
        ++p;

        const OpCodeDesc & ocd = sOpCodeTable[op];

        // get value of arguments
        GN::engine::EffectDesc::Token s0, s1;
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
bool GN::engine::EffectDesc::CondExp::evaluate() const
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
void GN::engine::EffectDesc::CondExp::compose(
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
bool GN::engine::EffectDesc::valid() const
{
    GN_GUARD;

    // check shaders
    for( std::map<StrA,ShaderDesc>::const_iterator i = shaders.begin(); i != shaders.end(); ++i )
    {
        const StrA & shaderName = i->first;
        const ShaderDesc & shader = i->second;

        if( shader.type < 0 || shader.type >= gfx::NUM_SHADER_TYPES )
        {
            GN_ERROR(sLogger)( "Shader(%s)的类型非法: %d。", shaderName.cptr(), shader.type );
            return false;
        }

        if( !shader.code.empty() )
        {
            if( shader.lang < 0 || shader.lang >= gfx::NUM_SHADING_LANGUAGES )
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

            if( stage >= gfx::MAX_TEXTURE_STAGES )
            {
                GN_ERROR(sLogger)( "Shader(%s)的贴图引用(%s)的stage超过允许上限(%d): %d.",
                    shaderName.cptr(), name.cptr(), gfx::MAX_TEXTURE_STAGES, stage );
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

            for( size_t i = 0; i < gfx::NUM_SHADER_TYPES; ++i )
            {
                const StrA & shaderName = pass.shaders[i];

                if( !sExist( shaders, shaderName ) )
                {
                    GN_ERROR(sLogger)( "Technique(%s)引用了无效的Shader: %s.", techName.cptr(), shaderName.cptr() );
                    return false;
                }

                const ShaderDesc & shader = getShader( shaderName );

                if( (gfx::ShaderType)i != shader.type )
                {
                    GN_ERROR(sLogger)( "Shader(%s)的类型(%s)与Technique(%s)所期望的类型(%s)不一致.",
                        shaderName.cptr(),
                        gfx::shaderType2Str( shader.type ),
                        techName.cptr(),
                        gfx::shaderType2Str( (gfx::ShaderType)i ) );
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
void GN::engine::EffectDesc::saveToXmlFile( File & )
{
}

// *****************************************************************************
// Effect class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::Effect::init( const EffectDesc & d )
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
bool GN::engine::Effect::init( const Effect & e )
{
    GN_GUARD;

    GN_ASSERT( e.mDesc.valid() );
    return init( e.mDesc );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::Effect::quit()
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
bool GN::engine::Effect::createEffect()
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
    GN_TRACE(sLogger)( "Using technique '%s'.", mTechniques.items[mActiveTechnique].name.cptr() );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::Effect::createShader( ShaderData & data, const StrA & name, const EffectDesc::ShaderDesc & desc )
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
    if( 0 == data.value && !desc.code.empty() )
    {
        data.value = mEngine.createShader( name, desc.type, desc.lang, desc.code, desc.hints );
        if( 0 == data.value )
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

            // TODO: make sure the shader do have this uniform
            urd.binding = uniBinding;
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
bool GN::engine::Effect::createTechnique( TechniqueData & data, const EffectDesc::TechniqueDesc & desc )
{
    GN_GUARD;

    data.name = desc.name;

    data.passes.resize( desc.passes.size() );

    for( size_t iPass = 0; iPass < desc.passes.size(); ++iPass )
    {
        const EffectDesc::PassDesc & passDesc = desc.passes[iPass];

        PassData & passData = data.passes[iPass];

        for( size_t iShader = 0; iShader < gfx::NUM_SHADER_TYPES; ++iShader )
        {
            const StrA & shaderName = passDesc.shaders[iShader];

            passData.shaders[iShader] = mShaders.find( shaderName );

            if( 0 == passData.shaders[iShader] )
            {
                GN_TRACE(sLogger)( "Technique '%s' is ignored, because shader '%s' is not found.", desc.name.cptr(), shaderName.cptr() );
                return false;
            }

            GN_ASSERT( mDesc.getShader(shaderName).type == (gfx::ShaderType)iShader );
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
void GN::engine::Effect::sSetFfpUniform(
    DrawContext & ctx, SInt32 type, const UniformData & data )
{
    GN_GUARD;

    const gfx::UniformValue & value = data.value;

    switch( type )
    {
        case FFP_TRANSFORM_WORLD :
            if( gfx::UVT_MATRIX44 == type )
            {
                GN_ASSERT( !value.matrix44s.empty() );
                ctx.setWorld( value.matrix44s[0] );
            }
            else
            {
                GN_ERROR(sLogger)( "FFP_TRANSFORM_WORLD only accepts matrix type." );
            }
            break;

        case FFP_TRANSFORM_VIEW :
            if( gfx::UVT_MATRIX44 == type )
            {
                GN_ASSERT( !value.matrix44s.empty() );
                ctx.setView( value.matrix44s[0] );
            }
            else
            {
                GN_ERROR(sLogger)( "FFP_TRANSFORM_VIEW only accepts matrix type." );
            }
            break;

        case FFP_TRANSFORM_PROJ :
            if( gfx::UVT_MATRIX44 == type )
            {
                GN_ASSERT( !value.matrix44s.empty() );
                ctx.setProj( value.matrix44s[0] );
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

// *****************************************************************************
// global entity functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::EntityTypeId GN::engine::getEffectEntityType( EntityManager & em )
{
    static EntityTypeId type = em.createEntityType( "effect" );
    return type;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::loadEffectEntityFromXmlFile( EntityManager & em, RenderEngine & re, const StrA & filename )
{
    GN_GUARD;

    StrA fullpath;
    normalizePathSeparator( fullpath, core::toNative( filename ) );

    // check if the entity is already loaded
    Entity * e = em.getEntityByName( fullpath, true );
    if( e ) return e;

    GN_INFO(sLogger)( "Load %s", fullpath.cptr() );

    // open XML file
    AutoObjPtr<File> fp( core::openFile( fullpath, "rt" ) );
    if( !fp ) return 0;

    // parse XML file
    XmlDocument doc;
    XmlParseResult xpr;
    if( !doc.parse( xpr, *fp ) )
    {
        GN_ERROR(sLogger)(
            "Fail to parse XML file (%s):\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            fullpath.cptr(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.cptr() );
        return 0;
    }
    GN_ASSERT( xpr.root );

    StrA basedir = dirName( fullpath );
    EffectDesc desc;
    if( !desc.loadFromXmlNode( *xpr.root, basedir ) ) return 0;

    // success
    return createEffectEntity( em, re, fullpath, desc );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::Entity * GN::engine::createEffectEntity( EntityManager & em, RenderEngine & re, const StrA & name, const EffectDesc & desc )
{
    GN_GUARD;

    // check if the entity is already loaded
    Entity * e = em.getEntityByName( name, true );
    if( e )
    {
        GN_ERROR(sLogger)( "effect entity creation failed: entity named '%s' does already exist!", name.cptr() );
        return 0;
    }

    // create effect instance
    AutoObjPtr<Effect> eff( new Effect(re) );
    if( !eff->init( desc ) ) return 0;

    // create entity
    e = em.createEntity<Effect*>( getMeshEntityType(em), name, eff.get(), &safeDelete<Effect> );
    if( 0 == e ) return 0;

    // success
    eff.detach();
    return e;

    GN_UNGUARD;
}
