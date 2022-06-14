#ifndef __GN_ENGINE_ENGINE_BASE_H__
#define __GN_ENGINE_ENGINE_BASE_H__
// *****************************************************************************
/// \file
/// \brief   Basic engine classes
/// \author  chen@@CHENLI-OLDPC (2011.2.24)
// *****************************************************************************

/// Declare an entity type
#define GN_ENGINE_DECLARE_ENTITY(self, parent)                    \
public:                                                           \
    static const EntityType & sGetType();                         \
    static bool               sIsTypeOf(const EntityType &);      \
    virtual bool              isTypeOf(const EntityType &) const; \
                                                                  \
private:                                                          \
    typedef self   SelfClass;                                     \
    typedef parent ParentClass;

/// Implement common entity methods
#define GN_ENGINE_IMPLEMENT_ENTITY(self, type)                                                                                \
    const GN::engine::EntityType & self::sGetType() { return type; }                                                          \
    bool                           self::sIsTypeOf(const EntityType & t) { return ParentClass::sIsTypeOf(t) || t == (type); } \
    bool                           self::isTypeOf(const EntityType & t) const { return ParentClass::isTypeOf(t) || t == (type); }

namespace GN {
namespace engine {
///
/// Base class of entity object that supports weak referencing
// -------------------------------------------------------------------------
class EntityBase {
public:
    mutable DoubleLink mRefs;

    /// Default constructor
    EntityBase() {
        // This context pointer is never used.
        mRefs.context = (void *) 0xbadbeef;
    }

    /// Copy constructor
    EntityBase(const EntityBase &) {
        // Nothing to copy
        mRefs.context = (void *) 0xbadbeef;
    }

    /// Destructor
    virtual ~EntityBase();

    /// Copy operator
    EntityBase & operator=(const EntityBase &) {
        // Nothing to copy.
        return *this;
    }
};

///
/// Base class of weak referencing pointer. This class is a building
/// block of weak-ref pointer. It should not be used directly by
/// client code.
// -------------------------------------------------------------------------
class EntityRefBase {
public:
    const EntityBase * mPtr;
    DoubleLink         mLink;

    /// Constructor
    EntityRefBase(const EntityBase * ptr): mPtr(ptr) {
        mLink.context = this;
        if (ptr) mLink.linkAfter(&ptr->mRefs);
    }

    /// copy constructor
    EntityRefBase(const EntityRefBase & ref): mPtr(ref.mPtr) {
        mLink.context = this;
        if (mPtr) mLink.linkAfter(&mPtr->mRefs);
    }

    /// non virtual destructor
    ~EntityRefBase() {}

    /// Attach to a new weak object (detach from current one)
    void attachTo(const EntityBase * ptr) {
        if (mPtr == ptr) return;

        // detach from current object
        mLink.detach();

        // attach to new object
        if (ptr) mLink.linkAfter(&ptr->mRefs);
        mPtr = ptr;
    }
};

///
/// Weak object destructor
// -------------------------------------------------------------------------
inline EntityBase::~EntityBase() {
    // Loop through reference list. Clear them all.
    DoubleLink *    next;
    EntityRefBase * ref;
    while (NULL != (next = mRefs.next)) {
        ref       = (EntityRefBase *) next->context;
        ref->mPtr = NULL;
        next->detach();
    }
}

///
/// Weak reference to entity object
// -------------------------------------------------------------------------
template<typename X>
class EntityRef : private EntityRefBase {
    typedef X * XPTR;
    typedef X & XREF;

public:
    ///
    /// constructor
    ///
    EntityRef(XPTR ptr = NULL): EntityRefBase(ptr) {}

    ///
    /// copy constructor
    ///
    EntityRef(const EntityRef & ref): EntityRefBase(ref) { attachTo(ref.mPtr); }

    ///
    /// Destructor
    ///
    ~EntityRef() { clear(); }

    ///
    /// clear the reference
    ///
    void clear() {
        mLink.detach();
        mPtr = NULL;
    }

    ///
    /// get the raw pointer
    ///
    XPTR rawptr() const { return (XPTR) mPtr; }

    ///
    /// check for empty reference
    ///
    bool empty() const { return NULL == mPtr; }

    ///
    /// set/reset the pointer. Null pointer is allowed.
    ///
    void set(XPTR ptr) {
        if (mPtr == ptr) return;
        attachTo(ptr);
    }

    ///
    /// copy operator
    ///
    EntityRef & operator=(const EntityRef & rhs) {
        attachTo(rhs.mPtr);
        return *this;
    }

    ///
    /// Convert to XPTR
    ///
    operator XPTR() const { return (XPTR) mPtr; }

    ///
    /// 比较操作
    ///
    bool operator==(const EntityRef & rhs) const throw() { return mPtr == rhs.mPtr; }

    ///
    /// 比较操作
    ///
    bool operator!=(const EntityRef & rhs) const throw() { return mPtr != rhs.mPtr; }

    ///
    /// 比较操作
    ///
    bool operator<(const EntityRef & rhs) const throw() { return mPtr < rhs.mPtr; }

    ///
    /// NOT operator
    ///
    bool operator!() const throw() { return !mPtr; }

    ///
    /// dereference operator.
    ///
    /// TODO: is this thread safe?
    ///
    XREF operator*() const throw() {
        GN_ASSERT(mPtr);
        return *mPtr;
    }

    ///
    /// arrow operator
    ///
    XPTR operator->() const throw() {
        GN_ASSERT(mPtr);
        return mPtr;
    }
};

/// Define entity type class
typedef Guid EntityType;

/// Entity class. Root class of game play object that could be placed into game world.
class GN_API Entity : public EntityBase, public NoCopy {
protected:
    /// constructor
    Entity();

public:
    /// destructor
    virtual ~Entity();

    /// See if the entity class implementes a specific entity type. (static version)
    static bool sIsTypeOf(const EntityType &) { return false; }

    /// See if the entity class implementes a specific entity type. (virtual version)
    virtual bool isTypeOf(const EntityType &) const { return false; }

    /// Get unique entity ID
    int getID() const { return mID; }

    /// Get entity's component. No increasing reference counter of the component.
    Entity * getComponent(const EntityType & type) const;

    /// Set entity's component.
    ///   - Decease reference counter of existing component by 1.
    ///   - Increase reference counter of the new component by 1.
    ///   - Null component pointer is allowed.
    void setComponent(const EntityType & type, Entity * comp);

    /// tempalte helpers
    //@{
    template<class T>
    T * getComponent() const {
        return (T *) getComponent(T::sGetType());
    }
    template<class T>
    void setComponent(T * t) {
        return setComponent(T::sGetType(), t);
    }
    //@}

    // public: virtual void processEvent( uint32 eventid, uint64 param1, void * param2 ) = 0;

private:
    typedef HashMap<EntityType, EntityRef<Entity>, 128, EntityType::Hash> ComponentMap;

    int          mID;
    ComponentMap mComponents;
};

// --------------------------------------------------------------------------
// Global engine functions
// --------------------------------------------------------------------------

//@{

// Initialize basic systems (file, memory, thread and etc.)
GN_API bool initialize();

// shutdown everthing.
GN_API void shutdown();

/// Graphics initialization options
struct GfxInitOptions {
    gfx::GpuOptions           gpuOptions;          ///< renderer options
    bool                      useMultithreadGpu;   ///< use multithread renderer or not.
    gfx::FontFaceCreationDesc defaultAsciiFont;    ///< default ascii font face creation descriptor.
    gfx::FontFaceCreationDesc defaultNonAsciiFont; ///< default non-ascii font face creation descriptor
};

GN_API bool inputInitialize(input::InputAPI);
GN_API void inputShutdown();

GN_API bool gfxInitialize(const GfxInitOptions &);
GN_API void gfxShutdown();

// bool soundInitialize();
// void soundShutdown();

// bool networkInitialize();
// void networkShutdown();

// Graphics stuff
GN_API gfx::Gpu * getGpu();
GN_API gfx::SpriteRenderer * getSpriteRenderer();
GN_API gfx::LineRenderer * getLineRenderer();
GN_API gfx::BitmapFont * getDefaultFontRenderer();
GN_API gfx::GpuResourceDatabase * getGdb();

// Entity management
GN_API Entity * findEntity(int id);

/// Functions in this namespace are reserved for engine module itself,
/// and should not be called by engine clients or entity/component developers.
namespace impl {
/// Entity management
//@{
GN_API int  onEntityCtor(Entity *);
GN_API void onEntityDtor(int);
//@}
} // namespace impl

//@}
} // namespace engine
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_ENGINE_BASE_H__
