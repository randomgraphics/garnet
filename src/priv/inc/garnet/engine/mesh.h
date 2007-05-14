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
    /// vertex buffer descriptor used by mesh class
    ///
    struct MeshVtxBuf
    {
        GraphicsResource * buffer; ///< buffer pointer
        size_t             offset; ///< offset (in bytes) of the first vertex
        size_t             stride; ///< vertex stride in bytes.
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
        Mesh( RenderEngine & e ) : engine(e) {}

        ///
        /// clear to empty mesh
        ///
        void clear()
        {
            vtxfmt = 0;
            vtxbufs.clear();
            idxbuf = 0;
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
            context.setVtxFmt( (gfx::VtxFmtHandle)vtxfmt );
            for( size_t i = 0; i < vtxbufs.size(); ++i )
            {
                GN_ASSERT( engine.checkResource( vtxbufs[i].buffer ) );
                context.setVtxBuf( i, (const gfx::VtxBuf *)vtxbufs[i].buffer, vtxbufs[i].offset, vtxbufs[i].stride );
            }
            if( idxbuf )
            {
                GN_ASSERT( engine.checkResource( idxbuf ) );
                context.setIdxBuf( (const gfx::IdxBuf*)idxbuf );
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
    };

    //@{

    EntityTypeId getMeshEntityType( EntityManager & em );

    ///
    /// try find exising texture entity named "filename", if not found, create new one.
    ///
    Entity * loadMesh( EntityManager & em, RenderEngine & re, const StrA & filename );

    ///
    /// Generate a cube mesh, with texcoord and normal
    ///
    Entity * generateCubeMesh( EntityManager & em, RenderEngine & re, const StrA & name, float edgeLength );

    ///
    /// ..
    ///
    void deleteMeshEntity( Entity * );

    ///
    /// ..
    ///
    void deleteAllMeshEntities( EntityManager & em );

    //@}
}}

// *****************************************************************************
//                           End of mesh.h
// *****************************************************************************
#endif // __GN_ENGINE_MESH_H__
