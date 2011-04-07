class EntityManager;


class Entity
{
public:

	virtual ~Entity();

    void _ref( Entity * referencee )
    {
    }

    void _deref( Entity * referencee )
    {
    }

    // todo: streaming (byte packer/unpacker)

protected:

	Entity( EntityManager & );

private:

	friend class EntityManager;

	struct EntityLink
	{
		Entity * prev;
		Entity * next;
	};

	EntityManager & mManager;
	uint64          mUniqueID;
	EntityLink      mGlobalLink; // Global linked list that links to all items.
	EntityLink      mNameLink;   // Linked list of items for all items with same name.
	EntityLink      mReferences; // LInked list of entities that are directly referencing this entity.
};

template<class T>
class EntityRef
{
    Entity & mContainer;
	Entity * mPtr;

public:

    EntityRef( Entity & container ) : mContainer(container), mPtr(NULL)
    {
    }

    ~EntityRef()
    {
        set(NULL);
    }

	void set( T * t )
	{
        if( t == mPtr ) return;
        mContainer._deref( mPtr );
        mContainer._ref( t );
        mPtr = t;
	}

	T * get() const
	{
        return mPtr;
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
