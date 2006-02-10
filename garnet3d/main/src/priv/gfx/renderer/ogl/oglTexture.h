#ifndef __GN_GFXOGL_OGLTEXTURE_H__
#define __GN_GFXOGL_OGLTEXTURE_H__
// *****************************************************************************
//! \file    ogl/oglTexture.h
//! \brief   OpenGL texture class
//! \author  chenlee (2005.11.13)
// *****************************************************************************

#include "../common/basicTexture.h"
#include "oglTypes.h"
#include "oglRenderer.h"

namespace GN { namespace gfx
{
    //!
    //! Basic OGL texture class
    //!
    class OGLBasicTexture : public BasicTexture,
                            public OGLResource,
                            public StdClass
    {
         GN_DECLARE_STDCLASS( OGLBasicTexture, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLBasicTexture( OGLRenderer & r ) :OGLResource(r) { clear(); }
        virtual ~OGLBasicTexture() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( TexType  type,
                   uint32_t sx, uint32_t sy, uint32_t sz,
                   uint32_t miplevels,
                   ClrFmt   format,
                   uint32_t usage );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear()
        {
            mOGLTexture = 0;
            mLockedLevel = 0;
            mLockedFlag = 0;
            mLockedBuffer = 0;

            mFilters[0] = mFilters[1] = TEXFILTER_LINEAR;
            mWraps[0] = mWraps[1] = mWraps[2] = TEXWRAP_REPEAT;
        }
        //@}

        // ********************************
        //       from OGLResource
        // ********************************

        //@{
    public:
        bool deviceCreate();
        bool deviceRestore() { return true; }
        void deviceDispose() {}
        void deviceDestroy();
        //@}

        // ********************************
        //! \name from Texture
        // ********************************
    public:

        //@{

        virtual void * lock1D( uint32_t level, uint32_t offset, uint32_t length, uint32_t flag )
        {
            GN_UNUSED_PARAM( level );
            GN_UNUSED_PARAM( offset );
            GN_UNUSED_PARAM( length );
            GN_UNUSED_PARAM( flag );
            GN_ERROR( "can't do 1D lock on %s texture", texType2Str( getType() ) );
            return 0;
        }
        virtual bool lock2D( LockedRect & result, uint32_t level, const Recti * area, uint32_t flag )
        {
            GN_UNUSED_PARAM( result );
            GN_UNUSED_PARAM( level );
            GN_UNUSED_PARAM( area );
            GN_UNUSED_PARAM( flag );
            GN_ERROR( "can't do 2D lock on %s texture", texType2Str( getType() ) );
            return false;
        }
        virtual bool lock3D( LockedBox & result, uint32_t level, const Boxi * box, uint32_t flag )
        {
            GN_UNUSED_PARAM( result );
            GN_UNUSED_PARAM( level );
            GN_UNUSED_PARAM( box );
            GN_UNUSED_PARAM( flag );
            GN_ERROR( "can't do 3D lock on %s texture", texType2Str( getType() ) );
            return false;
        }
        virtual bool lockCube( LockedRect & result, TexFace face, uint32_t level, const Recti * area, uint32_t flag )
        {
            GN_UNUSED_PARAM( result );
            GN_UNUSED_PARAM( face );
            GN_UNUSED_PARAM( level );
            GN_UNUSED_PARAM( area );
            GN_UNUSED_PARAM( flag );
            GN_ERROR( "can't do CUBE lock on %s texture", texType2Str( getType() ) );
            return false;
        }

        virtual void setFilter( TexFilter, TexFilter ) const;

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

            switch( getType() )
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
        static GLenum cubeface2GL( TexFace face )
        {
            GLenum sTable[NUM_TEXFACES] =
            {
                GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB,
                GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB,
                GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB,
            };
            GN_ASSERT( 0 <= face && face < 6 );
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

        mutable TexFilter mFilters[2]; //! filters (min,mag)
        mutable TexWrap   mWraps[3];   //! address modes (s,t,r)

        //!
        //! \name 2D locking related variables
        //!
        //@{
        GLenum      mLockedTarget;
        uint32_t    mLockedLevel;
        Recti       mLockedArea;
        uint32_t    mLockedFlag;
        uint8_t *   mLockedBuffer;
        size_t      mLockedBytes;
        //@}

        // ********************************
        //  protected functions
        // ********************************
    protected:

        //!
        //! set the 1st wrap mode
        //!
        void setWrapS( TexWrap mode ) const
        {
            if( mWraps[0] != mode )
            {
                mWraps[0] = mode;

                GN_OGL_CHECK( glTexParameteri(
                    mOGLTarget,
                    GL_TEXTURE_WRAP_S,
                    sTexWrap2OGL(TexWrap(mode)) ) );
            }
        }

        //!
        //! set the 2nd wrap mode
        //!
        void setWrapT( TexWrap mode ) const
        {
            if( mWraps[1] != mode )
            {
                mWraps[1] = mode;

                GN_OGL_CHECK( glTexParameteri(
                    mOGLTarget,
                    GL_TEXTURE_WRAP_T,
                    sTexWrap2OGL( mode ) ) );
            }
        }

        //!
        //! set the 3rd wrap mode
        //!
        void setWrapR( TexWrap mode ) const
        {
            GN_ASSERT( GLEW_EXT_texture3D );
            if( mWraps[2] != mode )
            {
                mWraps[2] = mode;

                GN_OGL_CHECK( glTexParameteri(
                    mOGLTarget,
                    GL_TEXTURE_WRAP_R,
                    sTexWrap2OGL( mode ) ) );
            }
        }

        //!
        //! create new texture instance
        //!
        virtual GLuint newOGLTexture( GLint   internalformat,
                                      GLsizei size_x,
                                      GLsizei size_y,
                                      GLsizei size_z,
                                      GLint   miplevels,
                                      GLenum  glformat,
                                      GLenum  gltype ) = 0;

        //!
        //! private 2D lock operation
        //!
        bool privateLock2D( LockedRect &  result,
                            GLenum        target,
                            uint32_t      miplevel,
                            const Recti * area,
                            uint32_t      flag );

        //!
        //! private 2D unlock function
        //!
        void privateUnlock2D();

        // ********************************
        //  private functions
        // ********************************
    private:

        //!
        //! map wrap mode to opengl constant
        //!
        GLenum sTexWrap2OGL( TexWrap w ) const
        {
            if( TEXWRAP_REPEAT == w ) return GL_REPEAT;
            else if( TEXWRAP_CLAMP == w ) return GL_CLAMP;
            else if( TEXWRAP_CLAMP_TO_EDGE == w )
            {
                if( GLEW_EXT_texture_edge_clamp )
                    return GL_CLAMP_TO_EDGE_EXT;
                else if (GLEW_SGIS_texture_edge_clamp )
                    return GL_CLAMP_TO_EDGE_SGIS;
                else
                {
                    GN_ERROR( "do not support clamp to edge!" );
                    return GL_CLAMP;
                }
            }
            else
            {
                GN_ASSERT_EX( 0, "invaid wrap type!" );
                return GL_REPEAT;
            }
        }
    };

    //!
    //! OpenGL 1D texture class
    //!
    class OGLTex1D : public OGLBasicTexture
    {
        // ********************************
        //! \name ctor/dtor
        // ********************************

        //@{
    public:
        OGLTex1D( OGLRenderer & r ) : OGLBasicTexture(r) {}
        ~OGLTex1D() {}
        //@}

        // ********************************
        //! \name from Texture
        // ********************************
    public:

        //@{

        virtual void getMipMapSize( uint32_t level, uint32_t * sx, uint32_t * sy, uint32_t * sz ) const;
        virtual void setWrap( TexWrap s, TexWrap t, TexWrap r ) const;
        virtual void * lock1D( uint32_t level, uint32_t offset, uint32_t length, uint32_t flag );
        virtual void unlock();

        //@}

        // ********************************
        //! \name from OGLBasicTexture
        // ********************************
    protected:
        virtual GLuint newOGLTexture( GLint   internalformat,
                                      GLsizei size_x,
                                      GLsizei size_y,
                                      GLsizei size_z,
                                      GLint   miplevels,
                                      GLenum  glformat,
                                      GLenum  gltype );
    private:
    };

    //!
    //! OpenGL 2D texture class
    //!
    class OGLTex2D : public OGLBasicTexture
    {
        // ********************************
        //! \name ctor/dtor
        // ********************************

        //@{
    public:
        OGLTex2D( OGLRenderer & r ) : OGLBasicTexture(r) {}
        ~OGLTex2D() {}
        //@}

        // ********************************
        //! \name from Texture
        // ********************************
    public:

        //@{

        virtual void getMipMapSize( uint32_t level, uint32_t * sx, uint32_t * sy, uint32_t * sz ) const;
        virtual void setWrap( TexWrap s, TexWrap t, TexWrap r ) const;
        virtual bool lock2D( LockedRect & result, uint32_t level, const Recti * area, uint32_t flag );
        virtual void unlock();

        //@}

        // ********************************
        //! \name from OGLBasicTexture
        // ********************************
    protected:
        virtual GLuint newOGLTexture( GLint   internalformat,
                                      GLsizei size_x,
                                      GLsizei size_y,
                                      GLsizei size_z,
                                      GLint   miplevels,
                                      GLenum  glformat,
                                      GLenum  gltype );
    private:
    };

    //!
    //! OpenGL 3D texture class
    //!
    class OGLTex3D : public OGLBasicTexture
    {
        // ********************************
        //! \name ctor/dtor
        // ********************************

        //@{
    public:
        OGLTex3D( OGLRenderer & r ) : OGLBasicTexture(r) {}
        ~OGLTex3D() {}
        //@}

        // ********************************
        //! \name from Texture
        // ********************************
    public:

        //@{

        virtual void getMipMapSize( uint32_t level, uint32_t * sx, uint32_t * sy, uint32_t * sz ) const;
        virtual void setWrap( TexWrap s, TexWrap t, TexWrap r ) const;
        virtual bool lock3D( LockedBox &  result, uint32_t level, const Boxi * box, uint32_t flag );
        virtual void unlock();

        //@}

        // ********************************
        //! \name from OGLBasicTexture
        // ********************************
    protected:
        virtual GLuint newOGLTexture( GLint   internalformat,
                                      GLsizei size_x,
                                      GLsizei size_y,
                                      GLsizei size_z,
                                      GLint   miplevels,
                                      GLenum  glformat,
                                      GLenum  gltype );
    private:
    };

    //!
    //! OpenGL cube texture class
    //!
    class OGLTexCube : public OGLBasicTexture
    {
        // ********************************
        //! \name ctor/dtor
        // ********************************

        //@{
    public:
        OGLTexCube( OGLRenderer & r ) : OGLBasicTexture(r) {}
        ~OGLTexCube() {}
        //@}

        // ********************************
        //! \name from Texture
        // ********************************
    public:

        //@{

        virtual void getMipMapSize( uint32_t level, uint32_t * sx, uint32_t * sy, uint32_t * sz ) const;
        virtual void setWrap( TexWrap, TexWrap, TexWrap ) const;
        virtual bool lockCube( LockedRect & result, TexFace face, uint32_t level, const Recti * area, uint32_t flag );
        virtual void unlock();

        //@}

        // ********************************
        //! \name from OGLBasicTexture
        // ********************************
    protected:
        virtual GLuint newOGLTexture( GLint   internalformat,
                                      GLsizei size_x,
                                      GLsizei size_y,
                                      GLsizei size_z,
                                      GLint   miplevels,
                                      GLenum  glformat,
                                      GLenum  gltype );
    private:
    };
}}

// *****************************************************************************
//                           End of oglTexture.h
// *****************************************************************************
#endif // __GN_GFXOGL_OGLTEXTURE_H__
