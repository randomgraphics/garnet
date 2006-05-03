#ifndef __GN_BASE_XML_H__
#define __GN_BASE_XML_H__
// *****************************************************************************
//! \file    base/xml.h
//! \brief   XML processor. Wrap functionality of other external XML parsers. (working in progress)
//! \author  chenlee (2006.5.2)
// *****************************************************************************

namespace GN
{
    struct XmlNode;

    //!
    //! XML attribute class
    //!
    struct XmlAttrib
    {
        XmlNode   * node;  //!< pointer to the node that this attribute belongs to. 
        XmlAttrib * next;  //!< pointer to next attribute
        StrA        name;  //!< attribute name
        StrA        value; //!< attribute value

    protected:

        //! \name protected ctor/dtor to prevent user from creatiing/deleting this class.
        //@{
        XmlAttrib()          {}
        virtual ~XmlAttrib() {}
        //@}
    };

    //!
    //! XML node class
    //!
    struct XmlNode
    {
        XmlNode   * parent;  //!< pointer to parent node
        XmlNode   * sibling; //!< pointer to next brother node
        XmlNode   * child;   //!< pointer to first child
        XmlAttrib * attrib;  //!< pointer to first attribute
        StrA        name;    //!< element name

    protected:

        //! \name protected ctor/dtor to prevent user from creatiing/deleting this class.
        //@{
        XmlNode()          {}
        virtual ~XmlNode() {}
        //@}
    };

    //!
    //! XML parse result
    //!
    struct XmlParseResult
    {
        XmlNode * root;   //!< root node of the xml document
        size_t errLine;   //!< error position
        size_t errColumn; //!< error position
        StrA errInfo;     //!< error information
    };

    //!
    //! XML processor
    //!
    class XmlProcessor
    {
        struct PooledNode : public XmlNode
        {
            PooledNode() {}
            ~PooledNode() {}
        };

        struct PooledAttrib : public XmlAttrib
        {
            PooledAttrib() {}
            ~PooledAttrib() {}
        };

        std::vector<PooledNode*> mNodes;
        std::vector<PooledAttrib*> mAttribs;

    public:

        //!
        //! ctor
        //!
        XmlProcessor() { mNodes.reserve(256); mAttribs.reserve(256); }

        //!
        //! dtor
        //!
        ~XmlProcessor() { releaseAllNodesAndAttribs(); }

        //!
        //! parse xml document
        //!
        bool parseBuffer( XmlParseResult & result, const char * content, size_t length = 0 );

        //!
        //! write xml document to file
        //!
        bool writeToFile( File & file, const XmlNode & root );

        //!
        //! Create new node. Nodes are created in pooled memory. No need
        //! to release them. They will be release automatically, when the
        //! XML processer is destroied or releaseNodesAndAttribs() is called.
        //!
        XmlNode * createNode();

        //!
        //! Create new attribute. Attributes are created in pooled memory also,
        //! just like XmlNode. 
        //!
        XmlAttrib * createAttrib();

        //!
        //! Release all attributes and nodes
        //!
        void releaseAllNodesAndAttribs();
    };
};

// *****************************************************************************
//                           End of xml.h
// *****************************************************************************
#endif // __GN_BASE_XML_H__
