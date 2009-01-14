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
            RendererCaps::GpuProgramProfile vsprofiles;  ///< required VS profiles.
            RendererCaps::GpuProgramProfile gsprofiles;  ///< required GS profiles.
            RendererCaps::GpuProgramProfile psprofiles;  ///< required PS profiles.
            UInt8                           numtextures; ///< minimal number of textures.
        };

        ///
        /// Texture descriptor
        ///
        struct TextureDesc
        {
            TextureSampler sampler;
            StrA           filename; ///< texture filename. Could be empty.
        };

        ///
        /// Uniform descriptor
        ///
        struct UniformDesc
        {
            DynaArray<UInt8> defval; ///< default uniform value. Could be empty if there's no default value.
            bool             shared; ///< shared uniform across effects.
        };

        ///
        /// Shader descriptor
        ///
        struct ShaderDesc
        {
            GpuProgramDesc      gpd;           ///< GPU Program descriptor
            std::map<StrA,StrA> vertices;      ///< vertices. Key is vertex element binding, value is vertex element name.
            std::map<StrA,StrA> textures;      ///< textures. Key is texture binding, value is user-visible texture name.
            std::map<StrA,StrA> uniforms;      ///< uniforms. Key is uniform binding, value is user-visible uniform name.
            ShaderPrerequisites prerequisites; ///< prerequisites of the shader.
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
            }

            /// set render state value
            RenderState & operator=( const RenderState<T> & rhs )
            {
                value = rhs.value;
                inherited = rhs.inherited;
            }
        };

        /// render state block
        struct RenderStateDesc
        {
            RenderState<UInt8> fillMode;
            RenderState<UInt8> cullMode;
            RenderState<UInt8> frontFace;
            RenderState<bool>  msaaEnabled;
            RenderState<bool>  depthTestEnabled;
            RenderState<bool>  depthWriteEnabled;
            RenderState<UInt8> depthFunc;

            // TODO: more render states.

            void clear()
            {
                fillMode.inherited = true;
                cullMode.inherited = true;
                frontFace.inherited = true;
                msaaEnabled.inherited = true;
                depthTestEnabled.inherited = true;
                depthWriteEnabled.inherited = true;
                depthFunc.inherited = true;
            }
        };

        ///
        /// Rendering pass descriptor
        ///
        struct PassDesc
        {
            StrA            gpuProgram; ///< Name of GPU program used in this pass. Can't be empty
            RenderStateDesc rsd;        ///< pass specific render states
        };

        ///
        /// Technique descriptor structure
        ///
        struct TechniqueDesc
        {
            StrA                     name;   ///< technique name
            DynaArray<PassDesc>      passes; ///< pass list.
            RenderStateDesc          rsd;    ///< Technique specific render states
        };

        std::map<StrA,TextureDesc> textures;   ///< Texture list
        std::map<StrA,UniformDesc> uniforms;   ///< Uniform list
        std::map<StrA,ShaderDesc>  shaders;    ///< Shader list
        DynaArray<TechniqueDesc>   techniques; ///< Technique list. Technique name must be unique.
        RenderStateDesc            rsd;        ///< effect specific render states

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
            rsd.clear();
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
        ///
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
        Effect( Renderer & r ) : mRenderer(r) { clear(); }
        virtual ~Effect() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const EffectDesc & );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        /// apply effect to renderer context
        void applyToRendererContext( RendererContext & rc ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        struct Pass
        {
            UInt16            gpuProgram;                              ///< index into GPU program array
            size_t            textures[RendererContext::MAX_TEXTURES]; ///< indices into texture array
            DynaArray<UInt16> uniforms;                                ///< indices into uniform array. Array length is the GPU program uniform number.
        };

        struct Technique
        {
            DynaArray<Pass> passes;
        };

        Renderer & mRenderer;
        EffectDesc mDesc;

        DynaArray<AutoRef<Texture> >         mTextures;
        DynaArray<AutoRef<GpuProgramParam> > mUniforms;
        DynaArray<AutoRef<GpuProgram> >      mGpuPrograms;
        DynaArray<Technique>                 mTechniques;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_EFFECT_H__
