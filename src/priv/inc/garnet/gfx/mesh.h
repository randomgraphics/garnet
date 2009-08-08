#ifndef __GN_GFX_MESH_H__
#define __GN_GFX_MESH_H__
// *****************************************************************************
/// \file
/// \brief   Graphics mesh class
/// \author  chenli@@REDMOND (2009.1.14)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// mesh descriptor
    ///
    struct MeshDesc
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
        MeshDesc()
        {
            memset( this, 0, sizeof(*this) );
        };
    };

    /// define a subset of the mesh
    struct MeshSubset
    {
        size_t startvtx;
        size_t numvtx;
        size_t startidx;
        size_t numidx;
    };

    ///
    /// Graphics mesh class
    ///
    class Mesh : public StdClass, public NoCopy
    {
        GN_DECLARE_STDCLASS( Mesh, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        Mesh( Gpu & r ) : mGpu(r) { clear(); }
        virtual ~Mesh() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const MeshDesc & desc );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        /// get mesh descriptor
        const MeshDesc & getDesc() const { return mDesc; }

        ///
        /// apply mesh to drawable
        ///
        void applyToDrawable( Drawable & drawable, const MeshSubset * subset = NULL ) const;

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

        Gpu   & mGpu;
        MeshDesc     mDesc;
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
