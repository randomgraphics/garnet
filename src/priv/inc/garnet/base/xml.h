#ifndef __GN_BASE_XML_H__
#define __GN_BASE_XML_H__
// *****************************************************************************
/// \file
/// \brief   XML processor. Wrap functionality of other external XML parsers. (working in progress)
/// \author  chenlee (2006.5.2)
// *****************************************************************************

namespace GN
{
    struct XmlCdata;
    struct XmlElement;
    struct XmlComment;
    class  XmlDocument;

    ///
    /// XML attribute class
    ///
    struct XmlAttrib
    {
        XmlDocument & doc;   ///< The document that this attribute belongs to.
        XmlElement  * node;  ///< pointer to the element that this attribute belongs to.
        XmlAttrib   * prev;  ///< pointer to previous attribute
        XmlAttrib   * next;  ///< pointer to next attribute
        StrA          name;  ///< attribute name
        StrA          value; ///< attribute value

        ///
        /// attach attribute to specific element
        ///
        void setOwner( XmlElement * element );

    protected:

        /// \name protected ctor/dtor to prevent user from creatiing/deleting this class.
        //@{
        XmlAttrib( XmlDocument & d ) : doc(d), node(0), prev(0), next(0) {}
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
        XmlDocument     & doc;     ///< reference to the owner document
        const XmlNodeType type;    ///< node type. can't be modified.
        XmlNode         * parent;  ///< pointer to parent node
        XmlNode         * prev;    ///< pointer to previous brother node
        XmlNode         * next;    ///< pointer to next brother node
        XmlNode         * child;   ///< pointer to first child

        /// \name method required by traversal class
        //@{
        XmlNode * getParent() const { return parent; }
        XmlNode * getPrevSibling() const { return prev; }
        XmlNode * getNextSibling() const { return next; }
        XmlNode * getFirstChild() const { return child; }
        //@}

        ///
        /// Change node parent
        ///
        void setParent( XmlNode * parent, XmlNode * prev = NULL );

        ///
        /// return location of this node in the document (unimplemented)
        ///
        const char * getLocation() const { return ""; }

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
        XmlNode( XmlDocument & d, XmlNodeType t )
            : doc(d)
            , type(t)
            , parent(0)
            , prev(0)
            , next(0)
            , child(0)
        {
            GN_ASSERT( 0 <= t && t < NUM_XML_NODE_TYPES );
        }
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
        XmlCdata( XmlDocument & d ) : XmlNode(d,XML_CDATA) {}
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
        XmlComment( XmlDocument & d ) : XmlNode(d,XML_COMMENT) {}
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
        XmlAttrib * findAttrib( const StrA & name, StringCompare sc = StringCompare::CASE_SENSITIVE ) const
        {
            for( XmlAttrib * a = attrib; a; a = a->next )
            {
                if( StringCompare::CASE_SENSITIVE == sc )
                {
                    if( name == a->name ) return a;
                }
                else
                {
                    if( 0 == strCmpI( name.cptr(), a->name.cptr() ) ) return a;
                }
            }
            return NULL;
        }

        ///
        /// find specific child of element
        ///
        XmlElement * findChildElement( const StrA & name, StringCompare sc = StringCompare::CASE_SENSITIVE ) const
        {
            for( XmlNode * n = child; n; n = n->next )
            {
                XmlElement * e = n->toElement();
                if( !e ) continue;

                if( StringCompare::CASE_SENSITIVE == sc )
                {
                    if( name == e->name ) return e;
                }
                else
                {
                    if( 0 == strCmpI( name.cptr(), e->name.cptr() ) ) return e;
                }
            }
            return NULL;
        }

    protected:

        ///
        /// protected ctor to prevent user from creatiing this class.
        ///
        XmlElement( XmlDocument & d )
            : XmlNode(d,XML_ELEMENT)
            , attrib(0)
        {
        }
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
        template<class T> struct PooledNode : public T
        {
            PooledNode( XmlDocument & d ) : T(d) {}
        };
        struct PooledAttrib : public XmlAttrib
        {
            PooledAttrib( XmlDocument & d ) : XmlAttrib(d) {}
        };

        std::vector<XmlNode*>      mNodes;
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
        /// XML document is deleted.
        ///
        XmlNode * createNode( XmlNodeType type, XmlNode * parent );

        ///
        /// Create element
        ///
        XmlElement * createElement( XmlNode * parent )
        {
            XmlNode * n = createNode( XML_ELEMENT, parent );
            return n ? n->toElement() : NULL;
        }

        ///
        /// Create new attribute. Attributes are created in pooled memory also,
        /// just like XmlNode.
        ///
        XmlAttrib * createAttrib( XmlElement * owner );

    private:

        ///
        /// Release all attributes and nodes
        ///
        void releaseAllNodesAndAttribs();
    };

    ///
    /// load something from XML file
    ///
    template<class T>
    inline bool loadFromXmlFile( T & t, File & fp, const StrA & basedir )
    {
        GN_GUARD;

        XmlDocument doc;
        XmlParseResult xpr;
        if( !doc.parse( xpr, fp ) )
        {
            static Logger * sLogger = getLogger( "GN.base.xml" );
            GN_ERROR(sLogger)(
                "Fail to parse XML file (%s):\n"
                "    line   : %d\n"
                "    column : %d\n"
                "    error  : %s",
                fp.name(),
                xpr.errLine,
                xpr.errColumn,
                xpr.errInfo.cptr() );
            return false;
        }
        GN_ASSERT( xpr.root );
        return t.loadFromXml( *xpr.root, basedir );

        GN_UNGUARD;
    }

    ///
    /// load something from XML file
    ///
    template<class T>
    inline bool loadFromXmlFile( T & t, const StrA & filename )
    {
        GN_GUARD;

        static Logger * sLogger = getLogger( "GN.base.xml" );
        GN_INFO(sLogger)( "Load '%s'", filename.cptr() );

        AutoObjPtr<File> fp( fs::openFile( filename, "rt" ) );
        if( !fp ) return false;

        StrA basedir = fs::dirName( filename );

        return loadFromXmlFile( t, *fp, basedir );

        GN_UNGUARD;
    }
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_XML_H__
