#include "pch.h"
#include "garnet/base/xml.h"
#include <rapidxml.hpp>

// *****************************************************************************
// local functions
// *****************************************************************************

static GN::Logger * sLogger = GN::getLogger("GN.base.xml");

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

            if( !e->text.empty() || e->child )
            {
                fp << ">\n";

                if( !e->text.empty() )
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
            if( !e->text.empty() || e->child )
            {
                fp << ">\n";
                if( !e->text.empty() ) fp << e->text;
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

static GN::XmlNode * sCreateXmlSubTree(
    GN::XmlDocument & doc,
    GN::XmlNode * parent,
    const rapidxml::xml_node<> * node )
{
    using namespace rapidxml;

    if( NULL == node ) return NULL;

    switch( node->type() )
    {
        case node_element:
        {
            GN::XmlElement * n = doc.createElement( parent );
            n->name = node->name();
            if( node->first_attribute() )
            {
                for( xml_attribute<> *attr = node->last_attribute();
                     attr;
                     attr = attr->previous_attribute())
                {
                    GN::XmlAttrib * a = doc.createAttrib( n );
                    a->name = attr->name();
                    a->value = attr->value();
                }
            }

            if( node->first_node() )
            {
                for( xml_node<> * child = node->last_node();
                     child;
                     child = child->previous_sibling() )
                {
                    sCreateXmlSubTree( doc, n, child );
                }
            }
            return n;
        }

        case node_data:
        {
            GN_ASSERT( parent );
            GN::XmlElement * n = parent->toElement();
            GN_ASSERT( n );
            n->text += node->value();
            return NULL;
        }

        case node_cdata:
        {
            GN::XmlCdata * n = doc.createNode( GN::XML_CDATA, parent )->toCdata();
            n->text = node->value();
            return n;
        }

        case node_comment:
        {
            GN::XmlComment * n = doc.createNode( GN::XML_COMMENT, parent )->toComment();
            n->text = node->value();
            return n;
        }

        default:
            // do nothing
            return NULL;
    }
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

    using namespace rapidxml;

    result.errInfo.clear();
    result.errLine = 0;
    result.errColumn = 0;

    xml_document<> doc;
    try
    {
        doc.parse<0>( doc.allocate_string( content, length ) );
    }
    catch(parse_error & e)
    {
        result.root = NULL;
        result.errInfo = e.what();
        return false;
    }

    result.root = sCreateXmlSubTree( *this, NULL, doc.first_node() );
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::XmlDocument::parse( XmlParseResult & result, File & fp )
{
    GN_GUARD;

    result.errInfo.clear();
    result.errLine = 0;
    result.errColumn = 0;

    if( 0 == fp.size() )
    {
        result.errInfo = "File is empty!";
        return false;
    }

    DynaArray<char> buf( fp.size() );

    size_t sz;

    if( !fp.read( &buf[0], fp.size(), &sz ) )
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

    file << "<?xml version=\"1.0\"?>";

    if( compact )
    {
        return sCompactNodes( file, &root );
    }
    else
    {
        file << "\n";
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
    mNodes.append( p );
    p->setParent( parent );
    return p;
}

//
//
// -----------------------------------------------------------------------------
GN::XmlAttrib * GN::XmlDocument::createAttrib( XmlElement * owner )
{
    PooledAttrib * a = new PooledAttrib( *this );

    mAttribs.append( a );

    a->setOwner( owner );

    return a;
}

//
//
// -----------------------------------------------------------------------------
void GN::XmlDocument::releaseAllNodesAndAttribs()
{
    GN_GUARD;

    for( size_t i = 0; i < mNodes.size(); ++i ) delete mNodes[i];
    mNodes.clear();

    for( size_t i = 0; i < mAttribs.size(); ++i ) delete mAttribs[i];
    mAttribs.clear();

    GN_UNGUARD;
}
