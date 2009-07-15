// Experiment with a world with infinit detail (like earth in google earth)
#include "GNgfx.h"

using namespace GN::gfx;

namespace GN
{
    struct SpatialInfo
    {
        Entity * parentEntity;
    };

    struct VisualInfo
    {
        ResourceHandle model;
        void           render();
    };

    struct Entity : public NoCopy
    {
        virtual SpatialInfo & getSpatialInfo() = 0;
        virtual VisualInfo  & getVisualInfo() = 0;
    };

    struct CameraInfo
    {
    };

    struct World : public NoCopy
    {
        // resource managers
        ResourceManagerTempl<Entity*>  entities;

        // render to world (draw to screen, play sound and etc.)
        virtual void render( const CameraInfo & ) = 0;
    };
}
