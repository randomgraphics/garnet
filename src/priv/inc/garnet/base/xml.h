#ifndef __GN_BASE_XML_H__
#define __GN_BASE_XML_H__
// *****************************************************************************
//! \file    base/xml.h
//! \brief   XML processor. Wrap functionality of other external XML parsers. (working in progress)
//! \author  chenlee (2006.5.2)
// *****************************************************************************

namespace GN
{
    class XmlNode;

    //!
    //! XML attribute class
    //!
    class XmlAttribute
    {
        XmlNode      * mNode;  
        XmlAttribute * mPrev;  //!< pointer to previous attribute
        XmlAttribute * mNnext; //!< pointer to next attribute

        friend class XmlProcessor;

    protected:

        //! \name protected ctor/dtor to prevent user from creatiing/deleting this class.
        //@{
        XmlAttribute()          {}
        virtual ~XmlAttribute() {}
        //@}

    public:

        StrA name; //!< attribute name
        StrA value; //!< attribute value

    public:

        //!
        //! pointer to the element that this attribute belongs to.
        //!
        XmlNode * node() const { return mNode; }

        //!
        //! get previous attribute
        //!
        XmlAttribute * prev() const { return mPrev; }

        //!
        //! get next attribute
        //!
        XmlAttribute * next() const { return mNext; }
    };

    //!
    //! XML node class
    //!
    class XmlNode
    {
        XmlNode      * mParent;
        XmlNode      * mSibling;
        XmlNode      * mFirstChild;
        XmlAttribute * mFirstAttrib;

        friend class XmlProcessor;

    protected:

        //! \name protected ctor/dtor to prevent user from creatiing/deleting this class.
        //@{
        XmlNode()          {}
        virtual ~XmlNode() {}
        //@}

    public:

        StrA name; //!< element name

    public:

        //!
        //! get parent node
        //!
        XmlNode * parent() const { return mParent; }

        //!
        //! get next sibling node
        //!
        XmlNode * sibling() const { return mSibling; }

        //!
        //! get first child node
        //!
        XmlNode * firstChild() const { return mFirstChild; }

        //!
        //! get first attribute
        //!
        XmlNode * firstAttrib() const { return mFirstAttrib; }
    };

    //!
    //! XML parse result
    //!
    struct ParseResult
    {
        XmlNode * root; //!< root node of the xml document
        int errLine;    //!< error position
        int errColume;  //!< error position
    };

    //!
    //! XML processor
    //!
    class XmlProcessor
    {
    public:

        //!
        //! parse xml document
        //!
        bool parse( ParseResult & result, const char * content, size_t length );
   };
};

// *****************************************************************************
//                           End of xml.h
// *****************************************************************************
#endif // __GN_BASE_XML_H__
