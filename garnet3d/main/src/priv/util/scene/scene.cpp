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

    t1.translate( mPosition + mPivot );
    t2.translate( -mPivot );

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

    Scene & s = getScene();

    GeometryBlock b( s.getRenderer() );

    // make a copy of the input effect
    b.effect = *inputEffect;

    // get list of standard parameters
    Uniform * const * globalParams = s.getGlobalParam();

    // handle standard parameters
    for( size_t i = 0; i < NUM_STANDARD_SCENE_PARAMETERS; ++i )
    {
        const StandardSceneParameterDesc & d = getStandardSceneParameterName( i );

        if( b.effect.hasGpuProgramParam( d.name ) )
        {
            Uniform * p;
            if( !d.global )
            {
                StdParam sp;
                sp.type  = (StandardSceneParameterType)i;
                sp.param.attach( s.getRenderer().createUniform(d.size) );
                mStdPerObjParams.append( sp );
                p = sp.param.get();
            }
            else
            {
                p = globalParams[i];
            }
            GN_ASSERT( p );

            b.effect.setGpuProgramParam( d.name, p );
        }
    }

    // create drawables
    size_t n = b.effect.getNumPasses();
    b.drawables.resize( n );
    for( size_t i = 0; i < n; ++i )
    {
        Drawable & d = b.drawables[i];
        b.effect.applyToDrawable( d, i );
        mesh->applyToDrawable( d, subset );
    }
}

///
///
/// ------------------------------------------------------------------------
void GN::scene::GeometryNode::draw()
{
    Scene & s = getScene();

    for( size_t i = 0; i < mStdPerObjParams.size(); ++i )
    {
        const StdParam & sp = mStdPerObjParams[i];
        gfx::Uniform * p = sp.param.get();

        // should be per-object parameter
        GN_ASSERT( !getStandardSceneParameterName( sp.type ).global );

        switch( sp.type )
        {
            case SCENE_PARAM_MATRIX_PVW :
            {
                const Matrix44f * pv = (const Matrix44f *)s.getGlobalParam()[SCENE_PARAM_MATRIX_PV]->getval();
                Matrix44f pvw = *pv * getLocal2Root();
                p->update( pvw );
                break;
            }

            default:
                // do nothing
                break;
        }
    }

    for( size_t i = 0; i < mBlocks.size(); ++i )
    {
        const GeometryBlock & b = mBlocks[i];

        for( size_t i = 0; i < b.drawables.size(); ++i )
        {
            const Drawable & d = b.drawables[i];
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

    DirtyFlags        mDirtyFlags;
    Uniform * mGlobalParams[NUM_STANDARD_SCENE_PARAMETERS];
    Matrix44f         mProj;
    Matrix44f         mView;

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

    void updateLights( Node * root )
    {
        TreeTraversePreOrder<Node> tt( root );
        for( Node * n = tt.first(); NULL != n; n = tt.next( n ) )
        {
            if( Node::LIGHT == n->getType() )
            {
                LightNode * l = (LightNode*)n;

                Vector4f localpos( l->getPosition(), 1.0f );
                Vector4f worldpos( l->getLocal2Root() * localpos );
                Vector4f diffuse(1,1,1,1);
                Vector4f ambient(0.2f,0.2f,0.2f,0.2f);
                Vector4f specular(0.6f,0.6f,0.6f,0.6f);

                mGlobalParams[SCENE_PARAM_LIGHT0_POSITION]->update( worldpos );
                mGlobalParams[SCENE_PARAM_LIGHT0_DIFFUSE]->update( diffuse );
                mGlobalParams[SCENE_PARAM_LIGHT0_AMBIENT]->update( ambient );
                mGlobalParams[SCENE_PARAM_LIGHT0_SPECULAR]->update( specular );

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
    virtual Uniform * const * getGlobalParam() const
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
