#ifndef __GN_SCENE_VISUALGRAPH_H__
#define __GN_SCENE_VISUALGRAPH_H__
// *****************************************************************************
/// \file
/// \brief   Visual graph implementation class
/// \author  chenli@@REDMOND (2009.9.10)
// *****************************************************************************

#include "scene.h"
#include "standardUniforms.h"
#include <list>

namespace GN { namespace scene
{
    ///
    /// VisualGraph implementation class
    ///
    class VisualGraph::Impl
    {
        // *****************************
        // public methods
        // *****************************

    public:

        //@{

        Impl( VisualGraph & owner );

        virtual ~Impl();

        //@}

        //@{

        /// get pointer to specific standard uniform resource, if
        /// that uniform is not initialized, then initialized it.
        gfx::UniformResource * getGlobalUniform( gfx::GpuResourceDatabase & db, StandardUniformType type );

        /// get pointer to specific standard uniform resource, assuming that it is initialized already.
        gfx::UniformResource * getGlobalUniform( StandardUniformType type ) const;

        /// draw all visual node associated with this graph
        void                   draw( Camera & camera );

        //@}

        /// These method should only be called by node constructor and destructor
        //@{
        std::list<VisualNode::Impl*>::iterator insertVisualNode( VisualNode::Impl * node );
        void                                   removeVisualNode( std::list<VisualNode::Impl*>::iterator iter );
        std::list<LightNode::Impl*>::iterator  insertLightNode( LightNode::Impl * node );
        void                                   removeLightNode( std::list<LightNode::Impl*>::iterator iter );
        //@}

        // *****************************
        // private methods
        // *****************************

    private:

        /// update values of global uniforms
        void updateTransformation( Camera & c );

        void updateDefaultLighting();

        // *****************************
        // private data
        // *****************************

    private:

        VisualGraph                 & mOwner;
        AutoRef<gfx::UniformResource> mUniforms[StandardUniformType::NUM_STANDARD_UNIFORMS];
        AutoRef<gfx::UniformResource> mDummyUniform;

        std::list<VisualNode::Impl*>  mVisualNodes;
        std::list<LightNode::Impl*>   mLightNodes;
    };
}}
// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_VISUALGRAPH_H__
