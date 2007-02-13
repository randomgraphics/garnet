#include "pch.h"

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const GN::gfx::ClrFmtDesc * GN::gfx::detail::generateClrFmtDescTable()
{
    GN_GUARD;

    static struct ClrDescGenerator
    {
        ClrFmtDesc table[NUM_CLRFMTS+1];

        #define MAKE_FOURCC_INT(ch0, ch1, ch2, ch3) \
                ((UInt32)(UInt8)(ch0) |         \
                ((UInt32)(UInt8)(ch1) << 8) |   \
                ((UInt32)(UInt8)(ch2) << 16) |  \
                ((UInt32)(UInt8)(ch3) << 24 ))

        #define SWIZZLE(x,y,z,w) MAKE_FOURCC_INT( *#x, *#y, *#z, *#w )

        #define CH(shift,bits,type) shift,bits,TYPE_##type

        #define CH1( sw, ch0 ) \
            pdesc->blockWidth  = 1; \
            pdesc->blockHeight = 1; \
            pdesc->swizzle = sw; \
            pdesc->numChannels = 1; \
            setupChannel( pdesc->channels[0], ch0 );
        #define CH2( sw, ch0, ch1 ) \
            pdesc->blockWidth  = 1; \
            pdesc->blockHeight = 1; \
            pdesc->swizzle = sw; \
            pdesc->numChannels = 2; \
            setupChannel( pdesc->channels[0], ch0 ); \
            setupChannel( pdesc->channels[1], ch1 );
        #define CH3( sw, ch0, ch1, ch2 ) \
            pdesc->blockWidth  = 1; \
            pdesc->blockHeight = 1; \
            pdesc->swizzle = sw; \
            pdesc->numChannels = 3; \
            setupChannel( pdesc->channels[0], ch0 ); \
            setupChannel( pdesc->channels[1], ch1 ); \
            setupChannel( pdesc->channels[2], ch2 );
        #define CH4( sw, ch0, ch1, ch2, ch3 ) \
            pdesc->blockWidth  = 1; \
            pdesc->blockHeight = 1; \
            pdesc->swizzle = sw; \
            pdesc->numChannels = 4; \
            setupChannel( pdesc->channels[0], ch0 ); \
            setupChannel( pdesc->channels[1], ch1 ); \
            setupChannel( pdesc->channels[2], ch2 ); \
            setupChannel( pdesc->channels[3], ch3 );

        #define FOURCC(c0,c1,c2,c3) \
            pdesc->numChannels = 0; \
            handleFourcc( *pdesc, MAKE_FOURCC_INT(*#c0, *#c1, *#c2, *#c3) );

        ///
        /// fourcc handler
        ///
        static inline void
        handleFourcc( ClrFmtDesc & desc, UInt32 fourcc )
        {

            switch( fourcc )
            {
                case MAKE_FOURCC_INT('D','X','T','1') :
                case MAKE_FOURCC_INT('D','X','T','2') :
                case MAKE_FOURCC_INT('D','X','T','3') :
                case MAKE_FOURCC_INT('D','X','T','4') :
                case MAKE_FOURCC_INT('D','X','T','5') :
                    desc.blockWidth  = 4;
                    desc.blockHeight = 4;
                    break;

                default:
                    GN_UNEXPECTED();
                    desc.blockWidth  = 1;
                    desc.blockHeight = 1;
            }
        }

        ///
        /// setup channel properties
        ///
        static inline void
        setupChannel(
            ChannelDesc & ch, unsigned int shift, unsigned int bits, unsigned int type )
        {
            ch.shift = shift;
            ch.bits = bits;
            ch.type = type;
        }

        ///
        /// constructor
        ///
        ClrDescGenerator()
        {
            // clear description table
            memset( table, 0, sizeof(table) );

            ClrFmtDesc * pdesc;

            // initialize the table
            #define GN_COLOR_FORMAT( format_, bits_, channels_ ) \
                pdesc = &table[FMT_##format_]; \
                pdesc->name = "FMT_"#format_; \
                pdesc->bits = bits_; \
                channels_;
            #include "garnet/gfx/colorFormatMeta.h"
            #undef GN_COLOR_FORMAT

            // special case for FMT_INVALID
            pdesc = &table[FMT_INVALID];
            pdesc->name = "FMT_INVALID";
            pdesc->numChannels = 0;
            pdesc->bits = 0;
        }
    } sGenerator;

    return sGenerator.table;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::ClrFmt GN::gfx::str2ClrFmt( const StrA & s )
{
    for( size_t i = 0; i <= NUM_CLRFMTS; ++i )
    {
        if( getClrFmtDesc( (ClrFmt)i ).name == s ) return (ClrFmt) i;
    }

    // handle format alias
#define CHECK_ALIAS( X ) if( #X == s ) return X;

    CHECK_ALIAS( FMT_RGBA32 );
    CHECK_ALIAS( FMT_BGRA32 );
    CHECK_ALIAS( FMT_D3DCOLOR );

    CHECK_ALIAS( FMT_DXT_FIRST );
    CHECK_ALIAS( FMT_DXT_LAST );

    CHECK_ALIAS( FMT_FLOAT4 );
    CHECK_ALIAS( FMT_FLOAT3 );
    CHECK_ALIAS( FMT_FLOAT2 );
    CHECK_ALIAS( FMT_FLOAT1 );

    CHECK_ALIAS( FMT_FLOAT16_4 );
    CHECK_ALIAS( FMT_FLOAT16_2 );

    CHECK_ALIAS( FMT_INT4 );
    CHECK_ALIAS( FMT_INT2 );
    CHECK_ALIAS( FMT_INT1 );

    CHECK_ALIAS( FMT_INT4N );
    CHECK_ALIAS( FMT_INT2N );
    CHECK_ALIAS( FMT_INT1N );

    CHECK_ALIAS( FMT_UINT4 );
    CHECK_ALIAS( FMT_UINT2 );
    CHECK_ALIAS( FMT_UINT1 );

    CHECK_ALIAS( FMT_UINT4N );
    CHECK_ALIAS( FMT_UINT2N );
    CHECK_ALIAS( FMT_UINT1N );

    CHECK_ALIAS( FMT_SHORT4 );
    CHECK_ALIAS( FMT_SHORT2 );

    CHECK_ALIAS( FMT_SHORT4N );
    CHECK_ALIAS( FMT_SHORT2N );

    CHECK_ALIAS( FMT_USHORT4 );
    CHECK_ALIAS( FMT_USHORT2 );

    CHECK_ALIAS( FMT_USHORT4N );
    CHECK_ALIAS( FMT_USHORT2N );

    CHECK_ALIAS( FMT_BYTE4 );
    CHECK_ALIAS( FMT_BYTE4N );

    CHECK_ALIAS( FMT_UBYTE4 );
    CHECK_ALIAS( FMT_UBYTE4N );

    CHECK_ALIAS( FMT_DEC4 );
    CHECK_ALIAS( FMT_DEC3 );
    CHECK_ALIAS( FMT_DEC4N );
    CHECK_ALIAS( FMT_DEC3N );

    CHECK_ALIAS( FMT_UDEC4 );
    CHECK_ALIAS( FMT_UDEC3 );
    CHECK_ALIAS( FMT_UDEC4N );
    CHECK_ALIAS( FMT_UDEC3N );

    CHECK_ALIAS( FMT_HEND4 );
    CHECK_ALIAS( FMT_HEND3 );
    CHECK_ALIAS( FMT_HEND4N );
    CHECK_ALIAS( FMT_HEND3N );

    CHECK_ALIAS( FMT_UHEND4 );
    CHECK_ALIAS( FMT_UHEND3 );
    CHECK_ALIAS( FMT_UHEND4N );
    CHECK_ALIAS( FMT_UHEND3N );

    CHECK_ALIAS( FMT_DHEN4 );
    CHECK_ALIAS( FMT_DHEN3 );
    CHECK_ALIAS( FMT_DHEN4N );
    CHECK_ALIAS( FMT_DHEN3N );

    CHECK_ALIAS( FMT_UDHEN4 );
    CHECK_ALIAS( FMT_UDHEN3 );
    CHECK_ALIAS( FMT_UDHEN4N );
    CHECK_ALIAS( FMT_UDHEN3N );

#undef CHECK_ALIAS

    return FMT_INVALID;
}
