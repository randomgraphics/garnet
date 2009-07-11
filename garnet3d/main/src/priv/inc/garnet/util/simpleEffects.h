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
    /// simple wireframe effect (no lighting, requires position only)
    ///
    class SimpleWireframeEffect : public StdClass
    {
        GN_DECLARE_STDCLASS( SimpleWireframeEffect, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        SimpleWireframeEffect() : mEffect(NULL) { clear(); }
        virtual ~SimpleWireframeEffect() { quit(); }
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
            mEffect = NULL;
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{
        gfx::Effect * getEffect() const { return mEffect; }
        void setTransformation( const Matrix44f & proj, const Matrix44f & view, const Matrix44f & world ); ///< Defaults are identity matrices.
        void setColor( const Vector4f & ); ///< set wireframe color, default is (1,1,1,1)
        void setMesh( const gfx::Mesh & mesh, const gfx::MeshSubset * subset = NULL ); ///< mesh should have position, normal and texcoord
        void setRenderTarget( const gfx::RenderTargetTexture * color, const gfx::RenderTargetTexture * depth ); ///< set both color and depth to NULL to render to back buffer, which is the default behavior.
        void draw();
        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        gfx::Effect              * mEffect;
        gfx::RenderTargetTexture * mColorTarget;
        gfx::RenderTargetTexture * mDepthTarget;
        gfx::Drawable              mDrawable;
        AutoRef<gfx::Uniform>    * mMatrixPvw;
        AutoRef<gfx::Uniform>    * mColor;

        // ********************************
        // private functions
        // ********************************
    private:
    };    
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
        void setTransformation( const Matrix44f & proj, const Matrix44f & view, const Matrix44f & world ); ///< Defaults are identity matrices.
        void setLightPos( const Vector4f & ); ///< set light position in world space, default is (0,0,0)
        void setLightColor( const Vector4f & ); ///< set light color, default is (1,1,1,1)
        void setAlbedoColor( const Vector4f & ); ///< set albedo color, default is (1,1,1,1)
        void setAlbedoTexture( gfx::Texture * ); ///< set to NULL to use pure white texture. Default is NULL.
        void setMesh( const gfx::Mesh & mesh, const gfx::MeshSubset * subset = NULL ); ///< mesh should have position, normal and texcoord
        void setRenderTarget( const gfx::RenderTargetTexture * color, const gfx::RenderTargetTexture * depth ); ///< set both color and depth to NULL to render to back buffer, which is the default behavior.
        void draw();
        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        gfx::Texture             * mDefaultTexture;
        gfx::Effect              * mEffect;
        AutoRef<gfx::Uniform>    * mMatrixPvw;
        AutoRef<gfx::Uniform>    * mMatrixWorld;
        AutoRef<gfx::Uniform>    * mMatrixWorldIT;
        AutoRef<gfx::Uniform>    * mLightPos;
        AutoRef<gfx::Uniform>    * mLightColor;
        AutoRef<gfx::Uniform>    * mAlbedoColor;
        AutoRef<gfx::Texture>    * mAlbedoTexture;
        gfx::RenderTargetTexture * mColorTarget;
        gfx::RenderTargetTexture * mDepthTarget;
        gfx::Drawable              mDrawable;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    ///
    /// simple normal map effect
    ///
    class SimpleNormalMapEffect : public StdClass
    {
        GN_DECLARE_STDCLASS( SimpleNormalMapEffect, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        SimpleNormalMapEffect() : mEffect(NULL) { clear(); }
        virtual ~SimpleNormalMapEffect() { quit(); }
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
            mDefaultAlbedoTexture = NULL;
            mDefaultNormalTexture = NULL;
            mEffect = NULL;
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //@{
        gfx::Effect * getEffect() const { return mEffect; }
        void setTransformation( const Matrix44f & proj, const Matrix44f & view, const Matrix44f & world ); ///< Defaults are identity matrices.
        void setLightPos( const Vector4f & ); ///< set light position in world space, default is (0,0,0)
        void setLightColor( const Vector4f & ); ///< set light color, default is (1,1,1,1)
        void setAlbedoColor( const Vector4f & ); ///< set albedo color, default is (1,1,1,1)
        void setAlbedoTexture( gfx::Texture * ); ///< set to NULL to use pure white texture. Default is NULL.
        void setNormalTexture( gfx::Texture * ); ///< set to NULL to use flat normal texture. Defau is NULL.
        void setMesh( const gfx::Mesh & mesh, const gfx::MeshSubset * subset = NULL ); ///< mesh should have position, normal, tangent and texcoord
        void setRenderTarget( const gfx::RenderTargetTexture * color, const gfx::RenderTargetTexture * depth ); ///< set both color and depth to NULL to render to back buffer, which is the default behavior.
        void draw();
        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        gfx::Texture             * mDefaultAlbedoTexture;
        gfx::Texture             * mDefaultNormalTexture;
        gfx::Effect              * mEffect;
        AutoRef<gfx::Uniform>    * mMatrixPvw;
        AutoRef<gfx::Uniform>    * mMatrixWorld;
        AutoRef<gfx::Uniform>    * mMatrixWorldIT;
        AutoRef<gfx::Uniform>    * mLightPos;
        AutoRef<gfx::Uniform>    * mLightColor;
        AutoRef<gfx::Uniform>    * mAlbedoColor;
        AutoRef<gfx::Texture>    * mAlbedoTexture;
        AutoRef<gfx::Texture>    * mNormalTexture;
        gfx::RenderTargetTexture * mColorTarget;
        gfx::RenderTargetTexture * mDepthTarget;
        gfx::Drawable              mDrawable;

        // ********************************
        // private functions
        // ********************************
    private:
    };}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_UTIL_SIMPLEEFFECTS_H__
