#ifndef __GN_GFX_MESH_H__
#define __GN_GFX_MESH_H__
// *****************************************************************************
//! \file    gfx/mesh.h
//! \brief   interface of mesh class
//! \author  chenli@@FAREAST (2007.2.13)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// vertex buffer descriptor used by mesh class
    ///
    struct MeshVtxBuf
    {
        AutoRef<VtxBuf> buffer; ///< buffer pointer
        size_t          offset; ///< offset (in bytes) of the first vertex
        size_t          stride; ///< vertex stride in bytes.
    };

    ///
    /// Mesh class, the atomic geometry chunk in garnet rendering system that
    /// can be renderer by single DIP().
    ///
    struct Mesh : public RefCounter
    {
        /// \name mesh data
        //@{
        VtxFmtHandle          vtxfmt;
        DynaArray<MeshVtxBuf> vtxbufs;
        AutoRef<IdxBuf>       idxbuf;
        PrimitiveType         primtype;
        UInt32                numprim;
        UInt32                startvtx;
        UInt32                minvtxidx;
        UInt32                numvtx;
        UInt32                startidx;
        //@}

        ///
        /// clear to empty mesh
        ///
        void clear()
        {
            vtxfmt = 0;
            vtxbufs.clear();
            idxbuf.clear();
            primtype = POINT_LIST;
            numprim = 0;
            startvtx = 0;
            minvtxidx = 0;
            numvtx = 0;
            startidx = 0;
        }

        ///
        /// update renderer context
        ///
        void updateContext( RendererContext & context ) const
        {
            context.setVtxFmt( vtxfmt );
            for( size_t i = 0; i < vtxbufs.size(); ++i )
                context.setVtxBuf( i, vtxbufs[i].buffer, vtxbufs[i].offset, vtxbufs[i].stride );
            if( idxbuf ) context.setIdxBuf( idxbuf );
        }

        ///
        /// update renderer context
        ///
        void updateContext() const
        {
            Renderer & r = gRenderer;
            r.setVtxFmt( vtxfmt );
            for( size_t i = 0; i < vtxbufs.size(); ++i )
                r.setVtxBuf( i, vtxbufs[i].buffer, vtxbufs[i].offset, vtxbufs[i].stride );
            if( idxbuf ) r.setIdxBuf( idxbuf );
        }

        ///
        /// draw the mesh
        ///
        void draw() const
        {
            Renderer & r = gRenderer;
            if( idxbuf )
            {
                r.drawIndexed( primtype, numprim, startvtx, minvtxidx, numvtx, startidx );
            }
            else
            {
                r.draw( primtype, numprim, startvtx );
            }
        }

        ///
        /// load from XML
        ///
        bool loadFromXml( const XmlNode * rootnode, const StrA & meshdir, Renderer & r );

        ///
        /// load from XML file
        ///
        bool loadFromXmlFile( File &, const StrA & meshdir, Renderer & r );
    };

    ///
    /// Generate a cube mesh, with texcoord and normal
    ///
    bool generateCubeMesh( Mesh & mesh, float edgeLength );
}}

// *****************************************************************************
//                           End of mesh.h
// *****************************************************************************
#endif // __GN_GFX_MESH_H__
