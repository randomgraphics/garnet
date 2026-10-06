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
    uint32_t        width;   ///< character width, in pixel
    uint32_t        height;  ///< character height, in pixel
    int32_t         xorig;   ///< origin of the image
    int32_t         yorig;   ///< origin of the image
    int32_t         advance; ///< x-advance of the character
    const uint8_t * bitmap;  ///< character image
};
extern const BitmapCharDesc * const gBitmapChars8x13[]; ///< 8x13 ASCII bitmaps.

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_CHARBITMAP_H__
