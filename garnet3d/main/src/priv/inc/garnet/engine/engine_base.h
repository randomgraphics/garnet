#ifndef __GN_ENGINE_ENGINE_BASE_H__
#define __GN_ENGINE_ENGINE_BASE_H__
// *****************************************************************************
/// \file
/// \brief   Basic engine classes
/// \author  chen@@CHENLI-OLDPC (2011.2.24)
// *****************************************************************************

namespace GN { namespace engine
{
    /// Component class root. Must be attached directly to an entity.
    class Component : public RefCounter
    {
    protected:

        Component()
        {
        }

    public:

        virtual ~Component()
        {
        }

        virtual const Guid & getType() const = 0;
    };

    /// Entity class. Root class of game play object that could be placed into game world.
    class Entity : public NoCopy
    {
    protected:

        /// constructor
        Entity();

    public:

        /// destructor
        virtual ~Entity();

        /// Get entity ID
        int getID() const { return mID; }

        /// Get component pointer, but do not increase the reference counter of the component.
        Component * getComponent( const Guid & type ) const;

        /// Add a new component to the entity. Increase component reference counter by one
        void addComponent( const Guid & type, Component * );

        /// Delete specific component. Decrease the component's reference counter by one.
        void delComponent( const Guid & type );

        /// tempalte helpers
        //@{
        template<class T> T *  getComponent() const { return (T*)getComponent( T::sGetType() ); }
        template<class T> void addComponent( T & t ) { return addComponent( T::sGetType(), t ); }
        //@}

        //public: virtual void processEvent( uint32 eventid, uint64 param1, void * param2 ) = 0;

    private:

        typedef HashMap<Guid, Component*, Guid::Hash> ComponentMap;

        int           mID;
        ComponentMap  mComponents;
    };

    // --------------------------------------------------------------------------
    // Global engine functions
    // --------------------------------------------------------------------------

    //@{

    // Initialize basic systems (file, memory, thread and etc.)
    bool initialize();

    // shutdown everthing.
    void shutdown();

    bool gfxInitialize();
    void gfxShutdown();

    bool inputInitialize();
    void inputShutdown();

    //bool soundInitialize();
    //void soundShutdown();

    //bool networkInitialize();
    //void networkShutdown();

    // Graphics stuff
    GN::gfx::Gpu                 * getGpu();
    GN::gfx::GpuResourceDatabase * getGdb();
    GN::gfx::SpriteRenderer      * getSpriteRenderer();
    GN::gfx::BitmapFont          * getBmfRenderer();

    // Entity management
    Entity * findEntity( int id );

    /// Functions in this namespace are reserved for engine module itself,
    /// and should not be called by engine clients or entity/component developer.
    namespace impl
    {
        /// Entity management
        //@{
        int  onEntityCtor( Entity * );
        void onEntityDtor( int );
        //@}
    }

    //@}
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_ENGINE_BASE_H__
