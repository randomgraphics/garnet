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
        class ConditionalExpression
        {
            enum TokenType
            {
                OPCODE,
                GFXCAPS,
                VALUE,
            };

            struct Token
            {
                TokenType type;
                union
                {
                    int32_t  opcode;
                    int32_t  gfxcaps;
                    uint32_t value;
                };
            };

            // TODO: use custom allocator to optimize runtime memory allocation performance.

            AutoArray<Token> mTokens;

            bool doEval( uint32_t & value, const Token * & p, const Token * e ) const;

        public:

            //!
            //! Default ctor
            //!
            ConditionalExpression() {}

            //!
            //! Construct from string
            //!
            explicit ConditionalExpression( const char * s, size_t strLen = 0 ) { fromStr( s, strLen ); }

            //!
            //! Evaluate the expression. Note that empty expression is treated as "1".
            //!
            bool evaluate( uint32_t & value ) const;

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
            //! Begin rendering.
            //!
            bool drawBegin( size_t * numPass ) const;

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
            //! set active technique. 0 means the first technique.
            //!
            void setActiveTechnique( uint32_t ) const;

            //!
            //! Set active technique.
            //! \param name Technique name. Empty string means the first technique.
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
                bool                  ready; //!< true, if tecnique is ready to use.
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
            bool initTechnique( uint32_t handle ) const; // initialize specific technique.

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
