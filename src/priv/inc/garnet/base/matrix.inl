namespace GN
{
    // *************************************************************************
    // 3x3 matrix
    // *************************************************************************

    //
    // Multiply the Matrix33<T> by another Matrix33<T>
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix33<T> & Matrix33<T>::operator *= ( const Matrix33<T> & m )
    {
        Matrix33<T> t;

        #define M33_CALCULATE_ONE_CELL(r, c) t[r][c] = rows[r][0] * m[0][c] + \
                                                       rows[r][1] * m[1][c] + \
                                                       rows[r][2] * m[2][c];

        M33_CALCULATE_ONE_CELL(0, 0);
        M33_CALCULATE_ONE_CELL(0, 1);
        M33_CALCULATE_ONE_CELL(0, 2);

        M33_CALCULATE_ONE_CELL(1, 0);
        M33_CALCULATE_ONE_CELL(1, 1);
        M33_CALCULATE_ONE_CELL(1, 2);

        M33_CALCULATE_ONE_CELL(2, 0);
        M33_CALCULATE_ONE_CELL(2, 1);
        M33_CALCULATE_ONE_CELL(2, 2);

        #undef M33_CALCULATE_ONE_CELL

        *this = t;

        return *this;
    }

    //
    // get inverse of a 3x3 matrix
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix33<T> & Matrix33<T>::inverse()
    {

        // Invert a 3x3 using cofactors.  This is about 8 times faster than
        // the Numerical Recipes code which uses Gaussian elimination.
        Matrix33<T> src = *this;
        Matrix33<T>  & result = *this;

        result[0][0] = src[1][1]*src[2][2] - src[1][2]*src[2][1];
        result[0][1] = src[0][2]*src[2][1] - src[0][1]*src[2][2];
        result[0][2] = src[0][1]*src[1][2] - src[0][2]*src[1][1];
        result[1][0] = src[1][2]*src[2][0] - src[1][0]*src[2][2];
        result[1][1] = src[0][0]*src[2][2] - src[0][2]*src[2][0];
        result[1][2] = src[0][2]*src[1][0] - src[0][0]*src[1][2];
        result[2][0] = src[1][0]*src[2][1] - src[1][1]*src[2][0];
        result[2][1] = src[0][1]*src[2][0] - src[0][0]*src[2][1];
        result[2][2] = src[0][0]*src[1][1] - src[0][1]*src[1][0];

        T fDet =
            src[0][0]*result[0][0]+
            src[0][1]*result[1][0]+
            src[0][2]*result[2][0];

        if( ::fabs(fDet) == 0.0f )
        {
            // Uninvertable matrix is rare used in 3D graphics, and usually
            // means error. So we output a warning message here.
            static Logger * sLogger = GetLogger("GN.base.Matrix33");
            GN_WARN(sLogger)( "Matrix is un-invertable!" );
            return identity();
        }

        T fInvDet = 1.0f/fDet;
        for (int iRow = 0; iRow < 3; iRow++)
        {
            for (int iCol = 0; iCol < 3; iCol++)
                result[iRow][iCol] *= fInvDet;
        }

        // success
        return *this;
    }

    //
    // generate a X-rotate matrix by 'angle' is in radians
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix33<T> & Matrix33<T>::rotateX( T angle )
    {
        // 1    0    0
        // 0  cos -sin
        // 0  sin  cos

        T s = (T)::sin( angle );
        T c = (T)::cos( angle );

        identity();

        rows[1][1] = c;
        rows[1][2] = -s;
        rows[2][1] = s;
        rows[2][2] = c;

        return *this;
    }

    //
    // generate a Y-rotate matrix by 'angle' is in radians
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix33<T> & Matrix33<T>::rotateY( T angle )
    {
        //  cos  0  sin
        //  0    1    0
        // -sin  0  cos

        T s = ::sin( angle );
        T c = ::cos( angle );

        identity();

        rows[0][0] = c;
        rows[0][2] = s;
        rows[2][0] = -s;
        rows[2][2] = c;

        return *this;
    }

    //
    // generate a Z-rotate matrix by 'angle' is in radians
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix33<T> & Matrix33<T>::rotateZ( T angle )
    {
        // cos -sin  0
        // sin  cos  0
        //   0    0  1

        T s = ::sin( angle );
        T c = ::cos( angle );

        identity();

        rows[0][0] = c;
        rows[0][1] = -s;
        rows[1][0] = s;
        rows[1][1] = c;

        return *this;
    }

    //
    // generate a arbitrary rotation matrix, angle is in radians
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix33<T> & Matrix33<T>::rotate( const Vector3<T> & v, T angle )
    {
        T sinA, cosA;
        T invCosA;
        Vector3<T> nrm = v;
        T x, y, z;
        T xSq, ySq, zSq;

        nrm.normalize();
        sinA = ::sin( angle );
        cosA = ::cos( angle );
        invCosA = 1.0F - cosA;

        x = nrm.x;
        y = nrm.y;
        z = nrm.z;

        xSq = x * x;
        ySq = y * y;
        zSq = z * z;

        rows[0][0] = (invCosA * xSq) + (cosA);
        rows[0][1] = (invCosA * x * y) - (sinA * z );
        rows[0][2] = (invCosA * x * z) + (sinA * y );

        rows[1][0] = (invCosA * x * y) + (sinA * z);
        rows[1][1] = (invCosA * ySq) + (cosA);
        rows[1][2] = (invCosA * y * z) - (sinA * x);

        rows[2][0] = (invCosA * x * z) - (sinA * y);
        rows[2][1] = (invCosA * y * z) + (sinA * x);
        rows[2][2] = (invCosA * zSq) + (cosA);

        return *this;
    }

    //
    //
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix33<T> &
    Matrix33<T>::lookAtLh( const Vector3<T> & forward, const Vector3<T> & up )
    {
        Vector3<T> F = forward;
        F.normalize();

        Vector3<T> S = Vector3<T>::sCross( Vector3<T>::sNormalize(up), F );
        S.normalize();

        Vector3<T> U = Vector3<T>::sCross( F, S );
        U.normalize();

        rows[0][0] = S.x;
        rows[0][1] = S.y;
        rows[0][2] = S.z;

        rows[1][0] = U.x;
        rows[1][1] = U.y;
        rows[1][2] = U.z;

        rows[2][0] = F.x;
        rows[2][1] = F.y;
        rows[2][2] = F.z;

        // success
        return *this;
    }

    //
    //
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix33<T> &
    Matrix33<T>::lookAtRh( const Vector3<T> & forward, const Vector3<T> & up )
    {
        Vector3<T> F = forward;
        F.normalize();

        Vector3<T> S = Vector3<T>::sCross( F, Vector3<T>::sNormalize(up) );
        S.normalize();

        Vector3<T> U = Vector3<T>::sCross( S, F );
        U.normalize();

        rows[0][0] = S.x;
        rows[0][1] = S.y;
        rows[0][2] = S.z;

        rows[1][0] = U.x;
        rows[1][1] = U.y;
        rows[1][2] = U.z;

        rows[2][0] = -F.x;
        rows[2][1] = -F.y;
        rows[2][2] = -F.z;

        // success
        return *this;
    }


    //
    // 将矩阵的内容打印到字符串中
    // -------------------------------------------------------------------------
    template < typename T >
    void Matrix33<T>::print( StrA & s ) const
    {
        s.Format(
            "%f,\t%f,\t%f\n"
            "%f,\t%f,\t%f\n"
            "%f,\t%f,\t%f\n",
            rows[0][0], rows[0][1], rows[0][2],
            rows[1][0], rows[1][1], rows[1][2],
            rows[2][0], rows[2][1], rows[2][2] );
    }

    // *************************************************************************
    // 4x4 matrix
    // *************************************************************************

    //
    // Multiply the Matrix44<T> by another Matrix44<T>
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> & Matrix44<T>::operator *= ( const Matrix44<T> & m )
    {
        Matrix44<T> t;

        #define M44_CALCULATE_ONE_CELL(r, c)  t[r][c] = rows[r][0] * m[0][c] + \
                                                        rows[r][1] * m[1][c] + \
                                                        rows[r][2] * m[2][c] + \
                                                        rows[r][3] * m[3][c];

        M44_CALCULATE_ONE_CELL(0, 0);
        M44_CALCULATE_ONE_CELL(0, 1);
        M44_CALCULATE_ONE_CELL(0, 2);
        M44_CALCULATE_ONE_CELL(0, 3);

        M44_CALCULATE_ONE_CELL(1, 0);
        M44_CALCULATE_ONE_CELL(1, 1);
        M44_CALCULATE_ONE_CELL(1, 2);
        M44_CALCULATE_ONE_CELL(1, 3);

        M44_CALCULATE_ONE_CELL(2, 0);
        M44_CALCULATE_ONE_CELL(2, 1);
        M44_CALCULATE_ONE_CELL(2, 2);
        M44_CALCULATE_ONE_CELL(2, 3);

        M44_CALCULATE_ONE_CELL(3, 0);
        M44_CALCULATE_ONE_CELL(3, 1);
        M44_CALCULATE_ONE_CELL(3, 2);
        M44_CALCULATE_ONE_CELL(3, 3);

        #undef M44_CALCULATE_ONE_CELL

        *this = t;
        return *this;
    }

    //
    // Invert the matrix
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> & Matrix44<T>::inverse()
    {
        Matrix44<T> a(*this);
        Matrix44<T>  & b = *this;
        b.identity();

        int r, c;
        int cc;
        int rowMax; // Points to max abs value row in this column
        int row;
        T tmp;

        // Go through columns
        for (c=0; c<4; c++)
        {

            // Find the row with max value in this column
            rowMax = c;
            for (r=c+1; r<4; r++)
            {
                if(::fabs(a[r][c]) > ::fabs(a[rowMax][c]))
                {
                    rowMax = r;
                }
            }

            // Check if the matrix is invertible. If the max value here is 0, we
            // can't inverse.  Return identity.
            if (a[rowMax][c] == 0.0F)
            {
                // Uninvertible matrix is rare used in 3D graphics, and usually
                // means error. So we output a warning message here.
                static Logger * sLogger = GetLogger("GN.base.Matrix44");
                GN_WARN(sLogger)( "Matrix is un-invertable!" );
                return identity();
            }

            // Swap row "rowMax" with row "c"
            for (cc=0; cc<4; cc++)
            {
                tmp = a[c][cc];
                a[c][cc] = a[rowMax][cc];
                a[rowMax][cc] = tmp;
                tmp = b[c][cc];
                b[c][cc] = b[rowMax][cc];
                b[rowMax][cc] = tmp;
            }

            // Now everything we do is on row "c".
            // Set the max cell to 1 by dividing the entire row by that value
            tmp = a[c][c];
            for (cc=0; cc<4; cc++)
            {
                a[c][cc] /= tmp;
                b[c][cc] /= tmp;
            }

            // Now do the other rows, so that this column only has a 1 and 0's
            for (row = 0; row < 4; row++)
            {
                if (row != c)
                {
                    tmp = a[row][c];
                    for (cc=0; cc<4; cc++)
                    {
                        a[row][cc] -= a[c][cc] * tmp;
                        b[row][cc] -= b[c][cc] * tmp;
                    }
                }
            }

        }

        // success
        return *this;
    }

    //
    // generate a rotate matrix by X-axis, angle is in radians
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> & Matrix44<T>::rotateX( T angle )
    {
        // cos -sin  0   0
        // sin  cos  0   0
        //   0    0  1   0
        //   0    0  0   1

        T s = ::sin( angle );
        T c = ::cos( angle );

        identity();

        rows[1][1] = c;
        rows[1][2] = -s;
        rows[2][1] = s;
        rows[2][2] = c;

        return *this;
    }

    //
    // generate a rotate matrix by Y-axis, angle is in radians
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> & Matrix44<T>::rotateY( T angle )
    {
        //  cos  0  sin   0
        //  0    1    0   0
        // -sin  0  cos   0
        //  0    0    0   1

        T s = ::sin( angle );
        T c = ::cos( angle );

        identity();

        rows[0][0] = c;
        rows[0][2] = s;
        rows[2][0] = -s;
        rows[2][2] = c;

        return *this;
    }

    //
    // generate a rotate matrix by X-axis, angle is in radians
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> & Matrix44<T>::rotateZ( T angle )
    {
        // cos -sin   0    0
        // sin  cos   0    0
        //  0    0    1    0
        //  0    0    0    1

        T s = ::sin( angle );
        T c = ::cos( angle );

        identity();

        rows[0][0] = c;
        rows[0][1] = -s;
        rows[1][0] = s;
        rows[1][1] = c;

        return *this;
    }

    //
    // generate a rotate matrix by vector 'v', angle is in radians
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> &
    Matrix44<T>::rotate( const Vector3<T> & v, T angle )
    {
        Vector3<T> nrm = v;
        nrm.normalize();

        T sinA, cosA, invCosA;
        T x, y, z;
        T xSq, ySq, zSq;

        sinA = ::sin( angle );
        cosA = ::cos( angle );
        invCosA = 1.0F - cosA;

        x = nrm.x;
        y = nrm.y;
        z = nrm.z;

        xSq = x * x;
        ySq = y * y;
        zSq = z * z;

        rows[0][0] = (invCosA * xSq) + (cosA);
        rows[0][1] = (invCosA * x * y) - (sinA * z );
        rows[0][2] = (invCosA * x * z) + (sinA * y );
        rows[0][3] = 0.0F;

        rows[1][0] = (invCosA * x * y) + (sinA * z);
        rows[1][1] = (invCosA * ySq) + (cosA);
        rows[1][2] = (invCosA * y * z) - (sinA * x);
        rows[1][3] = 0.0F;

        rows[2][0] = (invCosA * x * z) - (sinA * y);
        rows[2][1] = (invCosA * y * z) + (sinA * x);
        rows[2][2] = (invCosA * zSq) + (cosA);
        rows[2][3] = 0.0F;

        rows[3][0] = 0.0F;
        rows[3][1] = 0.0F;
        rows[3][2] = 0.0F;
        rows[3][3] = 1.0F;

        return *this;
    }

    //
    //
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> &
    Matrix44<T>::lookAtLh( const Vector3<T> & eye,
                           const Vector3<T> & to,
                           const Vector3<T> & up )
    {
        Vector3<T> F = to - eye;
        F.normalize();

        Vector3<T> S = Vector3<T>::sCross( Vector3<T>::sNormalize(up), F );
        S.normalize();

        Vector3<T> U = Vector3<T>::sCross( F, S );
        U.normalize();

        rows[0][0] = S.x;
        rows[0][1] = S.y;
        rows[0][2] = S.z;
        rows[0][3] = 0.0f;

        rows[1][0] = U.x;
        rows[1][1] = U.y;
        rows[1][2] = U.z;
        rows[1][3] = 0.0f;

        rows[2][0] = F.x;
        rows[2][1] = F.y;
        rows[2][2] = F.z;
        rows[2][3] = 0.0f;

        rows[3][0] = 0.0f;
        rows[3][1] = 0.0f;
        rows[3][2] = 0.0f;
        rows[3][3] = 1.0f;

        Matrix44 mt;
        mt.translate( -eye );
        *this *= mt;

        return *this;
    }

    //
    //
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> &
    Matrix44<T>::lookAtRh( const Vector3<T> & eye,
                           const Vector3<T> & to,
                           const Vector3<T> & up )
    {
        Vector3<T> F = to - eye;
        F.normalize();

        Vector3<T> S = Vector3<T>::sCross( F, Vector3<T>::sNormalize(up) );
        S.normalize();

        Vector3<T> U = Vector3<T>::sCross( S, F );
        U.normalize();

        rows[0][0] = S.x;
        rows[0][1] = S.y;
        rows[0][2] = S.z;
        rows[0][3] = 0.0f;

        rows[1][0] = U.x;
        rows[1][1] = U.y;
        rows[1][2] = U.z;
        rows[1][3] = 0.0f;

        rows[2][0] = -F.x;
        rows[2][1] = -F.y;
        rows[2][2] = -F.z;
        rows[2][3] = 0.0f;

        rows[3][0] = 0.0f;
        rows[3][1] = 0.0f;
        rows[3][2] = 0.0f;
        rows[3][3] = 1.0f;

        Matrix44 mt;
        mt.translate( -eye );
        *this *= mt;

        return *this;
    }

    //
    // 计算opengl的正交投影矩阵
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> &
    Matrix44<T>::orthoOGLLh( T left, T right,
                             T bottom, T top,
                             T znear, T zfar )
    {
        GN_ASSERT( left != right && bottom != top && znear != zfar );

        T w = right - left,
                  h = top - bottom,
                  d = zfar - znear;

        identity();
        rows[0][0] = 2.0f / w;
        rows[1][1] = 2.0f / h;
        rows[2][2] = 2.0f / d;                  // 此处和directx不同
        rows[0][3] = - (left + right) / w;
        rows[1][3] = - (bottom + top) / h;
        rows[2][3] = - (znear + zfar) / d;      // 此处和directx不同
        rows[3][3] = 1.0f;

        return *this;
    }

    //
    // 计算opengl的正交投影矩阵
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> &
    Matrix44<T>::orthoOGLRh( T left, T right,
                             T bottom, T top,
                             T znear, T zfar )
    {
        GN_ASSERT( left != right && bottom != top && znear != zfar );

        T w = right - left,
                  h = top - bottom,
                  d = zfar - znear;

        identity();
        rows[0][0] = 2.0f / w;
        rows[1][1] = 2.0f / h;
        rows[2][2] = -2.0f / d;                 // 此处和directx不同
        rows[0][3] = - (left + right) / w;
        rows[1][3] = - (bottom + top) / h;
        rows[2][3] = - (znear + zfar) / d;      // 此处和directx不同
        rows[3][3] = 1.0f;

        return *this;
    }

    //
    // 计算directx的正交投影矩阵
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> &
    Matrix44<T>::orthoD3DLh( T left, T right,
                             T bottom, T top,
                             T znear, T zfar )
    {
        GN_ASSERT( left != right && bottom != top && znear != zfar );

        T w = right - left,
                  h = top - bottom,
                  d = zfar - znear;

        identity();
        rows[0][0] = 2.0f / w;
        rows[1][1] = 2.0f / h;
        rows[2][2] = 1.0f / d;                  // 此处和opengl不同
        rows[0][3] = - (left + right) / w;
        rows[1][3] = - (bottom + top) / h;
        rows[2][3] = - znear / d;               // 此处和opengl不同
        rows[3][3] = 1.0f;

        return *this;
    }

    //
    // 计算directx的正交投影矩阵
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> &
    Matrix44<T>::orthoD3DRh( T left, T right,
                             T bottom, T top,
                             T znear, T zfar )
    {
        GN_ASSERT( left != right && bottom != top && znear != zfar );

        T w = right - left,
                  h = top - bottom,
                  d = zfar - znear;

        identity();
        rows[0][0] = 2.0f / w;
        rows[1][1] = 2.0f / h;
        rows[2][2] = -1.0f / d;                 // 此处和opengl不同
        rows[0][3] = - (left + right) / w;
        rows[1][3] = - (bottom + top) / h;
        rows[2][3] = - znear / d;               // 此处和opengl不同
        rows[3][3] = 1.0f;

        return *this;
    }

    //
    // 计算opengl的透视投影矩阵
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> &
    Matrix44<T>::perspectiveOGLLh( T fovy, T ratio,
                                   T znear, T zfar )
    {
        GN_ASSERT( 0.0f != fovy && 0.0f != ratio && znear != zfar );

        T h = tan( fovy / 2.0f ) * znear * 2.0f,
                  w = h * ratio,
                  d = zfar - znear;

        identity();
        rows[0][0] = 2.0f * znear / w;
        rows[1][1] = 2.0f * znear / h;
        rows[2][2] = (znear + zfar) / d;        // 此处和directx不同
        rows[3][2] = 1.0f;
        rows[2][3] = -2.0f * znear * zfar / d;  // 此处和directx不同
        rows[3][3] = 0.0f;

        return *this;
    }

    //
    // 计算opengl的透视投影矩阵
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> &
    Matrix44<T>::perspectiveOGLRh( T fovy, T ratio,
                                   T znear, T zfar )
    {
        GN_ASSERT( 0.0f != fovy && 0.0f != ratio && znear != zfar );

        T h = tan( fovy / 2.0f ) * znear * 2.0f,
                  w = h * ratio,
                  d = zfar - znear;

        identity();
        rows[0][0] = 2.0f * znear / w;
        rows[1][1] = 2.0f * znear / h;
        rows[2][2] = -(znear + zfar) / d;       // 此处和directx不同
        rows[3][2] = -1.0f;
        rows[2][3] = -2.0f * znear * zfar / d;  // 此处和directx不同
        rows[3][3] = 0.0f;

        return *this;
    }

    //
    // 计算directx的透视投影矩阵
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> &
    Matrix44<T>::perspectiveD3DLh( T fovy, T ratio,
                                   T znear, T zfar )
    {
        GN_ASSERT( 0.0f != fovy && 0.0f != ratio && znear != zfar );

        T h = tan( fovy/2.0f ) * znear * 2.0f,
        w = h * ratio,
        d = zfar - znear;

        identity();
        rows[0][0] = 2.0f * znear / w;
        rows[1][1] = 2.0f * znear / h;
        rows[2][2] = zfar / d;                  // 此处和opengl不同
        rows[3][2] = 1.0f;
        rows[2][3] = -zfar * znear / d;         // 此处和opengl不同
        rows[3][3] = 0.0f;

        return *this;
    }

    //
    // 计算directx的透视投影矩阵
    // -------------------------------------------------------------------------
    template < typename T >
    Matrix44<T> &
    Matrix44<T>::perspectiveD3DRh( T fovy, T ratio,
                                   T znear, T zfar )
    {
        GN_ASSERT( 0.0f != fovy && 0.0f != ratio && znear != zfar );

        T h = tan( fovy/2.0f ) * znear * 2.0f,
                  w = h * ratio,
                  d = zfar - znear;

        identity();
        rows[0][0] = 2.0f * znear / w;
        rows[1][1] = 2.0f * znear / h;
        rows[2][2] = -zfar / d;                 // 此处和opengl不同
        rows[3][2] = -1.0f;
        rows[2][3] = -zfar * znear / d;         // 此处和opengl不同
        rows[3][3] = 0.0f;

        return *this;
    }

    //
    //  将矩阵的内容打印到字符串中
    // -------------------------------------------------------------------------
    template < typename T >
    void Matrix44<T>::print( StrA & s ) const
    {
		s.Format(
            "%f,\t%f,\t%f,\t%f,\n"
            "%f,\t%f,\t%f,\t%f,\n"
            "%f,\t%f,\t%f,\t%f,\n"
            "%f,\t%f,\t%f,\t%f,\n",
            rows[0][0], rows[0][1], rows[0][2], rows[0][3],
            rows[1][0], rows[1][1], rows[1][2], rows[1][3],
            rows[2][0], rows[2][1], rows[2][2], rows[2][3],
            rows[3][0], rows[3][1], rows[3][2], rows[3][3] );
    }
}
