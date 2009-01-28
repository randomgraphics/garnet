#ifndef __GN_GFX_DRAWABLE_H__
#define __GN_GFX_DRAWABLE_H__
// *****************************************************************************
/// \file
/// \brief  Drawable class
/// \author  chenli@@REDMOND (2009.1.7)
// *****************************************************************************

namespace GN { namespace gfx
{
    class GpuProgramParam : public RefCounter
    {
        void * const mData;
        size_t const mSize;

    public:

        /// constructor
        GpuProgramParam( size_t size )
            : mData( heapAlloc( size ) )
            , mSize( size )
        {
        }

        /// dtor
        virtual ~GpuProgramParam()
        {
            heapFree( mData );
        }

        const void * get() const { return mData; }
        void         set( const void * data, size_t length ) { memcpy( mData, data, std::min(length,mSize) ); }
        template<typename T>
        void         set( const T & t ) { set( &t, sizeof(t) ); }
        size_t       size() const { return mSize; }
    };

    ///
    /// Drawable object, which is the building block of all visible objects.
    ///
    struct Drawable
    {
        Renderer                           * rndr;
        RendererContext                      rc;
        DynaArray<AutoRef<GpuProgramParam> > gpps; ///< GPU program parameters

        // parameters for DRAW_PRIMITIVE
        PrimitiveType prim;
        size_t        startvtx;
        size_t        numvtx;

        // additional parameters for DRAW_INDEXED_PRIMITIVE
        size_t        startidx;
        size_t        numidx;
        size_t        basevtx;

        ///
        /// clear to empty drawable
        ///
        void clear()
        {
            rndr = NULL;
            rc.clear();
            gpps.clear();
        }

        ///
        /// draw the drawable
        ///
        void draw() const;

        /// assignment
        Drawable & operator=( const Drawable & );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_DRAWABLE_H__
