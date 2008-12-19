#ifndef __GN_GFXOGL_OGLTEXTURE_H__
#define __GN_GFXOGL_OGLTEXTURE_H__
// *****************************************************************************
/// \file
/// \brief   OpenGL texture class
/// \author  chenlee (2005.11.13)
// *****************************************************************************

#include "../common/basicSurface.h"
#include "oglResource.h"
#include "oglRenderer.h"

namespace GN { namespace gfx
{
    ///
    /// OGL texture class
    ///
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
        bool init( const TextureDesc & );
        void quit();
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
        /// \name from Texture
        // ********************************
    public:

        //@{

        virtual void update( size_t face, size_t level, const Box<UInt32>* area, size_t rowPitch, size_t slicePitch, const void * data, UpdateFlag flag );
        virtual void readback( size_t, size_t, MipmapData & ) { GN_UNIMPL_WARNING(); }
        virtual void generateMipmap() { GN_UNIMPL_WARNING(); }
        virtual void * getAPIDependentData() const { return (void*)(uintptr_t)getOGLTexture(); }

        //@}

        // ********************************
        //  public functions
        // ********************************
    public:

        ///
        /// bind to current texture stage
        ///
        void bind() const
        {
            /*GN_GUARD_SLOW;

            switch( getDesc().dim )
            {
                case TEXDIM_1D   :
                case TEXDIM_2D   :
                    if( GLEW_EXT_texture3D )
                        glDisable( GL_TEXTURE_3D_EXT );
                case TEXDIM_3D   :
                    if( GLEW_ARB_texture_cube_map )
                        glDisable( GL_TEXTURE_CUBE_MAP_ARB );
                case TEXDIM_CUBE :
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
                if( TEXDIM_3D == getDesc().dim )
                {
                    GN_OGL_CHECK( glTexParameteri( mOGLTarget, GL_TEXTURE_WRAP_R, mOGLWraps[2] ) );
                }
            }

            GN_UNGUARD_SLOW;*/
        }

        ///
        /// \name get GL texture parameters
        ///
        //@{
        GLenum getOGLTarget()  const { return mOGLTarget; }
        GLuint getOGLTexture() const { return mOGLTexture; }
        GLint  getOGLInternalFormat() const { return mOGLInternalFormat; }
        //@}

        ///
        /// convert cubemap face to GL tag
        ///
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

        ///
        /// opengl texture target
        ///
        GLenum mOGLTarget;

        ///
        /// opengl texture handle
        ///
        GLuint mOGLTexture;

        ///
        /// \name opengl texture parameters
        ///
        //@{
        GLint  mOGLInternalFormat;
        GLuint mOGLFormat, mOGLType;
        bool   mOGLCompressed;
        //@}

        mutable bool   mFilterAndWrapDirty;
        mutable GLenum mOGLFilters[2]; /// filters (min,mag)
        mutable GLenum mOGLWraps[3];   /// address modes (s,t,r)

        // ********************************
        //  private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXOGL_OGLTEXTURE_H__
