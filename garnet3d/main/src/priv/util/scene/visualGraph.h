#ifndef __GN_SCENE_VISUALGRAPH_H__
#define __GN_SCENE_VISUALGRAPH_H__
// *****************************************************************************
/// \file
/// \brief   Visual graph implementation class
/// \author  chenli@@REDMOND (2009.9.10)
// *****************************************************************************

#include "standardUniforms.h"
#include <list>

namespace GN { namespace util
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

        Impl( VisualGraph & owner, gfx::GpuResourceDatabase & gdb );

        virtual ~Impl();

        //@}

        //@{

        gfx::GpuResourceDatabase & gdb() const { return mGdb; }

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
        gfx::GpuResourceDatabase    & mGdb;
        AutoRef<gfx::UniformResource> mUniforms[StandardUniformType::NUM_STANDARD_UNIFORMS];

        std::list<VisualNode::Impl*>  mVisualNodes;
        std::list<LightNode::Impl*>   mLightNodes;
    };
}}
// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_VISUALGRAPH_H__
