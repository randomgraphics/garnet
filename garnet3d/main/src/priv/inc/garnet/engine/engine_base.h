#ifndef __GN_ENGINE_ENGINE_BASE_H__
#define __GN_ENGINE_ENGINE_BASE_H__
// *****************************************************************************
/// \file
/// \brief   Basic engine classes
/// \author  chen@@CHENLI-OLDPC (2011.2.24)
// *****************************************************************************

namespace GN { namespace engine
{
    class Entity;

    /// Component class root. Can be attached to at most one entity at any moment.
    class Component : public RefCounter
    {
    protected:

        Component() : mEntity(NULL)
        {
        }

    public:

        virtual ~Component()
        {
        }

        Entity * getEntity() const { return mEntity; }

        virtual const Guid & getType() const = 0;

    private:

        friend class Entity;
        Entity * mEntity;
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

        /// Get entity's component. No increasing reference counter of the component.
        Component * getComponent( const Guid & type ) const;

        /// Set entity's component.
        ///   - Decease reference counter of existing component by 1.
        ///   - Increase reference counter of the new component by 1.
        ///   - Null component pointer is allowed.
        void setComponent( const Guid & type, Component * comp );

        /// tempalte helpers
        //@{
        template<class T> T *  getComponent() const { return (T*)getComponent( T::sGetType() ); }
        template<class T> void setComponent( T * t ) { return setComponent( T::sGetType(), t ); }
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
