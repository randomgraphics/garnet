#ifndef __GN_BASE_GEOMETRY_H__
#define __GN_BASE_GEOMETRY_H__
// *****************************************************************************
/// \file
/// \brief   geometry classes
/// \author  chenlee (2005.4.17)
// *****************************************************************************

// Garnet system uses right hand system by default. Define this macro to 1 to
// use left hand system.
//
// Note: changing of this macro requires recompile of the whole system.
#define GN_BUILD_LEFT_HAND 0

namespace GN
{
    namespace detail
    {
        ///
        /// Swap to elements
        ///
        template<typename T>
        inline void swap( T & a, T & b )
        {
            T tmp;
            tmp = a;
            a = b;
            b = tmp;
        }
    }

    ///
    /// 表示一个二维矢量（比如贴图坐标）。
    ///
    template < typename T >
    class Vector2
    {
    public :
        ///
        /// element type
        ///
        typedef T ElementType;

        // ********************************
        //      public data members
        // ********************************

    public :

        ///
        /// X coordinate
        ///
        union
        {
            T x;
            T u;
            T s;
            T width;
        };

        ///
        /// Y coordinate
        ///
        union
        {
            T y;
            T v;
            T t;
            T height;
        };

        // ********************************
        /// \name  constructors
        // ********************************

    public:
        //@{
        Vector2()                                      {}
        Vector2(const Vector2 & v) : x(v.x), y(v.y)    {}
        Vector2(T ix, T iy) : x(ix), y(iy) {}
        //@}

        // ********************************
        //         operators
        // ********************************

    public:
        ///
        /// Convert to T *
        ///
        operator T *() { return &x; }
        ///
        /// Convert to const T *
        ///
        operator const T *() const { return &x; }
        ///
        /// Are these two Vector2's equal?
        ///
        bool operator == ( const Vector2 & a ) const
        {
            return((x == a.x) && (y == a.y));
        }
        ///
        /// Are these two Vector2's not equal?
        ///
        bool operator != ( const Vector2 & a ) const
        {
            return((a.x != x) || (a.y != y));
        }
        ///
        /// less operator
        ///
        bool operator < ( const Vector2 & a ) const
        {
            return x < a.x || y < a.y;
        }
        ///
        /// Add a Vector2 to this one
        ///
        Vector2 & operator += ( const Vector2 & v )
        {
            x += v.x;
            y += v.y;
            return *this;
        }
        ///
        /// Subtract a Vector2 from this one
        ///
        Vector2 &operator -= ( const Vector2 & v )
        {
            x -= v.x;
            y -= v.y;
            return *this;
        }
        ///
        /// Multiply the Vector2 by a scalar
        ///
        Vector2 & operator *= ( T f )
        {
            x *= f;
            y *= f;
            return *this;
        }
        ///
        /// Multiplied by another vector
        ///
        Vector2 & operator *= ( const Vector2 & v )
        {
            x *= v.x;
            y *= v.y;
            return *this;
        }
        ///
        /// Divide the Vector2 by a scalar
        ///
        Vector2 & operator /= ( T f )
        {
            GN_ASSERT( ((T)0.0) != f );
            x /= f;
            y /= f;
            return *this;
        }
        ///
        /// Divided by another vector
        ///
        Vector2 & operator /= ( const Vector2 & v )
        {
            GN_ASSERT( v.x != ((T)0.0) && v.y != ((T)0.0) );
            x /= v.x;
            y /= v.y;
            return *this;
        }
        ///
        /// Negate this vector
        ///
        friend Vector2 operator - ( const Vector2 & a )
        {
            return Vector2(-a.x, -a.y);
        }
        ///
        /// Add two Vector2's
        ///
        friend Vector2 operator + ( const Vector2 & a, const Vector2 & b )
        {
            Vector2 ret(a);
            ret += b;
            return ret;
        }
        ///
        /// Subtract one Vector2 from another
        ///
        friend Vector2 operator - ( const Vector2 & a, const Vector2 & b )
        {
            Vector2 ret(a);
            ret -= b;
            return ret;
        }
        ///
        /// Multiply two vectors
        ///
        friend Vector2 operator * ( const Vector2 & a, const Vector2 & b )
        {
            return Vector2( a.x * b.x, a.y * b.y );
        }
        ///
        /// Multiply Vector2 by a scalar
        ///
        friend Vector2 operator * ( const Vector2 & v, T f )
        {
            return Vector2(f * v.x, f * v.y);
        }
        ///
        /// Multiply Vector2 by a scalar
        ///
        friend Vector2 operator * ( T f, const Vector2 & v )
        {
            return Vector2(f * v.x, f * v.y);
        }
        ///
        /// Divide Vector2 by a scalar
        ///
        friend Vector2 operator / ( const Vector2 & v, T f )
        {
            return Vector2( v.x / f, v.y / f );
        }
        ///
        /// Divide by another vector
        ///
        friend Vector2 operator / ( const Vector2 & a, const Vector2 & b )
        {
            return Vector2( a.x / b.x, a.y / b.y );
        }

        // ********************************
        //      public functions
        // ********************************

    public:
        ///
        /// type conversion
        ///
        const T * toPtr() const
        {
            return &x;
        }
        ///
        /// type conversion
        ///
        T * toPtr()
        {
            return &x;
        }
        ///
        /// Set Values
        ///
        Vector2 & set( T ix, T iy )
        {
            x = ix;
            y = iy;
            return *this;
        }
        ///
        /// Get length of a Vector2
        ///
        T length() const
        {
            return(T) sqrt(x*x + y*y);
        }
        ///
        /// Get squared length of a Vector2
        ///
        T lengthSqr() const
        {
            return(x*x + y*y);
        }
        ///
        /// Does Vector2 equal (0, 0)?
        ///
        bool isZero() const
        {
            return((x == 0.0F) && (y == 0.0F));
        }
        ///
        /// Normalize a Vector2
        ///
        Vector2 & normalize()
        {
			sNormalize( *this, *this );
            return *this;
        }

        ///
        /// Normalize a Vector2
        ///
        static Vector2 sNormalize( const Vector2 & v )
        {
            Vector2 r;
            sNormalize( r, v );
            return r;
        }

        ///
        /// Normalize a Vector2
        ///
        static void sNormalize( Vector2 & o, const Vector2 & i )
        {
            T m = i.length();

            if( m > 0.0F )
            {
                m = 1.0F / m;
                o.x = i.x * m;
                o.y = i.y * m;
            }
			else
			{
				o.x = ((T)0.0);
				o.y = ((T)0.0);
			}
        }

        ///
        /// dot production
        ///
        static T sDot( const Vector2 & v1, const Vector2 & v2 )
        {
            return v1.x * v2.x + v1.y * v2.y;
        }
    };

    ///
    /// 表示一个矢量，或是空间中的一个点。
    ///
    template < typename T >
    class Vector3
    {
        // ********************************
        //    public data members
        // ********************************
    public :

        ///
        /// element type
        ///
        typedef T ElementType;

        ///
        /// X-coordinate
        ///
        union
        {
            T x;
            T r;
            T pitch;
        };

        ///
        /// Y-coordinate
        ///
        union
        {
            T y;
            T g;
            T yaw;
        };

        ///
        /// Z-coordinate
        ///
        union
        {
            T z;
            T b;
            T roll;
        };

        // ********************************
        /// \name    constructors
        // ********************************
    public:

        //@{
        Vector3()
        {}
        template<typename TX, typename TY, typename TZ>
        Vector3( TX ix, TY iy, TZ iz )
            : x((T)ix), y((T)iy), z((T)iz)
        {}
        Vector3( const Vector3 & v )
            : x(v.x), y(v.y), z(v.z)
        {}
        Vector3( const Vector2<T> & v, T iz )
            : x(v.x), y(v.y), z(iz)
        {}
        //@}

        // ********************************
        /// \name operators
        // ********************************
    public:

        //@{

        operator T *() { return &x; }
        operator const T *() const { return &x; }
        bool operator < ( const Vector3 & a ) const
        {
            return x < a.x || y < a.y || z < a.z;
        }
        Vector3 & operator += ( const Vector3 & v )
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }
        Vector3 & operator -= ( const Vector3 & v )
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }
        Vector3 &operator *= ( T f )
        {
            x *= f;
            y *= f;
            z *= f;
            return *this;
        }
        Vector3 & operator *= ( const Vector3 & v )
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            return *this;
        }
        Vector3 & operator /= ( T f )
        {
            GN_ASSERT( ((T)0.0) != f );
            x /= f;
            y /= f;
            z /= f;
            return *this;
        }
        Vector3 & operator /= ( const Vector3 & v )
        {
            GN_ASSERT( v.x != ((T)0.0) && v.y != ((T)0.0) && v.z != ((T)0.0) );
            x /= v.x;
            y /= v.y;
            z /= v.z;
            return *this;
        }
        friend bool operator == ( const Vector3 & a, const Vector3 & b )
        {
            return((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
        }
        friend bool operator != ( const Vector3 & a, const Vector3 & b )
        {
            return((a.x != b.x) || (a.y != b.y) || (a.z != b.z));
        }
        friend Vector3 operator - ( const Vector3 & a )
        {
            return Vector3(-a.x, -a.y, -a.z);
        }
        friend Vector3 operator + ( const Vector3 & a, const Vector3 & b )
        {
            Vector3 ret(a);
            ret += b;
            return ret;
        }
        friend Vector3 operator - ( const Vector3 & a, const Vector3 & b )
        {
            Vector3 ret(a);
            ret -= b;
            return ret;
        }
        friend Vector3 operator * ( const Vector3 & a, const Vector3 & b )
        {
            return Vector3( a.x * b.x, a.y * b.y, a.z*b.z );
        }
        friend Vector3 operator * ( const Vector3 & v, T f )
        {
            return Vector3(f * v.x, f * v.y, f * v.z);
        }
        friend Vector3 operator * ( T f, const Vector3 & v )
        {
            return Vector3(f * v.x, f * v.y, f * v.z);
        }
        friend Vector3 operator / ( const Vector3 & v, T f )
        {
            return Vector3(v.x / f, v.y / f, v.z / f);
        }
        friend Vector3 operator / ( const Vector3 & a, const Vector3 & b )
        {
            return Vector3( a.x / b.x, a.y / b.y, a.z / b.z );
        }

        //@}

        // ********************************
        /// \name common operations
        // ********************************
    public:

        //@{

        const T * toPtr() const
        {
            return &x;
        }
        T * toPtr()
        {
            return &x;
        }
        Vector3 & set( T ix, T iy, T iz )
        {
            x = ix;
            y = iy;
            z = iz;
            return *this;
        }
        Vector3 & set( const Vector2<T> & ixy, T iz )
        {
            x = ixy.x;
            y = ixy.y;
            z = iz;
            return *this;
        }
        T length() const
        {
            return (T)sqrt(x*x + y*y + z*z);
        }
        T lengthSqr() const
        {
            return (x*x + y*y + z*z);
        }
        bool isZero() const
        {
            return((x == 0.0F) && (y == 0.0F) && (z == 0.0F));
        }
        Vector3 & normalize()
        {
			sNormalize( *this, *this );
            return *this;
        }
        static Vector3 sNormalize( const Vector3 & v )
        {
            Vector3 r;
            sNormalize( r, v );
            return r;
        }
        static void sNormalize( Vector3 & o, const Vector3 & i )
        {
            T m = i.length();

            if( m > 0.0F )
            {
                m = 1.0F / m;
                o.x = i.x * m;
                o.y = i.y * m;
				o.z = i.z * m;
            }
			else
			{
				o.x = ((T)0.0);
				o.y = ((T)0.0);
				o.z = ((T)0.0);
			}
        }
        static T sDot( const Vector3 & v1, const Vector3 & v2 )
        {
            return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
        }
        static Vector3 sCross( const Vector3 & v1, const Vector3 & v2 )
        {
            return Vector3 ( v1.y * v2.z - v1.z * v2.y,
                             v1.z * v2.x - v1.x * v2.z,
                             v1.x * v2.y - v1.y * v2.x);
        }
        static void sCross( Vector3 & o, const Vector3 & v1, const Vector3 & v2 )
        {
            o.set( v1.y * v2.z - v1.z * v2.y,
                   v1.z * v2.x - v1.x * v2.z,
                   v1.x * v2.y - v1.y * v2.x);
        }
        static T sDistance( const Vector3 & a, const Vector3 & b )
        {
            T x = a.x - b.x;
            T y = a.y - b.y;
            T z = a.z - b.z;
            return (T)sqrt( x*x + y*y + z*z );
        }
        static T sDistanceSqr( const Vector3 & a, const Vector3 & b )
        {
            T x = a.x - b.x;
            T y = a.y - b.y;
            T z = a.z - b.z;
            return x*x + y*y + z*z;
        }

        //@}
    };

    ///
    /// 四维坐标（三维齐次坐标）
    ///
    template < typename T >
    class Vector4
    {
        // ********************************
        //  public data members
        // ********************************
    public :

        ///
        /// element type
        ///
        typedef T ElementType;

        ///
        /// X-coordinate
        ///
        union
        {
            T x;
            T r;
        };

        ///
        /// Y-coordinate
        ///
        union
        {
            T y;
            T g;
        };

        ///
        /// Z-coordinate
        ///
        union
        {
            T z;
            T b;
        };

        ///
        /// W-coordinate
        ///
        union
        {
            T w;
            T a;
        };

        // ********************************
        /// \name    constructors
        // ********************************
    public:
        //@{
        Vector4() {}
        //
        Vector4( T ix, T iy, T iz, T iw )
            : x(ix), y(iy), z(iz), w(iw)
        {}
        //
        Vector4( const Vector4 & v )
            : x(v.x), y(v.y), z(v.z), w(v.w)
        {}
        //
        Vector4( const Vector2<T> & v, T iz, T iw )
            : x(v.x), y(v.y), z(iz), w(iw)
        {}
        //
        Vector4( const Vector3<T> & v, T iw )
            : x(v.x), y(v.y), z(v.z), w(iw)
        {}
        //@}

        // ********************************
        /// \name operators
        // ********************************
    public:

        //@{

        operator T *() { return &x; }
        operator const T *() const { return &x; }
        bool operator < ( const Vector4 & a ) const
        {
            return x < a.x || y < a.y || z < a.z || w < a.w;
        }
        Vector4 & operator += ( const Vector4 & v )
        {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
            return *this;
        }
        Vector4 & operator -= ( const Vector4 & v )
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.w;
            return *this;
        }
        Vector4 & operator *= ( T f )
        {
            x *= f;
            y *= f;
            z *= f;
            w *= f;
            return *this;
        }
        Vector4 & operator *= ( const Vector4 & v )
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            w *= v.w;
            return *this;
        }
        Vector4 & operator /= ( T f )
        {
            GN_ASSERT( ((T)0.0) != f );
            x /= f;
            y /= f;
            z /= f;
            w /= f;
            return *this;
        }
        Vector4 & operator /= ( const Vector4 & v )
        {
            GN_ASSERT( v.x != ((T)0.0) && v.y != ((T)0.0) && v.z != ((T)0.0) && v.w != ((T)0.0) );
            x /= v.x;
            y /= v.y;
            z /= v.z;
            w /= v.w;
            return *this;
        }
        friend bool operator == ( const Vector4 & a, const Vector4 & b )
        {
            return((a.x == b.x) && (a.y == b.y) &&
                (a.z == b.z) && (a.w == b.w));
        }
        friend bool operator != ( const Vector4 & a, const Vector4 & b )
        {
            return((a.x != b.x) || (a.y != b.y) ||
                (a.z != b.z) || (a.w != b.w));
        }
        friend Vector4 operator - ( const Vector4 & a )
        {
            return Vector4(-a.x, -a.y, -a.z, -a.w);
        }
        friend Vector4 operator + ( const Vector4 & a, const Vector4 & b )
        {
            Vector4 ret(a);
            ret += b;
            return ret;
        }
        friend Vector4 operator - ( const Vector4 & a, const Vector4 & b )
        {
            Vector4 ret(a);
            ret -= b;
            return ret;
        }
        friend Vector4 operator * ( const Vector4 & a, const Vector4 & b )
        {
            return Vector4( a.x * b.x, a.y * b.y, a.z*b.z, a.w*b.w );
        }
        friend Vector4 operator * ( const Vector4 & v, T f )
        {
            return Vector4(f * v.x, f * v.y, f * v.z, f * v.w);
        }
        friend Vector4 operator * ( T f, const Vector4 & v )
        {
            return Vector4(f * v.x, f * v.y, f * v.z, f * v.w);
        }
        friend Vector4 operator / ( const Vector4 & v, T f )
        {
            return Vector4(v.x / f, v.y / f, v.z / f, v.w / f);
        }
        friend Vector4 operator / ( const Vector4 & a, const Vector4 & b )
        {
            return Vector4( a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w );
        }

        //@}

        // ********************************
        /// \name vector operations
        // ********************************
    public:

        //@{

        const T * toPtr() const
        {
            return &x;
        }
        T * toPtr()
        {
            return &x;
        }
        Vector4 & set( T ix, T iy, T iz, T iw )
        {
            x = ix;
            y = iy;
            z = iz;
            w = iw;
            return *this;
        }
        Vector4 & set( const Vector2<T> & ixy, T iz, T iw )
        {
            x = ixy.x;
            y = ixy.y;
            z = iz;
            w = iw;
            return *this;
        }
        Vector4 & set( const Vector3<T> & ixyz, T iw )
        {
            x = ixyz.x;
            y = ixyz.y;
            z = ixyz.z;
            w = iw;
            return *this;
        }
        T length() const
        {
            return(T) sqrt(x*x + y*y + z*z + w*w);
        }
        T lengthSqr() const
        {
            return(x*x + y*y + z*z + w*w);
        }
        Vector4 & normalize()
        {
			normalize( *this, *this );
            return *this;
        }
        static Vector4 sNormalize( const Vector4 & v )
        {
            Vector4 r;
            normalize( r, v );
            return r;
        }
        static void sNormalize( Vector4 & o, const Vector4 & i )
        {
            T m = i.length();

            if( m > 0.0F )
            {
                m = 1.0F / m;
                o.x = i.x * m;
                o.y = i.y * m;
				o.z = i.z * m;
				o.w = i.w * m;
            }
			else
			{
				o.x = ((T)0.0);
				o.y = ((T)0.0);
				o.z = ((T)0.0);
				o.w = ((T)0.0);
			}
        }
        Vector3<T> toVec3() const
        {
            Vector3<T> ret( x / w, y / w, z / w );
            return ret;
        }
        Vector3<T> & toVec3( Vector3<T> & o ) const
        {
            o.set( x / w, y / w, z / w );
            return o;
        }
        static T sDot( const Vector4 & v1, const Vector4 & v2 )
        {
            return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
        }

        //@}
    };

    ///
    /// 3x3矩阵 ( row major )
    ///
    template< typename T >
    class Matrix33
    {
        // ********************************
        //    public data members
        // ********************************
    public:

        ///
        /// element type
        ///
        typedef T ElementType;

        ///
        /// matrix rows
        ///
        Vector3<T> rows[3];

        // ********************************
        //    static data members
        // ********************************
    public:

        // ********************************
        /// \name constructors
        // ********************************
    public:
        //@{
        Matrix33() {}
        //
        Matrix33(
            T _00, T _01, T _02,
            T _10, T _11, T _12,
            T _20, T _21, T _22 )
        {
            rows[0].set(_00,_01,_02);
            rows[1].set(_10,_11,_12);
            rows[2].set(_20,_21,_22);
        }
        //
        Matrix33( const Matrix33 & m )
        {
            rows[0] = m[0];
            rows[1] = m[1];
            rows[2] = m[2];
        }
        //@}

        // ********************************
        /// \name operators
        // ********************************
    public:

        //@{

        operator const Vector3<T> * () const
        {
            return rows;
        }
        operator Vector3<T> * ()
        {
            return rows;
        }
        Matrix33 &operator = ( const Matrix33 & m )
        {
            if (this == &m) return *this;

            rows[0] = m[0];
            rows[1] = m[1];
            rows[2] = m[2];
            return *this;
        }
        Matrix33 & operator += ( const Matrix33 & m )
        {
            rows[0] += m[0];
            rows[1] += m[1];
            rows[2] += m[2];
            return *this;
        }
        Matrix33 & operator -= ( const Matrix33 & m )
        {
            rows[0] -= m[0];
            rows[1] -= m[1];
            rows[2] -= m[2];
            return *this;
        }
        Matrix33<T> & operator *= ( const Matrix33<T> & m );
        Matrix33 & operator *= ( T f )
        {
            rows[0] *= f;
            rows[1] *= f;
            rows[2] *= f;
            return *this;
        }
        friend bool operator == ( const Matrix33 & a, const Matrix33 & b )
        {
            return((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
        }
        friend bool operator != ( const Matrix33 & a, const Matrix33 & b )
        {
            return((a[0] != b[0]) || (a[1] != b[1]) || (a[2] != b[2]));
        }
        friend Matrix33 operator + ( const Matrix33 & a, const Matrix33 & b )
        {
            Matrix33 ret(a);
            ret += b;
            return ret;
        }
        friend Matrix33 operator - ( const Matrix33 & a, const Matrix33 & b )
        {
            Matrix33 ret(a);
            ret -= b;
            return ret;
        }
        friend Matrix33   operator * ( const Matrix33 & a, const Matrix33 & b )
        {
            Matrix33 ret(a);
            ret *= b;
            return ret;
        }
        friend Vector3<T> operator * ( const Vector3<T> & v, const Matrix33 & m )
        {
            Vector3<T> ret;
            ret.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
            ret.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
            ret.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
            return ret;
        }
        friend Vector3<T> operator * ( const Matrix33 & m, const Vector3<T> & v )
        {
            Vector3<T> ret;
            ret.x = Vector3<T>::sDot( m[0], v );
            ret.y = Vector3<T>::sDot( m[1], v );
            ret.z = Vector3<T>::sDot( m[2], v );
            return ret;
        }
        friend Matrix33 operator * ( const Matrix33 & m, T f )
        {
            Matrix33 ret(m);
            ret *= f;
            return ret;
        }
        friend Matrix33 operator * ( T f, const Matrix33 & m )
        {
            Matrix33 ret(m);
            ret *= f;
            return ret;
        }
        friend std::ostream & operator<<( std::ostream & o, const Matrix33 & m )
        {
            StrA s;
            m.print( s );
            o << s;
            return o;
        }

        //@}

        // ********************************
        /// \name matrix operations
        // ********************************
    public:

        //@{

        Matrix33 & identity()
        {
            rows[0].set(1,0,0);
            rows[1].set(0,1,0);
            rows[2].set(0,0,1);
            return *this;
        }
        static Matrix33 sIdentity()
        {
            return Matrix33(
                (T)1, (T)0, (T)0,
                (T)0, (T)1, (T)0,
                (T)0, (T)0, (T)1 );
        }
        Matrix33 & transpose()
        {
            detail::swap( rows[0][1], rows[1][0] );
            detail::swap( rows[0][2], rows[2][0] );
            detail::swap( rows[1][2], rows[2][1] );
            return *this;
        }
        static void sTranspose( Matrix33 & dst, const Matrix33 & src )
        {
            dst = src;
            dst.transpose();
        }
        static Matrix33 sTranspose( const Matrix33 & src )
        {
            Matrix33 r(src);
            r.transpose();
            return r;
        }
        Matrix33 & inverse();
        static void sInverse( Matrix33 & dst, const Matrix33 & src )
        {
            dst = src;
            dst.inverse();
        }
        static Matrix33 sInverse( const Matrix33 & src )
        {
            Matrix33 r(src);
            r.inverse();
            return r;
        }
        Matrix33 & scale( const Vector3<T> & f )
        {
            identity();
            rows[0][0] = f[0];
            rows[1][1] = f[1];
            rows[2][2] = f[2];
            return *this;
        }
        ///
        /// 求旋转矩阵. Angle is in radius
        ///
        Matrix33 & rotateX( T angle );
        Matrix33 & rotateY( T angle );
        Matrix33 & rotateZ( T angle );
        Matrix33 & rotate( const Vector3<T> & v, T angle );
        Matrix33 & lookAtLh( const Vector3<T> & forward, const Vector3<T> & up );
        Matrix33 & lookAtRh( const Vector3<T> & forward, const Vector3<T> & up );
        Matrix33 & lookAt( const Vector3<T> & forward, const Vector3<T> & up )
        {
#if GN_BUILD_LEFT_HAND
            return lookAtLh( forward, up );
#else
            return lookAtRh( forward, up );
#endif
        }

        ///
        /// 打印矩阵内容到字符串中, mainly for debug purpose.
        ///
        void print( StrA & ) const;

        ///
        /// print to string
        ///
        StrA print() const { StrA s; print(s); return s; }

        //@}
    };

    ///
    /// 4x4 matrix ( row major )
    ///
    template < typename T >
    class Matrix44
    {
        // ********************************
        //  public data members
        // ********************************
    public:

        ///
        /// element type
        ///
        typedef T ElementType;

        ///
        /// matrix rows
        ///
        Vector4<T> rows[4];

        // ********************************
        /// \name constructors
        // ********************************
    public:
        //@{
        Matrix44() {};
        //
        Matrix44(
            T _00, T _01, T _02, T _03,
            T _10, T _11, T _12, T _13,
            T _20, T _21, T _22, T _23,
            T _30, T _31, T _32, T _33 )
        {
            rows[0].set(_00,_01,_02,_03);
            rows[1].set(_10,_11,_12,_13);
            rows[2].set(_20,_21,_22,_23);
            rows[3].set(_30,_31,_32,_33);
        }
        //
        Matrix44( const Matrix44 & m )
        {
            rows[0] = m[0];
            rows[1] = m[1];
            rows[2] = m[2];
            rows[3] = m[3];
        }
        //
        explicit Matrix44(
            const Matrix33<T> & m33,
            const Vector3<T>  & col3 = Vector3<T>(0,0,0),
            const Vector3<T>  & row3 = Vector3<T>(0,0,0),
            T        _33   = ((T)1.0) )
        {
            rows[0].set( m33[0], col3.x );
            rows[1].set( m33[1], col3.y );
            rows[2].set( m33[2], col3.z );
            rows[3].set( row3, _33 );
        }
        //@}

        // ********************************
        /// \name operators
        // ********************************
    public:

        //@{

        operator const Vector4<T> * () const
        {
            return rows;
        }
        operator Vector4<T> * ()
        {
            return rows;
        }
        Matrix44 & operator = ( const Matrix44 & m )
        {
            if (this == &m) return *this;
            rows[0] = m[0];
            rows[1] = m[1];
            rows[2] = m[2];
            rows[3] = m[3];
            return *this;
        }
        Matrix44 & operator = ( const Matrix33<T> & m )
        {
            rows[0].set( m[0], ((T)0.0) );
            rows[1].set( m[1], ((T)0.0) );
            rows[2].set( m[2], ((T)0.0) );
            rows[3].set( ((T)0.0), ((T)0.0), ((T)0.0), ((T)1.0) );
            return *this;
        }
        Matrix44 & operator += ( const Matrix44 & m )
        {
            rows[0] += m[0];
            rows[1] += m[1];
            rows[2] += m[2];
            rows[3] += m[3];
            return *this;
        }
        Matrix44 & operator -= ( const Matrix44 & m )
        {
            rows[0] -= m[0];
            rows[1] -= m[1];
            rows[2] -= m[2];
            rows[3] -= m[3];
            return *this;
        }
        Matrix44<T> & operator *= ( const Matrix44<T> & m );
        Matrix44 & operator *= ( T f )
        {
            rows[0] *= f;
            rows[1] *= f;
            rows[2] *= f;
            rows[3] *= f;
            return *this;
        }
        friend bool operator == ( const Matrix44 & a, const Matrix44 & b )
        {
            return( (a[0] == b[0]) && (a[1] == b[1]) &&
                    (a[2] == b[2]) && (a[3] == b[3]) );
        }
        friend bool operator != ( const Matrix44 & a, const Matrix44 & b )
        {
            return( (a[0] != b[0]) || (a[1] != b[1]) ||
                    (a[2] != b[2]) || (a[3] != b[3]) );
        }
        friend Matrix44
        operator + ( const Matrix44 & a, const Matrix44 & b )
        {
            Matrix44 ret(a);
            ret += b;
            return ret;
        }
        friend Matrix44
        operator - ( const Matrix44 & a, const Matrix44 & b )
        {
            Matrix44 ret(a);
            ret -= b;
            return ret;
        }
        friend Matrix44
        operator * ( const Matrix44 & a, const Matrix44 & b )
        {
            Matrix44 ret(a);
            ret *= b;
            return ret;
        }
        ///// Multiply a row 4-D vector by this matrix
        //friend Vector4<T> operator * ( const Vector4<T> & v,
        //    const Matrix44 & m )
        //{
        //    Vector4<T> ret;
        //    ret.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0];
        //    ret.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1];
        //    ret.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2];
        //    ret.w = v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3];
        //    return ret;
        //}
        friend Vector4<T>
        operator * ( const Matrix44 & m, const Vector4<T> & v )
        {
            Vector4<T> ret;
            ret.x = Vector4<T>::sDot( m[0], v );
            ret.y = Vector4<T>::sDot( m[1], v );
            ret.z = Vector4<T>::sDot( m[2], v );
            ret.w = Vector4<T>::sDot( m[3], v );
            return ret;
        }
        friend Matrix44 operator * ( const Matrix44 & m, T f )
        {
            Matrix44 ret(m);
            ret *= f;
            return ret;
        }
        friend Matrix44 operator * ( T f, const Matrix44 & m )
        {
            Matrix44 ret(m);
            ret *= f;
            return ret;
        }
        friend std::ostream & operator<<( std::ostream & o, const Matrix44 & m )
        {
            StrA s;
            m.print( s );
            o << s;
            return o;
        }

        //@}

        // ********************************
        /// \name matrix operations
        // ********************************
    public:

        //@{

        Matrix44 & set( T _00, T _01, T _02, T _03,
                        T _10, T _11, T _12, T _13,
                        T _20, T _21, T _22, T _23,
                        T _30, T _31, T _32, T _33 )
        {
            rows[0].set(_00,_01,_02,_03);
            rows[1].set(_10,_11,_12,_13);
            rows[2].set(_20,_21,_22,_23);
            rows[3].set(_30,_31,_32,_33);
            return *this;
        }
        Matrix44 & set( const Matrix33<T> & m33,
                        const Vector3<T> & col3 = Vector3<T>(0,0,0),
                        const Vector3<T> & row3 = Vector3<T>(0,0,0),
                        T         _33 = ((T)1.0) )
        {
            rows[0].set( m33[0], col3.x );
            rows[1].set( m33[1], col3.y );
            rows[2].set( m33[2], col3.z );
            rows[3].set( row3, _33 );
            return *this;
        }
        Matrix44 & identity()
        {
            rows[0].set( ((T)1.0), ((T)0.0), ((T)0.0), ((T)0.0) );
            rows[1].set( ((T)0.0), ((T)1.0), ((T)0.0), ((T)0.0) );
            rows[2].set( ((T)0.0), ((T)0.0), ((T)1.0), ((T)0.0) );
            rows[3].set( ((T)0.0), ((T)0.0), ((T)0.0), ((T)1.0) );
            return *this;
        }
        static Matrix44 sIdentity()
        {
            return Matrix44(
                (T)1,(T)0,(T)0,(T)0,
                (T)0,(T)1,(T)0,(T)0,
                (T)0,(T)0,(T)1,(T)0,
                (T)0,(T)0,(T)0,(T)1 );
        }
        Matrix44 & transpose()
        {
            detail::swap( rows[0][1], rows[1][0] );
            detail::swap( rows[0][2], rows[2][0] );
            detail::swap( rows[0][3], rows[3][0] );
            detail::swap( rows[1][2], rows[2][1] );
            detail::swap( rows[1][3], rows[3][1] );
            detail::swap( rows[2][3], rows[3][2] );
            return *this;
        }
        static void sTranspose( Matrix44 & dst, const Matrix44 & src )
        {
            dst = src;
            dst.transpose();
        }
        static Matrix44 sTranspose( const Matrix44 & src )
        {
            Matrix44 r(src);
            r.transpose();
            return r;
        }
        Matrix44 & inverse();
        static void sInverse( Matrix44 & dst, const Matrix44 & src )
        {
            dst = src;
            dst.inverse();
        }
        static Matrix44 sInverse( const Matrix44 & src )
        {
            Matrix44 r(src);
            r.inverse();
            return r;
        }
        Matrix44 & invtrans()
        {
            inverse();
            transpose();
            return *this;
        }
        static void sInvtrans( Matrix44 & dst, const Matrix44 & src )
        {
            dst = src;
            dst.invtrans();
        }
        static Matrix44 sInvtrans( const Matrix44 & src )
        {
            Matrix44 r(src);
            r.invtrans();
            return r;
        }
        ///
        /// 求旋转矩阵. Angle is in radians.
        ///
        Matrix44 & rotateX( T angle );
        Matrix44 & rotateY( T angle );
        Matrix44 & rotateZ( T angle );
        Matrix44 & rotate( const Vector3<T> & v, T angle );
        Matrix44 & translate( T x, T y = (T)0, T z = (T)0 )
        {
            identity();
            rows[0][3] = x;
            rows[1][3] = y;
            rows[2][3] = z;
            return *this;
        }
        Matrix44 & translate( const Vector3<T> & v )
        {
            return translate( v.x, v.y, v.z );
        }
        Matrix44 & translate( const Vector2<T> & v )
        {
            return translate( v.x, v.y, (T)0 );
        }
        static Matrix44 sTranslate( const Vector3<T> & v )
        {
            Matrix44 m;
            m.translate( v );
            return m;
        }
        Matrix44 & lookAtLh( const Vector3<T> & eye,
                             const Vector3<T> & to,
                             const Vector3<T> & up );
        Matrix44 & lookAtRh( const Vector3<T> & eye,
                             const Vector3<T> & to,
                             const Vector3<T> & up );
        Matrix44 & lookAt( const Vector3<T> & eye,
                           const Vector3<T> & to,
                           const Vector3<T> & up )
        {
#if GN_BUILD_LEFT_HAND
            return lookAtLh( eye, to, up );
#else
            return lookAtRh( eye, to, up );
#endif
        }
        ///
        /// 求左手正交投影矩阵
        ///
        /// 此投影矩阵将屏幕左下角映射为原点(left, bottom)，右上角为(right, top)
        /// Z轴范围：znear到zfar
        /// left不能等于right, bottom不能等于top, znear不能等于zfar，否则会
        /// 引起除0错误。
        ///
        /// \note
        ///     在标准的右手3维图形学中，投影变换后Z轴的范围应该是[1, -1]，
        ///     而directx和opengl分别使用的是[0, 1]和[-1, 1]。
        ///     因此，图形学教科书中给出的标准投影变换矩阵无法在opengl和directx
        ///     中使用；且对于同样的投影变换效果，在directx和opengl中必须使用不
        ///     同的变换矩阵。 因此特别设置了两套分别适用于opengl和directx的计算
        ///     投影矩阵的函数。
        ///
        Matrix44 & orthoOGLLh( T left, T right,
                               T bottom, T top,
                               T znear, T zfar );

        Matrix44 & orthoOGLRh( T left, T right,
                               T bottom, T top,
                               T znear, T zfar );

        Matrix44 & orthoOGL( T left, T right,
                             T bottom, T top,
                             T znear, T zfar )
        {
#if GN_BUILD_LEFT_HAND
            return orthoOGLLh( left, right, bottom, top, znear, zfar );
#else
            return orthoOGLRh( left, right, bottom, top, znear, zfar );
#endif
        }
        Matrix44 & orthoD3DLh( T left, T right,
                               T bottom, T top,
                               T znear, T zfar );
        Matrix44 & orthoD3DRh( T left, T right,
                               T bottom, T top,
                               T znear, T zfar );
        Matrix44 & orthoD3D( T left, T right,
                             T bottom, T top,
                             T znear, T zfar )
        {
#if GN_BUILD_LEFT_HAND
            return orthoD3DLh( left, right, bottom, top, znear, zfar );
#else
            return orthoD3DRh( left, right, bottom, top, znear, zfar );
#endif
        }
        ///
        /// 求左手透视投影矩阵
        ///
        /// \param fovy        垂直方向上的视角大小，以弧度为单位
        /// \param ratio       视场的宽度和高度的比值
        /// \param znear, zfar 可见的Z轴范围
        ///
        /// \note fovy和ratio不能为0，znear不能等于zfar，否则会引起除0错误。
        ///
        Matrix44 & perspectiveOGLLh( T fovy, T ratio,
                                     T znear, T zfar );
        Matrix44 & perspectiveOGLRh( T fovy, T ratio,
                                     T znear, T zfar );
        Matrix44 & perspectiveOGL( T fovy, T ratio,
                                   T znear, T zfar )
        {
#if GN_BUILD_LEFT_HAND
            return perspectiveOGLLh( fovy, ratio, znear, zfar );
#else
            return perspectiveOGLRh( fovy, ratio, znear, zfar );
#endif
        }
        Matrix44 & perspectiveD3DLh( T fovy, T ratio,
                                     T znear, T zfar );
        Matrix44 & perspectiveD3DRh( T fovy, T ratio,
                                     T znear, T zfar );
        Matrix44 & perspectiveD3D( T fovy, T ratio,
                                   T znear, T zfar )
        {
#if GN_BUILD_LEFT_HAND
            return perspectiveD3DLh( fovy, ratio, znear, zfar );
#else
            return perspectiveD3DRh( fovy, ratio, znear, zfar );
#endif
        }
        ///
        /// transform a 3-D point by this matrix.
        ///
        /// This function treads input vector as (x, y, z, 1)
        ///
        void transformPoint( Vector3<T> & dst, const Vector3<T> & src ) const
        {
            if( &dst == &src )
            {
                Vector3<T> tmp;
                transformPoint( tmp, src );
                dst = tmp;
            }
            else
            {
                dst.x = rows[0].x * src.x + rows[0].y * src.y + rows[0].z * src.z + rows[0].w;
                dst.y = rows[1].x * src.x + rows[1].y * src.y + rows[1].z * src.z + rows[1].w;
                dst.z = rows[2].x * src.x + rows[2].y * src.y + rows[2].z * src.z + rows[2].w;

                T k = rows[3].x*src.x + rows[3].y*src.y + rows[3].z*src.z + rows[3].w;

                if( ((T)0) != k )
                {
                    dst /= k;
                }
                else
                {
                    static Logger * sLogger = getLogger("GN.base.Matrix44");
                    GN_WARN(sLogger)( "the vertex is transformed to infinite place" );
                }
            }
        }
        Vector3<T> transformPoint( const Vector3<T> & src ) const
        {
			Vector3<T> dst;
			transformPoint( dst, src );
			return dst;
        }
        ///
        /// transform a vector by this matrix
        ///
        /// This function treads input vector as (x, y, z, 0).
        /// To transform an normal, you should use invtrans of the desired matrix.
        ///
        void transformVector( Vector3<T> & dst, const Vector3<T> & src ) const
        {
            if( &dst == &src )
            {
                Vector3<T> tmp;
                transformVector( tmp, src );
                dst = tmp;
            }
            else
            {
                dst.x = rows[0].x * src.x + rows[0].y * src.y + rows[0].z * src.z;
                dst.y = rows[1].x * src.x + rows[1].y * src.y + rows[1].z * src.z;
                dst.z = rows[2].x * src.x + rows[2].y * src.y + rows[2].z * src.z;
            }
        }
        Vector3<T> transformVector( const Vector3<T> & src ) const
        {
            Vector3<T> dst;
			transformVector( dst, src );
            return dst;
        }
        ///
        /// Breaks down a general 3D transformation matrix into its scalar, rotational, and translational components
        ///
        bool decompose( Vector3<T> & translation, Vector3<T> & rotation, Vector3<T> & scaling ) const;
        ///
        /// Print the matrix to string.
        ///
        void print( StrA & ) const;
        StrA print() const { StrA s; print(s); return s; }

        //@}
    };

    ///
    /// 四元数
    ///
    template < typename T >
    class Quaternion
    {
        // ********************************
        //    data members
        // ********************************
    public :

        ///
        /// element type
        ///
        typedef T ElementType;

        Vector3<T>  v; ///< axis
        T           w; ///< W-value

        // ********************************
        /// \name    constructors
        // ********************************
    public:

        //@{
        Quaternion() {};
        //
        Quaternion( const Quaternion & q ) : v(q.v), w(q.w) {}
        //
        Quaternion( T x_, T y_, T z_, T w_ )
            :v(x_,y_,z_), w(w_)
        {}
        //@}

        // ********************************
        //          operators
        // ********************************
    public:

        ///
        /// assign
        ///
        Quaternion & operator = ( const Quaternion & q )
        {
            v = q.v;
            w = q.w;
            return *this;
        }
        ///
        /// Array indexing
        ///
        T operator [] ( size_t i ) const
        {
            GN_ASSERT( i < 4 );
            return *(&v.x + i);
        }
        ///
        /// concatnate
        ///
        Quaternion & operator *= ( const Quaternion & q )
        {
            *this = (*this) * q;
            return *this;
        }
        ///
        /// concatnate
        ///
        friend Quaternion
        operator * ( const Quaternion & q1, const Quaternion & q2 )
        {
            Quaternion result;
            result.w = q1.w * q2.w - Vector3<T>::sDot( q1.v, q2.v );
            result.v = Vector3<T>::sCross( q1.v, q2.v );
            result.v += q1.w * q2.v + q2.w * q1.v;
            return result;
        }

        ///
        /// equality
        ///
        friend bool operator == ( const Quaternion & a, const Quaternion & b )
        {
            return a.w == b.w && a.v == b.v;
        }

        ///
        /// equality
        ///
        friend bool operator != ( const Quaternion & a, const Quaternion & b )
        {
            return a.w != b.w || a.v != b.v;
        }

    private :

        ///
        /// index operator
        ///
        T & operator [] ( unsigned int i )
        {
            GN_ASSERT( i < 4 );
            return *(&v.x + i);
        }

        // ********************************
        //        operations
        // ********************************
    public:

        template<typename T2>
        void set( T2 x_, T2 y_, T2 z_, T2 w_ )
        {
            v.x = (T)x_;
            v.y = (T)y_;
            v.z = (T)z_;
            w   = (T)w_;
        }

        /// 归一化
        Quaternion & identity()
        {
            w = ((T)1.0); v.set(0, 0, 0); return *this;
        }

        /// identity quaternion
        static Quaternion sIdentity()
        {
            return Quaternion( (T)0, (T)0, (T)0, (T)1 );
        }

        ///
        /// get norm
        ///
        T getNormal() const
        {
            return w * w + Vector3<T>::dot( v, v );
        }
        ///
        /// normalize a quaternion
        ///
        Quaternion & normalize()
        {
            T n = (T)sqrt( getNormal() );
            if (n > ((T)0.0)) n = 1/n;
            else n = ((T)0.0);

            v *= n;
            w *= n;
            return *this;
        }
        ///
        /// normalize a quaternion
        ///
        static void sNormalize( Quaternion & dst, const Quaternion & src )
        {
            dst = src;
            dst.normalize();
        }
        ///
        /// normalize a quaternion
        ///
        static Quaternion sNormalize( const Quaternion & src )
        {
            Quaternion r(src);
            r.normalize();
            return r;
        }
        ///
        /// conjugate (共扼)
        ///
        Quaternion & conjugate()
        {
            v = -v; return *this;
        }
        ///
        /// conjugate
        ///
        static void sConjugate( Quaternion & dst, const Quaternion & src )
        {
            dst = src;
            dst.conjugate();
        }
        ///
        /// conjugate
        ///
        static Quaternion sConjugate( const Quaternion & src )
        {
            Quaternion r(src);
            r.conjugate();
            return r;
        }
        ///
        /// inverse
        ///
        Quaternion & inverse()
        {
            T l = getNormal();

            if (l > ((T)0.0))
            { this->conjugate(); v /= l; w /= l; }
            else
            { w = ((T)0.0); v.set(0, 0, 0); }

            return *this;
        }
        ///
        /// inverse
        ///
        static void sInvert( Quaternion & dst, const Quaternion & src )
        {
            dst = src;
            dst.inverse();
        }
        ///
        /// inverse
        ///
        static Quaternion sInverse( const Quaternion & src )
        {
            Quaternion r(src);
            r.inverse();
            return r;
        }
        ///
        /// get from a UNIT rotation axis and an angle
        ///
        /// \param axis  Axis of rotation
        /// \param angle Angle of rotation, in radians
        ///
        Quaternion &
        fromRotation( const Vector3<T> & axis, const T angle )
        {
            T r = angle / (T)2.0;
            v = axis * sin(r);
            w = cos(r);

            return *this;
        }
        ///
        /// construct from two <b>UNIT</b> vectors
        ///
        /// \note v1 and v2 should be <b>UNIT</b> vectors
        ///
        Quaternion & fromArc( const Vector3<T> & v1, const Vector3<T> & v2 )
        {
            T  d = Vector3<T>::sDot( v1, v2 );
            T  s = (T)sqrt( ( 1 + d ) * 2 );
            Vector3<T> c = Vector3<T>::sCross( v1, v2 );

            v.x = c.x / s;
            v.y = c.y / s;
            v.z = c.z / s;
            w   = s / (T)2.0;

            return *this;
        }
        ///
        /// Construct from euler angles
        ///
        /// \param pitch, yaw, roll   Euler angles in radians
        ///
        Quaternion & fromEuler( T pitch,
                                T yaw,
                                T roll )
        {
            float cy = cos( yaw / (T)2.0 );
            float sy = sin( yaw / (T)2.0 );
            float cp = cos( pitch / (T)2.0 );
            float sp = sin( pitch / (T)2.0 );
            float cr = cos( roll / (T)2.0 );
            float sr = sin( roll / (T)2.0 );
            v.set( cr * sp * cy + sr * cp * sy,
                   cr * cp * sy - sr * sp * cy,
                   sr * cp * cy - cr * sp * sy );
            w =    cr * cp * cy + sr * sp * sy;
            return *this;
        }
        ///
        /// construct from euler angles
        ///
        Quaternion & fromEuler( const Vector3<T> & euler )
        {
            return fromEuler( euler.x, euler.y, euler.z );
        }
        ///
        /// construct from matrix33
        ///
        Quaternion & fromMatrix33( const Matrix33<T> & mat );
        ///
        /// convert to matrix33
        ///
        void toMatrix33( Matrix33<T> & out ) const;
        ///
        /// convert to matrix33
        ///
        Matrix33<T> toMatrix33() const
        {
			Matrix33<T> out;
			toMatrix33( out );
			return out;
        }
        ///
        /// convert to matrix44
        ///
        void toMatrix44( Matrix44<T> & out ) const;
        ///
        /// convert to matrix33
        ///
        Matrix44<T> toMatrix44() const
        {
			Matrix44<T> out;
			toMatrix44( out );
			return out;
        }
    };

    ///
    /// 3D plane class
    ///
    /// plane equation : a * X + b * Y + c * Z + d = 0,
    ///             or : n * V + d = 0;
    ///
    template < typename T >
    class Plane3
    {
    public :

        ///
        /// element type
        ///
        typedef T ElementType;

        Vector3<T>  n; ///< normal (X:a, Y:b, Z:c)
        T d; ///< constant factor

    public :
        /// \name constructors
        //@{
        Plane3() {}
        Plane3( T a_, T b_, T c_, T d_ )
            : n(a_,b_,c_), d(d_) {}
        Plane3( const Plane3 & p ) : n(p.n), d(p.d) {}
        Plane3( const Vector3<T> & point, const Vector3<T> & normal )
        {
            fromPointNormal( point, normal );
        }
        Plane3( const Vector3<T> & v1,
                   const Vector3<T> & v2,
                   const Vector3<T> & v3 )
        {
            fromPoints( v1, v2, v3 );
        }
        //@}

        /// \name operators
        //@{
    public:
        ///
        /// type conversion
        ///
        operator const T * () const
        {
            return n;
        }
        ///
        /// type conversion
        ///
        operator T * ()
        {
            return n;
        }
        ///
        /// equality
        ///
        bool operator == ( const Plane3 & p ) const
        {
            return n == p.n && d == p.d;
        }
        ///
        /// inequality
        ///
        bool operator != ( const Plane3 & p ) const
        {
            return n != p.n || d != p.d;
        }
        ///
        /// Negate a plane
        ///
        friend Plane3 operator - ( const Plane3 & p )
        {
            return Plane3( -p.n.x, -p.n.y, -p.n.z, -p.d );
        }
        ///
        /// dot production with vector3 ( ax + by + cz + d )
        ///
        friend T operator * ( const Plane3 & p, const Vector3<T> & v )
        {
            return Vector3<T>::dot( p.n, v ) + p.d;
        }
        ///
        /// dot production with 3D vector
        ///
        friend T operator * ( const Vector3<T> & v, const Plane3 & p )
        {
            return p * v;
        }
        ///
        /// dot production with vector4 ( ax + by + cz + dw )
        ///
        friend T operator * ( const Plane3 & p, const Vector4<T> & v )
        {
            return p.n.x * v.x + p.n.y * v.y + p.n.z * v.z + p.d * v.w;
        }
        ///
        /// dot production with 4D vector
        ///
        friend T operator * ( const Vector4<T> & v, const Plane3 & p )
        {
            return p * v;
        }
        //@}

        /// \name operations
        //@{
    public:
        ///
        /// update plane parameters
        ///
        Plane3 & set( T a_, T b_, T c_, T d_ )
        {
            n.set( a_, b_, c_ );
            d = d_;
            return *this;
        }
        ///
        /// update plane parameters
        ///
        Plane3 & set( const Vector3<T> & n_, T d_ )
        {
            n = n_;
            d = d_;
            return *this;
        }
        ///
        /// Normalize the plane (so that |a,b,c| == 1)
        ///
        Plane3 & normalize()
        {
            T f = n.length();
            if( f ) { n /= f; d /= f; }
            return *this;
        }
        ///
        /// Normalize the plane (so that |a,b,c| == 1)
        ///
        static void sNormalize( Plane3 & dst, const Plane3 & src )
        {
            dst = src;
            dst.normalize();
            return dst;
        }
        ///
        /// Normalize the plane (so that |a,b,c| == 1)
        ///
        static Plane3 sNormalize( const Plane3 & src )
        {
            Plane3 r(src);
            r.normalize();
            return r;
        }
        ///
        /// construct a plane from a point and a normal
        ///
        Plane3 & fromPointNormal( const Vector3<T> & point,
                                  const Vector3<T> & normal )
        {
            n = normal;
            d = Vector3<T>::dot( -n, point );
            return *this;
        }
        ///
        /// Construct a plane from 3 points
        ///
        Plane3 & fromPoints( const Vector3<T> & v1,
                             const Vector3<T> & v2,
                             const Vector3<T> & v3 )
        {
            n = Vector3<T>::cross( v2 - v1, v3 - v1 );
            d = Vector3<T>::dot( -n, v1 );
            return *this;
        }
        ///
        /// treate plane as an 4-D vector
        ///
        Vector4<T> & toVec4()
        {
            return *reinterpret_cast<Vector4<T>*>(this);
        }
        ///
        /// treate plane as an 4-D vector
        ///
        const Vector4<T> & toVec4() const
        {
            return *reinterpret_cast<const Vector4<T>*>(this);
        }
        //@}
    };

    ///
    /// rectangle structure
    ///
    /// \note  这个矩形的定义和Windows中的矩形RECT不同
    ///
    template < typename T >
    class Rect
    {
    public :

        ///
        /// element type
        ///
        typedef T ElementType;

        ///
        /// point type
        ///
        typedef Vector2<T> PointType;

        ///
        /// rect coordinates
        ///
        //@{
        T x, y, w, h;
        //@}

        ///
        /// Default constructor
        ///
        Rect() {}
        ///
        /// Constructor
        ///
        Rect( T x_, T y_, T w_, T h_ ) : x(x_), y(y_), w(w_), h(h_) {}
        ///
        /// Equality
        ///
        bool operator==( const Rect & rhs ) const { return x == rhs.x && y == rhs.y && w == rhs.w && h == rhs.h; }
        ///
        /// Equality
        ///
        bool operator!=( const Rect & rhs ) const { return x != rhs.x || y != rhs.y || w != rhs.w || h != rhs.h; }
        ///
        /// Set rectangle values
        ///
        Rect & set( T x_, T y_, T w_, T h_ )
        {
            x = x_;
            y = y_;
            w = w_;
            h = h_;
            return *this;
        }
        ///
        /// Get rectangle origin
        ///
        const PointType & origin() const
        {
            return *reinterpret_cast<const PointType*>(this);
        }
        ///
        /// get rectangle origin
        ///
        PointType & origin()
        {
            return *reinterpret_cast<PointType*>(this);
        }
        ///
        /// get rectangle size
        ///
        const PointType & size() const
        {
            return *reinterpret_cast<const PointType*>(&w);
        }
        ///
        /// get rectangle size
        ///
        PointType & size()
        {
            return *reinterpret_cast<PointType*>(&w);
        }
        ///
        /// Check if the point is inside the rectangle
        ///
        bool contain( const PointType & p ) const
        {
            return
                ( ( (w >= 0) && x <= p.x && p.x < (x + w) )
                  ||
                  ( (w <  0) && (x + w) <= p.x && p.x < x ) )
                &&
                ( ( (y >= 0) && y <= p.y && p.y < (y + h) )
                  ||
                  ( (y <  0) &&  y <= p.y && p.y < (y + h) ) );
        }
        ///
        /// Normalize the rectangle
        ///
        Rect & normalize()
        {
            if( w < 0 )
            {
                x += w;
                w = -w;
            }
            if( h < 0 )
            {
                y += h;
                h = -h;
            }
            return *this;
        }
        ///
        /// Get normalize rectangle
        ///
        static void sNormalize( Rect<T> & dst, const Rect<T> & src )
        {
			dst = src;
            dst.normalize();
        }
        ///
        /// Get normalize rectangle
        ///
        static Rect<T> sNormalize( const Rect<T> & src )
        {
            Rect<T> dst(src);
            dst.normalize();
            return dst;
        }
    };

    ///
    /// box class
    ///
    template < typename T >
    class Box
    {
        // ********************************
        // public data
        // ********************************
    public :

        ///
        /// element type
        ///
        typedef T ElementType;

        ///
        /// point type
        ///
        typedef Vector3<T> PointType;

        ///
        /// Box position and extend
        //@{
        T x, y, z, w, h, d;
        //@}

        // ********************************
        // constructors
        // ********************************
    public :

        ///
        /// default constructor
        ///
        Box() {}
        ///
        /// construct from position and extend
        ///
        Box( T x_, T y_, T z_, T w_, T h_, T d_ )
            : x(x_), y(y_), z(z_) , w(w_), h(h_), d(d_)
        {}
        ///
        /// construct from 2 points
        ///
        Box( const PointType & v1, const PointType & v2 )
            : x(v1.x), y(v1.y), z(v1.z) , w(v2.x-v1.x), h(v2.y-v1.y), d(v2.z-v1.z)
        {}
        ///
        /// copy constructor
        ///
        Box( const Box & b ) : x(b.x), y(b.y), z(b.z), w(b.w), h(b.h), d(b.d) {}

        // ********************************
        // public operators
        // ********************************
    public :

        ///
        /// assign operator
        ///
        Box & operator = ( const Box & b )
        {
            x = b.x;
            y = b.y;
            z = b.z;
            w = b.w;
            h = b.h;
            d = b.d;
            return *this;
        }
        ///
        /// equal operator
        ///
        bool operator == ( const Box & b ) const
        {
            return x == b.x
                && y == b.y
                && z == b.z
                && w == b.w
                && h == b.h
                && d == b.d;
        }
        ///
        /// non-equal operator
        ///
        bool operator != ( const Box & b ) const
        {
            return x != b.x
                || y != b.y
                || z != b.z
                || w != b.w
                || h != b.h
                || d != b.d;
        }

        // ********************************
        // public operations
        // ********************************
    public :

        ///
        /// Clear the box
        ///
        void clear()
        {
            x = y = z = w = h = d = (T)0;
        }

        ///
        /// set box value
        ///
        void set( T x_, T y_, T z_, T w_, T h_, T d_ )
        {
            x = x_; y = y_; z = z_ ; w = w_; h = h_; d = d_;
        }

        ///
        /// return box position
        ///
        const PointType & pos() const { return *(const PointType*)this; }

        ///
        /// return box position
        ///
        PointType & pos() { return *(PointType*)this; }

        ///
        /// return box extend
        ///
        const PointType & extend() const { return *(const PointType*)&w; }

        ///
        /// return box extend
        ///
        PointType & extend() { return *(PointType*)&w; }

        ///
        /// return box center
        ///
        PointType center() const { return pos() + extend() / (T)2.0; }

        ///
        /// return box center
        ///
        void center( PointType & out ) const { out = pos() + extend() / (T)2.0; }

        ///
        /// return a corner point of the box
        ///
        PointType corner( int i ) const
        {
            switch( i )
            {
                case 0 : return PointType( x,     y,     z );
                case 1 : return PointType( x + w, y,     z );
                case 2 : return PointType( x + w, y + h, z );
                case 3 : return PointType( x,     y + h, z );
                case 4 : return PointType( x,     y,     z + d );
                case 5 : return PointType( x + w, y,     z + d );
                case 6 : return PointType( x + w, y + h, z + d );
                case 7 : return PointType( x,     y + h, z + d );
                default: return PointType( 0, 0, 0 );
            }
        }

        ///
        /// return the maximum axis. 0 for W, 1 for H, 2 for D.
        ///
        int theLongestAxis() const
        {
            if( w < h )
            {
                return h < d ? 2 : 1;
            }
            else
            {
                return w < d ? 2 : 0;
            }
        }

        ///
        /// from two points
        ///
        void fromPoints( const PointType & v1, const PointType & v2 )
        {
            pos()    = v1;
            extend() = v2 - v1;
            normalize();
        }
        ///
        /// Normalization (that is, positive extend)
        ///
        Box & normalize()
        {
            if( w < 0 ) { x += w; w = -w; }
            if( h < 0 ) { y += h; h = -h; }
            if( d < 0 ) { z += d; d = -d; }
			return *this;
        }
        ///
        /// normalization (store result into another variable)
        ///
        static void sNormalize( Box & dst, const Box & src )
        {
			dst = src;
            dst.normalize();
        }
        ///
        /// normalization (store result into another variable)
        ///
        static Box sNormalize( const Box & src )
        {
            Box dst(src);
            dst.normalize();
            return dst;
        }
        ///
        /// Return true if point is inside the box.
        ///
        bool checkPoint( const PointType & v )
        {
            Box b;

            normalize( b, *this );

            return b.x <= v.x && v.x < (b.x+b.w)
                && b.y <= v.y && v.y < (b.y+b.h)
                && b.z <= v.z && v.z < (b.z+b.d);
        }
        ///
        /// get union of two boxes. Note that empty boxe are ignored.
        ///
        static void sGetUnion( Box & result, const Box & b1, const Box & b2 )
        {
            if( 0 == b1.w || 0 == b1.h || 0 == b1.d )
            {
                result = b2;
            }
            else if( 0 == b2.w || 0 == b2.h || 0 == b2.d )
            {
                result = b1;
            }
            else
            {
                Box a(b1), b(b2);
                a.normalize();
                b.normalize();
                result.x = math::getmin( a.x, b.x );
                result.y = math::getmin( a.y, b.y );
                result.z = math::getmin( a.z, b.z );
                result.w = math::getmax( a.x+a.w, b.x+b.w ) - result.x;
                result.h = math::getmax( a.y+a.h, b.y+b.h ) - result.y;
                result.d = math::getmax( a.z+a.d, b.z+b.d ) - result.z;
            }
        }
    };

    ///
    /// template 3D sphere class
    ///
    template < typename T >
    class Sphere
    {
    public :

        ///
        /// element type
        ///
        typedef T ElementType;

        ///
        /// sphere center
        ///
        Vector3<T> center;

        ///
        /// sphere radius
        ///
        T          radius;

        // ********************************
        /// \name constructors
        // ********************************
    public :
        //@{
        Sphere() {}
        Sphere( T x, T y, T z, T r ) : center(x,y,z), radius(r) {}
        Sphere( const Vector3<T> & c, T r ) : center(c), radius(r) {}
        Sphere( const Sphere & s ) : center(s.center), radius(s.radius) {}
        ///
        /// construct from a box
        ///
        explicit Sphere( const Box<T> & b )
            : center( b.center() )
            , radius( (b.vmax-b.vmin).length() / (T)2.0 )
        {}
        //@}

        // ********************************
        // operators
        // ********************************
    public :

        ///
        /// assign operator
        ///
        Sphere & operator = ( const Sphere & rhs )
        {
            center = rhs.center;
            radius = rhs.radius;
            return *this;
        }

        // ********************************
        // operations
        // ********************************
    public :

        ///
        /// value set
        ///
        Sphere & set( const Vector3<T> & c, T r )
        {
            center = c; radius = r;
            return *this;
        }
    };
}

#include "matrix.inl"
#include "quaternion.inl"

namespace GN
{
    /// \name Alias for commonly used geometry classes.
    //@{
    typedef Vector2<float>      Vector2f;
    typedef Vector2<double>     Vector2d;
    typedef Vector2<int>        Vector2i;

    typedef Vector3<float>      Vector3f;
    typedef Vector3<double>     Vector3d;
    typedef Vector3<int>        Vector3i;

    typedef Vector4<float>      Vector4f;
    typedef Vector4<double>     Vector4d;
    typedef Vector4<int>        Vector4i;

    typedef Matrix33<float>     Matrix33f;
    typedef Matrix33<double>    Matrix33d;
    typedef Matrix33<int>       Matrix33i;

    typedef Matrix44<float>     Matrix44f;
    typedef Matrix44<double>    Matrix44d;
    typedef Matrix44<int>       Matrix44i;

    typedef Quaternion<float>   Quaternionf;
    typedef Quaternion<double>  Quaterniond;

    typedef Plane3<float>       Plane3f;
    typedef Plane3<double>      Plane3d;
    typedef Plane3<int>         Plane3i;

    typedef Rect<float>         Rectf;
    typedef Rect<double>        Rectd;
    typedef Rect<int>           Recti;

    typedef Box<float>          Boxf;
    typedef Box<double>         Boxd;
    typedef Box<int>            Boxi;

    typedef Sphere<float>       Spheref;
    typedef Sphere<double>      Sphered;
    typedef Sphere<int>         Spherei;
    //@}

    ///
    /// Calculate bounding sphere.
    ///
    void calculateBoundingSphere(
        Spheref & result,
        const float * x, size_t strideX,
        const float * y, size_t strideY,
        const float * z, size_t strideZ,
        size_t count );

    ///
    /// Calculate bounding sphere.
    ///
    void calculateBoundingSphere( Spheref & result, const Vector3f * positions, size_t strideInBytes, size_t count );

    ///
    /// Calculate bounding sphere.
    ///
    void calculateBoundingSphereFromBoundingBox( Spheref & result, const Boxf & bbox );

    ///
    /// Calculate axis aligned bounding box.
    ///
    void calculateBoundingBox(
        Boxf & result,
        const float * x, size_t strideX,
        const float * y, size_t strideY,
        const float * z, size_t strideZ,
        size_t count );

    ///
    /// Calculate axis aligned bounding box.
    ///
    void calculateBoundingBox( Boxf & result, const Vector3f * positions, size_t strideInBytes, size_t count );
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_GEOMETRY_H__
