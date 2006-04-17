#ifndef __GN_GFX_EFFECT_H__
#define __GN_GFX_EFFECT_H__
// *****************************************************************************
//! \file    gfx/effect.h
//! \brief   Graphics effect class (not implemented)
//! \author  chenlee (2006.1.7)
// *****************************************************************************

//!
//! global effect dictionary
//!
#define gEffectDict (GN::gfx::EffectDictionary::sGetInstance())

//!
//! pointer to global effect dictionary
//!
#define gEffectDictPtr (GN::gfx::EffectDictionary::sGetInstancePtr())

namespace GN { namespace gfx {

    //!
    //! namespace for graphics effect classes
    //!
    namespace effect
    {
        //!
        //! Contidional expression for renderer caps check
        //!
        class CondExp
        {
            enum TokenType
            {
                OPCODE,
                GFXCAPS,
                VALUE,
            };

            enum Operation
            {
                CMP_LT,
                CMP_LE,
                CMP_EQ,
                CMP_NE,
                CMP_GE,
                CMP_GT,

                ALU_ADD,
                ALU_DEC,

                BIT_AND,
                BIT_OR,
                BIT_XOR,

                REL_AND,
                REL_OR,

                NUM_OPCODES,
            };

            struct Token
            {
                TokenType type;
                union
                {
                    int32_t  opcode;
                    int      gfxcaps;
                    uint32_t value;
                };
            };

            // TODO: use custom allocator to optimize runtime memory allocation performance.

            AutoArray<Token> mTokens;

            static bool sDoEval( uint32_t & value, const Token * & p, const Token * e );

            static uint32_t sCalc( int32_t op, uint32_t a0, uint32_t a1 )
            {
                GN_ASSERT( 0 <= op && op <= NUM_OPCODES );
                switch( op )
                {
                    case CMP_LT  : return a0 < a1;
                    case CMP_LE  : return a0 <= a1;
                    case CMP_EQ  : return a0 == a1;
                    case CMP_NE  : return a0 != a1;
                    case CMP_GE  : return a0 >= a1;
                    case CMP_GT  : return a0 > a1;
                    case REL_AND : return a0 && a1;
                    case REL_OR  : return a0 || a1;
                    default : GN_UNIMPL(); return 0;
                }
            }

            static void sCombine( CondExp & r, Operation op, const CondExp & c1, const CondExp & c2 )
            {
                static Token sEmptyToken = { VALUE, 1 };

                const Token * t1, * t2;
                size_t n1, n2;

                if( c1.mTokens.empty() )
                {
                    t1 = &sEmptyToken;
                    n1 = 1;
                }
                else
                {
                    t1 = c1.mTokens;
                    n1 = c1.mTokens.size();
                }

                if( c2.mTokens.empty() )
                {
                    t2 = &sEmptyToken;
                    n2 = 1;
                }
                else
                {
                    t2 = c2.mTokens;
                    n2 = c2.mTokens.size();
                }

                if( 1 == n1 && VALUE == t1->type && 1 == n2 && VALUE == t2->type )
                {
                    uint32_t newValue = sCalc( op, t1->value, t2->value );
                    if( 1 == newValue )
                    {
                        r.mTokens.clear();
                    }
                    else
                    {
                        r.mTokens.resize( 1 );
                        r.mTokens[0].type = VALUE;
                        r.mTokens[0].value = newValue;
                    }
                }
                else
                {
                    GN_ASSERT( n1 > 0 && n2 > 0 );
                    r.mTokens.resize( n1 + n2 + 1 );
                    r.mTokens[0].type = OPCODE;
                    r.mTokens[0].opcode = op;

                    memcpy( r.mTokens + 1, t1, n1 * sizeof(Token) );
                    memcpy( r.mTokens + n1 + 1, t2, n2 * sizeof(Token) );
                }
            }

        public:

            //!
            //! Default ctor
            //!
            CondExp() {}

            //!
            //! Copy constructor
            //!
            CondExp( const CondExp & c )
            {
                mTokens.resize( c.mTokens.size() );
                memcpy( mTokens, c.mTokens, sizeof(Token)*mTokens.size() );
            }

            //!
            //! Construct from string
            //!
            explicit CondExp( const char * s, size_t strLen = 0 ) { fromStr( s, strLen ); }

            //!
            //! Evaluate the expression. Return false for invalid expression.
            //!
            //! Note that empty expression is valid, and the value is "1"
            //!
            bool evaluate( uint32_t & value ) const;

            //!
            //! Evaluate the expression. Return 0 for invalid expression, and 1 for empty expression.
            //!
            uint32_t evaluate() const { uint32_t v; return evaluate(v) ? v : 0; }

            //!
            //! Construct expression from string. Setup a empty expression, if string is invalid.
            //!
            void fromStr( const char * s, size_t strLen = 0 );

            //!
            //! convert to string
            //!
            void toStr( StrA & s ) const;

            //!
            //! convert to string
            //!
            StrA toStr() const { StrA s; toStr(s); return s; }

            //!
            //! Copy operator
            //!
            CondExp & operator=( const CondExp & rhs )
            {
                mTokens.resize( rhs.mTokens.size() );
                memcpy( mTokens, rhs.mTokens, sizeof(Token)*mTokens.size() );
                return *this;
            }

            //@{

            //!
            //! make new CondExp from gfxcaps
            //!
            static CondExp sGfxCaps( RendererCaps c )
            {
                CondExp exp;
                exp.mTokens.resize(1);
                exp.mTokens[0].type = GFXCAPS;
                exp.mTokens[0].gfxcaps = c;
            }

            //!
            //! make new CondExp from gfxcaps
            //!
            static CondExp sValue( uint32_t v )
            {
                CondExp exp;
                exp.mTokens.resize(1);
                exp.mTokens[0].type = VALUE;
                exp.mTokens[0].value = v;
                return exp;
            }

#define GN_CONDEXP_OPERATOR( x, y ) \
            CondExp operator x ( const CondExp & rhs ) const \
            { \
                CondExp c; \
                sCombine( c, y, *this, rhs ); \
                return c; \
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

        //!
        //! Texture descriptor
        //!
        struct TextureDesc
        {
            StrA defaultValue; //!< Default texture resource name. Empty means no default value.
        };

        //!
        //! Uniform descriptor
        //!
        struct UniformDesc
        {
            bool hasDefaultValue;      //!< Indicates that the uniform has a default value.
            UniformValue defaultValue; //!< Default value of the uniform. Ignored if hasDefaultValue is false.

            UniformDesc() : hasDefaultValue(false) {}
        };

        //!
        //! Shader descriptor
        //!
        struct ShaderDesc
        {
            ShaderType type; //!< Shader type.
            ShadingLanguage lang; //!< Shading language. Ignored if code is empty.
            StrA code; //!< Shader code. Empty means fixed functional pipeline.
            StrA hints; //!< Shader hints.
            std::map<uint32_t,StrA> textures; //!< textures used by the shader. Key is texture stage, value is texture name.
            std::map<StrA,StrA>     uniforms; //!< uniforms used by the shader. Key is uniform binding, value is uniform name.
            CondExp conditions; //!< required renderer caps of this shader.
        };

        //!
        //! Rendering pass descriptor
        //!
        struct PassDesc
        {
            RenderStateBlockDesc rsb; //!< Pass specific render state block
            StrA shaders[NUM_SHADER_TYPES]; //!< shader names of the pass. Can't be empty

            //!
            //! Ctor
            //!
            PassDesc() : rsb(RenderStateBlockDesc::EMPTY) {}
        };

        //!
        //! Technique descriptor structure
        //!
        struct TechniqueDesc
        {
            std::vector<PassDesc> passes; //!< pass list.
            RenderStateBlockDesc  rsb; //!< Technique specific render states

            //!
            //! Ctor
            //!
            TechniqueDesc() : rsb(RenderStateBlockDesc::EMPTY) {}
        };

        //!
        //! Graphics effect descriptor
        //!
        struct EffectDesc
        {
            std::map<StrA,TextureDesc>   textures;   //!< Texture list
            std::map<StrA,UniformDesc>   uniforms;   //!< Uniform list
            std::map<StrA,ShaderDesc>    shaders;    //!< Shader list
            std::map<StrA,TechniqueDesc> techniques; //!< Technique list
            RenderStateBlockDesc         rsb;        //!< Effect global render states

            //!
            //! Ctor
            //!
            EffectDesc() : rsb(RenderStateBlockDesc::DEFAULT) {}

            //!
            //! Make sure the effect descriptor is valid.
            //!
            bool valid() const;

            //!
            //! clear to a empty descriptor
            //!
            void clear()
            {
                textures.clear();
                uniforms.clear();
                shaders.clear();
                techniques.clear();
                rsb.resetToDefault();
            }

            //!
            //! Get uniform by name.
            //! \param name
            //!     MUST be a valid uniform name.
            //!
            const UniformDesc & getUniform( const StrA & name ) const
            {
                std::map<StrA,UniformDesc>::const_iterator i = uniforms.find( name );
                GN_ASSERT( uniforms.end() != i );
                return i->second;
            }

            //!
            //! Get shader by name.
            //! \param name
            //!     MUST be a valid shader name.
            //!
            const ShaderDesc & getShader( const StrA & name ) const
            {
                std::map<StrA,ShaderDesc>::const_iterator i = shaders.find( name );
                GN_ASSERT( shaders.end() != i );
                return i->second;
            }
        };

        //!
        //! Effect class
        //!
        class Effect : public StdClass
        {
             GN_DECLARE_STDCLASS( Effect, StdClass );

            // ********************************
            // ctor/dtor
            // ********************************

            //@{
        public:
            Effect()          { clear(); }
            virtual ~Effect() { quit(); }
            //@}

            // ********************************
            // from StdClass
            // ********************************

            //@{
        public:
            bool init( const EffectDesc & ); //!< initialize from descriptor
            bool init( const Effect & ); //!< Make effect clone
            void quit();
            bool ok() const { return MyParent::ok(); }
        private:
            void clear() { mDrawBegun = false; mPassBegun = false; }
            //@}

            // ********************************
            //! \name rendering functions
            //!
            //! Standard call sequence:
            //! <pre>
            //!     set_common_uniforms_and_textures();
            //!     size_t numPasses;
            //!     if( myEffect->drawBegin( &numPasses ) )
            //!     {
            //!         for( size_t i = 0; i < numPasses; ++i )
            //!         {
            //!             myEffect->passBegin( i );
            //!             for_each_mesh
            //!             {
            //!                 set_mesh_specific_uniforms_and_textures();
            //!                 myEffect->commitChanges();
            //!                 draw_the_mesh();
            //!             }
            //!             myEffect->passEnd();
            //!         }
            //!         myEffect->drawEnd();
            //!     }
            //! </pre>
            // ********************************
        public:

            //@{

            //!
            //! Begin rendering. Return number of rendering passes.
            //!
            size_t drawBegin() const;

            //!
            //! End rendering.
            //!
            void drawEnd() const { GN_ASSERT(mDrawBegun); mDrawBegun = false; }

            //!
            //! apply render state of specific pass
            //! Must be called between drawBegin() and drawEnd().
            //!
            void passBegin( size_t ) const;

            //!
            //! end the rendering pass.
            //! Must be called between drawBegin() and drawEnd(), and after passBegin().
            //!
            void passEnd() const { GN_ASSERT(mPassBegun); mPassBegun = false; }

            //!
            //! Commit modified uniforms and textures to renderer.
            //! Must be called between passBegin() and passEnd().
            //!
            void commitChanges() const;

            //@}

            // ********************************
            //! \name technique management
            // ********************************
        public:

            //@{

            //!
            //! get technique handle
            //!
            uint32_t getTechniqueHandle( const StrA & name ) const;

            //!
            //! set active technique. 0 means the default one.
            //!
            void setActiveTechnique( uint32_t ) const;

            //!
            //! Set active technique.
            //! \param name Technique name. Empty string means the default one.
            //!
            void setActiveTechniqueByName( const StrA & name ) const;

            //@}

            // ********************************
            //! \name uniform management
            // ********************************
        public:

            //@{

            //!
            //! Get handle of uniform variable
            //!
            uint32_t getUniformHandle( const StrA & ) const;

            void setUniform( uint32_t, const UniformValue & ) const;

            void setUniformByName( const StrA &, const UniformValue & ) const;

            //@}

            // ********************************
            //! \name texture management
            // ********************************
        public:

            //@{

            //!
            //! get texture handle
            //!
            uint32_t getTextureHandle( const StrA & ) const;

            //!
            //! set texture
            //!
            void setTexture( uint32_t key, uint32_t id ) const;

            //!
            //! set texture by name
            //!
            void setTextureByName( const StrA & name, uint32_t id ) const;

            //@}

            // ********************************
            // private variables
            // ********************************
        private:

            enum FfpParameterType
            {
                FFP_TRANSFORM_WORLD,
                FFP_TRANSFORM_VIEW,
                FFP_TRANSFORM_PROJ,
                FFP_VIEWPORT,
                FFP_LIGHT0_POS,
                FFP_LIGHT0_DIFFUSE,
                FFP_MATERIAL_DIFFUSE,
                FFP_MATERIAL_SPECULAR,
            };

            struct TextureData
            {
                StrA     name;
                uint32_t value;
            };

            struct TextureRefData
            {
                uint32_t handle;
                uint32_t stage;
            };

            struct ShaderRefData
            {
                uint32_t shader; // shader handle
                size_t   index;  // index to shader's uniform referencing list
            };

            struct UniformData
            {
                StrA                        name;
                UniformValue                value;
                std::vector<ShaderRefData>  shaders; // shaders that are referencing this uniform.
            };

            struct UniformRefData
            {
                StrA     binding; //!< uniform binding name
                uint32_t handle;  //!< handle to mUniforms
                bool     ffp;     //!< is it binding to fixed functional pipeline?
                union
                {
                    uint32_t         shaderUniformHandle; // shader uniform handle. Effective only when ffp is false.
                    FfpParameterType ffpParameterType;    // ffp parameter type. Effective only when ffp is true.
                };                
            };

            struct ShaderData
            {
                StrA                        name;
                AutoRef<Shader>             value;
                std::vector<TextureRefData> textures;      // texture referencing list.
                std::vector<UniformRefData> uniforms;      // uniform referencing list.
                std::set<size_t>            dirtyUniforms; // dirty uniform list. Each item is a index into the shader's uniform list.
            };

            struct PassData
            {
                uint32_t shaders[NUM_SHADER_TYPES]; //!< shader handles of this pass.
                RenderStateBlockDesc rsb; //!< render state block
            };

            struct TechniqueData
            {
                StrA                  name;
                std::vector<PassData> passes;
            };

            template<typename T>
            struct NamedItemManager
            {
                std::map<StrA,uint32_t>   names;
                HandleManager<T,uint32_t> items;

                bool add( const StrA & name, const T & item )
                {
                    GN_ASSERT( names.end() == names.find(name) );
                    names[name] = items.add( item );
                    GN_ASSERT( items.validHandle( names[name] ) );
                    return true;
                }
                void clear() { names.clear(); items.clear(); }
                bool empty() const { return names.empty() && items.empty(); }
                uint32_t find( const StrA & name ) const
                {
                    if( 0 == name ) return 0;
                    std::map<StrA,uint32_t>::const_iterator i = names.find(name);
                    if( names.end() == i ) return 0;
                    GN_ASSERT( items.validHandle(i->second)  );
                    return i->second;
                }
            };

            EffectDesc mDesc;

            NamedItemManager<TextureData>   mTextures;
            NamedItemManager<UniformData>   mUniforms;
            NamedItemManager<ShaderData>    mShaders;
            NamedItemManager<TechniqueData> mTechniques;
            
            mutable uint32_t mActiveTechnique;
            mutable bool     mDrawBegun;
            mutable bool     mPassBegun;
            mutable size_t   mActivePass;

            // ********************************
            // private functions
            // ********************************
        private:

            bool createEffect(); // called by init()

            static void sSetFfpParameter( FfpParameterType, const UniformData & );
        };

        //!
        //! Effect manager class (singleton)
        //!
        typedef ResourceManager<Effect*,true> EffectDictionary;
    }
}}

#if GN_ENABLE_INLINE
#include "effect.inl"
#endif

// *****************************************************************************
//                           End of effect.h
// *****************************************************************************
#endif // __GN_GFX_EFFECT_H__
