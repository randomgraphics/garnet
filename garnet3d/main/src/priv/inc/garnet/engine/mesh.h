#ifndef __GN_ENGINE_MESH_H__
#define __GN_ENGINE_MESH_H__
// *****************************************************************************
//! \file    engine/mesh.h
//! \brief   mesh entity class
//! \author  chenli@@FAREAST (2007.5.8)
// *****************************************************************************

namespace GN { namespace engine
{
    ///
    /// garnet binary file header
    ///
    struct BinaryFileHeader
    {
        char   tag[2];   ///< must be 'G','N'
        UInt16 endian;   ///< endian type: 0x0201 means little endian, else, big endian
        UInt32 reserved; ///< reserved.
        UInt64 bytes;    ///< file size in bytes, not including this header.
        char   name[16]; ///< up to 16 characters to idenity chunk name
    };
    GN_CASSERT( sizeof(BinaryFileHeader) == 32 );

    ///
    /// mesh binary header
    ///
    struct MeshBinaryHeader
    {
        ///
        /// mesh vertex buffer header
        ///
        struct VtxBufHeader
        {
            //@{
            UInt32 offset;
            UInt16 stride;
            UInt8  dynamic;
            UInt8  readback;
            //@}
        };
        GN_CASSERT( sizeof(VtxBufHeader) == 8 );

        ///
        /// mesh index buffer header
        ///
        struct IdxBufHeader
        {
            //@{
            UInt8  dynamic;
            UInt8  readback;
            UInt16 reserved;
            //@}
        };
        GN_CASSERT( sizeof(IdxBufHeader) == 4 );

        //@{
        gfx::PrimitiveType    primtype;
        UInt32                numprim;
        UInt32                startvtx;
        UInt32                minvtxidx;
        UInt32                numvtx;
        UInt32                startidx;
        gfx::VtxFmtDesc       vtxfmt;
        VtxBufHeader          vtxbuf[16]; // support 16 streams at most
        IdxBufHeader          idxbuf;
        //@}
    };

    ///
    /// vertex buffer descriptor used by mesh class
    ///
    struct MeshVtxBuf
    {
        GraphicsResource * buffer; ///< buffer pointer
        size_t             offset; ///< offset (in bytes) of the first vertex
        size_t             stride; ///< vertex stride in bytes.

        ///
        /// ctor
        ///
        MeshVtxBuf() : buffer( 0 ) {}
    };

    ///
    /// Mesh class, the atomic geometry chunk in garnet rendering system that
    /// can be renderer by single DIP().
    ///
    struct Mesh
    {
        /// \name mesh data
        //@{
        RenderEngine &        engine;
        GraphicsResource *    vtxfmt;
        DynaArray<MeshVtxBuf> vtxbufs;
        GraphicsResource *    idxbuf;
        gfx::PrimitiveType    primtype;
        UInt32                numprim;
        UInt32                startvtx;
        UInt32                minvtxidx;
        UInt32                numvtx;
        UInt32                startidx;
        //@}

        ///
        /// ctor
        ///
        Mesh( RenderEngine & e )
            : engine( e )
            , vtxfmt( 0 )
            , idxbuf( 0 )
        {}

        ///
        /// dtor
        ///
        ~Mesh() { clear(); }

        ///
        /// clear to empty mesh
        ///
        void clear()
        {
            safeFreeGraphicsResource( vtxfmt );
            for( size_t i = 0; i < vtxbufs.size(); ++i )
            {
                safeFreeGraphicsResource( vtxbufs[i].buffer );
            }
            vtxbufs.clear();
            safeFreeGraphicsResource( idxbuf );
            primtype = gfx::POINT_LIST;
            numprim = 0;
            startvtx = 0;
            minvtxidx = 0;
            numvtx = 0;
            startidx = 0;
        }

        ///
        /// update renderer context
        ///
        void updateContext( DrawContext & context ) const
        {
            GN_ASSERT( engine.checkResource( vtxfmt ) );
            context.setVtxFmt( vtxfmt );
            for( size_t i = 0; i < vtxbufs.size(); ++i )
            {
                GN_ASSERT( engine.checkResource( vtxbufs[i].buffer ) );
                context.setVtxBuf( i, vtxbufs[i].buffer, vtxbufs[i].offset, vtxbufs[i].stride );
            }
            if( idxbuf )
            {
                GN_ASSERT( engine.checkResource( idxbuf ) );
                context.setIdxBuf( idxbuf );
            }
        }

        ///
        /// draw the mesh
        ///
        void draw() const
        {
            if( idxbuf )
            {
                GN_ASSERT( engine.checkResource( idxbuf ) );
                engine.drawIndexed( primtype, numprim, startvtx, minvtxidx, numvtx, startidx );
            }
            else
            {
                engine.draw( primtype, numprim, startvtx );
            }
        }

        ///
        /// load from XML
        ///
        bool loadFromXmlNode( const XmlNode & root, const StrA & basedir );

        ///
        /// load from binary stream
        ///
        bool loadFromBinaryStream( File & );

        ///
        /// load mesh from file, support both XML and bin format
        ///
        bool loadFromFile( const StrA & filename );
    };

    //@{

    EntityTypeId getMeshEntityType( EntityManager & em );

    ///
    /// try find exising entity named "filename", if not found, create new one.
    ///
    Entity * loadMeshEntityFromFile( EntityManager & em, RenderEngine & re, const StrA & filename );

    ///
    /// Generate a cube mesh, with texcoord and normal
    ///
    Entity * generateCubeMeshEntity( EntityManager & em, RenderEngine & re, const StrA & name, float edgeLength );

    //@}
}}

// *****************************************************************************
//                           End of mesh.h
// *****************************************************************************
#endif // __GN_ENGINE_MESH_H__
