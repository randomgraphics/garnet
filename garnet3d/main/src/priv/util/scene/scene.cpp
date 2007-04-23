#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.scene.Scene");

// *****************************************************************************
// ctor / dtor
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::Scene::Scene()
{
    clear();
}

//
//
// -----------------------------------------------------------------------------
GN::scene::Scene::~Scene()
{
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::Scene::clear()
{
    mLight0.position.set( 0, 10000.0f, 0 );
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::Actor *
GN::scene::Scene::loadActorHiearachyFromXmlNode( const XmlNode & node, const StrA & basedir )
{
    GN_GUARD;

    AutoObjPtr<Actor> root( new Actor(*this) );

    if( !root->loadFromXmlNode( node, basedir ) ) return false;

    // Try load children. Note that invalid child node is ignored.
    const XmlNode * c = node.child;
    while( c )
    {
        if( c->toElement() && "actor" == c->toElement()->name )
        {
            Actor * node = loadActorHiearachyFromXmlNode( *c, basedir );
            if( node ) node->setParent( root, 0 );
        }

        c = c->sibling;
    }

    // success
    return root.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::scene::Actor *
GN::scene::Scene::loadActorHiearachyFromXmlFile( const StrA & filename, const StrA & objname )
{
    GN_GUARD;

    GN_INFO(sLogger)( "Load actor '%s' from file '%s'", objname.cptr(), filename.cptr() );

    AutoObjPtr<File> fp( core::openFile( filename, "rt" ) );
    if( !fp ) return false;

    StrA basedir = dirName( filename );

    XmlDocument doc;
    XmlParseResult xpr;
    if( !doc.parse( xpr, *fp ) )
    {
        static Logger * sLogger = getLogger( "GN.scene.util" );
        GN_ERROR(sLogger)(
            "Fail to parse XML file (%s):\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            fp->name().cptr(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.cptr() );
        return false;
    }
    GN_ASSERT( xpr.root );

    // search a actor element with name equals "objname"
    TreeTraversePreOrder<XmlNode> tt(xpr.root);
    XmlNode * n = tt.first();
    while( n )
    {
        XmlElement * e = n->toElement();
        if( e &&  "actor" == e->name )
        {
            XmlAttrib  * a = e->findAttrib("name");

            if( a && objname == a->value )
            {
                // found!
                return loadActorHiearachyFromXmlNode( *n, basedir );
            }
        }
        n = tt.next( n );
    }

    GN_ERROR(sLogger)( "object named '%s' not found in file %s", objname.cptr(), filename.cptr() );
    return false;

    GN_UNGUARD;
}

// *****************************************************************************
// global functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::scene::releaseActorHiearacy( Actor * root )
{
    if( !root ) return;

    TreeTraversePostOrder<Actor> tt( root );

    Actor * a1 = tt.first(), * a2;

    while( a1 )
    {
        a2 = tt.next( a1 );
        delete a1;
        a1 = a2;
    }
}

//
//
// -----------------------------------------------------------------------------
GN::scene::Actor * GN::scene::cloneActorHiearacy( const Actor * root )
{
    if( !root )
    {
        GN_ERROR(sLogger)( "NULL root!" );
        return 0;
    }

    Actor * r = new Actor( root->getScene() );
    root->copyto( *r );

    for( const Actor * a = root->getLastChild(); a; a = a->getPrevSibling() )
    {
        Actor * c = cloneActorHiearacy( a );
        if( 0 == c )
        {
            releaseActorHiearacy(r);
            return 0;
        }

        GN_ASSERT( 0 == c->getParent() );

        c->setParent( r );
    }

    return r;
}
