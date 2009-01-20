#include "pch.h"
#include "sceneGeom.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.scene.Scene");

// *****************************************************************************
// Global data Node class
// *****************************************************************************

const GN::scene::EffectParameterDesc GN::scene::STANDARD_EFFECT_PARAMETER_DESCRIPTIONS[] =
{
    { "pvw", sizeof(Matrix44f) },
};

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

// *****************************************************************************
// Light class implementation
// *****************************************************************************

class PointLightImpl : public LightNode
{
public:

    ///
    ///
    /// ------------------------------------------------------------------------
    PointLightImpl( Scene & s ) : LightNode( s, LightNode::POINT_LIGHT )
    {
    }
};

// *****************************************************************************
// Geometry class implementation
// *****************************************************************************

class GeometryNodeImpl : public GeometryNode
{
public:
    ///
    /// ctor
    /// ------------------------------------------------------------------------
    GeometryNodeImpl( Scene & s ) : GeometryNode( s )
    {
    }
};

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
GN::scene::GeometryNode::addGeometry( gfx::Effect * effect, gfx::Mesh * mesh, size_t firstidx, size_t numidx )
{
    if( NULL == effect || NULL == mesh )
    {
        GN_ERROR(sLogger)( "NULL parameter." );
        return;
    }

    mBlocks.resize( mBlocks.size() + 1 );

    GeomBlock & g = mBlocks.back();

    g.effect.set( effect );

    g.drawables.resize( effect->getNumPasses() );

    for( size_t i = 0; i < g.drawables.size(); ++i )
    {
        Drawable & d = g.drawables[i];
        effect->applyToDrawable( d, i );
        mesh->applyToDrawable( d, firstidx, numidx );
    }
}

// *****************************************************************************
// Scene class implementation
// *****************************************************************************

class SceneImpl : public Scene
{
public:

    /// ctor
    SceneImpl( Renderer & r )
        : mRenderer( r )
    {
    }

    /// dtor
    ~SceneImpl()
    {
        clear();
    }

public:

    /// methods inherited from Scene class
    //@{

    ///
    ///
    /// ------------------------------------------------------------------------
    virtual void clear()
    {
    }

    ///
    ///
    /// ------------------------------------------------------------------------
    virtual GeometryNode * createGeometry()
    {
        return new GeometryNodeImpl(*this);
    }

    ///
    ///
    /// ------------------------------------------------------------------------
    virtual LightNode * createLight( LightNode::LightType type )
    {
        switch( type )
        {
            case LightNode::POINT_LIGHT:
                return  new PointLightImpl( *this, type );

            default :
                GN_ERROR(sLogger)( "invalid light type: %d", type );
                return NULL;
        }
    }

    ///
    ///
    /// ------------------------------------------------------------------------
    virtual void setProj( const Matrix44f & )
    {
    }

    ///
    ///
    /// ------------------------------------------------------------------------
    virtual void setView( const Matrix44f & )
    {
    }

    ///
    ///
    /// ------------------------------------------------------------------------
    virtual void setViewport( const Rect<UInt32> & )
    {
    }

    //@}

private:

    struct GpuParamData
    {
    };

    DynaArray< DynaArray<UInt

    Renderer & mRenderer;
};

///
///
/// ------------------------------------------------------------------------
void GN::scene::GeometryNode::draw()
{
    for( size_t i = 0; i < mBlocks.size(); ++i )
    {
        const GeomBlock & g = mBlocks[i];

        /// apply effect parameters
        size_t n = GN_ARRAY_COUNT(STANDARD_EFFECT_PARAMETER_DESCRIPTIONS);
        for( size_t i = 0; i < n; ++i )
        {
            const EffectParameterDesc & d = STANDARD_EFFECT_PARAMETER_DESCRIPTIONS[i];

            g.effect->getGpuProgramParam( d.name )->set( ... );
        }

        for( size_t i = 0; i < g.drawables.size(); ++i )
        {
            const Drawable & d = g.drawables[i];
            d.draw();
        }
    }
}
