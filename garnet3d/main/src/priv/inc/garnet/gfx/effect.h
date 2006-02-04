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
        enum
        {
            MAX_PASSES = 16 //!< Max number of rendering passes
        };

        //!
        //! Texture descriptor
        //!
        struct TextureDesc
        {
            StrA name; //!< texture name
        };

        //!
        //! Uniform descriptor
        //!
        struct UniformDesc
        {
            StrA name; //!< uniform name
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
            StrA name; //!< Shader name. Should be unique in one effect.
            StrA code; //!< Shader code. If code string is empty, it means fixed pipeline.
                       //!< Shader type and shading language will be ignored in this case.
            ShaderType type; //!< Shader type
            ShadingLanguage lang; //!< Shading language
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
        };

        //!
        //! Technique descriptor structure
        //!
        struct TechniqueDesc
        {
            PassDesc             passes[MAX_PASSES]; //!< pass list.
            uint32_t             numPasses; //!< pass count
            RenderStateBlockDesc rsb; //!< Technique specific render states
        };

        //!
        //! Graphics effect descriptor
        //!
        struct EffectDesc
        {
            std::vector<ShaderDesc>    shaders;    //!< Shader list
            std::vector<TextureDesc>   textures;   //!< Texture list
            std::vector<UniformDesc>   uniforms;   //!< Uniform list
            std::vector<TechniqueDesc> techniques; //!< Technique list
            RenderStateBlockDesc       rsb;        //!< Effect global render states

            //!
            //! Make sure the effect descriptor is valid.
            //!
            bool valid() const;
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
            bool init( Renderer & r, const EffectDesc & ); //!< initialize from descriptor
            bool init( const Effect & ); //!< Make effect clone
            void quit();
            bool ok() const { return MyParent::ok(); }
        private:
            void clear() {}
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
            void render( const GeometryData * geometryDataArray, size_t count );

            // ********************************
            // uniform management
            // ********************************
        public:

            //!
            //! Get handle of uniform variable
            //!
            uint32_t getUniformHandle( const char * );

            //!
            //! \name Set value of uniform variable.
            //!
            //@{
            void setUniform( uint32_t, const bool *, size_t );
            void setUniform( uint32_t, const int32_t *, size_t );
            void setUniform( uint32_t, const float *, size_t );
            void setUniform( uint32_t, const Vector4f *, size_t );
            void setUniform( uint32_t, const Matrix44f *, size_t ); //!< \note Matrix should be row major

            void setUniform( uint32_t, bool );
            void setUniform( uint32_t, int32_t );
            void setUniform( uint32_t, float );
            void setUniform( uint32_t, const Vector4f & );
            void setUniform( uint32_t, const Matrix44f & ); //!< \note Matrix should be row major
            //@}

            //!
            //! \name Set uniform variable by name
            //!
            //@{
            void setUniformByName( const char *, const bool *, size_t );
            void setUniformByName( const char *, const int32_t *, size_t );
            void setUniformByName( const char *, const float *, size_t );
            void setUniformByName( const char *, const Vector4f *, size_t );
            void setUniformByName( const char *, const Matrix44f *, size_t );

            void setUniformByName( const char *, bool );
            void setUniformByName( const char *, int32_t );
            void setUniformByName( const char *, float );
            void setUniformByName( const char *, const Vector4f & );
            void setUniformByName( const char *, const Matrix44f & );
            //@}

            // ********************************
            // texture management
            // ********************************
        public:

            //@{

            //!
            //! get texture handle
            //!
            uint32_t getTextureHandle( const char * );

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

            // ********************************
            // private functions
            // ********************************
        private:
        };
    }
}}

// *****************************************************************************
//                           End of effect.h
// *****************************************************************************
#endif // __GN_GFX_EFFECT_H__
