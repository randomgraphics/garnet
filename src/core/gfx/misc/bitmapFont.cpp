#include "pch.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.util.BitmapFont");

//
//
// -----------------------------------------------------------------------------
static bool sDetermineTextureSizeAndCount(size_t & texwidth, size_t & texheight, size_t & texcount, Gpu & gpu, size_t rectw, size_t recth, size_t maxchars) {
    // determine texture size
    size_t maxw = gpu.caps().maxTex2DSize[0];
    size_t maxh = gpu.caps().maxTex2DSize[1];
    size_t maxc = gpu.caps().maxTextures;

    for (texcount = 1; texcount <= maxc; ++texcount) {
        for (texwidth = texheight = 1; (texwidth <= maxw) && (texheight <= maxh); texwidth <<= 1, texheight <<= 1) {
            size_t numcols = texwidth / rectw;
            size_t numrows = texheight / recth;
            if (texcount * numcols * numrows >= maxchars) { return true; }
        }
    }

    return false;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BitmapFont::init(SpriteRenderer * sr, FontFace * ff, size_t maxchars) {
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    if (NULL == sr || NULL == ff) {
        GN_ERROR(sLogger)("Null parameters.");
        return failure();
    }

    Gpu &                gpu = sr->getGpu();
    const FontFaceDesc & ffd = ff->getDesc();

    // initialize font slots
    if (!slotInit(gpu, ffd.maxGlyphWidth(), ffd.maxGlyphHeight(), maxchars)) return failure();

    // create character list
    for (int i = 0; i < MAX_TEXTURES; ++i) {
        mCharList[i] = new CharInfo[mMaxSlots];
        mNumChars[i] = 0;
    }

    // success
    mSpriteRenderer = sr;
    mFont.set(ff);
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BitmapFont::quit() {
    GN_GUARD;

    // delete character array
    for (int i = 0; i < MAX_TEXTURES; ++i) { safeDeleteArray(mCharList[i]); }

    // delete font map array
    safeDeleteArray(mFontSlots);

    // delete per-texture resources
    mTextures.clear();

    // deref font face
    mFont.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
//                       public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BitmapFont::drawText(const TextDesc & td) {
    GN_GUARD_SLOW;

    if (str::empty(td.text)) return; // skip empty text.

    GN_ASSERT(mFont);

    // initialize local variables
    const FontSlot *     fs;
    CharInfo *           ci;
    size_t               index   = 0;
    float                penx    = td.x;
    float                peny    = td.y;
    const wchar_t *      text    = td.text;
    size_t               textlen = (0 == td.len) ? static_cast<size_t>(-1) : td.len;
    const FontFaceDesc & ffd     = mFont->getDesc();

    // align baseline distance to interger for better clarity
    float baselineDistance = ceil(ffd.vdistance);

    Rectf bbox(td.x + ffd.xmin, td.y + ffd.ymin, 0, baselineDistance);

    const float TAB_SIZE = 8.0f;

    // draw characters one by one
    while (*text && index < textlen) {
        wchar_t ch = *text;
        ++text;
        ++index;

        if (index > mMaxSlots) {
            GN_ERROR(sLogger)("text is too long!");
            return;
        }

        if (L'\n' == ch) {
            penx = td.x;
            peny += baselineDistance;
            bbox.h += baselineDistance;
        } else if (L'\t' == ch) {
            fs = getSlot(L' ');
            if (NULL == fs) return;

            float tabx = fs->advx * TAB_SIZE;
            penx       = floor((penx + tabx - 1) / tabx) * tabx;
        } else if (L' ' == ch) {
            fs = getSlot(L' ');
            if (NULL == fs) return;
            penx += fs->advx;
        } else // normal character
        {
            // find the slot
            fs = getSlot(ch);
            if (NULL == fs) return;

            // add character to charlist
            size_t texidx = fs->texidx;
            ci            = &mCharList[texidx][mNumChars[texidx]];
            ci->fs        = fs;
            ci->x         = penx + fs->offx;
            ci->y         = peny + fs->offy;
            ++mNumChars[texidx];

            // increase pen position
            penx += fs->advx;

            // adjust bounding box
            bbox.w = math::getmax(penx - td.x, bbox.w);
        }
    }

    // draw background box
    if (td.background) {
        mSpriteRenderer->drawSingleSolidSprite(GN_RGBA8(0, 0, 0, 128), SpriteRenderer::TRANSPARENT_2D_IMAGE, bbox.x, bbox.y, bbox.w, bbox.h, td.z);
    }

    // draw all characters in character list
    for (size_t i = 0; i < mTextures.size(); ++i) {
        size_t numchars = mNumChars[i];
        mNumChars[i]    = 0;

        if (0 == numchars) continue;

        mSpriteRenderer->drawBegin(mTextures[i], SpriteRenderer::TRANSPARENT_2D_IMAGE);

        ci = mCharList[i];

        for (size_t c = 0; c < numchars; ++c, ++ci) {
            mSpriteRenderer->drawTextured(ci->x, ci->y, ci->fs->w, ci->fs->h, ci->fs->u, ci->fs->v, ci->fs->tw, ci->fs->th, td.z);
        };

        mSpriteRenderer->drawEnd();
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
//                       private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline const GN::gfx::BitmapFont::FontSlot * GN::gfx::BitmapFont::getSlot(wchar_t ch) {
    // find font slot in slotmap
    size_t * slot = mSlotMap.find(ch);
    if (NULL != slot) {
        // found!
        return &mFontSlots[*slot];
    } else {
        return createSlot(ch);
    }
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::BitmapFont::FontSlot * GN::gfx::BitmapFont::createSlot(wchar_t ch) {
    GN_GUARD_SLOW;

    // make sure that the slot does not exist.
    GN_ASSERT(NULL == mSlotMap.find(ch));

    if (mNumUsedSlots >= mMaxSlots) {
        GN_ERROR(sLogger)("Run out of charactor slots!");
        return NULL;
    }

    mSlotMap[ch] = mNumUsedSlots;
    GN_ASSERT(mNumUsedSlots == *mSlotMap.find(ch));
    FontSlot & slot = mFontSlots[mNumUsedSlots];
    ++mNumUsedSlots;

    // load font image
    FontImage fbm;
    if (!mFont->loadFontImage(fbm, ch)) {
        wchar_t s[] = {ch, L'\0'};
        GN_ERROR(sLogger)(L"fail to get font bitmap for character '%s'!", s);
        return NULL;
    }

    // update slot fields
    slot.ch   = ch;
    slot.offx = fbm.horiBearingX;
    slot.offy = fbm.horiBearingY;
    slot.advx = fbm.horiAdvance;
    slot.advy = fbm.vertAdvance;

    // copy font image into RGBA
    GN_ASSERT(fbm.width <= slot.w && fbm.height <= slot.h);
    DynaArray<uint8> tmpbuf(slot.w * slot.h * 4);
    std::fill(tmpbuf.begin(), tmpbuf.end(), (uint8) 0);
    for (size_t y = 0; y < fbm.height; ++y) {
        for (size_t x = 0; x < fbm.width; ++x) {
            if (FontImage::GRAYSCALE == fbm.format) {
                size_t s = y * fbm.width + x;
                size_t d = (y * slot.w + x) * 4;

#if GN_BIG_ENDIAN
                tmpbuf[d + 0] = fbm.buffer[s];
                tmpbuf[d + 1] = 0xFF;
                tmpbuf[d + 2] = 0xFF;
                tmpbuf[d + 3] = 0xFF;
#else
                tmpbuf[d + 0] = 0xFF;
                tmpbuf[d + 1] = 0xFF;
                tmpbuf[d + 2] = 0xFF;
                tmpbuf[d + 3] = fbm.buffer[s];
#endif
            } else {
                GN_ASSERT(FontImage::RGBA == fbm.format);

                size_t s = (y * fbm.width + x) * 4;
                size_t d = (y * slot.w + x) * 4;

#if GN_BIG_ENDIAN
                tmpbuf[d + 0] = fbm.buffer[s + 3];
                tmpbuf[d + 1] = fbm.buffer[s + 2];
                tmpbuf[d + 2] = fbm.buffer[s + 1];
                tmpbuf[d + 3] = fbm.buffer[s + 0];
#else
                tmpbuf[d + 0] = fbm.buffer[s + 0];
                tmpbuf[d + 1] = fbm.buffer[s + 1];
                tmpbuf[d + 2] = fbm.buffer[s + 2];
                tmpbuf[d + 3] = fbm.buffer[s + 3];
#endif
            }
        }
    }

    // update texture
    Box<uint32> area(slot.x, slot.y, 0, slot.w, slot.h, 1);
    mTextures[slot.texidx]->updateMipmap(0, 0, &area, slot.w * 4, (uint32) tmpbuf.size(), tmpbuf.data());

    // success
    return &slot;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BitmapFont::slotInit(Gpu & gpu, uint16 fontw, uint16 fonth, size_t maxchars) {
    GN_GUARD;

    uint16 rectw = fontw + 0;
    uint16 recth = fonth + 0;

    // determine texture size
    size_t texwidth, texheight, texcount;
    if (!sDetermineTextureSizeAndCount(texwidth, texheight, texcount, gpu, rectw, recth, maxchars)) {
        GN_ERROR(sLogger)("Fail to determine font texture size, please decrease font size or maxchars.");
        return false;
    }

    // create slot map array
    size_t numcols = texwidth / rectw;
    size_t numrows = texheight / recth;
    mMaxSlots      = numcols * numrows * texcount;
    GN_ASSERT(mMaxSlots >= maxchars);
    mFontSlots = new FontSlot[mMaxSlots];

    // initialize font slots
    float                       stepu = float(rectw) / texwidth;
    float                       stepv = float(recth) / texheight;
    uint16                      x, y;
    float                       u, v;
    SafeArrayAccessor<FontSlot> slot(mFontSlots, mMaxSlots);
    for (size_t itex = 0; itex < texcount; ++itex) {
        u = 0;
        x = 0;
        for (size_t icol = 0; icol < numcols; ++icol) {
            v = 0;
            y = 0;
            for (size_t irow = 0; irow < numrows; ++irow) {
                // setup slot
                GN_ASSERT(itex < 256);
                slot->texidx = (uint8) itex;
                slot->u      = u;
                slot->v      = v;
                slot->tw     = float(fontw) / texwidth;
                slot->th     = float(fonth) / texheight;
                slot->x      = x;
                slot->y      = y;
                slot->w      = fontw;
                slot->h      = fonth;

                // next row
                v += stepv;
                y = y + recth;

                // next slot
                ++slot;
            }

            // next column
            u += stepu;
            x = x + rectw;
        }
    }

    // all slots are free
    mNumUsedSlots = 0;

    // and slot map is also empty
    mSlotMap.clear();

    // DynaArray<uint8> texels( texwidth * texheight * 4 );
    // std::fill( texels.begin(), texels.end(), 0 );

    // create font textures
    TextureDesc td = {PixelFormat::RGBA_8_8_8_8_UNORM(), (uint32) texwidth, (uint32) texheight, 1, 1, 1, TextureUsage::DEFAULT};
    GN_ASSERT(texcount <= gpu.caps().maxTextures);
    mTextures.resize(texcount);
    for (size_t i = 0; i < texcount; ++i) {
        mTextures[i].attach(gpu.createTexture(td));

        if (0 == mTextures[i]) {
            GN_ERROR(sLogger)("fail to create font texture #%d!", i);
            return failure();
        }

        // clear texture to pure black
        // mTextures[i]->updateMipmap( 0, 0, 0, texwidth * 4, texels.size(), texels.data() );
    }

    // success
    return true;

    GN_UNGUARD;
}
