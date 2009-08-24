#ifndef __GN_SCENE_MANAGER_H__
#define __GN_SCENE_MANAGER_H__
// *****************************************************************************
/// \file
/// \brief   SceneManager manager
/// \author  chen@@CHENLI-HOMEPC (2009.3.29)
// *****************************************************************************

namespace GN { namespace scene
{
    ///
    /// (read-only) global uniform collection
    ///
    class GlobalUniformCollection
    {
    public:

        /// bracket operator
        gfx::Uniform & operator[]( size_t type ) const
        {
            if( type >= NUM_STANDARD_SCENE_PARAMETERS )
            {
                static Logger * sLogger = getLogger("GN.scene");
                GN_ERROR(sLogger)( "Invalid scene parameter type: %d", type );
                GN_ASSERT( mDummy );
                return *mDummy;
            }

            const StandardSceneParameterDesc & desc = getStandardSceneParameterName( type );

            if( !desc.global )
            {
                static Logger * sLogger = getLogger("GN.scene");
                GN_ERROR(sLogger)( "Non-global parameter \"%s\" is not accessible through global uniform collection.", desc.name );
                GN_ASSERT( mDummy );
                return *mDummy;
            }

            GN_ASSERT( mUniforms[type] );
            return *mUniforms[type];
        }

    protected:

        /// protected ctor
        GlobalUniformCollection() : mUniforms(NULL), mDummy(NULL) {}

        /// protected dtor
        virtual ~GlobalUniformCollection() {}

    protected:

        // Note: sub-class should initialize those pointers.

        /// global uniform array
        gfx::Uniform * const * mUniforms;

        /// dummy uniform pointer
        gfx::Uniform *         mDummy;
    };

    ///
    /// public scene manager interface
    ///
    class SceneManager : public NoCopy
    {
    public:

        /// get renderer
        virtual gfx::Renderer & getRenderer() const = 0;

        /// \name spatial scene graph
        //@{
        virtual SpatialSceneGraph & getSpatialSceneGraph() const = 0;
        //@}

        /// \name global uniform management
        //@{
        GlobalUniformCollection & globalUniforms;
        virtual void setProj( const Matrix44f & ) = 0;
        virtual void setView( const Matrix44f & ) = 0;
        virtual void setDefaultLight0Position( const Vector3f & ) = 0;
        virtual void updateGlobalUniforms() = 0;
        //@}

    protected:

        SceneManager( GlobalUniformCollection & uc ) : globalUniforms(uc) {}
    };

    ///
    /// create simple scene object
    ///
    SceneManager * createSceneManager( gfx::Renderer & );
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_MANAGER_H__
