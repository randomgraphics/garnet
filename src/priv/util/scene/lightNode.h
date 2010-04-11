#ifndef __GN_SCENE_LIGHTNODE_H__
#define __GN_SCENE_LIGHTNODE_H__
// *****************************************************************************
/// \file
/// \brief   Light node implementation class
/// \author  chenli@@REDMOND (2009.9.9)
// *****************************************************************************

#include <list>

namespace GN { namespace util
{
    /// LightNode implementation class
    class LightNode::Impl
    {
        // *****************************
        // public methods
        // *****************************

    public:

        //@{

        Impl( LightNode & owner, VisualGraph & graph );

        virtual ~Impl();

        //@}

        // methods from LightNode
        //@{

        LightNode       & owner() const { return mOwner; }
        const LightDesc & getDesc() const { return mDesc; }
        void              setDesc( const LightDesc & desc ) { mDesc = desc; }

        //@}

        // *****************************
        // private data
        // *****************************

    private:

        LightNode                & mOwner;
        VisualGraph              & mGraph;
        std::list<Impl*>::iterator mGraphIter;

        LightDesc                  mDesc;

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
