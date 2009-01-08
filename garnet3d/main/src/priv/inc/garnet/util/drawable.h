#ifndef __GN_UTIL_DRAWABLE_H__
#define __GN_UTIL_DRAWABLE_H__
// *****************************************************************************
/// \file
/// \brief  Drawable class
/// \author  chenli@@REDMOND (2009.1.7)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN { namespace util
{
    class GpuProgramParam : public RefCounter
    {
        void * const mData;
        size_t const mSize;
        bool   const mShared;

    protected:

        /// constructor
        GpuProgramParam( bool shared, size_t size )
            : mData( heapAlloc( size ) )
            , mSize( size )
            , mShared( shared )
        {
        }

    public:

        /// dtor
        virtual ~GpuProgramParam()
        {
            heapFree( mData );
        }

        const void * get() const { return mData; }
        void         set( const void * data, size_t length ) { memcpy( mData, data, std::min(length,mSize) ); }
        bool         shared() const { return mShared; }
        size_t       size() const { return mSize; }
    };

    /// must call these 2 functions to create new GPU program parameter instance
    //@{
    GpuProgramParam * createPrivateGpuProgramParam( size_t size );
    GpuProgramParam * createSharedGpuProgramParam( const StrA & name, size_t size );
    //@}

    ///
    /// Drawable object, which is the building block of all visible objects.
    ///
    struct Drawable
    {
        gfx::Renderer                      * rndr;
        gfx::RendererContext                 rc;
        DynaArray<AutoRef<GpuProgramParam> > gpps; ///< GPU program parameters

        // parameters for DRAW_PRIMITIVE
        gfx::PrimitiveType prim;
        size_t             numvtx;
        size_t             startvtx;

        // additional parameters for DRAW_INDEXED_PRIMITIVE
        size_t             numidx;
        size_t             minvtxidx;
        size_t             startidx;

        ///
        /// clear to empty drawable
        ///
        void clear()
        {
            rndr = NULL;
            rc.resetToDefault();
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
#endif // __GN_UTIL_DRAWABLE_H__
