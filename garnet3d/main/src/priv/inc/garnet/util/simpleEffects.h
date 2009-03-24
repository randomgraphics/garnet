#ifndef __GN_UTIL_SIMPLEEFFECTS_H__
#define __GN_UTIL_SIMPLEEFFECTS_H__
// *****************************************************************************
/// \file
/// \brief   simple (commonly used) effects
/// \author  chenli@@REDMOND (2009.1.20)
// *****************************************************************************

namespace GN { namespace util
{
    ///
    /// simple diffuse effect
    ///
    class SimpleDiffuseEffect : public StdClass
    {
        GN_DECLARE_STDCLASS( SimpleDiffuseEffect, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        SimpleDiffuseEffect() : mEffect(NULL) { clear(); }
        virtual ~SimpleDiffuseEffect() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( gfx::Renderer & );
        void quit();
    private:
        void clear()
        {
            mDefaultTexture = NULL;
            mEffect = NULL;
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{
        gfx::Effect * getEffect() const { return mEffect; }
        void setTransformation( const Matrix44f & proj, const Matrix44f & view, const Matrix44f & world );
        void setLightPos( const Vector4f & ); ///< set light position in world space
        void setLightColor( const Vector4f & ); ///< set light color
        void setDiffuseColor( const Vector4f & ); ///< set diffuse color
        void setDiffuseTexture( gfx::Texture * ); ///< set to NULL to use pure white texture.
        void setMesh( const gfx::Mesh & mesh, const gfx::MeshSubset * subset = NULL );
        void draw();
        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        gfx::Texture                * mDefaultTexture;
        gfx::Effect                 * mEffect;
        gfx::Uniform                * mMatrixPvw;
        gfx::Uniform                * mMatrixWorld;
        gfx::Uniform                * mMatrixWorldIT;
        gfx::Uniform                * mLightPos;
        gfx::Uniform                * mLightColor;
        gfx::Uniform                * mDiffuseColor;
        gfx::Drawable                 mDrawable;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_UTIL_SIMPLEEFFECTS_H__
