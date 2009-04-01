#ifndef __GN_SCENE_SPATIALSCENEGRAPHIMPL_H__
#define __GN_SCENE_SPATIALSCENEGRAPHIMPL_H__
// *****************************************************************************
/// \file
/// \brief   spatial scene graph implementation class
/// \author  chen@@CHENLI-HOMEPC (2009.3.29)
// *****************************************************************************

#include "garnet/GNscene.h"

namespace GN { namespace scene
{
    ///
    /// query result implementation class
    ///
    class QueryResultImpl : public SpatialSceneGraph::QueryResult
    {
    public:
        virtual SpatialSceneGraph::Node * getNodeAt( size_t i ) const;
        virtual size_t                    numNodes() const;
    };

    ///
    /// spatial scene graph implementation class
    ///
    class SpatialSceneGraphImpl : public SpatialSceneGraph
    {
        GN_DECLARE_STDCLASS( SpatialSceneGraphImpl, SpatialSceneGraph );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        SpatialSceneGraphImpl() : mRoot(NULL) { clear(); }
        virtual ~SpatialSceneGraphImpl() { quit(); }
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
        // from SpatialSceneGraph
        // ********************************
    public:

        //@{
        virtual void setProj( const Matrix44f & ) = 0;
        virtual void setView( const Matrix44f & ) = 0;
        virtual QueryResult * getVisibleNodes() = 0;
        virtual QueryResult * getNodesNearby( size_t maxNodeCount ) = 0;
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        HandleType onNodeConstruction( Node & );
        void       onNodeDestruction( HandleType );

        // ********************************
        // private variables
        // ********************************
    private:

        Node * mRoot; ///< the root node

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_SPATIALSCENEGRAPHIMPL_H__