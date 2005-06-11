#include "pch.h"

//
//
// -------------------------------------------------------------------------
const GN::ColorFormatDesc & GN::getColorFormatDesc( GN::ColorFormat fmt )
{
    GN_GUARD;

    static struct ClrDescTable
    {
        ColorFormatDesc table[NUM_COLOR_FORMATS+1];

        #define FOURCC(ch1,ch2,ch3,ch4) \
            FOURCC2(ch1##ch2##ch3##ch4)
        #define FOURCC2(x) #x

        #define CH(shift,bits,type) shift,bits,TYPE_##type
        #define CH0() \
            pdesc->numChannels = 0;
        #define CH1( ch0 ) \
            pdesc->numChannels = 1; \
            setupChannel( pdesc->channels[0], ch0 );
        #define CH2( ch0, ch1 ) \
            pdesc->numChannels = 2; \
            setupChannel( pdesc->channels[0], ch0 ); \
            setupChannel( pdesc->channels[1], ch1 );
        #define CH3( ch0, ch1, ch2 ) \
            pdesc->numChannels = 3; \
            setupChannel( pdesc->channels[0], ch0 ); \
            setupChannel( pdesc->channels[1], ch1 ); \
            setupChannel( pdesc->channels[2], ch2 );
        #define CH4( ch0, ch1, ch2, ch3 ) \
            pdesc->numChannels = 4; \
            setupChannel( pdesc->channels[0], ch0 ); \
            setupChannel( pdesc->channels[1], ch1 ); \
            setupChannel( pdesc->channels[2], ch2 ); \
            setupChannel( pdesc->channels[3], ch3 );

        //!
        //! fourcc handler
        //!
        static inline void
        handleFourcc( ColorFormatDesc & desc, const char * tag )
        {
            #ifndef MAKE_FOURCC
            #define MAKE_FOURCC(ch0, ch1, ch2, ch3)    \
                    ((uint32_t)(uint8_t)(ch0) |         \
                    ((uint32_t)(uint8_t)(ch1) << 8) |   \
                    ((uint32_t)(uint8_t)(ch2) << 16) |  \
                    ((uint32_t)(uint8_t)(ch3) << 24 ))
            #endif

            desc.fourcc = MAKE_FOURCC(tag[0],tag[1],tag[2],tag[3]);
            switch( desc.fourcc )
            {
                case MAKE_FOURCC('D','X','T','1') :
                case MAKE_FOURCC('D','X','T','2') :
                case MAKE_FOURCC('D','X','T','3') :
                case MAKE_FOURCC('D','X','T','4') :
                case MAKE_FOURCC('D','X','T','5') :
                    desc.blockWidth  = 4;
                    desc.blockHeight = 4;
                    break;

                default:
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
        ClrDescTable()
        {
            // clear description table
            memset( table, 0, sizeof(table) );

            ColorFormatDesc * pdesc;

            // initialize the table
            #define GN_COLOR_FORMAT( format_, bits_, fourcc_, channels_ ) \
                pdesc = &table[FMT_##format_]; \
                pdesc->name = "FMT_"#format_; \
                pdesc->bits = bits_; \
                handleFourcc( *pdesc, fourcc_ ); \
                channels_;
            #include "garnet/base/formatMeta.h"
            #undef GN_COLOR_FORMAT

            // special case for FMT_INVALID
            pdesc = &table[FMT_INVALID];
            pdesc->name = "FMT_INVALID";
            pdesc->numChannels = 0;
            pdesc->bits = 0;
        }
    } s_table;

    GN_ASSERT( 0 <= fmt && fmt <= NUM_COLOR_FORMATS );
    return s_table.table[fmt];

    GN_UNGUARD;
}
