#ifndef __GN_GFX_MISC_H__
#define __GN_GFX_MISC_H__
// *****************************************************************************
//! \file    gfx/misc.h
//! \brief   misc. types and functions of GFX module
//! \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN { namespace gfx
{
    //!
    //! Display mode structure
    //!
    struct DisplayMode
    {
        uint32_t width;   //!< Screen width. Zero means using current screen width. Default value is zero.
        uint32_t height;  //!< Screen height. Zero means using current screen height. Defualt value is zero.
        uint32_t depth;   //!< Color depth. Zero means using current color depth. Default value is zero.
        uint32_t refrate; //!< Referesh rate. Zero means using adapter default rate. Default value is zero.

        //!
        //! Set display mode parameters
        //!
        void set( uint32_t w, uint32_t h, uint32_t d, uint32_t r )
        {
            width = w; height = h; depth = d; refrate = r;
        }
    };

    //!
    //! Resource usage.
    //!
    enum ResourceUsage
    {
        USAGE_STATIC           = 0,    //!< normal static resource
        USAGE_DYNAMIC          = 1<<0, //!< dynamic resource
        USAGE_AUTOGEN_MIPMAP   = 1<<1, //!< texture that can auto-generate mipmaps
        USAGE_RENDERTARGET     = 1<<2, //!< render target texture
        USAGE_DEPTH            = 1<<3, //!< depth texture
        USAGE_NORMAL           = USAGE_STATIC
    };

    //!
    //! 锁定标志
    //!
    enum LockFlag
    {
        LOCK_RO = 1,                 //!< 只读锁定
        LOCK_WO = 1 << 1,            //!< 只写锁定
        LOCK_DISCARD = 1 << 2,       //!< Discard old content
        LOCK_NO_OVERWRITE = 1 << 3,  //!< no overwrite to existing content
        LOCK_RW = LOCK_RO | LOCK_WO  //!< 读写锁定
    };

    //!
    //! 2D锁定的返回结果
    //!
    struct LockedRect
    {
        size_t rowBytes; //!< bytes per line
        void * data;     //!< 指向被锁定图象的第一个字节
    };

    //!
    //! 3D锁定的返回结果
    //!
    struct LockedBox
    {
        size_t rowBytes;   //!< bytes per line
        size_t sliceBytes; //!< bytes per slice
        void * data;       //!< 指向被锁定图象的第一个字节
    };

    //!
    //! 绘图原语
    //!
    enum PrimitiveType
    {
        POINT_LIST,     //!< point list
        LINE_LIST,      //!< line list
        LINE_STRIP,     //!< line strip
        TRIANGLE_LIST,  //!< triangle list
        TRIANGLE_STRIP, //!< triangle strip
        NUM_PRIMITIVES  //!< number of available primitive types
    };

    //! \name Primitive utils
    //@{

    //!
    //! convert primitive type to string
    //!
    inline StrA primitiveType2Str( PrimitiveType pt )
    {
        GN_GUARD_SLOW;
        switch( pt )
        {
            case POINT_LIST     : return "POINT_LIST";
            case LINE_LIST      : return "LINE_LIST";
            case LINE_STRIP     : return "LINE_STRIP";
            case TRIANGLE_LIST  : return "TRIANGLE_LIST";
            case TRIANGLE_STRIP : return "TRIANGLE_STRIP";
            default             : return "Invalid primitive type!";
        }
        GN_UNGUARD_SLOW;
    }

    //!
    //! convert string to primitive
    //!
    inline PrimitiveType str2PrimitiveType( const StrA & str )
    {
        GN_GUARD_SLOW;
        if( "POINT_LIST" == str ) return POINT_LIST;
        else if( "LINE_LIST" == str ) return LINE_LIST;
        else if( "LINE_STRIP" == str ) return LINE_STRIP;
        else if( "TRIANGLE_LIST" == str ) return TRIANGLE_LIST;
        else if( "TRIANGLE_STRIP" == str ) return TRIANGLE_STRIP;
        else
        {
            // failed
            GN_ERROR( "invalid primitive string : '%s'!", str.cstr() );
            return NUM_PRIMITIVES;
        }
        GN_UNGUARD_SLOW;
    }

    //!
    //! calculate primitive count from vertex count
    //!
    //! \param pt       rendering primitive
    //! \param numvert  vertex count
    //! \return         primitive count
    //!
    inline size_t calcPrimitiveCount( PrimitiveType pt, size_t numvert )
    {
        GN_GUARD_SLOW;
        switch( pt )
        {
            case POINT_LIST     : return numvert;
            case LINE_LIST      : return (numvert >= 2) ? numvert / 2 : 0;
            case LINE_STRIP     : return (numvert >= 2) ? numvert - 1 : 0;
            case TRIANGLE_LIST  : return (numvert >= 3) ? numvert / 3 : 0;
            case TRIANGLE_STRIP : return (numvert >= 3) ? numvert - 2 : 0;
            default             :
                GN_ERROR( "Invalid primitive type!" );
                return 0;
        }
        GN_UNGUARD_SLOW;
    }

    //!
    //! calculate vertex count from primitive count
    //!
    //! \param pt       rendering primitive
    //! \param numprim  primitive count
    //! \return         vertex count
    //!
    inline size_t calcVertexCount( PrimitiveType pt, size_t numprim )
    {
        GN_GUARD_SLOW;
        switch( pt )
        {
            case POINT_LIST     : return numprim;
            case LINE_LIST      : return numprim * 2;
            case LINE_STRIP     : return numprim > 0 ? numprim + 1 : 0;
            case TRIANGLE_LIST  : return numprim * 3;
            case TRIANGLE_STRIP : return numprim > 0 ? numprim + 2 : 0;
            default :
                GN_ERROR( "Invalid primitive type!" );
                return 0;
        }
        GN_UNGUARD_SLOW;
    }

    //@}

}}

// *****************************************************************************
//                           End of misc.h
// *****************************************************************************
#endif // __GN_GFX_MISC_H__
