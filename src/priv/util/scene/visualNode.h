#ifndef __GN_SCENE_VISUALNODE_H__
#define __GN_SCENE_VISUALNODE_H__
// *****************************************************************************
/// \file
/// \brief   Visual node implementation class
/// \author  chenli@@REDMOND (2009.9.9)
// *****************************************************************************

#include "standardUniforms.h"
#include <list>

namespace GN { namespace util
{
    /// VisualNode implementation class
    class VisualNode::Impl
    {
        // *****************************
        // public methods
        // *****************************

    public:

        //@{

        Impl( VisualNode & owner, VisualGraph & graph );

        virtual ~Impl();

        //@}

        // methods from VisualNode
        //@{

        VisualGraph & graph() const { return mGraph; }
        int           addModel( gfx::GpuResource * model );
        void          removeAllModels();
        bool          loadModelsFromFile( const char * filename );
        void          draw() const;

        //@}

        // *****************************
        // private data
        // *****************************

    private:

        typedef HandleManager<AutoRef<gfx::ModelResource>,int> ModelManager;

        typedef FixedArray<AutoRef<gfx::UniformResource>,StandardUniformType::NUM_STANDARD_UNIFORMS> StandardUniformArray;

        VisualNode               & mOwner;
        VisualGraph              & mGraph;
        std::list<Impl*>::iterator mGraphIter;
        ModelManager               mModels;
        StandardUniformArray       mStandardPerObjectUniforms;

        // *****************************
        // private methods
        // *****************************

    private:

        gfx::UniformResource * getPerObjectUniform( StandardUniformType type );
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
