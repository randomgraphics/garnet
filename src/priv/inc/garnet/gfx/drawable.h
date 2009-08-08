#ifndef __GN_GFX_DRAWABLE_H__
#define __GN_GFX_DRAWABLE_H__
// *****************************************************************************
/// \file
/// \brief  Drawable class
/// \author  chenli@@REDMOND (2009.1.7)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// Drawable object, which is the building block of all visible objects.
    ///
    struct Drawable
    {
        Gpu         * gpu;
        GpuContext    rc;

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
            gpu = NULL;
            rc.clear();
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
