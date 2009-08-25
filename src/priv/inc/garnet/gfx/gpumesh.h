#ifndef __GN_GFX_MESH_H__
#define __GN_GFX_MESH_H__
// *****************************************************************************
/// \file
/// \brief   GPU mesh class
/// \author  chenli@@REDMOND (2009.1.14)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// mesh descriptor
    ///
    struct GpuMeshDesc
    {
        VertexFormat  vtxfmt; ///< vertex format
        PrimitiveType prim;   ///< primitive type
        bool          idx32;  ///< true for 32-bit index buffer
        bool          dynavb; ///< true for dynamic vertex buffer
        bool          dynaib; ///< trur for dynamic index buffer
        size_t        numvtx; ///< number of vertices
        size_t        numidx; ///< number of indices. 0 means non-indexed mesh
        size_t        strides[GpuContext::MAX_VERTEX_BUFFERS];  // vertex buffer strides. 0 means using minimal stride defined by vertex format.
        const void *  vertices[GpuContext::MAX_VERTEX_BUFFERS]; // NULL pointer means vertex data undefined
        const void *  indices; // Null means index data undefined.

        /// default ctor
        GpuMeshDesc()
        {
            memset( this, 0, sizeof(*this) );
        };
    };

    /// define a subset of the GPU mesh. (0,0,0,0) means the whole mesh.
    struct GpuMeshSubset
    {
        size_t startvtx;
        size_t numvtx;
        size_t startidx;
        size_t numidx;
    };

    ///
    /// Graphics mesh class
    ///
    class GpuMesh : public StdClass, public NoCopy
    {
        GN_DECLARE_STDCLASS( GpuMesh, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        GpuMesh( Gpu & r ) : mGpu(r) { clear(); }
        virtual ~GpuMesh() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const GpuMeshDesc & desc );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        /// get mesh descriptor
        const GpuMeshDesc & getDesc() const { return mDesc; }

        ///
        /// apply mesh to drawable
        ///
        void applyToDrawable( Drawable & drawable, const GpuMeshSubset * subset = NULL ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        struct VertexBuffer
        {
            AutoRef<VtxBuf>  gpudata;
            UInt16           stride;
        };

        struct IndexBuffer
        {
            AutoRef<IdxBuf>  gpudata;
        };

        Gpu        & mGpu;
        GpuMeshDesc  mDesc;
        VertexBuffer mVtxBufs[GpuContext::MAX_VERTEX_BUFFERS];
        IndexBuffer  mIdxBuf;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_MESH_H__
