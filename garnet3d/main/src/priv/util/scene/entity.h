#ifndef __GN_SCENE_ENTITY_H__
#define __GN_SCENE_ENTITY_H__
// *****************************************************************************
/// \file
/// \brief   Entity implementation class
/// \author  chenli@@REDMOND (2009.9.9)
// *****************************************************************************

namespace GN { namespace util
{
    ///
    /// Entity implementation class
    ///
    class Entity::Impl
    {
    public:

        /// constructor
        Impl( World::Impl & w, Entity & o, int id ) : mWorld(w), mOwner(o), mID(id) {}

        /// destructor
        ~Impl();

        /// methods from Entity
        //@{

        World             & world() const;
        int                 id() const { return mID; }
        const char        * name() const;

        bool                hasNode( const Guid & nodeType ) const;
        const NodeBase    * getNode( const Guid & nodeType ) const;
        NodeBase          * getNode( const Guid & nodeType );
        void                attachNode( const Guid & nodeType, NodeBase * node );

        //@}

    private:

        typedef std::map<Guid, NodeBase*> NodeMap;

        World::Impl & mWorld;
        Entity      & mOwner;
        const int     mID;
        NodeMap       mNodes;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_ENTITY_H__
