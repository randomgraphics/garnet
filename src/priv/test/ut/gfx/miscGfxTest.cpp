#include "../testCommon.h"
#include "garnet/GNgfx.h"

namespace GN { namespace gfx
{
    ///
    /// GPU program resource binding
    ///
    class GpuProgramResourceBinding
    {
    public:

        /// constructor
        GpuProgramResourceBinding()
            : mBindBy(NOTHING)
        {
        }

        /// clear current binding
        void clear()
        {
            mBindBy = NOTHING;
         }

        /// binding utilities
        //@{
        bool         isBindByName() const { return NAME == mBindBy; }

       bool         isBindByIndex() const { return INDEX == mBindBy; }
        const char * getBindingName() const { GN_ASSERT( isBindByName() ); return mBindingName; }
        size_t       getBindingIndex() const { GN_ASSERT( isBindByIndex() ); return mBindingIndex; }
        void         bindTo( const char * name ) { doBindByName( name ); }
        void         bindTo( size_t index ) { mBindingIndex = index; mBindBy = INDEX; }
        //@}

    private:

        enum BindBy
        {
            NOTHING,
            NAME,
            INDEX,
        };

        union
        {
            size_t              mBindingIndex;
            char                mBindingName[16];
        };

        BindBy                  mBindBy;

    private:

        void doBindByName( const char * name )
        {
            size_t len = StringLength( name );
            if( 0 == len )
            {
                GN_ERROR(getLogger("GN.gfx.GpuProgramResourceBinding"))( "Empty binding string is not allowed." );
                return;
            }

            if( len >= GN_ARRAY_COUNT(mBindingName) )
            {
                GN_ERROR(getLogger("GN.gfx.GpuProgramResourceBinding"))(
                    "GPU program parameter name (%s) is too long. Maxinum length is %d characters including ending zero.",
                    name,
                    GN_ARRAY_COUNT(mBindingName) );
            }
            len = math::getmin<size_t>( GN_ARRAY_COUNT(mBindingName), len+1 );
            memcpy( mBindingName, name, len );
            mBindBy = NAME;
        }
    };
}}

class MiscGfxTest : public CxxTest::TestSuite
{
public:

    void testGpuProgramResourceBinding()
    {
        using namespace GN;
        using namespace GN::gfx;

        GpuProgramResourceBinding b;

        TS_ASSERT( !b.isBindByIndex() );
        TS_ASSERT( !b.isBindByName() );

        b.bindTo( "t0" );
        TS_ASSERT( b.isBindByName() );
        TS_ASSERT( !b.isBindByIndex() );
        TS_ASSERT_EQUALS( "t0", b.getBindingName() );

        b.bindTo( 2 );
        TS_ASSERT( !b.isBindByName() );
        TS_ASSERT( b.isBindByIndex() );
        TS_ASSERT_EQUALS( 2, b.getBindingIndex() );
    }

    void testImageDesc()
    {
        using namespace GN;
        using namespace GN::gfx;

        ImageDesc id;

        id.format.alias = ColorFormat::RGBA_8_8_8_8_UNORM;
        id.SetFaceAndLevel( 1, 1 );

        MipmapDesc & md = id.mipmaps[0];
        md.width = 0x4000;
        md.height = 0x4000;
        md.depth = 1;
        md.rowPitch = 0x10000;
        md.slicePitch = 0x40000000;
        md.levelPitch = 0x40000000;

        size_t totalBytes = id.getTotalBytes();
        TS_ASSERT_EQUALS( totalBytes, md.levelPitch );
    }
};
