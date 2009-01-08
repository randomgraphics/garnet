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
        mutable bool mDirty;

    protected:

        /// constructor
        GpuProgramParam( bool shared, size_t size )
            : mData( heapAlloc( size ) )
            , mSize( size )
            , mShared( shared )
            , mDirty( false )
        {
        }

    public:

        /// dtor
        virtual ~GpuProgramParam()
        {
            heapFree( mData );
        }

        bool         dirty() const { return mShared || mDirty; } // note: shared parameter is always dirty
        const void * get() const { mDirty = false; return mData; }
        void         set( const void * data, size_t length ) { memcpy( mData, data, std::min(length,mSize) ); mDirty = true; }
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
        void draw() const
        {
            GN_ASSERT( rndr );

            // apply dirty parameters
            if( rc.gpuProgram )
            {
                for( size_t i = 0; i < gpps.size(); ++i )
                {
                    GpuProgramParam * gpp = gpps[i].get();

                    if( gpp && gpp->dirty() )
                    {
                        rc.gpuProgram->setParameter( i, gpp->get(), gpp->size() );
                    }
                }
            }

            // bind context
            rndr->bindContext( rc );

            // do rendering
            if( rc.idxbuf )
            {
                rndr->drawIndexed( prim, numidx, startvtx, minvtxidx, numvtx, startidx );
            }
            else
            {
                rndr->draw( prim, numvtx, startvtx );
            }
        }

        /// assignment
        Drawable & operator=( const Drawable & rhs )
        {
            rndr = rhs.rndr;
            rc = rhs.rc;
            prim = rhs.prim;
            numvtx = rhs.numvtx;
            numidx = rhs.numidx;
            minvtxidx = rhs.minvtxidx;
            startidx = rhs.startidx;

            // copy parameters
            gpps.resize( rhs.gpps.size() );
            for( size_t i = 0; i < gpps.size(); ++i )
            {
                AutoRef<GpuProgramParam> & d = gpps[i];
                const AutoRef<GpuProgramParam> & s = rhs.gpps[i];

                if( s->shared() )
                {
                    d = s;
                }
                else if( s )
                {
                    // make a clone for private parameter
                    d.attach( createPrivateGpuProgramParam( s->size() ) );
                    d->set( s->get(), s->size() );
                }
            }

            return *this;
        }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_UTIL_DRAWABLE_H__
