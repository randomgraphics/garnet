#ifndef __GN_BASE_XML_H__
#define __GN_BASE_XML_H__
// *****************************************************************************
/// \file    base/xml.h
/// \brief   XML processor. Wrap functionality of other external XML parsers. (working in progress)
/// \author  chenlee (2006.5.2)
// *****************************************************************************

namespace GN
{
    struct XmlCdata;
    struct XmlElement;
    struct XmlComment;

    ///
    /// XML attribute class
    ///
    struct XmlAttrib
    {
        XmlElement * node;  ///< pointer to the element that this attribute belongs to. 
        XmlAttrib  * next;  ///< pointer to next attribute
        StrA         name;  ///< attribute name
        StrA         value; ///< attribute value

    protected:

        /// \name protected ctor/dtor to prevent user from creatiing/deleting this class.
        //@{
        XmlAttrib()          {}
        virtual ~XmlAttrib() {}
        //@}
    };

    ///
    /// XML node type
    ///
    enum XmlNodeType
    {
        XML_CDATA,   ///< cdata node
        XML_COMMENT, ///< comment node
        XML_ELEMENT, ///< element node
        NUM_XML_NODE_TYPES, ///< number of node types.
    };

    ///
    /// XML node class
    ///
    struct XmlNode
    {
        const XmlNodeType type;    ///< node type. can't be modified.
        XmlNode         * parent;  ///< pointer to parent node
        XmlNode         * sibling; ///< pointer to next brother node
        XmlNode         * child;   ///< pointer to first child

        ///
        /// conver to cdata node
        ///
        XmlCdata * toCdata() { return XML_CDATA == type ? (XmlCdata*)this : 0; }

        ///
        /// conver to comment node
        ///
        const XmlCdata * toCdata() const { return XML_CDATA == type ? (const XmlCdata*)this : 0; }

        ///
        /// conver to comment node
        ///
        XmlComment * toComment() { return XML_COMMENT == type ? (XmlComment*)this : 0; }

        ///
        /// conver to comment node
        ///
        const XmlComment * toComment() const { return XML_COMMENT == type ? (const XmlComment*)this : 0; }

        ///
        /// Convert to element node
        ///
        XmlElement * toElement() { return XML_ELEMENT == type ? (XmlElement*)this : 0; }

        ///
        /// Convert to element node
        ///
        const XmlElement * toElement() const { return XML_ELEMENT == type ? (const XmlElement*)this : 0; }

        ///
        /// virtual dtor
        ///
        virtual ~XmlNode() {}

    protected:

        ///
        /// protected ctor to prevent user from creatiing this class.
        ///
        XmlNode( XmlNodeType t ) : type(t) { GN_ASSERT( 0 <= t && t < NUM_XML_NODE_TYPES ); }
    };

    ///
    /// XML cdata node
    ///
    struct XmlCdata : public XmlNode
    {
        StrA text;  ///< text content.

    protected:

        ///
        /// protected ctor to prevent user from creatiing this class.
        ///
        XmlCdata() : XmlNode(XML_CDATA) {}
    };

    ///
    /// XML comment node
    ///
    struct XmlComment : public XmlNode
    {
        StrA text; ///< comment text

    protected:

        ///
        /// protected ctor to prevent user from creatiing this class.
        ///
        XmlComment() : XmlNode(XML_COMMENT) {}
    };

    ///
    /// XML element node
    ///
    /// \note
    ///     Here we assume that one element can have, at most, one text section.
    ///     If there were multiple text sections, they would be merged into one.
    ///
    struct XmlElement : public XmlNode
    {
        XmlAttrib * attrib;  ///< pointer to first attribute
        StrA        name;    ///< element name
        StrA        text;    ///< optional text section

        ///
        /// find specific attribute of element
        ///
        XmlAttrib * findAttrib( const char * name ) const
        {
            for( XmlAttrib * a = attrib; a; a = a->next )
            {
                if( name == a->name ) return a;
            }
            return NULL;
        }

        ///
        /// find specific child of element
        ///
        XmlElement * findChildElement( const char * name ) const
        {
            for( XmlNode * n = child; n; n = n->sibling )
            {
                XmlElement * e = n->toElement();
                if( !e ) continue;
                if( name == e->name ) return e;
            }
            return NULL;
        }

    protected:

        ///
        /// protected ctor to prevent user from creatiing this class.
        ///
        XmlElement() : XmlNode(XML_ELEMENT) {}
    };

    ///
    /// XML parse result
    ///
    struct XmlParseResult
    {
        XmlNode * root;   ///< root node of the xml document
        size_t errLine;   ///< error position
        size_t errColumn; ///< error position
        StrA errInfo;     ///< error information
    };

    ///
    /// XML document
    ///
    class XmlDocument
    {
        // T must be one of XML node class
        template<class T> struct PooledNode : public T {};
        struct PooledAttrib : public XmlAttrib {};

        std::vector<XmlNode*> mNodes;
        std::vector<PooledAttrib*> mAttribs;

    public:

        ///
        /// ctor
        ///
        XmlDocument() { mNodes.reserve(256); mAttribs.reserve(256); }

        ///
        /// dtor
        ///
        ~XmlDocument() { releaseAllNodesAndAttribs(); }

        ///
        /// parse xml string buffer
        ///
        bool parse( XmlParseResult & result, const char * content, size_t length = 0 );

        ///
        /// parse xml file
        ///
        bool parse( XmlParseResult &, File & );

        ///
        /// write xml document to file. If compact if false, the format output with newline and ident
        ///
        bool writeToFile( File & file, const XmlNode & root, bool compact );

        ///
        /// Create new node. Nodes are created in pooled memory. No need
        /// to release them. They will be release automatically, when the
        /// XML processer is destroied or releaseNodesAndAttribs() is called.
        ///
        XmlNode * createNode( XmlNodeType );

        ///
        /// Create new attribute. Attributes are created in pooled memory also,
        /// just like XmlNode. 
        ///
        XmlAttrib * createAttrib();

        ///
        /// Release all attributes and nodes
        ///
        void releaseAllNodesAndAttribs();
    };
}

// *****************************************************************************
//                           End of xml.h
// *****************************************************************************
#endif // __GN_BASE_XML_H__
