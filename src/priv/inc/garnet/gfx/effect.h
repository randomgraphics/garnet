#ifndef __GN_GFX_EFFECT_H__
#define __GN_GFX_EFFECT_H__
// *****************************************************************************
/// \file
/// \brief   Graphics effect
/// \author  chenli@@REDMOND (2009.1.13)
// *****************************************************************************

namespace GN { namespace gfx
{
    struct EffectDesc
    {
        ///
        /// Shader Prerequisites
        ///
        struct ShaderPrerequisites
        {
            UInt8 numTextures; ///< minimal number of textures required.

            /// default constructor
            ShaderPrerequisites()
                : numTextures(0)
            {
            }
        };

        ///
        /// Texture descriptor
        ///
        struct TextureDesc
        {
            /// sampler descriptor
            SamplerDesc sampler;

            /// default ctor
            TextureDesc()
            {
                sampler.clear();
            }
        };

        ///
        /// Uniform descriptor
        ///
        struct UniformDesc
        {
            size_t           size;   ///< uniform size
            DynaArray<UInt8> defval; ///< default uniform value. Could be empty if there's no default value.

            /// default ctor
            UniformDesc() : size(0) {}
        };

        ///
        /// Effect render target descriptor
        ///
        struct RenderTargetDesc
        {
            // TODO: minimal size, format and usage requirements, and etc.
        };

        ///
        /// Shader descriptor
        ///
        struct ShaderDesc
        {
            GpuProgramDesc      gpd;                ///< GPU Program descriptor
            std::map<StrA,StrA> vertices;           ///< vertices. Key is vertex element binding, value is vertex element name.
            std::map<StrA,StrA> textures;           ///< textures. Key is texture binding, value is user-visible texture name defined in EffectDesc::textures.
            std::map<StrA,StrA> uniforms;           ///< uniforms. Key is uniform binding, value is user-visible uniform name defined in EffectDesc::uniforms.
            ShaderPrerequisites prerequisites;      ///< prerequisites of the shader.
        };

        /// template for single render state
        template<typename T>
        struct RenderState
        {
            T    value;
            bool inherited; ///< if true, then this effect will inherit this value from current renderer context.

            /// default ctor
            RenderState() : inherited(true) {}

            /// set render state value
            RenderState & operator=( const T & rhs )
            {
                value = rhs;
                inherited = false;
                return *this;
            }

            /// set render state value
            RenderState & operator=( const RenderState<T> & rhs )
            {
                value = rhs.value;
                inherited = rhs.inherited;
                return *this;
            }
        };

        /// render state block
        struct RenderStateDesc
        {
            RenderState<UInt8> fillMode;
            RenderState<UInt8> cullMode;
            RenderState<UInt8> frontFace;
            RenderState<bool>  msaaEnabled;
            RenderState<bool>  depthTest;
            RenderState<bool>  depthWrite;
            RenderState<UInt8> depthFunc;

            // TODO: more render states.

            void clear()
            {
                fillMode.inherited = true;
                cullMode.inherited = true;
                frontFace.inherited = true;
                msaaEnabled.inherited = true;
                depthTest.inherited = true;
                depthWrite.inherited = true;
                depthFunc.inherited = true;
            }
        };

        ///
        /// Rendering pass descriptor
        ///
        struct PassDesc
        {
            StrA            shader;       ///< Name of shader used in this pass. Can't be empty
            RenderStateDesc rsd;          ///< pass specific render states
            DynaArray<StrA> colortargets; ///< color render targets. Values are user-visible render target names defined in EffectDesc::rendertargets.
            StrA            depthstencil; ///< depth render targets. Value is user-visible render target name defined in EffectDesc::rendertargets.

            PassDesc()
            {
                colortargets.resize( 1 );
                colortargets[0] = "color0";
                depthstencil = "depth";
            }
        };

        ///
        /// Technique descriptor structure
        ///
        struct TechniqueDesc
        {
            int                      quality; ///< user defined rendering quality. Effect class uses
                                              ///< the technique, among available/valid techniques,
                                              ///< with the hightest quality as default active technique.
            DynaArray<PassDesc>      passes;  ///< pass list.
            RenderStateDesc          rsd;     ///< Technique specific render states

            /// default ctor
            TechniqueDesc() : quality(100) {}
        };

        std::map<StrA,TextureDesc>      textures;      ///< Texture list
        std::map<StrA,UniformDesc>      uniforms;      ///< Uniform list
        std::map<StrA,RenderTargetDesc> rendertargets; ///< Render taret list. Empty means using default setttings: one "color0", one "depth".
        std::map<StrA,ShaderDesc>       shaders;       ///< Shader list
        std::map<StrA,TechniqueDesc>    techniques;    ///< Technique list. Technique name must be unique.
        RenderStateDesc                 rsd;           ///< effect specific render states

        ///
        /// constructor
        ///
        EffectDesc()
        {
            clear();
        }

        ///
        /// Make sure the effect descriptor is valid.
        ///
        bool valid() const;

        ///
        /// clear to a default descriptor
        ///
        void clear()
        {
            textures.clear();
            uniforms.clear();
            rendertargets.clear();
            rendertargets["color0"];
            rendertargets["depth"];
            shaders.clear();
            techniques.clear();
            rsd.clear();

        }

        ///
        /// setup the descriptor from XML
        ///
        bool loadFromXmlNode( const XmlNode & root, const StrA & basedir );

        ///
        /// write the descriptor to XML
        ///
        void saveToXmlNode( const XmlNode & root );
    };

    ///
    /// Graphics effect
    ///
    class Effect : public StdClass
    {
        GN_DECLARE_STDCLASS( Effect, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        Effect( Renderer & r );
        Effect( const Effect & ); // copy ctor
        virtual ~Effect();
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const EffectDesc & desc, const StrA & activeTechName = "" );
        void quit();
    private:
        void clear() { mActiveTech = NULL; }
        //@}

        // ********************************
        // public data type
        // ********************************
    public:

        ///
        /// Effect parameter collection template
        ///
        template<typename T>
        class EffectParamCollection : public NoCopy
        {
            typedef std::map<StrA,T>                          MapType;
            typedef typename std::map<StrA,T>::iterator       Iterator;
            typedef typename std::map<StrA,T>::const_iterator ConstIter;
            typedef Delegate0<T&>                             GetDummyFunc;

            MapType     * mMap;
            GetDummyFunc  mGetDummyFunc;

            friend class Effect; // effect class need to update the data members.

        public:

            /// check if the collection has item with specific name
            bool contains( const StrA & name ) const
            {
                GN_ASSERT( mMap );
                return mMap->end() != mMap->find( name );
            }

            /// constant bracket operator
            const T & operator[]( const StrA & name ) const
            {
                GN_ASSERT( mMap );

                ConstIter it = mMap->find( name );

                if( mMap->end() == it )
                {
                    return mGetDummyFunc();
                }
                else
                {
                    return it->second;
                }
            }

            /// non-const bracket operator
            T & operator[]( const StrA & name )
            {
                GN_ASSERT( mMap );

                Iterator it = mMap->find( name );

                if( mMap->end() == it )
                {
                    return mGetDummyFunc();
                }
                else
                {
                    return it->second;
                }
            }
        };

        // ********************************
        // public property
        // ********************************
    public:

        ///
        /// uniform collection
        ///
        EffectParamCollection<AutoRef<Uniform> > uniforms;

        ///
        /// texture collection
        ///
        EffectParamCollection<AutoRef<Texture> > textures;

        ///
        /// render target collection
        ///
        EffectParamCollection<RenderTargetTexture> rendertargets;

        // ********************************
        // public functions
        // ********************************
    public:

        /// get descriptor
        const EffectDesc & getDesc() const { return mDesc; }

        /// get number of passes
        size_t getNumPasses() const { return mActiveTech->passes.size(); }

        /// Apply the effect to drawable.
        ///
        /// After this function call, the drawable will share same uniforms and
        /// textures with the effect. Which means that any changings to the
        /// value of the current uniforms and/or content of the textuers will
        /// affect the drawable as well, until new uniforms and/or textures are
        /// assigned to the effect (though EffectParamCollection interface).
        bool applyToDrawable( Drawable & drawable, size_t pass ) const;

        /// copy operator (make clone)
        ///
        /// Note that this method only clones the effect itself. Resources referenced by the effect,
        /// like uniforms and textures, will be shared by the original effect and the clone, which means that:
        ///     - if you change content of the uniform or the texture, both the original effect and the clone are affected.
        ///     - if you change the original effect itself (like point diffuse texture parameter to another texture),
        ///       the clone won't be affected.
        Effect & operator=( const Effect & rhs ) { if( this != &rhs ) { clone( rhs ); } return *this; }

        // ********************************
        // private variables
        // ********************************
    private:

        typedef std::map<StrA,AutoRef<Uniform> >::iterator   UniformIter;
        typedef std::map<StrA,AutoRef<Texture> >::iterator   TextureIter;
        typedef std::map<StrA,RenderTargetTexture>::iterator RenderTargetIter;

        struct PerShaderTextureParam
        {
            /// texture parameter iterator into global texture map (Effect::mTextures)
            TextureIter iter;

            /// pointer to sampler, pointer to Effec
            const SamplerDesc * sampler;

            /// default ctor
            PerShaderTextureParam()
            {
            }

            /// copy ctor
            PerShaderTextureParam( const PerShaderTextureParam & p )
                : iter(p.iter)
                , sampler(p.sampler)
            {
            }

            /// assign operator
            PerShaderTextureParam & operator=( const PerShaderTextureParam & rhs )
            {
                iter    = rhs.iter;
                sampler = rhs.sampler;
                return *this;
            }
        };

        struct Pass
        {
            GpuProgram                     * gpuProgram;   ///< Pointer to the GPU program
            DynaArray<PerShaderTextureParam> textures;     ///< Textures used in the pass. Note that
                                                           ///< the index of the texture in this array
                                                           ///< is the binding index to the GPU
                                                           ///< program used in this pass.
            DynaArray<UniformIter>           uniforms;     ///< uniforms used in the pass. Note that
                                                           ///< the index of the uniform in this array
                                                           ///< is the binding index to the GPU
                                                           ///< program used in this pass.
            DynaArray<RenderTargetIter>      colortargets; ///< color render targets used in this pass
            RenderTargetIter                 depthstencil; ///< depth render target used in this pass
            EffectDesc::RenderStateDesc rsd;               ///< render states
        };

        struct Technique
        {
            DynaArray<Pass> passes;
        };

        Renderer & mRenderer;
        EffectDesc mDesc;

        std::map<StrA,AutoRef<Uniform> >      mUniforms;
        std::map<StrA,AutoRef<Texture> >      mTextures;
        std::map<StrA,RenderTargetTexture>    mRenderTargets;
        std::map<StrA,AutoRef<GpuProgram> >   mGpuPrograms;
        std::map<StrA,Technique>              mTechniques;
        Technique *                           mActiveTech;

        /// dummy uniform
        AutoRef<Uniform> mDummyUniform;

        /// dummy texture
        AutoRef<Texture> mDummyTexture;

        /// dummy render target
        RenderTargetTexture mDummyRenderTargetTexture;

        // ********************************
        // private functions
        // ********************************
    private:

        /// initialize technique
        bool initTech( Technique & tech, const StrA & name, const EffectDesc::TechniqueDesc & desc );

        /// make clone
        void clone( const Effect & );

        /// get dummy uniform
        AutoRef<Uniform> & getDummyUniform();

        /// get dummy texture
        AutoRef<Texture> & getDummyTexture();

        /// get dummy render target
        RenderTargetTexture & getDummyRenderTargetTexture() { return mDummyRenderTargetTexture; }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_EFFECT_H__
