#ifndef __GN_GFX_SAMPLER_H__
#define __GN_GFX_SAMPLER_H__
// *****************************************************************************
/// \file
/// \brief   texture sampler class
/// \author  chenli@@FAREAST (2007.6.11)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// texture filter
    ///
    enum TextureFilter
    {
        TF_POINT,  ///< point filter
        TF_LINEAR, ///< linear filter
        NUM_TEXTURE_FILTERS,
    };

    ///
    /// texture addressing mode
    ///
    enum TextureAddressMode
    {
        //@{
        TA_REPEAT,
        TA_CLAMP,
        TA_CLAMP_TO_EDGE,
        //TA_MIRROR,
        //TA_MIRROR_ONCE,
        //TA_BORDER,
        NUM_TEXTURE_ADDRESS_MODES,
        //@}
    };

    ///
    /// sampler descriptor
    ///
    struct SamplerDesc
    {
        //@{

        float mipBias;

        float border[4]; ///< border color (R,G,B,A)

        float minlod;
        float maxlod;

        unsigned int addressU : 8;
        unsigned int addressV : 8;
        unsigned int addressW : 8;

        unsigned int mipFilter   : 1; ///< mipmap filter
        unsigned int magFilter   : 1; ///< magnify filter
        unsigned int minFilter   : 1; ///< minify filter
        unsigned int anisoFitler : 3; ///< aniso filter. 0: disable, 1-5: 2^(n-1)
        unsigned int _           : 2; ///< reserved;

        //@}

        ///
        /// equality
        ///
        bool operator==( const SamplerDesc & rhs ) const
        {
            return this == &rhs || 0 == memcmp( this, &rhs, sizeof(*this) );
        }

        ///
        /// default sampler descriptor:
        /// - zero bias
        /// - black border
        /// - lod range: [-FLT_MAX,FLT_MAX]
        /// - linear filters
        /// - wrap mode
        ///
        static const SamplerDesc DEFAULT;
    };
    GN_CASSERT( sizeof(SamplerDesc) == 4*8 );

    //@{

    ///
    /// convert filter tag to string
    // -------------------------------------------------------------------------
    inline bool
    texFilter2Str( StrA & str, int filter )
    {
        static const char * sTable [] =
        {
            "POINT", "LINEAR",
        };

        if( 0 <= filter && filter < NUM_TEXTURE_FILTERS )
        {
            str = sTable[filter];
            return true;
        }
        else return false;
    }

    ///
    /// convert filter tag to string
    // -------------------------------------------------------------------------
    inline const char *
    texFilter2Str( int filter )
    {
        static const char * sTable [] =
        {
            "NEAREST", "LINEAR",
            "N_MIP_N", "N_MIP_L",
            "L_MIP_N", "L_MIP_L",
        };

        if( 0 <= filter && filter < NUM_TEXTURE_FILTERS )
        {
            return sTable[filter];
        }
        else return "BAD_TEXTURE_FILTER";
    }

    ///
    /// convert filter tag to string
    // -------------------------------------------------------------------------
    inline bool
    str2TexFilter( TextureFilter & value, const char * name )
    {
        static const char * sTable [] =
        {
            "NEAREST", "LINEAR",
            "N_MIP_N", "N_MIP_L",
            "L_MIP_N", "L_MIP_L",
        };

        if( name )
        {
            for( size_t i = 0; i < NUM_TEXTURE_FILTERS; ++i )
            {
                if( 0 ==::strcmp(sTable[i],name) )
                {
                    value = static_cast<TextureFilter>(i);
                    return true;
                }
            }
        }
        return false;
    }

    ///
    /// convert address mode to string
    // -------------------------------------------------------------------------
    inline bool
    texAddrMode2Str( StrA & str, TextureAddressMode ta )
    {
        static const char * sTable [] =
        { "REPEAT", "CLAMP", "CLAMP_TO_EDGE" };

        if( 0 <= ta && ta < NUM_TEXTURE_ADDRESS_MODES )
        {
            str = sTable[ta];
            return true;
        }
        else return false;
    }
    //
    inline const char *
    texAddrMode2Str( TextureAddressMode ta )
    {
        static const char * sTable [] =
        { "REPEAT", "CLAMP", "CLAMP_TO_EDGE" };

        if( 0 <= ta && ta < NUM_TEXTURE_ADDRESS_MODES )
        {
            return sTable[ta];
        }
        else return "BAD_TEXTURE_WRAP";
    }
    //
    inline bool
    str2TexAddrMode( TextureAddressMode & value, const char * name )
    {
        static const char * sTable [] =
        { "REPEAT", "CLAMP", "CLAMP_TO_EDGE" };

        if( name )
        {
            for( size_t i = 0; i < NUM_TEXTURE_ADDRESS_MODES; ++i )
            {
                if( 0 ==::strcmp(sTable[i],name) )
                {
                    value = static_cast<TextureAddressMode>(i);
                    return true;
                }
            }
        }
        return false;
    }

    //@}

}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_SAMPLER_H__
