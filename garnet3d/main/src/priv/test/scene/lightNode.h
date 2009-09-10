#ifndef __GN_SCENE_LIGHTNODE_H__
#define __GN_SCENE_LIGHTNODE_H__
// *****************************************************************************
/// \file
/// \brief   Light node implementation class
/// \author  chenli@@REDMOND (2009.9.9)
// *****************************************************************************

#include "scene.h"

namespace GN { namespace scene
{
    /// LightNode implementation class
    class LightNode::Impl
    {
        // *****************************
        // public methods
        // *****************************

    public:

        //@{

        Impl( LightNode & owner, VisualGraph & graph ) : mOwner(owner), mGraph(graph) {}

        virtual ~Impl() {}

        //@}

        // methods from LightNode
        //@{

        //@}

        // *****************************
        // private data
        // *****************************

    private:

        LightNode   & mOwner;
        VisualGraph & mGraph;

        // *****************************
        // private methods
        // *****************************

    private:

    };

    ///
    /// create new instance of LightNode
    ///
    LightNode * newLightNode( Entity & entity, VisualGraph & graph );
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_LIGHTNODE_H__
