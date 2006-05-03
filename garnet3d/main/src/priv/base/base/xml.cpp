#include "pch.h"
#include "garnet/base/xml.h"
#include <expat.h>

#pragma warning(disable:4100) // unused parameter
#pragma warning(disable:4702) // unreachable code

// *****************************************************************************
// local functions
// *****************************************************************************

static void * sXmlMalloc( size_t sz ) { return GN::memAlloc( sz ); }
static void * sXmlRealloc( void * p, size_t sz ) { return GN::memReAlloc( p, sz ); }
static void sXmlFree( void * p ) { return GN::memFree( p ); }

struct AutoFree
{
    XML_Parser ptr;
    AutoFree( XML_Parser p ) : ptr(p) { GN_ASSERT(p); }
    ~AutoFree() { XML_ParserFree( ptr ); }
};

// This is used to trace where we are, during parsing the XML documents.
struct ParseTracer
{
    XML_Parser parser;
    GN::XmlProcessor * proc;
    GN::XmlNode * parent;
    GN::XmlNode * prev;
};

// *****************************************************************************
// Expat handlers
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void XMLCALL sStartElementHandler(
    void * userData,
    const XML_Char * name,
    const XML_Char ** atts )
{
    GN_ASSERT( userData && name );
    
    ParseTracer * tracer = (ParseTracer*)userData;

    // create new node
    GN::XmlNode * n = tracer->proc->createNode();
    n->parent = tracer->parent;
    n->sibling = NULL;
    n->child = NULL;
    n->name = name;

    // setup attribute list
    while( *atts )
    {
        GN::XmlAttrib * a = tracer->proc->createAttrib();
        if( 0 == a )
        {
            XML_ParserStop( tracer->parser );
            return;
        }

        a->node = n;
        a->next = NULL;
        a->name = atts[0];
        a->value = atts[1];

        atts += 2;
    }

    // update tracer
    if( tracer->prev )
    {
        // this is not the first node in this level. Let its previous sibling points to this.
        tracer->prev->sibling = n;
    }
    else if( tracer->parent )
    {
        // this is the first node in this level. Let the parent node points to this.
        tracer->parent->child = n;
    }
    tracer->parent = n;
    tracer->prev = NULL;
}

//
//
// -----------------------------------------------------------------------------
void XMLCALL sEndElementHandler(
    void * userData,
    const XML_Char * name )
{
    ParseTracer * tracer = (ParseTracer*)userData;

    // update tracer
    tracer->prev = tracer->parent;
    tracer->parent = tracer->parent ? tracer->parent->parent : NULL;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::XmlProcessor::parseBuffer(
    XmlParseResult & result, const char * content, size_t length )
{
    GN_GUARD;

    // create parser
    XML_Memory_Handling_Suite mm = { &sXmlMalloc, &sXmlRealloc, &sXmlFree };
    XML_Parser parser = XML_ParserCreate_MM( 0, &mm, 0 );
    if( 0 == parser )
    {
        result.errInfo = "Fail to create parser.";
        result.errLine = 0;
        result.errColume = 0;
        return false;
    }
    AutoFree af(parser); // free the parser automatically when go out of this function.

    // setup user data
    ParseTracer userData = { parser, this, NULL, NULL };
    XML_SetUserData( parser, &userData );

    // setup handlers
    XML_SetElementHandler( parser, &sStartElementHandler, &sEndElementHandler );

    // start parse
    XML_Status status = XML_Parse( parser, content, length, true );
    if( XML_STATUS_OK != status )
    {
        result.errInfo = "XML_Parse() failed.";
        result.errLine = XML_GetCurrentLineNumber( parser );
        result.errColume = XML_GetCurrentColumnNumber( parser );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::XmlProcessor::writeToFile( File & file, const XmlNode & root )
{
    GN_GUARD;

    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::XmlNode * GN::XmlProcessor::createNode()
{
    PooledNode * p = new PooledNode;
    mNodes.push_back( p );
    return p;
}

//
//
// -----------------------------------------------------------------------------
GN::XmlAttrib * GN::XmlProcessor::createAttrib()
{
    PooledAttrib * p = new PooledAttrib;
    mAttribs.push_back( p );
    return p;
}

//
//
// -----------------------------------------------------------------------------
void GN::XmlProcessor::releaseAllNodesAndAttribs()
{
    GN_GUARD;

    for( size_t i = 0; i < mNodes.size(); ++i ) delete mNodes[i];
    mNodes.clear();

    for( size_t i = 0; i < mAttribs.size(); ++i ) delete mAttribs[i];
    mAttribs.clear();

    GN_UNGUARD;
}
