#include "pch.h"

//
//
// -------------------------------------------------------------------------
const GN::gfx::ClrFmtDesc * GN::gfx::detail::sGenerateClrFmtDescTable()
{
    GN_GUARD;

    static struct ClrDescGenerator
    {
        ClrFmtDesc table[NUM_CLRFMTS+1];

        #define MAKE_FOURCC_INT(ch0, ch1, ch2, ch3) \
                ((uint32_t)(uint8_t)(ch0) |         \
                ((uint32_t)(uint8_t)(ch1) << 8) |   \
                ((uint32_t)(uint8_t)(ch2) << 16) |  \
                ((uint32_t)(uint8_t)(ch3) << 24 ))

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

        //!
        //! fourcc handler
        //!
        static inline void
        handleFourcc( ClrFmtDesc & desc, uint32_t fourcc )
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

        //!
        //! setup channel properties
        //!
        static inline void
        setupChannel(
            ChannelDesc & ch, unsigned int shift, unsigned int bits, unsigned int type )
        {
            ch.shift = shift;
            ch.bits = bits;
            ch.type = type;
        }

        //!
        //! constructor
        //!
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
