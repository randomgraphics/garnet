/* implement PrintToString(...) */

#include <ctype.h>
#include "tinyxml.h"

//
//
// -----------------------------------------------------------------------------
void TiXmlAttribute::PrintToString( TiXmlString & str, int /*depth*/ ) const
{
    TIXML_STRING n, v;

    PutString( name, &n );
    PutString( value, &v );

    if (value.find ('\"') == TIXML_STRING::npos)
    {
        str.append( n );
        str.append( "=\"" );
        str.append( v );
        str.append( '\"' );
    }
    else
    {
        //fprintf (cfile, "%s='%s'", n.c_str(), v.c_str() );
        str.append( n );
        str.append( "='" );
        str.append( v );
        str.append( "'" );
    }
}

//
//
// -----------------------------------------------------------------------------
void TiXmlElement::PrintToString( TiXmlString & str, int depth ) const
{
    int i;
    for ( i=0; i<depth; i++ )
    {
        str.append( "    " );
    }

    str.append( '<' ); str.append( value );

    TiXmlAttribute* attrib;
    for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
    {
        str.append( " " );
        attrib->PrintToString( str, depth );
    }

    // There are 4 different formatting approaches:
    // 1) An element without children is printed as a <foo /> node
    // 2) An element with only a text child is printed as <foo> text </foo>
    // 3) An element with only a cdata child is printed as <foo> cdata </foo>
    // 4) An element with children is printed on multiple lines.
    TiXmlNode* node;
    if ( !firstChild )
    {
        str.append( " />" );
    }
    else if ( firstChild == lastChild && (firstChild->ToText()||firstChild->ToCData()) )
    {
        str.append( ">" );
        firstChild->PrintToString( str, depth + 1 );
        str.append( "</" ); str.append( value ); str.append( ">" );
    }
    else
    {
        str.append( ">" );

        for ( node = firstChild; node; node=node->NextSibling() )
        {
            if ( !node->ToText() )
            {
                str.append( "\n" );
            }
            node->PrintToString( str, depth+1 );
        }
        str.append( "\n" );
        for( i=0; i<depth; ++i )
            str.append( "    " );
        str.append( "</" ); str.append( value ); str.append( ">" );
    }
}

//
//
// -----------------------------------------------------------------------------
void TiXmlComment::PrintToString( TiXmlString & str, int depth ) const
{
    for ( int i=0; i<depth; i++ )
    {
        str.append( "    " );
    }
    str.append( "<!--" ); str.append( value ); str.append( "-->" );
}

//
//
// -----------------------------------------------------------------------------
void TiXmlText::PrintToString( TiXmlString & str, int depth ) const
{
    TIXML_STRING buffer;
    PutString( value, &buffer );
    str.append( buffer );
}

//
//
// -----------------------------------------------------------------------------
void TiXmlDeclaration::PrintToString( TiXmlString & str, int depth ) const
{
    str.append( "<?xml " );

    if ( !version.empty() )
    {
        str.append( "version=\"" );
        str.append( version );
        str.append( "\" " );
    }
    if ( !encoding.empty() )
    {
        str.append( "encoding=\"" );
        str.append( encoding );
        str.append( "\" " );
    }
    if ( !standalone.empty() )
    {
        str.append( "standalone=\"" );
        str.append( standalone );
        str.append( "\" " );
    }
    str.append( "?>" );
}

//
//
// -----------------------------------------------------------------------------
void TiXmlCData::PrintToString( TiXmlString & str, int depth ) const
{
    str.append( "<![CDATA[" );
    str.append( value );
    str.append( "]]>" );
}

//
//
// -----------------------------------------------------------------------------
void TiXmlUnknown::PrintToString( TiXmlString & str, int depth ) const
{
    for ( int i=0; i<depth; i++ )
        str.append( "    " );
    str.append( "<" );
    str.append( value );
    str.append( ">" );
}

//
//
// -----------------------------------------------------------------------------
void TiXmlDocument::PrintToString( TiXmlString & str, int depth ) const
{
    TiXmlNode* node;
    for ( node=FirstChild(); node; node=node->NextSibling() )
    {
        node->PrintToString( str, depth );
        str.append( "\n" );
    }
}
