class EntityManager;

typedef int EntityType;

class Entity : public WeakObject
{
public:

    static EntityType sGetType()
    {
    }

    virtual bool IsTypeOf( const EntityType & ) const;

	virtual ~Entity()
	{
	}

protected:

	Entity( EntityManager & m ) : mManager(m)
	{
        mReferences.context = this;
	}

private:

	friend class EntityManager;
    friend struct EntityRefBase;

	EntityManager & mManager;
	uint64          mUniqueID;
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

private:

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
