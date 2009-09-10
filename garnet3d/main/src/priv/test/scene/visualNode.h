#ifndef __GN_SCENE_VISUALNODE_H__
#define __GN_SCENE_VISUALNODE_H__
// *****************************************************************************
/// \file
/// \brief   Visual node implementation class
/// \author  chenli@@REDMOND (2009.9.9)
// *****************************************************************************

#include "scene.h"

namespace GN { namespace scene
{
    /// VisualNode implementation class
    class VisualNode::Impl
    {
        // *****************************
        // public methods
        // *****************************

    public:

        //@{

        Impl( VisualNode & owner, VisualGraph & graph ) : mOwner(owner), mGraph(graph) {}

        virtual ~Impl() {}

        //@}

        // methods from VisualNode
        //@{

        VisualGraph & graph() const { return mGraph; }
        void          addModel( gfx::GpuResource * model );
        void          draw( Camera & ) const;

        //@}

        // *****************************
        // private data
        // *****************************

    private:

        VisualNode  & mOwner;
        VisualGraph & mGraph;

        // *****************************
        // private methods
        // *****************************

    private:

    };

    ///
    /// create new instance of VisualNode
    ///
    VisualNode * newVisualNode( Entity & entity, VisualGraph & graph );
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_VISUALNODE_H__
