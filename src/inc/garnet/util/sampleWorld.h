#ifndef __GN_UTIL_SAMPLEWORLD_H__
#define __GN_UTIL_SAMPLEWORLD_H__
// *****************************************************************************
/// \file
/// \brief   Sample world
/// \author  chen@@chenli-homepc (2009.9.6)
// *****************************************************************************

#include <garnet/GNengine.h>

namespace GN {
namespace util {
///
/// Virtual world used in sample application
///
class SampleWorld {
    engine::Entity *                  mRoot;
    StringMap<char, engine::Entity *> mEntities; // entities (not including root)
    bool                              mShowBBox;

public:
    //@{

    SampleWorld();
    ~SampleWorld();
    void             clear();
    bool             createEntites(const gfx::ModelHierarchyDesc & desc);
    engine::Entity * getRootEntity() const { return mRoot; }
    void             draw(const Matrix44f & proj, const Matrix44f & view) const; //< Draw all entities in the world.
    void             showBoundingBoxes(bool s) { mShowBBox = s; }
    //@}
};
} // namespace util
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_UTIL_SAMPLEWORLD_H__
