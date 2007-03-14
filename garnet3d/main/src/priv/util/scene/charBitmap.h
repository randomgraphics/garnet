#ifndef __GN_SCENE_CHARBITMAP_H__
#define __GN_SCENE_CHARBITMAP_H__
// *****************************************************************************
/// \file    scene/charBitmap.h
/// \brief   Bitmap font image
/// \author  chenlee (2006.5.26)
// *****************************************************************************

///
/// Bitmap character descriptor
///
struct BitmapCharDesc
{
    UInt32 width;  ///< character width, in pixel
    UInt32 height; ///< character height, in pixel
    SInt32  xorig;  ///< origin of the image
    SInt32  yorig;  ///< origin of the image
    SInt32  advance; ///< x-advance of the character
    const UInt8 * bitmap; ///< character image
};
extern const BitmapCharDesc * const gBitmapChars8x13[]; ///< 8x13 ASCII bitmaps.

// *****************************************************************************
//                           End of charBitmap.h
// *****************************************************************************
#endif // __GN_SCENE_CHARBITMAP_H__
