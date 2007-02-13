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
    struct Mesh
    {
        /// \name mesh data
        //@{
        VtxFmtHandle          vtxfmt;
        DynaArray<MeshVtxBuf> vtxbufs;
        AutoRef<IdxBuf>       idxbuf;
        PrimitiveType         primType;
        UInt32                primCount;
        UInt32                startVtx;
        UInt32                minVtxIdx;
        UInt32                numVtx;
        UInt32                startIdx;
        //@}

        ///
        /// clear to empty mesh
        ///
        void clear()
        {
            vtxfmt = 0;
            vtxbufs.clear();
            idxbuf.clear();
            primType = POINT_LIST;
            primCount = 0;
            startVtx = 0;
            minVtxIdx = 0;
            numVtx = 0;
            startIdx = 0;
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
        void updateContext( Renderer & r ) const
        {
            r.setVtxFmt( vtxfmt );
            for( size_t i = 0; i < vtxbufs.size(); ++i )
                r.setVtxBuf( i, vtxbufs[i].buffer, vtxbufs[i].offset, vtxbufs[i].stride );
            if( idxbuf ) r.setIdxBuf( idxbuf );
        }

        ///
        /// draw the mesh
        ///
        void draw( Renderer & r ) const
        {
            if( idxbuf )
            {
                r.drawIndexed( primType, primCount, startVtx, minVtxIdx, numVtx, startIdx );
            }
            else
            {
                r.draw( primType, primCount, startVtx );
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
}}

// *****************************************************************************
//                           End of mesh.h
// *****************************************************************************
#endif // __GN_GFX_MESH_H__
