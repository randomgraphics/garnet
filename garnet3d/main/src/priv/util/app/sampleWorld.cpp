#include "pch.h"
#include "garnet/GNutil.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.util");

// *****************************************************************************
// Local Stuff
// *****************************************************************************

class SampleSpacialEntity : public engine::Entity
{
    engine::SpacialComponent * mComp;

public:

    SampleSpacialEntity() : mComp( new engine::SpacialComponent() )
    {
        this->setComponent( mComp );
    }

    ~SampleSpacialEntity()
    {
        safeDecref( mComp );
    }

    engine::SpacialComponent * spacial() const { return mComp; }
};

class SampleVisualEntity : public SampleSpacialEntity
{
    engine::VisualComponent * mComp;

public:

    SampleVisualEntity() : mComp( new engine::VisualComponent() )
    {
        this->setComponent( mComp );
    }

    ~SampleVisualEntity()
    {
        safeDecref( mComp );
    }

    engine::VisualComponent * visual() const { return mComp; }
};

//
//
// -----------------------------------------------------------------------------
static Entity * sCreateEntity(
    Entity                             * root,
    const ModelHierarchyDesc           & worldDesc,
    StringMap<char,Entity*>            & entities,
    const StrA                         & entityName,
    const ModelHierarchyDesc::NodeDesc & entityDesc )
{
    // recursively populate parent entities
    SampleSpacialEntity * parent = NULL;
    if( !entityDesc.parent.empty() )
    {
        const ModelHierarchyDesc::NodeDesc * parentDesc = worldDesc.nodes.find( entityDesc.parent );
        if( NULL == parentDesc )
        {
            GN_ERROR(sLogger)( "Entity '%s' has a invalid parent: '%s'", entityName.rawptr(), entityDesc.parent.rawptr() );
        }
        else
        {

            parent = (SampleSpacialEntity*)sCreateEntity( root, worldDesc, entities, entityDesc.parent, *parentDesc );
            if( NULL == parent ) return NULL;
        }
    }

    // check if the entity has been initialized;
    Entity ** ppe = entities.find( entityName );
    if( ppe ) return *ppe;

    // create a new entity instance
    SampleSpacialEntity * e = entityDesc.models.empty() ? new SampleSpacialEntity() : new SampleVisualEntity();
    if( !e )
    {
        GN_ERROR(sLogger)( "Out of memory." );
        return NULL;
    }
    entities[entityName] = e;
    GN_ASSERT( (Entity*)e == *entities.find( entityName ) );

    // set spacial properties
    e->spacial()->setParent( parent ? parent->spacial() : ((SampleSpacialEntity*)root)->spacial() );
    e->spacial()->setPosition( entityDesc.position );
    e->spacial()->setRotation( entityDesc.orientation );
    e->spacial()->setScale( entityDesc.scaling );
    e->spacial()->setSelfBoundingBox( entityDesc.bbox );

    for( size_t i = 0; i < entityDesc.models.size(); ++i )
    {
        const StrA & modelName = entityDesc.models[i];

        const GN::gfx::ModelResourceDesc * pModelDesc = worldDesc.models.find( modelName );
        if( NULL == pModelDesc )
        {
            GN_ERROR(sLogger)(
                "Entity %s references invalid model named \"%s\".",
                entityName.rawptr(),
                modelName.rawptr() );
            continue;
        }

        // this variable is used to keep a reference to mesh resource,
        // to prevent it from being deleted, until the model is created.
        AutoRef<MeshResource> mesh;

        GpuResourceDatabase * gdb = engine::getGdb();
        if( !pModelDesc->mesh.empty() && gdb )
        {
            mesh = gdb->findResource<MeshResource>( pModelDesc->mesh );
            if( !mesh )
            {
                const GN::gfx::MeshResourceDesc * pMeshDesc = worldDesc.meshes.find(pModelDesc->mesh);

                if( NULL == pMeshDesc )
                {
                    GN_ERROR(sLogger)(
                        "Model \"%s\" references a mesh \"%s\" that does not belong to this scene.",
                        modelName.rawptr(),
                        pModelDesc->mesh.rawptr() );
                    continue; // ignore the model
                }

                // create new mesh
                mesh = gdb->createResource<MeshResource>( pModelDesc->mesh );
                if( !mesh || !mesh->reset( pMeshDesc ) ) continue;
            }
        }

        AutoRef<ModelResource> model = gdb->createResource<ModelResource>( NULL );
        if( !model->reset( pModelDesc ) ) continue;

        ((SampleVisualEntity*)e)->visual()->addModel( model );
    }

    return e;
}

// *****************************************************************************
// SampleWorld
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::util::SampleWorld::SampleWorld()
    : mRoot(NULL)
    , mShowBBox(false)
{
}

//
//
// -----------------------------------------------------------------------------
GN::util::SampleWorld::~SampleWorld()
{
    clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SampleWorld::clear()
{
    for( const StringMap<char,Entity*>::KeyValuePair * i = mEntities.first();
         i != NULL;
         i = mEntities.next( i ) )
    {
        delete i->value;
    }
    mEntities.clear();
    safeDelete( mRoot );
}

//
//
// -----------------------------------------------------------------------------
bool GN::util::SampleWorld::createEntites( const ModelHierarchyDesc & desc )
{
    // clear old data.
    clear();

    // create root spatial entity of the whole world
    mRoot = new SampleSpacialEntity();
    if( NULL == mRoot ) return NULL;

    for( const StringMap<char,ModelHierarchyDesc::NodeDesc>::KeyValuePair * i = desc.nodes.first();
         i != NULL;
         i = desc.nodes.next( i ) )
    {
        const StrA & entityName = i->key;
        const ModelHierarchyDesc::NodeDesc & entityDesc = i->value;

        if( NULL == sCreateEntity( mRoot, desc, mEntities, entityName, entityDesc ) )
        {
            // Failed.
            clear();
            return false;
        }
    }

    return true;
}

//
// Draw all entities in the world.
// -----------------------------------------------------------------------------
void GN::util::SampleWorld::draw( const Matrix44f & proj, const Matrix44f & view ) const
{
    engine::getGdb()->setTransform( proj, view );

    LineRenderer * lr = engine::getLineRenderer();

    for( const StringMap<char,Entity*>::KeyValuePair * i = mEntities.first();
         i != NULL;
         i = mEntities.next( i ) )
    {
        Entity * e = i->value;

        VisualComponent * visual = e->getComponent<VisualComponent>();

        if( visual )
        {
            visual->draw();
        }

        // draw each entitie's bounding box
        SpacialComponent * spacial = e->getComponent<SpacialComponent>();
        if( mShowBBox && NULL != spacial )
        {
            const Boxf & bbox = spacial->getSelfBoundingBox();
            lr->drawBox( bbox, 0xFFFF00FF, proj * view * spacial->getLocal2Root() );
        }
    }

    // draw overall bounding box in red
    SpacialComponent * spacial = mRoot->getComponent<SpacialComponent>();
    if( mShowBBox && NULL != spacial )
    {
        const Boxf & bbox = spacial->getUberBoundingBox();
        lr->drawBox( bbox, 0xFF000000, proj * view * spacial->getLocal2Root() );
    }
}
