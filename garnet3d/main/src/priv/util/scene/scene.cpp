#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.scene.Scene");

// *****************************************************************************
// Node class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::scene::Node::setParent( Node * parent, Node * prevSibling )
{
    if( parent != getParent() || prevSibling != getPrevSibling() )
    {
        TreeNode<Node>::setParent( parent, prevSibling );
        mTransformDirty = true;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::Node::setPosition( const Vector3f & p )
{
    if( p != mPosition )
    {
        mPosition = p;
        mTransformDirty = true;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::Node::setPivot( const Vector3f & p )
{
    if( p != mPivot )
    {
        mPivot = p;
        mTransformDirty = true;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::Node::setRotation( const Quaternionf & q )
{
    if( q != mRotation )
    {
        mRotation = q;
        mTransformDirty = true;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::Node::setBoundingSphere( const Spheref & s )
{
    mBoundingSphere = s;
}


//
//
// -----------------------------------------------------------------------------
void GN::scene::Node::calcTransform()
{
    GN_ASSERT( mTransformDirty );

    Matrix33f r33;
    Matrix44f r44, t1, t2;

    // object will be:
    //
    //  1. moved to "position", in parent space
    //  2. rotated around "pivot" point in parent space, which includes 3 sub steps:
    //     a) moved -"pivot" points, in parent space
    //     b) rotated, in parent space
    //     c) moved +"pivot" points, in parent space

    mRotation.toMatrix33( r33 );
    r44.set( r33 );

    t1.translate( mPosition - mPivot );
    t2.translate( mPivot );

    mLocal2Parent = t2 * r44 * t1;
    mParent2Local = Matrix44f::sInverse( mLocal2Parent );

    Node * p = getParent();
    if( p )
    {
        mLocal2Root = p->getLocal2Root() * mLocal2Parent;
        mRoot2Local = Matrix44f::sInverse( mLocal2Root );
    }
    else
    {
        mLocal2Root = mLocal2Parent;
        mRoot2Local = mParent2Local;
    }

    mTransformDirty = false;
}

// *****************************************************************************
// Geometry Node class
// *****************************************************************************

///
///
/// ----------------------------------------------------------------------------
GN::scene::GeometryNode::~GeometryNode()
{
    for( size_t i = 0; i < mBlocks.size(); ++i )
    {
        delete mBlocks[i];
    }
    mBlocks.clear();
}

///
///
/// -----------------------------------------------------------------------------
void
GN::scene::GeometryNode::addModel( gfx::GpuResource * model )
{
    Scene & s = getScene();

    GpuResourceDatabase & db = s.database();

    if( !db.validResource( ModelResource::guid(), model ) )
    {
        GN_ERROR(sLogger)( "Invalid model resource pointer." );
        return;
    }

    ModelResource * m = GpuResource::castTo<ModelResource>( model );

    AutoObjPtr<GeometryBlock> b( new GeometryBlock );

    // make a copy of the input effect
    b->model.set( m );

    // get list of standard parameters
    Scene::UniformCollection & globalUniforms = s.globalUniforms;

    // handle standard parameters
    for( size_t i = 0; i < NUM_STANDARD_SCENE_PARAMETERS; ++i )
    {
        const StandardSceneParameterDesc & d = getStandardSceneParameterName( i );

        AutoRef<UniformResource> unires = m->getUniform( d.name );

        if( unires )
        {
            AutoRef<Uniform> u;
            if( !d.global )
            {
                StandardUniform su;
                su.type  = (StandardSceneParameterType)i;
                su.uniform.attach( s.database().gpu().createUniform(d.size) );
                mStandardPerObjectUniforms.append( su );
                u = su.uniform;
            }
            else
            {
                u.set( &globalUniforms[i] );
            }
            GN_ASSERT( u );

            unires->setUniform( u );
        }
    }

    // insert b into block array
    mBlocks.append( b );
    b.detach();
}

///
///
/// ------------------------------------------------------------------------
void GN::scene::GeometryNode::draw()
{
    Scene & s = getScene();

    for( size_t i = 0; i < mStandardPerObjectUniforms.size(); ++i )
    {
        const StandardUniform & su = mStandardPerObjectUniforms[i];
        gfx::Uniform * u = su.uniform.get();

        // should be per-object parameter
        GN_ASSERT( !getStandardSceneParameterName( su.type ).global );

        switch( su.type )
        {
            case SCENE_PARAM_MATRIX_PVW :
            {
                const Matrix44f & pv = *(const Matrix44f *)s.globalUniforms[SCENE_PARAM_MATRIX_PV].getval();
                const Matrix44f & world = getLocal2Root();
                Matrix44f pvw = pv * world;
                u->update( pvw );
                break;
            }

            case SCENE_PARAM_MATRIX_PVW_INV:
            {
                const Matrix44f & pv = *(const Matrix44f *)s.globalUniforms[SCENE_PARAM_MATRIX_PV].getval();
                const Matrix44f & world = getLocal2Root();
                Matrix44f pvw = pv * world;
                u->update( Matrix44f::sInverse( pvw ) );
                break;
            }

            case SCENE_PARAM_MATRIX_PVW_IT:
            {
                const Matrix44f & pv = *(const Matrix44f *)s.globalUniforms[SCENE_PARAM_MATRIX_PV].getval();
                const Matrix44f & world = getLocal2Root();
                Matrix44f pvw = pv * world;
                u->update( Matrix44f::sInvtrans( pvw ) );
                break;
            }

            case SCENE_PARAM_MATRIX_WORLD :
            {
                const Matrix44f & world = getLocal2Root();
                u->update( world );
                break;
            }

            case SCENE_PARAM_MATRIX_WORLD_INV:
            {
                const Matrix44f & world = getLocal2Root();
                u->update( Matrix44f::sInverse(world) );
                break;
            }

            case SCENE_PARAM_MATRIX_WORLD_IT:
            {
                const Matrix44f & world = getLocal2Root();
                u->update( Matrix44f::sInvtrans(world) );
                break;
            }

            default:
                // do nothing
                break;
        }
    }

    for( size_t i = 0; i < mBlocks.size(); ++i )
    {
        const GeometryBlock * b = mBlocks[i];

        if( b->model ) b->model->draw();
    }
}

// *****************************************************************************
// Scene class implementation
// *****************************************************************************

class SceneImpl : public Scene
{
    union DirtyFlags
    {
        UInt64 u64;

        struct
        {

        UInt64 transform : 1;
        UInt64 light0    : 1;

        };
    };

    struct LightDesc
    {
        Vector4f  position;
        Vector4f  direction;
        Vector4f  diffuse;
        Vector4f  ambient;
        Vector4f  specular;

        LightDesc()
            : position(0,0,0,1)
            , direction(0,0,1,0)
            , diffuse(1.0f,1.0f,1.0f,1.0f)
            , ambient(0.2f,0.2f,0.2f,0.2f)
            , specular(0.6f,0.6f,0.6f,0.6f)
        {
        }
    };

    struct UniformCollectionImpl : public UniformCollection
    {
        /// ctor
        UniformCollectionImpl() {}

        /// dtor
        virtual ~UniformCollectionImpl() {}

        /// update uniform pointers
        void updateUniformPointers( AutoRef<Uniform> * uniforms, Uniform * dummy )
        {
            mUniforms = uniforms[0].addr();
            mDummy    = dummy;
        }
    };

    GpuResourceDatabase & mDatabase;
    DirtyFlags            mDirtyFlags;
    UniformCollectionImpl mUniformCollection;
    AutoRef<Uniform>      mGlobalUniforms[NUM_STANDARD_SCENE_PARAMETERS];
    AutoRef<Uniform>      mDummyUniform;
    Matrix44f             mProj;
    Matrix44f             mView;
    LightDesc             mDefaultLight0;

    void updateTransformation()
    {
        if( mDirtyFlags.transform )
        {
            mDirtyFlags.transform = false;

            Matrix44f pv   = mProj * mView;
            Matrix44f ipv  = Matrix44f::sInverse( pv );
            Matrix44f itpv = Matrix44f::sInverse( Matrix44f::sTranspose( pv ) );

            Matrix44f ip  = Matrix44f::sInverse( mProj );
            Matrix44f itp = Matrix44f::sInverse( Matrix44f::sTranspose( mProj ) );

            Matrix44f iv  = Matrix44f::sInverse( mView );
            Matrix44f itv = Matrix44f::sInverse( Matrix44f::sTranspose( mView ) );

            mGlobalUniforms[SCENE_PARAM_MATRIX_PV]->update( pv );
            mGlobalUniforms[SCENE_PARAM_MATRIX_PV_INV]->update( ipv );
            mGlobalUniforms[SCENE_PARAM_MATRIX_PV_IT]->update( itpv );

            mGlobalUniforms[SCENE_PARAM_MATRIX_PROJ]->update( mProj );
            mGlobalUniforms[SCENE_PARAM_MATRIX_PROJ_INV]->update( ip );
            mGlobalUniforms[SCENE_PARAM_MATRIX_PROJ_IT]->update( itp );

            mGlobalUniforms[SCENE_PARAM_MATRIX_VIEW]->update( mView );
            mGlobalUniforms[SCENE_PARAM_MATRIX_VIEW_INV]->update( iv );
            mGlobalUniforms[SCENE_PARAM_MATRIX_VIEW_IT]->update( itv );
        }
    }

    void updateLight( size_t index, const LightDesc & desc )
    {
        GN_ASSERT( 0 == index );
        GN_UNUSED_PARAM( index );
        mGlobalUniforms[SCENE_PARAM_LIGHT0_POSITION]->update( desc.position );
        mGlobalUniforms[SCENE_PARAM_LIGHT0_DIRECTION]->update( desc.direction );
        mGlobalUniforms[SCENE_PARAM_LIGHT0_DIFFUSE]->update( desc.diffuse );
        mGlobalUniforms[SCENE_PARAM_LIGHT0_AMBIENT]->update( desc.ambient );
        mGlobalUniforms[SCENE_PARAM_LIGHT0_SPECULAR]->update( desc.specular );
    }

    void updateLights( Node * root )
    {
        // setup default light parameters
        updateLight( 0, mDefaultLight0 );

        LightDesc light0;
        TreeTraversePreOrder<Node> tt( root );
        for( Node * n = tt.first(); NULL != n; n = tt.next( n ) )
        {
            if( Node::LIGHT == n->getType() )
            {
                LightNode * l = (LightNode*)n;

                Vector4f localpos( l->getPosition(), 1.0f );
                light0.position = l->getLocal2Root() * localpos; // world position
                updateLight( 0, light0 );

                break;
            }
        }
    }

public:

    /// ctor
    SceneImpl( GpuResourceDatabase & db )
        : Scene( mUniformCollection )
        , mDatabase( db )
    {
    }

    /// dtor
    ~SceneImpl()
    {
    }

public:

    ///
    /// Initialization
    /// ------------------------------------------------------------------------
    bool init()
    {
        // initialize uniforms
        for( size_t i = 0; i < GN_ARRAY_COUNT(mGlobalUniforms); ++i )
        {
            const StandardSceneParameterDesc & d = getStandardSceneParameterName( i );
            if( d.global )
            {
                mGlobalUniforms[i].attach( mDatabase.gpu().createUniform( d.size ) );

                if( NULL == mGlobalUniforms[i] ) return false;
            }
        }
        mDummyUniform.attach( mDatabase.gpu().createUniform( 1 ) );
        if( NULL == mDummyUniform ) return false;
        mUniformCollection.updateUniformPointers( mGlobalUniforms, mDummyUniform );

        mProj.identity();
        mView.identity();

        // mark all fields dirty
        mDirtyFlags.u64 = (UInt64)-1;

        return true;
    }

    /// methods inherited from Scene class
    //@{

    ///
    ///
    /// ------------------------------------------------------------------------
    virtual gfx::GpuResourceDatabase & database() const { return mDatabase; }

    ///
    ///
    /// ------------------------------------------------------------------------
    virtual void setProj( const Matrix44f & proj )
    {
        mProj = proj;
        mDirtyFlags.transform = 1;
    }

    ///
    ///
    /// ------------------------------------------------------------------------
    virtual void setView( const Matrix44f & view )
    {
        mView = view;
        mDirtyFlags.transform = 1;
    }

    ///
    ///
    /// ------------------------------------------------------------------------
    virtual void setDefaultLight0Position( const Vector3f & position )
    {
        mDefaultLight0.position.set( position, 1 );
    }

    ///
    ///
    /// ------------------------------------------------------------------------
    virtual void renderNodeHierarchy( Node * root )
    {
        if( NULL == root ) return;

        updateTransformation();

        updateLights( root );

        // render geometry nodes
        TreeTraversePreOrder<Node> tt( root );
        for( Node * n = tt.first(); NULL != n; n = tt.next( n ) )
        {
            if( Node::GEOMETRY == n->getType() )
            {
                ((GeometryNode*)n)->draw();
            }
        }
    }

    //@}
};

///
///
/// ----------------------------------------------------------------------------
GN::scene::Scene *
GN::scene::createScene( gfx::GpuResourceDatabase & db )
{
    AutoObjPtr<SceneImpl> s( new SceneImpl(db) );
    if( !s->init() ) return NULL;
    return s.detach();
}
