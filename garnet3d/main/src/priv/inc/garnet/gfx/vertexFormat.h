#ifndef __GN_GFX_VERTEXFORMAT_H__
#define __GN_GFX_VERTEXFORMAT_H__
// *****************************************************************************
//! \file    vertexFormat.h
//! \brief   Vertex format structure
//! \author  chenlee (2005.11.13)
// *****************************************************************************

namespace GN { namespace gfx
{
    //!
    //! Vertex semantic types
    //!
    typedef FOURCC VtxSem;

    //! \name standard vertex semantics
    //@{
    const VtxSem VTXSEM_NULL = FOURCC::sMake( 0 , 0 , 0 , 0 ); //!< indicate invalid vertex semantic.
    const VtxSem VTXSEM_POS0 = FOURCC::sMake('P','O','S','0'); //!< position
    const VtxSem VTXSEM_WGHT = FOURCC::sMake('W','G','H','T'); //!< blending weight
    const VtxSem VTXSEM_NML0 = FOURCC::sMake('N','M','L','0'); //!< normal
    const VtxSem VTXSEM_CLR0 = FOURCC::sMake('C','L','R','0'); //!< diffuse color
    const VtxSem VTXSEM_CLR1 = FOURCC::sMake('C','L','R','1'); //!< specular color
    const VtxSem VTXSEM_FOG  = FOURCC::sMake('F','O','G','0'); //!< for coordinates
    const VtxSem VTXSEM_TANG = FOURCC::sMake('T','A','N','G'); //!< tangent
    const VtxSem VTXSEM_BNML = FOURCC::sMake('B','N','M','L'); //!< binormal
    const VtxSem VTXSEM_TEX0 = FOURCC::sMake('T','E','X','0'); //!< texture coordinate 0
    const VtxSem VTXSEM_TEX1 = FOURCC::sMake('T','E','X','1'); //!< texture coordinate 1
    const VtxSem VTXSEM_TEX2 = FOURCC::sMake('T','E','X','2'); //!< texture coordinate 2
    const VtxSem VTXSEM_TEX3 = FOURCC::sMake('T','E','X','3'); //!< texture coordinate 3
    const VtxSem VTXSEM_TEX4 = FOURCC::sMake('T','E','X','4'); //!< texture coordinate 4
    const VtxSem VTXSEM_TEX5 = FOURCC::sMake('T','E','X','5'); //!< texture coordinate 5
    const VtxSem VTXSEM_TEX6 = FOURCC::sMake('T','E','X','6'); //!< texture coordinate 6
    const VtxSem VTXSEM_TEX7 = FOURCC::sMake('T','E','X','7'); //!< texture coordinate 7
    const VtxSem VTXSEM_TEX8 = FOURCC::sMake('T','E','X','8'); //!< texture coordinate 8
    const VtxSem VTXSEM_TEX9 = FOURCC::sMake('T','E','X','9'); //!< texture coordinate 9
    const VtxSem VTXSEM_TEXA = FOURCC::sMake('T','E','X','A'); //!< texture coordinate 10
    const VtxSem VTXSEM_TEXB = FOURCC::sMake('T','E','X','B'); //!< texture coordinate 11
    const VtxSem VTXSEM_TEXC = FOURCC::sMake('T','E','X','C'); //!< texture coordinate 12
    const VtxSem VTXSEM_TEXD = FOURCC::sMake('T','E','X','D'); //!< texture coordinate 13
    const VtxSem VTXSEM_TEXE = FOURCC::sMake('T','E','X','E'); //!< texture coordinate 14
    const VtxSem VTXSEM_TEXF = FOURCC::sMake('T','E','X','F'); //!< texture coordinate 15
    //@}
    
    enum
    {
        MAX_VERTEX_ATTRIBUTES = 32, //!< max number of attributes in single vertex format descriptor
    };

    //!
    //! Vertex format descriptor
    //!
    struct VtxFmtDesc
    {
        //!
        //! attribute descriptor
        //!
        struct AttribDesc
        {
            VtxSem   semantic; //!< FORCC encoded attribute sementic.
                               //!< Could be one of standard semantics, or any user-defined value.
                               //!< Note that semantic must be unique in descriptor.
            ClrFmt   format;   //!< attribute format (FMT_XXX).
            uint16_t offset;   //!< offset in stream.
            uint8_t  stream;   //!< stream index.

            //!
            //! set values in attribute descriptor
            //!
            void set( size_t stream_, size_t offset_, VtxSem sem_, ClrFmt fmt_ )
            {
                semantic = sem_;
                format   = fmt_;
                stream   = (uint8_t)stream_;
                offset   = (uint16_t)offset_;
            }
        };

        AttribDesc attribs[MAX_VERTEX_ATTRIBUTES]; //!< vertex attribute list
        size_t     count;                          //!< vertex attribute count

        //! \name commonly used vertex format descriptor constants
        //@{
        static const VtxFmtDesc XYZ_NORM_UV; //!< position, normal, texture
        static const VtxFmtDesc XYZ_UV;      //!< position, texture
        static const VtxFmtDesc XYZ;         //!< position
        static const VtxFmtDesc XYZW_UV;     //!< 4D-position, texture
        static const VtxFmtDesc XYZW;        //!< 4D-position
        //@}

        //!
        //! Clear to empty declarator.
        //!
        void clear() { count = 0; }

        //!
        //! Check validity the descriptor.
        //!
        bool validate() const;

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
            size_t stream,
            size_t offset,
            VtxSem semantic,
            ClrFmt format );

        //!
        //! Find specific attribute by semantic
        //!
        const AttribDesc * findAttrib( VtxSem semantic ) const;

        //!
        //! Calculate number of streams
        //!
        size_t calcNumStreams();

        //!
        //! Calculate stream stride
        //!
        size_t calcStreamStride( size_t );

        //!
        //! µÈÖµÅÐ¶Ï
        //!
        bool operator == ( const VtxFmtDesc & rhs ) const { return attribs == rhs.attribs; }
    };
}}

#if GN_ENABLE_INLINE
#include "vertexFormat.inl"
#endif

// *****************************************************************************
//                           End of vertexFormat.h
// *****************************************************************************
#endif // __GN_GFX_VERTEXFORMAT_H__
