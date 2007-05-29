#ifndef __GN_ENGINE_EFFECT_H__
#define __GN_ENGINE_EFFECT_H__
// *****************************************************************************
/// \file    engine/effect.h
/// \brief   Graphics effect class
/// \author  chenlee (2006.1.7)
// *****************************************************************************

namespace GN { namespace engine {
    ///
    /// Graphics effect descriptor
    ///
    struct EffectDesc
    {
        ///
        /// conditional expression opcode
        ///
        enum OpCode
        {
            CMP_LT, ///< s0 < s1
            CMP_LE, ///< s0 <= s1
            CMP_EQ, ///< s0 == s1
            CMP_NE, ///< s0 != s1
            CMP_GE, ///< s0 >= s1
            CMP_GT, ///< s0 > s1

            ALU_ADD, ///< s0 + s1
            ALU_DEC, ///< s0 - s1
            ALU_NEG, ///< -s0

            BIT_AND, ///< s0 & s1
            BIT_OR,  ///< s0 | s1
            BIT_XOR, ///< s0 ^ s1
            BIT_NOT, ///< ~s0

            REL_AND, ///< s0 && s1
            REL_OR,  ///< s0 || s1
            REL_NOT, ///< !s0

            CHECK_RENDERER_CAPS, ///< Renderer::getCaps( s0 )
            CHECK_SHADER_PROFILE, ///< Renderer::supportShader( s0, s1 )

            NUM_OPCODES, ///< number of avaliable opcode(s)

            OPCODE_INVALID = NUM_OPCODES, ///< invalid opcode
        };

        ///
        /// Convert string to opcode. Return OPCODE_INVALID if failed.
        ///
        static OpCode sStr2OpCode( const StrA & );

        ///
        /// Conditional expression token type.
        ///
        enum TokenType
        {
            OPCODE, ///< opcode
            VALUEI, ///< integer value
            VALUES, ///< string value
        };

        ///
        /// Conditional expression token structure.
        ///
        struct Token
        {
            TokenType type; ///< token type

            static const size_t MAX_STRLEN = 12; ///< max length of string value.

            union
            {
                SInt32 opcode; ///< opcode
                SInt32 valueI; ///< integer value
                char    valueS[MAX_STRLEN]; ///< string buffer
            };

            ///
            /// Set token as an opcode
            ///
            void setOp( SInt32 op )
            {
                GN_CASSERT( sizeof(Token) == 16 );
                GN_ASSERT( 0 <= op && op < NUM_OPCODES );
                type = OPCODE;
                opcode = op;
            }

            ///
            /// Set token as an integer value
            ///
            void setI( SInt32 i )
            {
                type = VALUEI;
                valueI = i;
            }

            ///
            /// Set token as an string
            ///
            void setS( const StrA & s )
            {
                type = VALUES;
                size_t l = min( s.size(), MAX_STRLEN-1 );
                memcpy( valueS, s.cptr(), l );
                valueS[l] = 0;
            }
        };

        ///
        /// Contidional expression for renderer caps check
        ///
        struct CondExp
        {
            // TODO: use custom allocator to optimize runtime memory allocation performance.

            ///
            /// expression tokens
            ///
            std::vector<Token> tokens;

            ///
            /// Dummy instance.
            ///
            static const CondExp DUMMY;

            ///
            /// Default ctor ( do nothing)
            ///
            CondExp() {}

            ///
            /// Construct from integer
            ///
            CondExp( SInt32 i )
            {
                tokens.resize(1);
                tokens[0].setI( i );
            }

            ///
            /// Construct from string
            ///
            CondExp( const char * s )
            {
                tokens.resize(1);
                tokens[0].setS( s );
            }

            ///
            /// Construct from string
            ///
            CondExp( const StrA & s )
            {
                tokens.resize(1);
                tokens[0].setS( s );
            }

            ///
            /// Construct expression from specific operation.
            ///
            CondExp( OpCode op, const CondExp & c1, const CondExp & c2 = DUMMY )
            {
                compose( op, c1, c2 );
            }

            ///
            /// Copy constructor
            ///
            CondExp( const CondExp & c )
            {
                tokens = c.tokens;
            }

            ///
            /// copy operator
            ///
            CondExp & operator=( const CondExp & rhs )
            {
                if( this != &rhs ) tokens = rhs.tokens;
                return *this;
            }

            ///
            /// Evaluate the expression.
            ///
            /// - Return true for:
            ///   - empty expression
            ///   - zero integer
            ///   - empty string
            /// - Return false for:
            ///   - invalid expression
            ///   - non-zero integer
            ///   - non-empty string
            ///
            bool evaluate( RenderEngine & eng ) const;

            ///
            /// Construct expression from two existing expression
            ///
            void compose( OpCode op, const CondExp & c1, const CondExp & c2 = DUMMY );

            //@{
            /// \name CondExp constructors

            static CondExp sBitAnd( const CondExp & a0, const CondExp & a1 )
            {
                return CondExp( BIT_AND, a0, a1 );
            }

            static CondExp sBitOr( const CondExp & a0, const CondExp & a1 )
            {
                return CondExp( BIT_OR, a0, a1 );
            }

            static CondExp sBitXor( const CondExp & a0, const CondExp & a1 )
            {
                return CondExp( BIT_XOR, a0, a1 );
            }

#define GN_CONDEXP_OPERATOR( x, y ) \
            CondExp operator x ( const CondExp & rhs ) const \
            { \
                return CondExp( y, *this, rhs ); \
            }

            GN_CONDEXP_OPERATOR( <  , CMP_LT  );
            GN_CONDEXP_OPERATOR( <= , CMP_LE  );
            GN_CONDEXP_OPERATOR( == , CMP_EQ  );
            GN_CONDEXP_OPERATOR( != , CMP_NE  );
            GN_CONDEXP_OPERATOR( >= , CMP_GE  );
            GN_CONDEXP_OPERATOR( >  , CMP_GT  );
            GN_CONDEXP_OPERATOR( && , REL_AND );
            GN_CONDEXP_OPERATOR( || , REL_OR  );

#undef GN_CONDEXP_OPERATOR

            //@}
        };

        ///
        /// Texture descriptor
        ///
        struct TextureDesc
        {
            // TODO: filters, wraps ...
        };

        ///
        /// Uniform descriptor
        ///
        struct UniformDesc
        {
            bool              hasDefaultValue; ///< Indicates that the uniform has a default value.
            gfx::UniformValue defaultValue;    ///< Default value of the uniform. Ignored if hasDefaultValue is false.

            UniformDesc() : hasDefaultValue(false) {}
        };

        ///
        /// Shader descriptor
        ///
        struct ShaderDesc
        {
            gfx::ShaderType       type; ///< Shader type.
            gfx::ShadingLanguage  lang; ///< Shading language. Ignored if code is empty.
            StrA                  code; ///< Shader code. Empty means fixed functional pipeline.
            StrA                  hints; ///< Shader hints.
            std::map<UInt32,StrA> textures; ///< textures used by the shader. Key is texture stage, value is texture name.
            std::map<StrA,StrA>   uniforms; ///< uniforms used by the shader. Key is uniform binding, value is uniform name.
            CondExp               prerequisites; ///< prerequisites of the shader.
        };

        ///
        /// Rendering pass descriptor
        ///
        struct PassDesc
        {
            gfx::RenderStateBlockDesc rsb; ///< Pass specific render state block
            StrA shaders[gfx::NUM_SHADER_TYPES]; ///< shader names of the pass. Can't be empty

            ///
            /// Ctor
            ///
            PassDesc() : rsb(gfx::RenderStateBlockDesc::EMPTY) {}
        };

        ///
        /// Technique descriptor structure
        ///
        struct TechniqueDesc
        {
            StrA                      name; ///< technique name
            std::vector<PassDesc>     passes; ///< pass list.
            gfx::RenderStateBlockDesc rsb; ///< Technique specific render states

            ///
            /// Ctor
            ///
            TechniqueDesc() : rsb(gfx::RenderStateBlockDesc::EMPTY) {}
        };

        std::map<StrA,TextureDesc> textures;   ///< Texture list
        std::map<StrA,UniformDesc> uniforms;   ///< Uniform list
        std::map<StrA,ShaderDesc>  shaders;    ///< Shader list
        std::vector<TechniqueDesc> techniques; ///< Technique list. Technique name must be unique.
        gfx::RenderStateBlockDesc  rsb;        ///< Effect global render states

        ///
        /// Ctor
        ///
        EffectDesc() : rsb(gfx::RenderStateBlockDesc::DEFAULT) {}

        ///
        /// Make sure the effect descriptor is valid.
        ///
        bool valid() const;

        ///
        /// clear to a empty descriptor
        ///
        void clear()
        {
            textures.clear();
            uniforms.clear();
            shaders.clear();
            techniques.clear();
            rsb.resetToDefault();
        }

        ///
        /// setup the descriptor from XML string
        ///
        bool loadFromXmlNode( const XmlNode & root, const StrA & basedir );

        ///
        /// write the descriptor to file
        ///
        void saveToXmlFile( File & );

        ///
        /// Find uniform by name. Return NULL if not found.
        ///
        const UniformDesc * findUniform( const StrA & name ) const
        {
            std::map<StrA,UniformDesc>::const_iterator i = uniforms.find( name );
            return ( uniforms.end() != i ) ? &i->second : NULL;
        }

        ///
        /// Get uniform by name.
        /// \param name
        ///     MUST be a valid uniform name.
        ///
        const UniformDesc & getUniform( const StrA & name ) const
        {
            std::map<StrA,UniformDesc>::const_iterator i = uniforms.find( name );
            GN_ASSERT( uniforms.end() != i );
            return i->second;
        }

        ///
        /// Find texture by name. Return NULL if not found.
        ///
        const TextureDesc * findTexture( const StrA & name ) const
        {
            std::map<StrA,TextureDesc>::const_iterator i = textures.find( name );
            return ( textures.end() != i ) ? &i->second : NULL;
        }

        ///
        /// Get texture by name.
        /// \param name
        ///     MUST be a valid texture name.
        ///
        const TextureDesc & getTexture( const StrA & name ) const
        {
            std::map<StrA,TextureDesc>::const_iterator i = textures.find( name );
            GN_ASSERT( textures.end() != i );
            return i->second;
        }

        ///
        /// Find shader by name. Return NULL if not found.
        ///
        const ShaderDesc * findShader( const StrA & name ) const
        {
            std::map<StrA,ShaderDesc>::const_iterator i = shaders.find( name );
            return ( shaders.end() != i ) ? &i->second : NULL;
        }

        ///
        /// Get shader by name.
        /// \param name
        ///     MUST be a valid shader name.
        ///
        const ShaderDesc & getShader( const StrA & name ) const
        {
            std::map<StrA,ShaderDesc>::const_iterator i = shaders.find( name );
            GN_ASSERT( shaders.end() != i );
            return i->second;
        }
    };

    typedef UInt32 EffectItemID; ///< effect item (such as technique, uniform and texture) ID.

    ///
    /// Effect class
    ///
    class Effect : public StdClass
    {
        GN_DECLARE_STDCLASS( Effect, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        Effect( RenderEngine & engine ) : mEngine(engine) { clear(); }
        virtual ~Effect() { quit(); }
        RenderEngine & renderEngine() const { return mEngine; }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const EffectDesc & ); ///< initialize from descriptor
        bool init( const Effect & ); ///< Make effect clone
        void quit();
    private:
        void clear() { mPassBegun = false; mActiveContext = NULL; }
        //@}

        // ********************************
        /// \name rendering functions
        ///
        /// Standard call sequence:
        /// <pre>
        ///     DrawContext ctx;
        ///     set_common_uniforms_and_textures( ctx );
        ///     size_t numPasses = myEffect->getNumPasses();
        ///     for( size_t i = 0; i < numPasses; ++i )
        ///     {
        ///         myEffect->passBegin( ctx, i );
        ///         for_each_mesh
        ///         {
        ///             set_mesh_specific_uniforms_and_textures( ctx );
        ///             set_mesh_vertex_and_index_buffers( ctx );
        ///             myEffect->commitChanges();
        ///             engine.setContext( ctx );
        ///             engine.drawIndexed( ... );
        ///         }
        ///         myEffect->passEnd();
        ///     }
        /// </pre>
        // ********************************
    public:

        //@{

        ///
        /// Get rendering pass count. This value may change when active technique changes.
        ///
        size_t getNumPasses() const;

        ///
        /// apply render state of specific pass to renderer context.
        ///
        void passBegin( DrawContext &, size_t ) const;

        ///
        /// end the rendering pass.
        /// Must be called pair with passBegin().
        ///
        void passEnd() const
        {
            GN_ASSERT( mPassBegun );
            mPassBegun = false;
            mActiveContext = NULL;
        }

        ///
        /// Commit modified uniforms and textures to renderer.
        /// Can be called only between passBegin() and passEnd().
        ///
        void commitChanges() const;

        //@}

        // ********************************
        /// \name technique management
        // ********************************
    public:

        //@{

        bool hasTechnique( const StrA & name, EffectItemID * id ) const;

        ///
        /// get technique ID
        ///
        EffectItemID getTechniqueID( const StrA & name ) const;

        ///
        /// set active technique. 0 means the default one.
        ///
        void setActiveTechnique( EffectItemID ) const;

        ///
        /// Set active technique.
        /// \param name Technique name. Empty string means the default one.
        ///
        void setActiveTechniqueByName( const StrA & name ) const;

        //@}

        // ********************************
        /// \name uniform management
        // ********************************
    public:

        //@{
        bool hasUniform( const StrA & name, EffectItemID * id ) const;
        EffectItemID getUniformID( const StrA & ) const;
        void setUniform( EffectItemID, const gfx::UniformValue & ) const;
        void setUniformByName( const StrA &, const gfx::UniformValue & ) const;
        //@}

        // ********************************
        /// \name texture management
        ///
        /// Note that effect class won't hold reference of the textures.
        // ********************************
    public:

        //@{
        bool hasTexture( const StrA & name, EffectItemID * id ) const;
        EffectItemID getTextureID( const StrA & ) const;
        void setTexture( EffectItemID, GraphicsResource * ) const;
        void setTextureByName( const StrA &, GraphicsResource * ) const;
        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct TextureData
        {
            StrA               name;  // texture name
            GraphicsResource * value;
            TextureData() : value(0) {}
        };

        struct TextureRefData
        {
            EffectItemID id; // texture ID that is referenced.
            UInt32       stage;
        };

        struct ShaderRefData
        {
            EffectItemID id;    // shader ID that is referenced
            size_t       index; // index to ShaderData::uniforms: which uniform are we referencing to.
        };

        struct UniformData
        {
            StrA                        name;
            gfx::UniformValue           value;
            std::vector<ShaderRefData>  shaders; // shaders that are referencing this uniform.
        };

        struct UniformRefData
        {
            EffectItemID id;      ///< uniform ID that is being referenced.
            StrA         binding; ///< shader specific binding name of the uniform.
            SInt32       ffpType; ///< FFP uniform type. Effective only when ffp is true.
            bool         ffp;     ///< is this binding to fixed functional pipeline? (determined by binding name)
        };

        struct ShaderData
        {
            StrA                        name;
            GraphicsResource *          value;
            std::vector<TextureRefData> textures;      // texture referencing list.
            std::vector<UniformRefData> uniforms;      // uniform referencing list.
            std::vector<size_t>         dirtyUniforms; // dirty uniform list. Each item is a index into ShaderData::uniforms

            ShaderData() : value(0) {}
        };

        struct PassData
        {
            EffectItemID              shaders[gfx::NUM_SHADER_TYPES]; ///< shader IDs of this pass.
            gfx::RenderStateBlockDesc rsb; ///< render state block
        };

        struct TechniqueData
        {
            StrA                  name;
            std::vector<PassData> passes;
        };

        template<typename T>
        struct NamedItemManager
        {
            std::map<StrA,EffectItemID>   names;
            HandleManager<T,EffectItemID> items;

            bool add( const StrA & name, const T & item )
            {
                GN_ASSERT( names.end() == names.find(name) );
                names[name] = items.add( item );
                GN_ASSERT( items.validHandle( names[name] ) );
                return true;
            }
            void clear() { names.clear(); items.clear(); }
            bool empty() const { GN_ASSERT(names.size() == items.size()); return items.empty(); }
            size_t size() const { GN_ASSERT(names.size() == items.size()); return items.size(); }
            EffectItemID find( const StrA & name ) const
            {
                if( 0 == name ) return 0;
                std::map<StrA,EffectItemID>::const_iterator i = names.find(name);
                if( names.end() == i ) return 0;
                GN_ASSERT( items.validHandle(i->second)  );
                return i->second;
            }
        };

        RenderEngine & mEngine;

        EffectDesc mDesc;

        NamedItemManager<TextureData>   mTextures;
        NamedItemManager<UniformData>   mUniforms;
        NamedItemManager<ShaderData>    mShaders;
        NamedItemManager<TechniqueData> mTechniques;

        mutable DrawContext * mActiveContext;
        mutable size_t mActivePass;
        mutable UInt32 mActiveTechnique;
        mutable bool   mPassBegun;

        static Logger * sLogger;

        // ********************************
        // private functions
        // ********************************
    private:

        bool createEffect(); // called by init()
        bool createShader( ShaderData &, const StrA &, const EffectDesc::ShaderDesc & );
        bool createTechnique( TechniqueData &, const EffectDesc::TechniqueDesc & );
        static void sSetFfpUniform( DrawContext &, SInt32, const UniformData & );
    };

    //@{

    EntityTypeId getEffectEntityType( EntityManager & em );

    ///
    /// try find exising effect entity named "filename", if not found, create new one.
    ///
    Entity * loadEffectEntityFromXmlFile( EntityManager & em, RenderEngine & re, const StrA & filename );

    ///
    /// create new effect entity. name must be unique.
    ///
    Entity * createEffectEntity( EntityManager & em, RenderEngine & re, const StrA & name, const EffectDesc & desc );

    //@}
}}

#include "effect.inl"

// *****************************************************************************
//                           End of effect.h
// *****************************************************************************
#endif // __GN_ENGINE_EFFECT_H__
