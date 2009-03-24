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
void GN::scene::Node::calcTransform()
{
    GN_ASSERT( mTransformDirty );

    Matrix33f r3;
    Matrix44f r4, t1, t2;

    mRotation.toMatrix33( r3 );
    r4.set( r3 );

    t1.translate( mPivot ); // Note: pivor is in parent space
    t2.translate( mPosition );

    mLocal2Parent =  t1 * r4 * t2;
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
GN::scene::GeometryNode::addGeometryBlock( const gfx::Effect * inputEffect, const gfx::Mesh * mesh, const gfx::MeshSubset * subset )
{
    if( NULL == inputEffect || NULL == mesh )
    {
        GN_ERROR(sLogger)( "NULL parameter." );
        return;
    }

    if( !inputEffect->ok() )
    {
        GN_ERROR(sLogger)( "Unintialized effect class is not allowed." );
        return;
    }

    Scene & s = getScene();

    AutoObjPtr<GeometryBlock> b( new GeometryBlock( s.getRenderer() ) );

    // make a copy of the input effect
    b->effect = *inputEffect;

    // get list of standard parameters
    Uniform * const * globalUniforms = s.getGlobalUniforms();

    // handle standard parameters
    for( size_t i = 0; i < NUM_STANDARD_SCENE_PARAMETERS; ++i )
    {
        const StandardSceneParameterDesc & d = getStandardSceneParameterName( i );

        if( b->effect.uniforms.contains( d.name ) )
        {
            Uniform * u;
            if( !d.global )
            {
                StandardUniform su;
                su.type  = (StandardSceneParameterType)i;
                su.uniform.attach( s.getRenderer().createUniform(d.size) );
                mStandardPerObjectUniforms.append( su );
                u = su.uniform.get();
            }
            else
            {
                u = globalUniforms[i];
            }
            GN_ASSERT( u );

            b->effect.uniforms[d.name].set( u );
        }
    }

    // create drawables
    size_t n = b->effect.getNumPasses();
    b->drawables.resize( n );
    for( size_t i = 0; i < n; ++i )
    {
        Drawable & d = b->drawables[i];
        b->effect.applyToDrawable( d, i );
        mesh->applyToDrawable( d, subset );
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
                const Matrix44f & pv = *(const Matrix44f *)s.getGlobalUniforms()[SCENE_PARAM_MATRIX_PV]->getval();
                const Matrix44f & world = getLocal2Root();
                Matrix44f pvw = pv * world;
                u->update( pvw );
                break;
            }

            case SCENE_PARAM_MATRIX_PVW_INV:
            {
                const Matrix44f & pv = *(const Matrix44f *)s.getGlobalUniforms()[SCENE_PARAM_MATRIX_PV]->getval();
                const Matrix44f & world = getLocal2Root();
                Matrix44f pvw = pv * world;
                u->update( Matrix44f::sInverse( pvw ) );
                break;
            }

            case SCENE_PARAM_MATRIX_PVW_IT:
            {
                const Matrix44f & pv = *(const Matrix44f *)s.getGlobalUniforms()[SCENE_PARAM_MATRIX_PV]->getval();
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

        for( size_t i = 0; i < b->drawables.size(); ++i )
        {
            const Drawable & d = b->drawables[i];
            d.draw();
        }
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

    DirtyFlags mDirtyFlags;
    Uniform  * mGlobalParams[NUM_STANDARD_SCENE_PARAMETERS];
    Matrix44f  mProj;
    Matrix44f  mView;
    LightDesc  mDefaultLight0;

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

            mGlobalParams[SCENE_PARAM_MATRIX_PV]->update( pv );
            mGlobalParams[SCENE_PARAM_MATRIX_PV_INV]->update( ipv );
            mGlobalParams[SCENE_PARAM_MATRIX_PV_IT]->update( itpv );

            mGlobalParams[SCENE_PARAM_MATRIX_PROJ]->update( mProj );
            mGlobalParams[SCENE_PARAM_MATRIX_PROJ_INV]->update( ip );
            mGlobalParams[SCENE_PARAM_MATRIX_PROJ_IT]->update( itp );

            mGlobalParams[SCENE_PARAM_MATRIX_VIEW]->update( mView );
            mGlobalParams[SCENE_PARAM_MATRIX_VIEW_INV]->update( iv );
            mGlobalParams[SCENE_PARAM_MATRIX_VIEW_IT]->update( itv );
        }
    }

    void updateLight( size_t index, const LightDesc & desc )
    {
        GN_ASSERT( 0 == index );
        GN_UNUSED_PARAM( index );
        mGlobalParams[SCENE_PARAM_LIGHT0_POSITION]->update( desc.position );
        mGlobalParams[SCENE_PARAM_LIGHT0_DIRECTION]->update( desc.direction );
        mGlobalParams[SCENE_PARAM_LIGHT0_DIFFUSE]->update( desc.diffuse );
        mGlobalParams[SCENE_PARAM_LIGHT0_AMBIENT]->update( desc.ambient );
        mGlobalParams[SCENE_PARAM_LIGHT0_SPECULAR]->update( desc.specular );
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
    SceneImpl( Renderer & r )
        : mRenderer( r )
    {
        mDirtyFlags.u64 = (UInt64)-1; // all dirty

        memset( mGlobalParams, 0, sizeof(mGlobalParams) );
        for( size_t i = 0; i < GN_ARRAY_COUNT(mGlobalParams); ++i )
        {
            const StandardSceneParameterDesc & d = getStandardSceneParameterName( i );
            if( d.global )
            {
                mGlobalParams[i] = r.createUniform( d.size );
            }
        }

        mProj.identity();
        mView.identity();
    }

    /// dtor
    ~SceneImpl()
    {
        for( size_t i = 0; i < GN_ARRAY_COUNT(mGlobalParams); ++i )
        {
            safeDecref( mGlobalParams[i] );
        }
    }

public:

    /// methods inherited from Scene class
    //@{

    ///
    ///
    /// ------------------------------------------------------------------------
    virtual gfx::Renderer & getRenderer() const { return mRenderer; }

    ///
    ///
    /// ------------------------------------------------------------------------
    virtual Uniform * const * getGlobalUniforms() const
    {
        return mGlobalParams;
    }

    virtual void setGlobalParam( StandardSceneParameterType type, const void * value )
    {
        const StandardSceneParameterDesc & d = getStandardSceneParameterName( type );

        if( d.global )
        {
            mGlobalParams[type]->update( 0, d.size, value );
        }
        else
        {
            GN_ERROR(sLogger)( "Updating per-object parameter through Scene::setGlobalParam() is effectless." );
        }
    }

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

private:

    Renderer & mRenderer;
};

///
///
/// ----------------------------------------------------------------------------
GN::scene::Scene *
GN::scene::createScene( gfx::Renderer & r )
{
    SceneImpl * s = new SceneImpl(r);
    return s;
}
