#ifndef __GN_ENGINE_WORLD_H__
#define __GN_ENGINE_WORLD_H__
// *****************************************************************************
/// \file
/// \brief   Basic engine classes
/// \author  chen@@CHENLI-OLDPC (2011.2.24)
// *****************************************************************************

namespace GN { namespace engine
{
    // forward declarations
    class World;

    /// Component class root. Must be attached directly to an entity.
    class Component : public NoCopy
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

        Entity( World & );

    public:

        virtual ~Entity();

        Component *            getComponent( const Guid & type ) const;
        void                   addComponent( const Guid & type, Component & );
        void                   delComponent( const Guid & type );

        template<class T> T *  getComponent() const { return (T*)getComponent( T::sGetType() ); }
        template<class T> void addComponent( T & t ) { return addComponent( T::sGetType(), t ); }

        //public: virtual void processEvent( uint32 eventid, uint64 param1, void * param2 ) = 0;

    private:
    };

    /// The game world
    class World
    {
    public:

        /// Initialization and shutdown
        //@{

        // Initialize basic systems (file, memory, thread and etc.)
        bool initialize();

        // shutdown everthing.
        void shutdown();

        bool gfxInitialize();
        void gfxShutdown();

        bool inputInitialize();
        void inputShutdown();

        bool soundInitialize();
        void soundShutdown();

        bool networkInitialize();
        void networkShutdown();
        //@}

    public:

        // Graphics stuff
        GN::gfx::Gpu                 * getGpu() const;
        GN::gfx::GpuResourceDatabase * getGdb() const;
        GN::gfx::SpriteRenderer      * getSpriteRenderer() const;
        GN::gfx::BitmapFont          * getBmfRenderer() const;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_WORLD_H__
