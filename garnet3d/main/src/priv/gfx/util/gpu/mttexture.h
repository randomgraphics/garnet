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
        MultiThreadTexture( MultiThreadGpu & r ) : mGpu(r) { clear(); }
        virtual ~MultiThreadTexture() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( Texture * );
        void quit();
    private:
        void clear() { mTexture = NULL; }
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

        void   updateMipmap( uint32              face,
                             uint32              level,
                             const Box<uint32> * area,
                             uint32              rowPitch,
                             uint32              slicePitch,
                             const void        * data,
                             SurfaceUpdateFlag   flag );
        void   readMipmap( uint32 face, uint32 level, MipmapData & data );
        void   blobWrite( const void * data, uint32 length );
        uint32 blobRead( void * data );
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
