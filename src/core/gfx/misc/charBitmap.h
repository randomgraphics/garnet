#ifndef __GN_SCENE_CHARBITMAP_H__
#define __GN_SCENE_CHARBITMAP_H__
// *****************************************************************************
/// \file
/// \brief   Bitmap font image
/// \author  chenlee (2006.5.26)
// *****************************************************************************

///
/// Bitmap character descriptor
///
struct BitmapCharDesc {
    uint32        width;   ///< character width, in pixel
    uint32        height;  ///< character height, in pixel
    sint32        xorig;   ///< origin of the image
    sint32        yorig;   ///< origin of the image
    sint32        advance; ///< x-advance of the character
    const uint8 * bitmap;  ///< character image
};
extern const BitmapCharDesc * const gBitmapChars8x13[]; ///< 8x13 ASCII bitmaps.

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_CHARBITMAP_H__
