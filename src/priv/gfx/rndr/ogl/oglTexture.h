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
            mSamplerDirty = true;
        }
        //@}

        // ********************************
        /// \name from Texture
        // ********************************
    public:

        //@{

        virtual void   updateMipmap( size_t face, size_t level, const Box<UInt32>* area, size_t rowPitch, size_t slicePitch, const void * data, SurfaceUpdateFlag flag );
        virtual void   readMipmap( size_t, size_t, MipmapData & ) { GN_UNIMPL(); }
        virtual void   blobWrite( const void *, size_t ) { GN_UNIMPL(); }
        virtual size_t blobRead( void * ) { GN_UNIMPL(); return 0; }
        virtual void   generateMipmapPyramid() { GN_UNIMPL(); }
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
            switch( mTarget )
            {
                case GL_TEXTURE_1D           : glDisable( GL_TEXTURE_2D );
                case GL_TEXTURE_2D           : if( GLEW_EXT_texture3D ) glDisable( GL_TEXTURE_3D_EXT );
                case GL_TEXTURE_3D_EXT       : if( GLEW_ARB_texture_cube_map ) glDisable( GL_TEXTURE_CUBE_MAP_ARB );
            }
            GN_OGL_CHECK( glEnable(mTarget) );
            GN_OGL_CHECK( glBindTexture(mTarget, mOGLTexture) );

            if( mSamplerDirty )
            {
                GN_OGL_CHECK( glTexParameteri( mTarget, GL_TEXTURE_MIN_FILTER, mOGLFilters[0] ) );
                GN_OGL_CHECK( glTexParameteri( mTarget, GL_TEXTURE_MAG_FILTER, mOGLFilters[1] ) );
                GN_OGL_CHECK( glTexParameteri( mTarget, GL_TEXTURE_WRAP_S, mOGLWraps[0] ) );
                GN_OGL_CHECK( glTexParameteri( mTarget, GL_TEXTURE_WRAP_T, mOGLWraps[1] ) );
                if( GL_TEXTURE_3D_EXT == mTarget )
                {
                    GN_OGL_CHECK( glTexParameteri( mTarget, GL_TEXTURE_WRAP_R, mOGLWraps[2] ) );
                }
                mSamplerDirty = false;
            }
        }

        ///
        /// set texture sampler (not effective until next binding)
        ///
        void setSampler( const TextureSampler & sampler, bool forceUpdate = false ) const;

        ///
        /// \name get GL texture parameters
        ///
        //@{
        GLenum getOGLTarget()  const { return mTarget; }
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
        GLenum mTarget;

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

        /// \name sampler stuff
        //@{
        mutable bool           mSamplerDirty;
        mutable TextureSampler mSampler;
        mutable GLint          mOGLFilters[2]; /// filters (min,mag)
        mutable GLint          mOGLWraps[3];   /// address modes (s,t,r)
        //@}

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
