#ifndef __GN_ENGINE_ENGINE_BASE_H__
#define __GN_ENGINE_ENGINE_BASE_H__
// *****************************************************************************
/// \file
/// \brief   Basic engine classes
/// \author  chen@@CHENLI-OLDPC (2011.2.24)
// *****************************************************************************

/// Declare an entity type
#define GN_ENGINE_DECLARE_ENTITY( self, parent ) \
    public: static  const EntityType & sGetType(); \
            static  bool               sIsTypeOf( const EntityType & ); \
            virtual bool               isTypeOf( const EntityType & ) const; \
    private: typedef self SelfClass; typedef parent ParentClass;

/// Implement common entity methods
#define GN_ENGINE_IMPLEMENT_ENTITY( self, type ) \
    const GN::engine::EntityType & self::sGetType() { return type; } \
    bool                           self::sIsTypeOf( const EntityType & t ) { return ParentClass::sIsTypeOf(t) || t == (type); } \
    bool                           self::isTypeOf( const EntityType & t ) const { return ParentClass::isTypeOf(t) || t == (type); }

namespace GN { namespace engine
{
    /// Define entity type class
    typedef Guid EntityType;

    ///
    /// Entity refernce class
    ///
    template<typename T>
    class EntityRef : public WeakRef<T>
    {
    public:

        ///
        /// Destructor (non virtual)
        ///
        ~EntityRef()
        {
            clear();
        }
    };

    /// Entity class. Root class of game play object that could be placed into game world.
    class Entity : public WeakObject, public NoCopy
    {
    protected:

        /// constructor
        Entity();

    public:

        /// destructor
        virtual ~Entity();

        /// See if the entity class implementes a specific entity type. (static version)
        static bool sIsTypeOf( const EntityType & ) { return false; }

        /// See if the entity class implementes a specific entity type. (virtual version)
        virtual bool isTypeOf( const EntityType & ) const { return false; }

        /// Get unique entity ID
        int getID() const { return mID; }

        /// Get entity's component. No increasing reference counter of the component.
        Entity * getComponent( const EntityType & type ) const;

        /// Set entity's component.
        ///   - Decease reference counter of existing component by 1.
        ///   - Increase reference counter of the new component by 1.
        ///   - Null component pointer is allowed.
        void setComponent( const EntityType & type, Entity * comp );

        /// tempalte helpers
        //@{
        template<class T> T *  getComponent() const { return (T*)getComponent( T::sGetType() ); }
        template<class T> void setComponent( T * t ) { return setComponent( T::sGetType(), t ); }
        //@}

        //public: virtual void processEvent( uint32 eventid, uint64 param1, void * param2 ) = 0;

    private:

        typedef HashMap<EntityType, EntityRef<Entity>, 128, EntityType::Hash> ComponentMap;

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

    /// Graphics initialization options
    struct GfxInitOptions
    {
        gfx::GpuOptions           gpuOptions;          ///< renderer options
        bool                      useMultithreadGpu;   ///< use multithread renderer or not.
        gfx::FontFaceCreationDesc defaultAsciiFont;    ///< default ascii font face creation descriptor.
        gfx::FontFaceCreationDesc defaultNonAsciiFont; ///< default non-ascii font face creation descriptor
    };

    bool inputInitialize( input::InputAPI );
    void inputShutdown();

    bool gfxInitialize( const GfxInitOptions & );
    void gfxShutdown();

    //bool soundInitialize();
    //void soundShutdown();

    //bool networkInitialize();
    //void networkShutdown();

    // Graphics stuff
    gfx::Gpu                 * getGpu();
    gfx::SpriteRenderer      * getSpriteRenderer();
    gfx::LineRenderer        * getLineRenderer();
    gfx::BitmapFont          * getDefaultFontRenderer();
    gfx::GpuResourceDatabase * getGdb();

    // Entity management
    Entity * findEntity( int id );

    /// Functions in this namespace are reserved for engine module itself,
    /// and should not be called by engine clients or entity/component developers.
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
