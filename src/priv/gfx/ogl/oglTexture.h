#ifndef __GN_GFXOGL_OGLTEXTURE_H__
#define __GN_GFXOGL_OGLTEXTURE_H__
// *****************************************************************************
//! \file    ogl/oglTexture.h
//! \brief   OpenGL texture class
//! \author  chenlee (2005.11.13)
// *****************************************************************************

#include "../common/basicTexture.h"
#include "oglResource.h"
#include "oglRenderer.h"

namespace GN { namespace gfx
{
    //!
    //! OGL texture class
    //!
    class OGLTexture : public BasicTexture,
                       public OGLResource,
                       public StdClass
    {
         GN_DECLARE_STDCLASS( OGLTexture, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLTexture( OGLRenderer & r ) :OGLResource(r) { clear(); }
        virtual ~OGLTexture() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( TextureDesc );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear()
        {
            mOGLTexture = 0;
            mFilterAndWrapDirty = true;
            mOGLFilters[0] = mOGLFilters[1] = GL_LINEAR;
            mOGLWraps[0] = mOGLWraps[1] = mOGLWraps[2] = GL_REPEAT;
        }
        //@}

        // ********************************
        //! \name from Texture
        // ********************************
    public:

        //@{

        virtual void setFilter( TexFilter, TexFilter ) const;
        virtual void setWrap( TexWrap s, TexWrap t, TexWrap r ) const;
        virtual bool lock( TexLockedResult & result, size_t face, size_t level, const Boxi * area, LockFlag flag );
        virtual void unlock();
        virtual void updateMipmap() { GN_ERROR( "no implementation" ); }
        virtual void * getAPIDependentData() const { return (void*)getOGLTexture(); }

        //@}

        // ********************************
        //  public functions
        // ********************************
    public:

        //!
        //! bind to current texture stage
        //!
        void bind() const
        {
            GN_GUARD_SLOW;

            switch( getDesc().type )
            {
                case TEXTYPE_1D   :
                case TEXTYPE_2D   :
                    if( GLEW_EXT_texture3D )
                        glDisable( GL_TEXTURE_3D_EXT );
                case TEXTYPE_3D   :
                    if( GLEW_ARB_texture_cube_map )
                        glDisable( GL_TEXTURE_CUBE_MAP_ARB );
                case TEXTYPE_CUBE :
                    break;
                default :
                    GN_ASSERT_EX( 0, "invalid texture type!" );
            }
            GN_OGL_CHECK( glEnable(mOGLTarget) );
            GN_OGL_CHECK( glBindTexture(mOGLTarget, mOGLTexture) );

            if( mFilterAndWrapDirty )
            {
                GN_OGL_CHECK( glTexParameteri( mOGLTarget, GL_TEXTURE_MIN_FILTER, mOGLFilters[0] ) );
                GN_OGL_CHECK( glTexParameteri( mOGLTarget, GL_TEXTURE_MAG_FILTER, mOGLFilters[1] ) );
                GN_OGL_CHECK( glTexParameteri( mOGLTarget, GL_TEXTURE_WRAP_S, mOGLWraps[0] ) );
                GN_OGL_CHECK( glTexParameteri( mOGLTarget, GL_TEXTURE_WRAP_T, mOGLWraps[1] ) );
                if( TEXTYPE_3D == getDesc().type )
                {
                    GN_OGL_CHECK( glTexParameteri( mOGLTarget, GL_TEXTURE_WRAP_R, mOGLWraps[2] ) );
                }
            }

            GN_UNGUARD_SLOW;
        }

        //!
        //! \name get GL texture parameters
        //!
        //@{
        GLenum getOGLTarget()  const { return mOGLTarget; }
        GLuint getOGLTexture() const { return mOGLTexture; }
        GLint  getOGLInternalFormat() const { return mOGLInternalFormat; }
        //@}

        //!
        //! convert cubemap face to GL tag
        //!
        static GLenum sCubeface2OGL( size_t face )
        {
            GLenum sTable[NUM_CUBEFACES] =
            {
                GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB,
                GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB,
                GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB,
            };
            GN_ASSERT( face < 6 );
            return sTable[face];
        }

        // ********************************
        //  private variables
        // ********************************
    private:

        //!
        //! opengl texture target
        //!
        GLenum mOGLTarget;

        //!
        //! opengl texture handle
        //!
        GLuint mOGLTexture;

        //!
        //! \name opengl texture parameters
        //!
        //@{
        GLint  mOGLInternalFormat;
        GLuint mOGLFormat, mOGLType;
        bool   mOGLCompressed;
        //@}

        mutable bool   mFilterAndWrapDirty;
        mutable GLenum mOGLFilters[2]; //! filters (min,mag)
        mutable GLenum mOGLWraps[3];   //! address modes (s,t,r)

        //!
        //! \name 2D locking related variables
        //!
        //@{
        GLenum      mLockedTarget;
        size_t      mLockedLevel;
        Boxi        mLockedArea;
        LockFlag    mLockedFlag;
        uint8_t *   mLockedBuffer;
        size_t      mLockedBytes;
        //@}

        // ********************************
        //  private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of oglTexture.h
// *****************************************************************************
#endif // __GN_GFXOGL_OGLTEXTURE_H__
