#include "pch.h"

// *****************************************************************************
// local functions
// *****************************************************************************

#define ENCODE_KEY(from, to) (((((int16_t) from) & 0xFF) << 8) | (((int16_t) to) & 0xFF))

static GN::Logger * sLogger = GN::getLogger("GN.base.Variant");

// *****************************************************************************
// public methods
// *****************************************************************************

//
// set value
// -----------------------------------------------------------------------------
GN_API void GN::Variant::setb(bool b) { mValue = b ? "1" : "0"; }
GN_API void GN::Variant::seti(int i) { mValue.formatInplace("{}", i); }
GN_API void GN::Variant::setf(float f) { mValue.formatInplace("{}", f); }
GN_API void GN::Variant::setp(void * p) {
    if (0 == p)
        mValue = "0";
    else
        mValue.format("{}", p);
}
GN_API void GN::Variant::setv(const Vector4f & v) { mValue.formatInplace("{},{},{},{}", v.x, v.y, v.z, v.w); }
GN_API void GN::Variant::setm(const Matrix44f & m) {
    mValue.formatInplace("{},{},{},{},\n"
                         "{},{},{},{},\n"
                         "{},{},{},{},\n"
                         "{},{},{},{}",
                         m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1], m[1][2], m[1][3], m[2][0], m[2][1], m[2][2], m[2][3], m[3][0], m[3][1], m[3][2],
                         m[3][3]);
}

//
// get value
// -----------------------------------------------------------------------------
GN_API bool GN::Variant::getb(bool & b) const {
    int   i;
    float f;
    if (0 == str::compareI("yes", mValue.data()) || 0 == str::compareI("true", mValue.data()) || 0 == str::compareI("on", mValue.data()) ||
        0 == str::compare("1", mValue.data())) {
        b = true;
        return true;
    } else if (0 == str::compareI("no", mValue.data()) || 0 == str::compareI("false", mValue.data()) || 0 == str::compareI("off", mValue.data()) ||
               0 == str::compare("0", mValue.data())) {
        b = false;
        return true;
    } else if (geti(i)) {
        b = 0 != i;
        return true;
    } else if (getf(f)) {
        b = .0f != f;
        return true;
    } else {
        GN_ERROR(sLogger)("Can't convert string '{}' to boolean.", mValue.data());
        return false;
    }
}
//
GN_API bool GN::Variant::geti(int & i) const { return 0 != str::toInetger<int>(i, mValue.data()); }
//
GN_API bool GN::Variant::getf(float & f) const { return 0 != str::toFloat(f, mValue.data()); }
//
GN_API bool GN::Variant::getp(void *& p) const {
    bool b = 0 != str::toInetger<size_t>((size_t &) p, mValue.data());
    if (!b) p = NULL;
    return b;
}
//
GN_API bool GN::Variant::getv(Vector4f & v) const { return 4 == str::toFloatArray(v, 4, mValue.data(), mValue.size()); }
//
GN_API bool GN::Variant::getm(Matrix44f & m) const { return 16 == str::toFloatArray(m[0], 16, mValue.data(), mValue.size()); }

// *****************************************************************************
// private methods
// *****************************************************************************
