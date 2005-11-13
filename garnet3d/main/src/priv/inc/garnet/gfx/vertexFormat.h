#ifndef __GN_GFX_VERTEXFORMAT_H__
#define __GN_GFX_VERTEXFORMAT_H__
// *****************************************************************************
//! \file    vertexFormat.h
//! \brief   Vertex format structure
//! \author  chenlee (2005.11.13)
// *****************************************************************************

namespace GN { namespace gfx
{
    //! \def GNGFX_DEFINE_VTXSEM
    //! define vertex semantic tag

    //!
    //! vertex semantic tags
    //!
    enum VtxSem
    {
        #define GNGFX_DEFINE_VTXSEM( tag, d3decl, d3dindex, glname, glindex, cgname ) VTXSEM_##tag,
        #include "vertexSemanticMeta.h"
        #undef GNGFX_DEFINE_VTXSEM

        //!
        //! num of vertex semantic tags
        //!
        NUM_VTXSEMS,

        //!
        //! indicate an invalid vertex semantic.
        //!
        VTXSEM_INVALID,
    };

    //!
    //! Vertex format descriptor
    //!
    struct VtxFmtDesc
    {
        //!
        //! Vertex attribute descriptor
        //!
        struct AttribDesc
        {
            bool        used;     //!< Is this attribute used or not?
            uint8_t     stream;   //!< stream index.
            uint8_t     offset;   //!< offset in vertex stream.
            ClrFmt      format;   //!< attribute format (FMT_XXX).
        };

        //!
        //! Vertex stream descriptor
        //!
        struct StreamDesc
        {
            VtxSem attribs[NUM_VTXSEMS]; //!< attribute indices into attribute array.
            uint8_t        numAttribs;   //!< number of attributes in this stream.
            uint8_t        stride;       //!< stream stride in bytes.
        };

        AttribDesc attribs[NUM_VTXSEMS]; //!< vertex attribute array, indexed by vertex semantic.
        StreamDesc streams[NUM_VTXSEMS]; //!< vertex stream array.
        uint8_t    numStreams;           //!< stream count.

        //!
        //! Constructor
        //!
        VtxFmtDesc() { reset(); }

        //!
        //! Reset to empty declarator.
        //!
        GN_INLINE void reset();

        //!
        //! Add new attribute into vertex format declarator.
        //!
        //! \param stream
        //!     StreamDesc index of the attribute.
        //! \param offset
        //!     AttribDesc offset in the stream. Can be 0, which means packed with last attribute.
        //! \param semantic
        //!     AttribDesc semantic (VTXSEM_XXX)
        //! \param format
        //!     AttribDesc format (FMT_XXXX)
        //!
        bool addAttrib(
            uint8_t        stream,
            uint8_t        offset,
            VtxSem         semantic,
            ClrFmt         format );

        //!
        //! µÈÖµÅÐ¶Ï
        //!
        bool operator == ( const VtxFmtDesc & rhs ) const;
    };

    //!\name Util functions
    //@{

    //!
    //! Convert vertex semantic tag to string.
    //!
    //! \return Return error message if failed.
    //!
    const char * vtxSem2Str( VtxSem );

    //!
    //! Convert vertex semantic tag to string
    //!
    //! \return Return false if failed.
    //!
    bool vtxSem2Str( StrA &, VtxSem );

    //!
    //! Convert string to vertex semantic
    //!
    //! \return Return VTXSEM_INVALID, if failed.
    VtxSem str2VtxSem( const char * );

    //!
    //! Convert string to vertex semantic
    //!
    //! \return Return false if failed.
    //!
    bool str2VtxSem( VtxSem &, const char * );

    //@}
}}

#if GN_ENABLE_INLINE
#include "vertexFormat.inl"
#endif

// *****************************************************************************
//                           End of vertexFormat.h
// *****************************************************************************
#endif // __GN_GFX_VERTEXFORMAT_H__
