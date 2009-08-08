#ifndef __GN_SCENE_RESMGR_H__
#define __GN_SCENE_RESMGR_H__
// *****************************************************************************
/// \file
/// \brief   Resource manager used in scene model
/// \author  chenli@@REDMOND (2009.7.13)
// *****************************************************************************

namespace GN { namespace scene
{
    struct ModelDesc
    {
        struct TextureDesc
        {
            StrA             filename; /// if empty, then use the descriptor
            gfx::TextureDesc desc;
        };

        struct UniformDesc
        {
            DynaArray<UInt8> defaultValue; ///< if empty, then no default value.
        };

        struct EffectDesc
        {
            StrA                  filename;        // effect file name
            std::map<StrA,size_t> textureBindings; // bind texture (value) to effect parameter (key)
            std::map<StrA,size_t> uniformBindings; // bind uniform (value) to effect parameter (key)
        };

        struct GpuMeshDesc
        {
            StrA          filename; ///< if empty, then use the descriptor
            gfx::GpuMeshDesc desc;
        };

        EffectDesc                 effect;
        std::map<StrA,TextureDesc> textures;
        std::map<StrA,UniformDesc> uniforms;

        GpuMeshDesc                   mesh;
        size_t                     startvtx;
        size_t                     numvtx;
        size_t                     startidx;
        size_t                     numidx;
    };

    ///
    /// graphice resource handle type
    ///
    typedef UInt32 GraphicsResourceHandle;

    class GraphicsResourceDatabase;

    ///
    /// Model, a glue class for effect, mesh and textures.
    ///
    class Model : public StdClass
    {
        GN_DECLARE_STDCLASS( Model, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    protected:
        Model( GraphicsResourceDatabase & gdb ) : mDatabase(gdb) { clear(); }
        virtual ~Model() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const ModelDesc & desc );
        void quit();
    private:
        void clear() { mDatabase = NULL; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        // ********************************
        // private variables
        // ********************************
    private:

        GraphicsResourceDatabase        & mDatabase;
        ModelDesc                         mDesc;
        GraphicsResourceHandle            mEffect;
        DynaArray<GraphicsResourceHandle> mTextures;
        DynaArray<GraphicsResourceHandle> mUniforms;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    ///
    /// Map name/handle to graphics resource instance.
    ///
    class GraphicsResourceDatabase
    {
    public:

        //@{
        GraphicsResourceDatabase( gfx::Renderer & r );
        virtual ~GraphicsResourceDatabase();
        //@}

        //@{
        GraphicsResourceHandle  openTextureHandle( const StrA & name );
        GraphicsResourceHandle  openUniformHandle( const StrA & name );
        GraphicsResourceHandle  openEffectHandle( const StrA & name );
        GraphicsResourceHandle  openMeshHandle( const StrA & name );
        GraphicsResourceHandle  openModelHandle( const StrA & name );

        void                    closeHandle( GraphicsResourceHandle );
        void                    closeAllHandles();

        AutoRef<gfx::Texture> & handle2Texture( GraphicsResourceHandle );
        AutoRef<gfx::Uniform> & handle2Uniform( GraphicsResourceHandle );
        gfx::Effect           & handle2Effect( GraphicsResourceHandle );
        gfx::GpuMesh          & handle2Mesh( GraphicsResourceHandle );
        gfx::Model            & handle2Model( GraphicsResourceHandle );
        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_RESMGR_H__
