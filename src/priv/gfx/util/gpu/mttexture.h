#ifndef __GN_GFX_UTIL_GPU_MTTEXTURE_H__
#define __GN_GFX_UTIL_GPU_MTTEXTURE_H__
// *****************************************************************************
/// \file
/// \brief   multi-thread texture wrappers
/// \author  chenli@@REDMOND (2009.1.3)
// *****************************************************************************

#include "mtgpu.h"

namespace GN { namespace gfx
{
    ///
    /// multi thread texture wrapper
    ///
    class MultiThreadTexture : public Texture, public StdClass
    {
        GN_DECLARE_STDCLASS( MultiThreadTexture, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        MultiThreadTexture( MultiThreadGpu & r ) : mGpu(r) { Clear(); }
        virtual ~MultiThreadTexture() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( Texture * );
        void Quit();
    private:
        void Clear() { mTexture = NULL; }
        //@}

        // ********************************
        // public methods
        // ********************************
    public:

        Texture * getRealTexture() const { return mTexture; }

        // ********************************
        // from Texture
        // ********************************
    public:

        void   updateMipmap( size_t              face,
                             size_t              level,
                             const Box<UInt32> * area,
                             size_t              rowPitch,
                             size_t              slicePitch,
                             const void        * data,
                             SurfaceUpdateFlag   flag );
        void   readMipmap( size_t face, size_t level, MipmapData & data );
        void   blobWrite( const void * data, size_t length );
        size_t blobRead( void * data );
        void   generateMipmapPyramid();
        void * getAPIDependentData() const;

        // ********************************
        // private variables
        // ********************************
    private:

        MultiThreadGpu & mGpu;
        Texture             * mTexture;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UTIL_GPU_MTTEXTURE_H__
