#ifndef __GN_D3D9_D3D9CHARBITMAP_H__
#define __GN_D3D9_D3D9CHARBITMAP_H__
// *****************************************************************************
//! \file    d3d9/d3d9CharBitmap.h
//! \brief   Bitmap font image (for Xenon only)
//! \author  chenlee (2006.5.26)
// *****************************************************************************

#if GN_XENON

struct BitmapCharDesc
{
    uint32_t width;
    uint32_t height;
    int32_t  xorig;
    int32_t  yorig;
    int32_t  advance;
    const uint8_t * bitmap;
};

extern const BitmapCharDesc gBitmapChars8x13[];

#endif // GN_XENON

// *****************************************************************************
//                           End of d3d9CharBitmap.h
// *****************************************************************************
#endif // __GN_D3D9_D3D9CHARBITMAP_H__
