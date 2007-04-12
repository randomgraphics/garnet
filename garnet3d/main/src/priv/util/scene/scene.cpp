#include "pch.h"

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
            AutoObjPtr<Actor> node( new Actor(*this) );

            if( node->loadFromXmlNode( *c, basedir ) )
            {
                node.detach()->setParent( root, 0 );
            }
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
GN::scene::Scene::loadActorHiearachyFromXmlFile( const StrA & filename )
{
    GN_GUARD;

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

    return loadActorHiearachyFromXmlNode( *xpr.root, basedir );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::Scene::releaseActorHiearacy( Actor * root )
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
