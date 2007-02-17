#ifndef __GN_SCENE_GNSCENE_H__
#define __GN_SCENE_GNSCENE_H__
// *****************************************************************************
//! \file    garnet/GNscene.h
//! \brief   public header of scene module
//! \author  chenli@@FAREAST (2007.2.17)
// *****************************************************************************

#include "GNcore.h"
#include "GNgfx.h"
#include "GNutil.h"

namespace GN
{
    ///
    /// namespace for scene module
    ///
    namespace scene
    {
    }
}

namespace GN { namespace scene
{
    class Scene; // forward declaration

    ///
    /// basic scene object
    ///
    class SceneObjectBase
    {
        Scene & mScene;

    public:

        ///
        /// constructor
        ///
        SceneObjectBase( Scene & s ) : mScene(s) {}


        ///
        /// destructor
        ///
        virtual ~SceneObjectBase()
        {
        }
    };

    ///
    /// scene camera
    ///
    class Camera : public SceneObjectBase
    {
    public:

        ///
        /// ctor
        ///
        Camera( Scene & s ) : SceneObjectBase(s) {}
    };
    
    ///
    /// global scene manager
    ///
    class Scene : public StdClass
    {
        GN_DECLARE_STDCLASS( Scene, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        Scene()          { clear(); }
        virtual ~Scene() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// draw the whole scene to screen
        ///
        void draw();

        // ********************************
        // private variables
        // ********************************
    private:

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of GNscene.h
// *****************************************************************************
#endif // __GN_SCENE_GNSCENE_H__
