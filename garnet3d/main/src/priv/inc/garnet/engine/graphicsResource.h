#ifndef __GN_ENGINE_GRAPHICSRESOURCE_H__
#define __GN_ENGINE_GRAPHICSRESOURCE_H__
// *****************************************************************************
/// \file
/// \brief   interface of graphics resource
/// \author  chenli@@FAREAST (2007.4.27)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// fake const buffer class
    ///
    struct ConstBuf : public RefCounter
    {
    };
}}

namespace GN { namespace engine
{
    ///
    /// shader resourece descriptor
    ///
    struct ShaderDesc
    {
        gfx::ShaderType      type;   ///< shader type
        gfx::ShadingLanguage lang;   ///< shading language
        StrA                 code;   ///< shader code
        StrA                 hints;  ///< shader creation hints
    };

    ///
    /// coming soon ...
    ///
    struct ConstBufDesc
    {
        //@{
        int type;  ///< vector4, int4, bool, matrix44
        int count; ///< number of items, not bytes.
        //@}
    };

    ///
    /// graphics resource type
    ///
    enum GraphicsResourceType
    {
        //@{
        GRT_SHADER,
        GRT_CONSTBUF,
        GRT_VTXFMT,
        GRT_VTXBUF,
        GRT_IDXBUF,
        GRT_TEXTURE,
        GRT_SAMPLER,
        NUM_GRAPHICS_RESOURCE_TYPES,
        GRT_INVALID = NUM_GRAPHICS_RESOURCE_TYPES,
        //@}
    };

    class RenderEngine;

    ///
    /// graphics resource descriptor
    ///
    struct GraphicsResourceDesc
    {
        //@{
        StrA                 name; ///< this field is for log and debug. you may set it to any value.
        GraphicsResourceType type;
        ShaderDesc           sd;
        gfx::VtxFmtDesc      fd;
        gfx::VtxBufDesc      vd;
        gfx::IdxBufDesc      id;
        ConstBufDesc         cd;
        gfx::TextureDesc     td;
        gfx::SamplerDesc     sampler;
        DynaArray<UInt8>     userdata; ///< unchangable during life time of the resource.
        //@}

        StrA toString() const; ///< convert to string
    };

    ///
    /// Graphics resource class.
    ///
    /// \note
    ///     Do not read/write data member of this class, unless in
    ///     GraphicsResourceLoader class.
    ///
    struct GraphicsResource : public NoCopy
    {
        RenderEngine             & engine; ///< reference to the engine that created this resource.
        const GraphicsResourceDesc desc;   ///< resource descriptor
        union
        {
            //@{
            void            * data;     ///< coming soon ...
            gfx::Shader     * shader;   ///< coming soon ...
            gfx::Texture    * texture;  ///< coming soon ...
            gfx::VtxBuf     * vtxbuf;   ///< coming soon ...
            gfx::IdxBuf     * idxbuf;   ///< coming soon ...
            gfx::ConstBuf   * constbuf; ///< coming soon ...
            gfx::VtxFmtHandle vtxfmt;   ///< coming soon ...
            //@}
        };

    protected:

        ///
        /// protected constructor
        ///
        GraphicsResource( RenderEngine & engine_, const GraphicsResourceDesc & desc_ )
            : engine(engine_), desc(desc_), data(0)
        {}

        ///
        /// protected destructor
        ///
        ~GraphicsResource() {}
    };

    ///
    /// Application defined graphics resource loader.
    ///
    /// Details about concurrency:
    ///  - load() won't be called concurrently with itself, but might be called concurrently with other methods
    ///  - copy() won't be called concurrently with itself, but might be called concurrently with other methods
    ///  - decompress() and freebuf() could be called concurrently with any methods.
    ///
    /// So, to achieve maximum performance, it is advised to avoid using sync objects as much as possible.
    /// The possible implementation could be:
    ///  - keep data used by each method separated. So they won't mess with each other, when called concurrently.
    ///  - do not modify any states in decompress(). So it can be safely called anytime anywhere, w/o using sync objects.
    ///
    struct GraphicsResourceLoader : public RefCounter
    {
        ///
        /// load from external/slow storage (disk, cdrom, network)
        ///
        virtual bool load( const GraphicsResourceDesc & desc, void * & outbuf, size_t & outbytes, int lod ) = 0;

        ///
        /// decompress or do other process to prepare for copy to graphics resource.
        ///
        virtual bool decompress( const GraphicsResourceDesc & desc, void * & outbuf, size_t & outbytes, const void * inbuf, size_t inbytes, int lod ) = 0;

        ///
        /// copy data to graphics resource
        ///
        virtual bool copy( GraphicsResource & res, const void * inbuf, size_t inbytes, int lod ) = 0;

        ///
        /// free data buffer returned by load() and decompress()
        ///
        virtual void freebuf( void * inbuf, size_t inbytes ) = 0;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_GRAPHICSRESOURCE_H__
