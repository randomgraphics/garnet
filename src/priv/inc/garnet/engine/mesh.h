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
        GraphicsResourceId buffer; ///< buffer ID
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
        GraphicsResourceId    vtxfmt;
        DynaArray<MeshVtxBuf> vtxbufs;
        GraphicsResourceId    idxbuf;
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
            context.setVtxFmt( (gfx::VtxFmtHandle)vtxfmt );
            for( size_t i = 0; i < vtxbufs.size(); ++i )
            {
                context.setVtxBuf( i, (const gfx::VtxBuf *)vtxbufs[i].buffer, vtxbufs[i].offset, vtxbufs[i].stride );
            }
            if( idxbuf )
            {
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

    Mesh * entity2Mesh( EntityId );

    ///
    /// try find exising texture entity named "filename", if not found, create new one.
    ///
    EntityId loadMesh( EntityManager & em, RenderEngine & re, const StrA & filename );

    ///
    /// Generate a cube mesh, with texcoord and normal
    ///
    EntityId generateCubeMesh( EntityManager& em, RenderEngine & re, const StrA & name, float edgeLength );

    ///
    /// ..
    ///
    void deleteMeshEntity( EntityManager & em, EntityId );

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
