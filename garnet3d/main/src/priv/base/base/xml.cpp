#include "pch.h"
#include "garnet/base/xml.h"
#include <expat.h>

// *****************************************************************************
// local functions
// *****************************************************************************

static GN::Logger * sLogger = GN::GetLogger("GN.base.xml");

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
    GN::XmlDocument * doc;
    GN::XmlParseResult * result;
    GN::XmlNode * parent;
    GN::XmlNode * prev;
};

static void * sXmlMalloc( size_t sz ) { return GN::HeapMemory::Alloc( sz ); }
static void * sXmlRealloc( void * p, size_t sz ) { return GN::HeapMemory::Realloc( p, sz ); }
static void sXmlFree( void * p ) { return GN::HeapMemory::Free( p ); }

static void sIdent( GN::File & fp, int ident )
{
    for( int i = 0; i < ident; ++i ) fp <<"\t";
}

static void sFormatAttributes( GN::File & fp, const GN::XmlAttrib * att, int ident )
{
    for( ; 0 != att; att = att->next )
    {
        sIdent( fp, ident );
        fp << att->name << "=\"" << att->value << "\"\n";
    }
}

static bool sFormatNodes( GN::File & fp, const GN::XmlNode * root, int ident )
{
    GN_ASSERT( root );

    switch( root->type )
    {
        case GN::XML_CDATA:
        {
            const GN::XmlCdata * c = root->toCdata();
            GN_ASSERT( c && !c->child ); // cdata node should have no child.
            sIdent( fp, ident );
            fp << "<![CDATA[" << c->text << "]]>\n";
            break;
        }

        case GN::XML_COMMENT:
        {
            const GN::XmlComment * c = root->toComment();
            GN_ASSERT( c && !c->child ); // comment node should have no child.
            sIdent( fp, ident );
            fp << "<!-- " << c->text << " -->\n";
            break;
        }

        case GN::XML_ELEMENT:
        {
            const GN::XmlElement * e = root->toElement();
            GN_ASSERT( e );

            sIdent( fp, ident );
            fp << "<" << e->name;

            if( e->attrib )
            {
                fp << "\n";
                sFormatAttributes( fp, e->attrib, ident + 1 );
                sIdent( fp, ident );
            }

            if( !e->text.Empty() || e->child )
            {
                fp << ">\n";

                if( !e->text.Empty() )
                {
                    sIdent( fp, ident + 1 );
                    fp << e->text << "\n";
                }

                if( e->child )
                {
                    sFormatNodes( fp, e->child, ident + 1 );
                }

                sIdent( fp, ident );
                fp << "</" << e->name << ">\n";
            }
            else
            {
                fp << "/>\n";
            }
            break;
        }

        default:
            GN_UNEXPECTED(); // program should not reach here
            GN_ERROR(sLogger)( "Unknown node type!" );
            return false;
    };

    // print brothers
    return root->next ? sFormatNodes( fp, root->next, ident ) : true;
}

static void sCompactAttributes( GN::File & fp, const GN::XmlAttrib * att )
{
    for( ; 0 != att; att = att->next )
    {
        fp << " " << att->name << "=\"" << att->value << "\"";
    }
}

static bool sCompactNodes( GN::File & fp, const GN::XmlNode * root );
static bool sCompactNodeAndChildren( GN::File & fp, const GN::XmlNode * root )
{
    GN_ASSERT( root );

    switch( root->type )
    {
        case GN::XML_CDATA:
        {
            const GN::XmlCdata * c = root->toCdata();
            GN_ASSERT( !c->child ); // cdata node should have no child.
            fp << "<![CDATA[" << c->text << "]]>\n";
            break;
        }

        case GN::XML_COMMENT:
        {
            const GN::XmlComment * c = root->toComment();
            GN_ASSERT( c && !c->child ); // comment node should have no child.
            fp << "<!-- " << c->text << " -->\n";
            break;
        }

        case GN::XML_ELEMENT:
        {
            const GN::XmlElement * e = root->toElement();
            GN_ASSERT( e );
            fp << "<" << e->name;
            if( e->attrib )
            {
                sCompactAttributes( fp, e->attrib );
            }
            if( !e->text.Empty() || e->child )
            {
                fp << ">\n";
                if( !e->text.Empty() ) fp << e->text;
                if( e->child ) sCompactNodes( fp, e->child );
                fp << "</" << e->name << ">\n";
            }
            else
            {
                fp << "/>\n";
            }
            break;
        }

        default:
            GN_UNEXPECTED(); // program should not reach here
            GN_ERROR(sLogger)( "Unknown node type!" );
            return false;
    };

    return true;
}

static bool sCompactNodes( GN::File & fp, const GN::XmlNode * root )
{
    GN_ASSERT( root );

    do{
        if( !sCompactNodeAndChildren( fp, root ) ) return false;
        root = root->next;
    } while( root );

    return true;
}

static void sParseFail( ParseTracer * tracer, const char * errInfo )
{
    XML_StopParser( tracer->parser, XML_FALSE );
    tracer->result->errInfo = errInfo;
    tracer->result->errLine = XML_GetCurrentLineNumber( tracer->parser );
    tracer->result->errColumn = XML_GetCurrentColumnNumber( tracer->parser );
}

static GN::XmlNode * sNewNode( ParseTracer * tracer, GN::XmlNodeType type )
{
    GN::XmlNode * n = tracer->doc->createNode( type, NULL );
    if( 0 == n )
    {
        sParseFail(
            tracer,
            GN::StringFormat( "Fail to create node with type of '%d'", type ).ToRawPtr() );
        return NULL;
    }

    // update tree links
    n->setParent( tracer->parent, tracer->prev );

    // update tracer
    tracer->parent = n;
    tracer->prev = NULL;

    return n;
}

//
//
// -----------------------------------------------------------------------------
static GN::StrA sMangleText( const char * s, int len )
{
    // skip leading spaces
    while( len > 0 && (' '==*s || '\t'==*s || '\n'==*s) )
    {
        ++s; --len;
    }
    if( 0 == len ) return "";

    // skip tailing spaces
    const XML_Char * e = s + len - 1;
    while( len > 0 && (' '==*e || '\t'==*e || '\n'==*e) )
    {
        --e; --len;
    }
    if( 0 == len ) return "";

    GN_TODO( "convert special characters" );

    return GN::StrA( s, len );
}

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
    GN::XmlNode * n = sNewNode( tracer, GN::XML_ELEMENT );
    if( 0 == n ) return;
    GN::XmlElement * e = n->toElement();
    GN_ASSERT( e );

    e->attrib = NULL;
    e->name = name;

    // create attribute list
    while( *atts )
    {
        GN::XmlAttrib * a = tracer->doc->createAttrib( e );

        if( 0 == a )
        {
            sParseFail( tracer, "Fail to create attribute." );
            return;
        }

        a->name = atts[0];
        a->value = atts[1];

        atts += 2;
    }
}

//
//
// -----------------------------------------------------------------------------
void XMLCALL sEndElementHandler( void * userData, const XML_Char * )
{
    ParseTracer * tracer = (ParseTracer*)userData;

    // update tracer
    GN_ASSERT( tracer->parent );
    tracer->prev = tracer->parent;
    tracer->parent = tracer->parent->parent;
}

//
//
// -----------------------------------------------------------------------------
static void XMLCALL sStartCdataSectionHandler( void * userData )
{
    ParseTracer * tracer = (ParseTracer*)userData;

    // create new node
    GN::XmlNode * n = sNewNode( tracer, GN::XML_CDATA );
    if( 0 == n ) return;
}

//
//
// -----------------------------------------------------------------------------
static void XMLCALL sEndCdataSectionHandler( void * userData )
{
    ParseTracer * tracer = (ParseTracer*)userData;

    GN_ASSERT( tracer->parent && tracer->parent->type == GN::XML_CDATA );

    // update tracer
    GN_ASSERT( tracer->parent );
    tracer->prev = tracer->parent;
    tracer->parent = tracer->parent->parent;
}

//
//
// -----------------------------------------------------------------------------
static void XMLCALL sCharacterDataHandler(
    void * userData, const XML_Char * s, int len )
{
    ParseTracer * tracer = (ParseTracer*)userData;

    GN_ASSERT( tracer->parent );

    if( tracer->parent->type == GN::XML_CDATA )
    {
        GN::XmlCdata * t = tracer->parent->toCdata();
        t->text.Append( s, len );
    }
    else
    {
        GN::StrA text = sMangleText( s, len );
        if( !text.Empty() && tracer->parent->type == GN::XML_ELEMENT )
        {
            GN::StrA & currentText = tracer->parent->toElement()->text;

            if( !currentText.Empty() ) currentText += ' ';

            currentText += text;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
static void XMLCALL sCommentHandler( void * userData, const XML_Char * data )
{
    ParseTracer * tracer = (ParseTracer*)userData;

    // create new node
    GN::XmlNode * n = sNewNode( tracer, GN::XML_COMMENT );
    if( 0 == n ) return;
    GN::XmlComment * c = n->toComment();
    c->text.Assign( data );

    // update tracer
    GN_ASSERT( c == tracer->parent );
    tracer->prev = tracer->parent;
    tracer->parent = tracer->parent->parent;
}

// *****************************************************************************
// XmlAttrib class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::XmlAttrib::setOwner( XmlElement * newOwner )
{
    if( node == newOwner ) return;

    if( newOwner && &newOwner->doc != &this->doc )
    {
        GN_ERROR(sLogger)( "Cannot attach attribute to node that belongs to different XML document." );
        return;
    }

    // detach from old owner
    if( node && node->attrib == this )
    {
        node->attrib = this->next;
    }
    if( this->next ) this->next->prev = this->prev;
    if( this->prev ) this->prev->next = this->next;
    this->node = NULL;
    this->prev = NULL;
    this->next = NULL;

    // attach to new node
    if( newOwner )
    {
        this->node = newOwner;
        this->next = newOwner->attrib;
        if( newOwner->attrib )
        {
            newOwner->attrib->prev = this;
        }
        newOwner->attrib = this;
    }
}

// *****************************************************************************
// XmlNode class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::XmlNode::setParent( XmlNode * newParent, XmlNode * newPrev )
{
    if( parent == newParent ) return;

    if( newParent && &newParent->doc != &this->doc )
    {
        GN_ERROR(sLogger)( "Can not link nodes belong to different document." );
        return;
    }

    if( newPrev && newPrev->parent != newParent )
    {
        GN_ERROR(sLogger)( "New previous node does not belong to the new parent node." );
        return;
    }

    // detach from old parent
    if( parent && parent->child == this )
    {
        parent->child = this->next;
    }
    if( this->next ) this->next->prev = this->prev;
    if( this->prev ) this->prev->next = this->next;
    this->parent = NULL;
    this->prev = NULL;
    this->next = NULL;

    // attach to new parent
    if( newParent )
    {
        this->parent = newParent;

        if( newPrev )
        {
            this->prev = newPrev;
            this->next = newPrev->next;
            if( newPrev->next )
            {
                newPrev->next->prev = this;
            }
            newPrev->next = this;
        }
        else
        {
            this->next = newParent->child;
            if( newParent->child )
            {
                newParent->child->prev = this;
            }
            newParent->child = this;
        }
    }
}

// *****************************************************************************
// XmlDocument class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::XmlDocument::parse(
    XmlParseResult & result, const char * content, size_t length )
{
    GN_GUARD;

    result.errInfo.Clear();
    result.errLine = 0;
    result.errColumn = 0;

    // create parser
    XML_Memory_Handling_Suite mm = { &sXmlMalloc, &sXmlRealloc, &sXmlFree };
    XML_Parser parser = XML_ParserCreate_MM( 0, &mm, 0 );
    if( 0 == parser )
    {
        result.errInfo = "Fail to create parser.";
        return false;
    }
    AutoFree af(parser); // free the parser automatically when go out of this function.

    // setup user data
    ParseTracer userData = { parser, this, &result, NULL, NULL };
    XML_SetUserData( parser, &userData );

    // setup handlers
    XML_SetElementHandler( parser, &sStartElementHandler, &sEndElementHandler );
    XML_SetCdataSectionHandler( parser, &sStartCdataSectionHandler, &sEndCdataSectionHandler );
    XML_SetCharacterDataHandler( parser, &sCharacterDataHandler );
    XML_SetCommentHandler( parser, &sCommentHandler );

    // start parse
    XML_Status status = XML_Parse( parser, content, (int)length, XML_TRUE );
    if( XML_STATUS_OK != status )
    {
        if( result.errInfo.Empty() )
        {
            result.errInfo = "XML_Parse() failed.";
            result.errLine = XML_GetCurrentLineNumber( parser );
            result.errColumn = XML_GetCurrentColumnNumber( parser );
        }
        return false;
    }

    GN_ASSERT( userData.prev );
    result.root = userData.prev;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::XmlDocument::parse( XmlParseResult & result, File & fp )
{
    GN_GUARD;

    result.errInfo.Clear();
    result.errLine = 0;
    result.errColumn = 0;

    if( 0 == fp.Size() )
    {
        result.errInfo = "File is empty!";
        return false;
    }

    DynaArray<char> buf( fp.Size() );

    size_t sz;

    if( !fp.read( &buf[0], fp.Size(), &sz ) )
    {
        result.errInfo = "Fail to read the file!";
        return false;
    }

    return parse( result, &buf[0], sz );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::XmlDocument::writeToFile( File & file, const XmlNode & root, bool compact )
{
    GN_GUARD;

    //static const UInt8 bom[3] = { 0xEF, 0xBB, 0xBF };
    //if( sizeof(bom) != file.write( bom, sizeof(bom) ) ) return false;

    file << "<?xml version=\"1.0\" encoding=\""
         << CharacterEncodingConverter::sEncoding2Str( GetCurrentSystemEncoding() )
         << "\"?>\n";

    if( compact )
    {
        return sCompactNodes( file, &root );
    }
    else
    {
        return sFormatNodes( file, &root, 0 );
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::XmlNode * GN::XmlDocument::createNode( XmlNodeType type, XmlNode * parent )
{
    XmlNode * p;
    switch( type )
    {
        case XML_CDATA   : p = new PooledNode<XmlCdata>(*this); break;
        case XML_COMMENT : p = new PooledNode<XmlComment>(*this); break;
        case XML_ELEMENT : p = new PooledNode<XmlElement>(*this); break;
        default          : GN_ERROR(sLogger)( "invalid node type : %d", type ); return NULL;
    }
    mNodes.Append( p );
    p->setParent( parent );
    return p;
}

//
//
// -----------------------------------------------------------------------------
GN::XmlAttrib * GN::XmlDocument::createAttrib( XmlElement * owner )
{
    PooledAttrib * a = new PooledAttrib( *this );

    mAttribs.Append( a );

    a->setOwner( owner );

    return a;
}

//
//
// -----------------------------------------------------------------------------
void GN::XmlDocument::releaseAllNodesAndAttribs()
{
    GN_GUARD;

    for( size_t i = 0; i < mNodes.Size(); ++i ) delete mNodes[i];
    mNodes.Clear();

    for( size_t i = 0; i < mAttribs.Size(); ++i ) delete mAttribs[i];
    mAttribs.Clear();

    GN_UNGUARD;
}
