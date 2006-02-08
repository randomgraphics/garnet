#ifndef __GN_GFX_EFFECT_H__
#define __GN_GFX_EFFECT_H__
// *****************************************************************************
//! \file    gfx/effect.h
//! \brief   Graphics effect class (not implemented)
//! \author  chenlee (2006.1.7)
// *****************************************************************************

namespace GN { namespace gfx {
    //!
    //! namespace for graphics effect classes
    //!
    namespace effect
    {
        //!
        //! Texture descriptor
        //!
        struct TextureDesc
        {
        };

        //!
        //! Uniform descriptor
        //!
        struct UniformDesc
        {
            bool hasDefaultValue;      //!< Indicates that the uniform has a default value.
            UniformValue defaultValue; //!< Default value of the uniform. Ignored if hasDefaultValue is false.
        };

        //!
        //! Texture reference descriptor
        //!
        struct TextureRefDesc
        {
            StrA     name;  //!< Texture name
            uint32_t stage; //!< Texture/Sampler Stage index
        };

        //!
        //! Uniform reference descriptor
        //!
        struct UniformRefDesc
        {
            StrA name;    //!< Uniform name
            StrA binding; //!< binding parameter of the uniform. Could be:
                          //!< - register name (for asm shader),
                          //!< - variable name (for high-level shader), or
                          //!< - fixed pipleine parameter name (such as "TransformWorld").
        };

        //!
        //! Shader descriptor
        //!
        struct ShaderDesc
        {
            ShaderType type; //!< Shader type.
            ShadingLanguage lang; //!< Shading language. Ignored if code is empty.
            StrA code; //!< Shader code. Empty means fixed functional pipeline.
            StrA entry; //!< Entry function of the code. Ignored, if code is empty.
            std::vector<TextureRefDesc> textures; //!< textures used by the shader.
            std::vector<UniformRefDesc> uniforms; //!< uniforms used by the shader.
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
            PassDesc() : rsb(RenderStateBlockDesc::INVALID) {}
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
            TechniqueDesc() : rsb(RenderStateBlockDesc::INVALID) {}
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
                rsb.reset( RenderStateBlockDesc::RESET_TO_DEFAULT );
            }

            //!
            //! Get uniform by name. Name must be a valid uniform name.
            //!
            const UniformDesc & getUniform( const StrA & name ) const
            {
                std::map<StrA,UniformDesc>::const_iterator i = uniforms.find( name );
                GN_ASSERT( uniforms.end() != i );
                return i->second;
            }

            //!
            //! Get shader by name. Name must be a valid shader name.
            //!
            const ShaderDesc & getShader( const StrA & name ) const
            {
                std::map<StrA,ShaderDesc>::const_iterator i = shaders.find( name );
                GN_ASSERT( shaders.end() != i );
                return i->second;
            }
        };

        //!
        //! Define geometry data used by effect class
        //!
        struct GeometryData
        {
            AutoRef<VtxBuf> vtxBufs[MAX_VERTEX_STREAMS]; //!< vertex buffer list.
            uint32_t        numVtxBufs; //!< vertex buffer count
            AutoRef<IdxBuf> idxBuf; //!< index buffer
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
            bool init( Renderer &, const EffectDesc & ); //!< initialize from descriptor
            bool init( const Effect & ); //!< Make effect clone
            void quit();
            bool ok() const { return MyParent::ok(); }
        private:
            void clear() { mRenderer = 0; }
            //@}

            // ********************************
            // public functions
            // ********************************
        public:

            //!
            //! Set active technique.
            //! \param name Technique name. NULL means default technique.
            //!
            void setActiveTechnique( const char * name );

            //!
            //! Do the rendering.
            //!
            void render( const GeometryData * geometryDataArray, size_t count ) const;

            // ********************************
            // uniform management
            // ********************************
        public:

            //!
            //! Get handle of uniform variable
            //!
            uint32_t getUniformHandle( const char * ) const;

            //!
            //! \name Set value of uniform variable.
            //!
            //@{
            void setUniformB( uint32_t, const int32_t *, size_t );
            void setUniformI( uint32_t, const int32_t *, size_t );
            void setUniformF( uint32_t, const float *, size_t );
            void setUniformV( uint32_t, const Vector4f *, size_t );
            void setUniformM( uint32_t, const Matrix44f *, size_t ); //!< \note Matrix should be row major

            void setUniformB( uint32_t, bool );
            void setUniformI( uint32_t, int32_t );
            void setUniformF( uint32_t, float );
            void setUniformV( uint32_t, const Vector4f & );
            void setUniformM( uint32_t, const Matrix44f & ); //!< \note Matrix should be row major
            //@}

            //!
            //! \name Set uniform variable by name
            //!
            //@{
            void setUniformByNameB( const char *, const int32_t *, size_t );
            void setUniformByNameI( const char *, const int32_t *, size_t );
            void setUniformByNameF( const char *, const float *, size_t );
            void setUniformByNameV( const char *, const Vector4f *, size_t );
            void setUniformByNameM( const char *, const Matrix44f *, size_t );

            void setUniformByNameB( const char *, bool );
            void setUniformByNameI( const char *, int32_t );
            void setUniformByNameF( const char *, float );
            void setUniformByNameV( const char *, const Vector4f & );
            void setUniformByNameM( const char *, const Matrix44f & );
            //@}

            // ********************************
            // texture management
            // ********************************
        public:

            //@{

            //!
            //! get texture handle
            //!
            uint32_t getTextureHandle( const char * ) const;

            //!
            //! set texture value
            //!
            void setTexture( uint32_t, const Texture * );

            //!
            //! set texture value by name
            //!
            void setTextureByName( const char *, const Texture * );

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
                StrA             name;
                AutoRef<Texture> value;
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
                bool                        isTextureStates;
                UniformValue                value;
                TextureStateBlockDesc       textureStates;
                std::vector<ShaderRefData>  shaders; // shaders that are referencing this uniform.
            };

            struct UniformRefData
            {
                uint32_t handle; //!< handle to mUniforms
                bool     ffp;    //!< is it binding to fixed functional pipeline?
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
                std::vector<uint32_t>       dirtyUniforms; // dirty uniform list. Each item is a index into the shader's uniform list.
            };

            struct PassData
            {
                uint32_t shaders[NUM_SHADER_TYPES]; //!< shader handles of this pass.
                uint32_t rsb; //!< render state block handle.
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

            Renderer * mRenderer;
            EffectDesc mDesc;

            NamedItemManager<TextureData>   mTextures;
            NamedItemManager<UniformData>   mUniforms;
            NamedItemManager<ShaderData>    mShaders;
            NamedItemManager<TechniqueData> mTechniques;
            uint32_t                        mActiveTechnique;

            // ********************************
            // private functions
            // ********************************
        private:

            bool createEffect(); // called by init()
            bool initTechnique( uint32_t handle ) const; // initialize specific technique.

            static bool sCheckFfpParameterType( const StrA & name, FfpParameterType & type );
            static void sSetFfpParameter( Renderer &, FfpParameterType, const UniformData & );
        };
    }
}}

#if GN_ENABLE_INLINE
#include "effect.h"
#endif

// *****************************************************************************
//                           End of effect.h
// *****************************************************************************
#endif // __GN_GFX_EFFECT_H__
