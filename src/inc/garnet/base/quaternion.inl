//
//
// -----------------------------------------------------------------------------
template<typename T>
GN::Quaternion<T> & GN::Quaternion<T>::fromMatrix33(const Matrix33<T> & m) {
    ElementType Tr = m[0][0] + m[1][1] + m[2][2] + 1, fourD;
    int         i, j, k;

    // w >= 0.5 ?
    if (Tr >= 1.0f) {
        fourD = 2.0f * (ElementType) sqrt(Tr);
        w     = fourD / 4.0f;
        v.x   = (m[2][1] - m[1][2]) / fourD;
        v.y   = (m[0][2] - m[2][0]) / fourD;
        v.z   = (m[1][0] - m[0][1]) / fourD;
    } else {
        // Find the largest component.
        if (m[0][0] > m[1][1]) {
            i = 0;
        } else {
            i = 1;
        }
        if (m[2][2] > m[i][i]) { i = 2; }

        // Set j and k to point to the next two components
        j = (i + 1) % 3;
        k = (j + 1) % 3;

        // fourD = 4 * largest component
        fourD = 2.0f * (ElementType) sqrt(m[i][i] - m[j][j] - m[k][k] + 1.0f);

        // Set the largest component
        (*this)[i] = fourD / 4.0f;

        // Calculate remaining components
        (*this)[j] = (m[j][i] + m[i][j]) / fourD;
        (*this)[k] = (m[k][i] + m[i][k]) / fourD;
        (*this)[3] = (m[k][j] - m[j][k]) / fourD;
    }

    return *this;
}

//
//
// -----------------------------------------------------------------------------
template<typename T>
void GN::Quaternion<T>::toMatrix33(Matrix33<T> & out) const {
    /*
    If q is guaranteed to be a unit quaternion, s will always
    be 1.  In that case, this calculation can be optimized out.
    */
    ElementType norm = v.x * v.x + v.y * v.y + v.z * v.z + w * w, s = (norm > 0) ? 2 / norm : 0,

                /*
                Precalculate coordinate products
                */
        xx = v.x * v.x * s, yy = v.y * v.y * s, zz = v.z * v.z * s, xy = v.x * v.y * s, xz = v.x * v.z * s, yz = v.y * v.z * s, wx = w * v.x * s,
                wy = w * v.y * s, wz = w * v.z * s;

    /*
    Calculate 3x3 matrix from orthonormal basis
    */

    /*
    x axis
    */
    out[0][0] = 1.0f - (yy + zz);
    out[1][0] = xy + wz;
    out[2][0] = xz - wy;

    /*
    y axis
    */
    out[0][1] = xy - wz;
    out[1][1] = 1.0f - (xx + zz);
    out[2][1] = yz + wx;

    /*
    z axis
    */
    out[0][2] = xz + wy;
    out[1][2] = yz - wx;
    out[2][2] = 1.0f - (xx + yy);
}

//
//
// -----------------------------------------------------------------------------
template<typename T>
void GN::Quaternion<T>::toMatrix44(Matrix44<T> & out) const {
    /*
    If q is guaranteed to be a unit quaternion, s will always
    be 1.  In that case, this calculation can be optimized out.
    */
    ElementType norm = v.x * v.x + v.y * v.y + v.z * v.z + w * w, s = (norm > 0) ? 2 / norm : 0,

                /*
                Precalculate coordinate products
                */
        xx = v.x * v.x * s, yy = v.y * v.y * s, zz = v.z * v.z * s, xy = v.x * v.y * s, xz = v.x * v.z * s, yz = v.y * v.z * s, wx = w * v.x * s,
                wy = w * v.y * s, wz = w * v.z * s;

    /*
    Calculate 4x4 matrix from orthonormal basis
    */

    /*
    x axis
    */
    out[0][0] = 1.0f - (yy + zz);
    out[1][0] = xy + wz;
    out[2][0] = xz - wy;
    out[3][0] = 0.0f;

    /*
    y axis
    */
    out[0][1] = xy - wz;
    out[1][1] = 1.0f - (xx + zz);
    out[2][1] = yz + wx;
    out[3][1] = 0.0f;

    /*
    z axis
    */
    out[0][2] = xz + wy;
    out[1][2] = yz - wx;
    out[2][2] = 1.0f - (xx + yy);
    out[3][2] = 0.0f;

    out[0][3] = 0.0f;
    out[1][3] = 0.0f;
    out[2][3] = 0.0f;
    out[3][3] = 1.0f;
}
