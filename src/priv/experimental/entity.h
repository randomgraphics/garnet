class EntityManager;

typedef int EntityType;

class EntityLink
{
    EntityLink * mPrev;
    EntityLink * mNext;

    void detach()
    {
    }

public:

    EntityLink() : mPrev(NULL), mNext(NULL)
    {
    }

    ~EntityLink()
    {
        detach();
    }

    EntityLink * getPrev() const { return mPrev; }

    EntityLink * getNext() const { return mNext; }

    static void sLink( EntityLink * a, EntityLink * b )
    {
        // TODO: unimpl
    }
};

class Entity
{
public:

    static EntityType sGetType()
    {
    }

    virtual bool IsTypeOf( const EntityType & ) const;

	virtual ~Entity()
	{
        // Loop through mReferences list, and clear all of them.
        EntityLink * next;
        while( NULL != (next = mReferences.getNext()) )
        {
        }
	}


protected:

	Entity( EntityManager & );

private:

	friend class EntityManager;
    friend struct EntityRefBase;

	EntityManager & mManager;
	uint64          mUniqueID;
	EntityLink      mGlobalLink; // Global linked list that links to all items.

    EntityLink      mReferences; // Linked list of entities that are directly referencing this entity.

	//EntityLink    mNameLink;   // Linked list of items for all items with same name.
};

class EntityRefBase : EntityLink
{
    Entity * mPtr;  //< Pointer to the entity that is being referenced.

public:

    EntityRefBase() : mPtr(NULL)
    {
    }

    ~EntityRefBase()
    {
        mPtr = NULL;
    }

	void attach( Entity * e )
	{
        if( e == mPtr ) return;

        // detach from old pointer
        if( mPrev ) mPrev->mNext = mNext;
        if( mNext ) mNext->mPrev = mPrev;

        // then attach to new pointer
        if( e )
        {
        }
        else
        {
            mPtr  = NULL;
            mPrev = NULL;
            mNext = NULL;
        }
	}

	Entity * rawptr() const
	{
        return mPtr;
	}
};

template<typename T>
class EntityRef : private EntityRefBase
{
public:

    void attach( T * t )
    {
        EntityRefBase::attach( t );
    }

    T * rawptr() const
    {
        return (T*)EntityRefBase::rawptr();
    }
};

class MyEntity1 : public Entity
{
public:

    MyEntity1( EntityManager & em )
        : Entity(em)
    {
    }
};

class MyEntity2 : public Entity
{
	EntityRef<MyEntity1> e2;

public:

    MyEntity2( EntityManager & em )
        : Entity( em )
        , e2( *(Entity*)this )
    {
    }
};

class EntityManager
{
public:

	EntityManager()
		: mFirstEntity(NULL)
		, mLastEntity(NULL)
	{
	}

	void * onEntityConstruction( Entity * entity )
	{
		EntityInstance * instance = (EntityInstance*)mEntityPool.allocate();
		return instance;
	}

	void onEntityDestruction( void * instance )
	{
	}

    void onEntityRef( Entity * referencer, Entity * referencee )
    {
    }

    void onEntittyDeref( Entity * referencer, Entity * referencee )
    {
    }

private:

	FixedRawMemoryPool<sizeof(EntityInstance),8> mEntityPool;

	Entity * mFirstEntity;
	Entity * mLastEntity;
};

inline Entity::Entity( EntityManager & em )
	: mManager( em )
{
	mInstance = mManager.onEntityConstruction( this );
}

inline Entity::~Entity()
{
	mManager.onEntityDestruction( mInstance );
}
