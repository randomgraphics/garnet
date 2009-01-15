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
        size_t        numidx; ///< number of indices
        size_t        strides[RendererContext::MAX_VERTEX_BUFFERS];  // vertex buffer strides. 0 means using minimal stride defined by vertex format.
        const void *  vertices[RendererContext::MAX_VERTEX_BUFFERS]; // NULL pointer means vertex data undefined
        const void *  indices; // null means index data undefined.

        /// default ctor
        MeshDesc()
        {
            memset( this, 0, sizeof(*this) );
        };
    };

    ///
    /// Graphics mesh class
    ///
    class Mesh : public StdClass
    {
        GN_DECLARE_STDCLASS( Mesh, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        Mesh( Renderer & r ) : mRenderer(r) { clear(); }
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

        /// release CPU mesh data, keep GPU mesh data unchanged.
        void releaseCpuData();

        /// calculate vertex normal
        void calculateNormal( const StrA & positionSemantic = "position" );

        /// calculate tagent and binormal vectors
        void calculateTengentSpaceVector( const StrA & positionSemantic = "position", const StrA & texcoordSemantic = "tex0" );

        // ********************************
        // private variables
        // ********************************
    private:

        struct VertexBuffer
        {
            DynaArray<UInt8> cpudata;
            AutoRef<VtxBuf>  gpudata;
        };

        struct IndexBuffer
        {
            DynaArray<UInt8> cpudata;
            AutoRef<IdxBuf>  gpudata;
        };

        Renderer   & mRenderer;
        MeshDesc     mDesc;
        VertexBuffer mVtxBufs[RendererContext::MAX_VERTEX_BUFFERS];
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
