#include "pch.h"
#include "garnet/base/xml.h"

#define USE_RAPIDXML 0

#if USE_RAPIDXML
    #include <rapidxml.hpp>
#else
    #include <expat.h>
#endif

static GN::Logger * sLogger = GN::getLogger("GN.base.xml");

// *****************************************************************************
// XML print utilities
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static void sIdent(GN::File & fp, int ident) {
    for (int i = 0; i < ident; ++i) fp.output() << "\t";
}

//
//
// -----------------------------------------------------------------------------
static void sFormatAttributes(GN::File & fp, const GN::XmlAttrib * att, int ident) {
    for (; 0 != att; att = att->next) {
        sIdent(fp, ident);
        fp.output() << att->name << "=\"" << att->value << "\"\n";
    }
}

//
//
// -----------------------------------------------------------------------------
static bool sFormatNodes(GN::File & fp, const GN::XmlNode * root, int ident) {
    GN_ASSERT(root);

    switch (root->type) {
    case GN::XML_CDATA: {
        const GN::XmlCdata * c = root->toCdata();
        GN_ASSERT(c && !c->firstc); // cdata node should have no child.
        sIdent(fp, ident);
        fp.output() << "<![CDATA[" << c->text << "]]>\n";
        break;
    }

    case GN::XML_COMMENT: {
        const GN::XmlComment * c = root->toComment();
        GN_ASSERT(c && !c->firstc); // comment node should have no child.
        sIdent(fp, ident);
        fp.output() << "<!-- " << c->text << " -->\n";
        break;
    }

    case GN::XML_ELEMENT: {
        const GN::XmlElement * e = root->toElement();
        GN_ASSERT(e);

        sIdent(fp, ident);
        fp.output() << "<" << e->name;

        if (e->firsta) {
            fp.output() << "\n";
            sFormatAttributes(fp, e->firsta, ident + 1);
            sIdent(fp, ident);
        }

        if (!e->text.empty() || e->firstc) {
            fp.output() << ">\n";

            if (!e->text.empty()) {
                sIdent(fp, ident + 1);
                fp.output() << e->text << "\n";
            }

            if (e->firstc) { sFormatNodes(fp, e->firstc, ident + 1); }

            sIdent(fp, ident);
            fp.output() << "</" << e->name << ">\n";
        } else {
            fp.output() << "/>\n";
        }
        break;
    }

    default:
        GN_UNEXPECTED(); // program should not reach here
        GN_ERROR(sLogger)("Unknown node type!");
        return false;
    };

    // print brothers
    return root->nexts ? sFormatNodes(fp, root->nexts, ident) : true;
}

//
//
// -----------------------------------------------------------------------------
static void sCompactAttributes(GN::File & fp, const GN::XmlAttrib * att) {
    for (; 0 != att; att = att->next) { fp.output() << " " << att->name << "=\"" << att->value << "\""; }
}

//
//
// -----------------------------------------------------------------------------
static bool sCompactNodes(GN::File & fp, const GN::XmlNode * root);
static bool sCompactNodeAndChildren(GN::File & fp, const GN::XmlNode * root) {
    GN_ASSERT(root);

    switch (root->type) {
    case GN::XML_CDATA: {
        const GN::XmlCdata * c = root->toCdata();
        GN_ASSERT(!c->firstc); // cdata node should have no child.
        fp.output() << "<![CDATA[" << c->text << "]]>\n";
        break;
    }

    case GN::XML_COMMENT: {
        const GN::XmlComment * c = root->toComment();
        GN_ASSERT(c && !c->firstc); // comment node should have no child.
        fp.output() << "<!-- " << c->text << " -->\n";
        break;
    }

    case GN::XML_ELEMENT: {
        const GN::XmlElement * e = root->toElement();
        GN_ASSERT(e);
        fp.output() << "<" << e->name;
        if (e->firsta) { sCompactAttributes(fp, e->firsta); }
        if (!e->text.empty() || e->firstc) {
            fp.output() << ">\n";
            if (!e->text.empty()) fp.output() << e->text;
            if (e->firstc) sCompactNodes(fp, e->firstc);
            fp.output() << "</" << e->name << ">\n";
        } else {
            fp.output() << "/>\n";
        }
        break;
    }

    default:
        GN_UNEXPECTED(); // program should not reach here
        GN_ERROR(sLogger)("Unknown node type!");
        return false;
    };

    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool sCompactNodes(GN::File & fp, const GN::XmlNode * root) {
    GN_ASSERT(root);

    do {
        if (!sCompactNodeAndChildren(fp, root)) return false;
        root = root->nexts;
    } while (root);

    return true;
}

#if USE_RAPIDXML

// *****************************************************************************
// RapidXml utilities
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static GN::XmlNode * sCreateXmlSubTree(GN::XmlDocument & doc, GN::XmlNode * parent, const rapidxml::xml_node<> * node) {
    using namespace rapidxml;

    if (NULL == node) return NULL;

    switch (node->type()) {
    case node_element: {
        GN::XmlElement * n = doc.createElement(parent);
        n->name            = node->name();
        for (xml_attribute<> * attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
            GN::XmlAttrib * a = doc.createAttrib(n);
            a->name           = attr->name();
            a->value          = attr->value();
        }

        for (xml_node<> * child = node->first_node(); child; child = child->next_sibling()) { sCreateXmlSubTree(doc, n, child); }
        return n;
    }

    case node_data: {
        GN_ASSERT(parent);
        GN::XmlElement * n = parent->toElement();
        GN_ASSERT(n);
        n->text += node->value();
        return NULL;
    }

    case node_cdata: {
        GN::XmlCdata * n = doc.createNode(GN::XML_CDATA, parent)->toCdata();
        n->text          = node->value();
        return n;
    }

    case node_comment: {
        GN::XmlComment * n = doc.createNode(GN::XML_COMMENT, parent)->toComment();
        n->text            = node->value();
        return n;
    }

    default:
        // do nothing
        return NULL;
    }
}

#else

// *****************************************************************************
// Expat utilities
// *****************************************************************************

struct AutoFree {
    XML_Parser ptr;
    AutoFree(XML_Parser p): ptr(p) { GN_ASSERT(p); }
    ~AutoFree() { XML_ParserFree(ptr); }
};

// This is used to trace where we are, during parsing the XML documents.
struct ParseTracer {
    XML_Parser           parser;
    GN::XmlDocument *    doc;
    GN::XmlParseResult * result;
    GN::XmlNode *        parent;
    GN::XmlNode *        prev;
};

//
//
// -----------------------------------------------------------------------------
static void * sXmlMalloc(size_t sz) { return GN::HeapMemory::alloc(sz); }
static void * sXmlRealloc(void * p, size_t sz) { return GN::HeapMemory::realloc(p, sz); }
static void   sXmlFree(void * p) { return GN::HeapMemory::dealloc(p); }

//
//
// -----------------------------------------------------------------------------
static void sParseFail(ParseTracer * tracer, const char * errInfo) {
    XML_StopParser(tracer->parser, XML_FALSE);
    tracer->result->errInfo   = errInfo;
    tracer->result->errLine   = XML_GetCurrentLineNumber(tracer->parser);
    tracer->result->errColumn = XML_GetCurrentColumnNumber(tracer->parser);
}

//
//
// -----------------------------------------------------------------------------
static GN::XmlNode * sNewNode(ParseTracer * tracer, GN::XmlNodeType type) {
    GN::XmlNode * n = tracer->doc->createNode(type, NULL);
    if (0 == n) {
        sParseFail(tracer, GN::str::format("Fail to create node with type of '%d'", type).rawptr());
        return NULL;
    }

    // update tree links
    n->setParent(tracer->parent, tracer->prev);

    // update tracer
    tracer->parent = n;
    tracer->prev   = NULL;

    return n;
}

//
//
// -----------------------------------------------------------------------------
static GN::StrA sMangleText(const char * s, int len) {
    // skip leading spaces
    while (len > 0 && (' ' == *s || '\t' == *s || '\n' == *s)) {
        ++s;
        --len;
    }
    if (0 == len) return "";

    // skip tailing spaces
    const XML_Char * e = s + len - 1;
    while (len > 0 && (' ' == *e || '\t' == *e || '\n' == *e)) {
        --e;
        --len;
    }
    if (0 == len) return "";

    GN_TODO("convert special characters");

    return GN::StrA(s, len);
}

//
//
// -----------------------------------------------------------------------------
void XMLCALL sStartElementHandler(void * userData, const XML_Char * name, const XML_Char ** atts) {
    GN_ASSERT(userData && name);

    ParseTracer * tracer = (ParseTracer *) userData;

    // create new node
    GN::XmlNode * n = sNewNode(tracer, GN::XML_ELEMENT);
    if (0 == n) return;
    GN::XmlElement * e = n->toElement();
    GN_ASSERT(e);

    e->name = name;

    // create attribute list
    while (*atts) {
        GN::XmlAttrib * a = tracer->doc->createAttrib(e);

        if (0 == a) {
            sParseFail(tracer, "Fail to create attribute.");
            return;
        }

        a->name  = atts[0];
        a->value = atts[1];

        atts += 2;
    }
}

//
//
// -----------------------------------------------------------------------------
void XMLCALL sEndElementHandler(void * userData, const XML_Char *) {
    ParseTracer * tracer = (ParseTracer *) userData;

    // update tracer
    GN_ASSERT(tracer->parent);
    tracer->prev   = tracer->parent;
    tracer->parent = tracer->parent->parent;
}

//
//
// -----------------------------------------------------------------------------
static void XMLCALL sStartCdataSectionHandler(void * userData) {
    ParseTracer * tracer = (ParseTracer *) userData;

    // create new node
    GN::XmlNode * n = sNewNode(tracer, GN::XML_CDATA);
    if (0 == n) return;
}

//
//
// -----------------------------------------------------------------------------
static void XMLCALL sEndCdataSectionHandler(void * userData) {
    ParseTracer * tracer = (ParseTracer *) userData;

    GN_ASSERT(tracer->parent && tracer->parent->type == GN::XML_CDATA);

    // update tracer
    GN_ASSERT(tracer->parent);
    tracer->prev   = tracer->parent;
    tracer->parent = tracer->parent->parent;
}

//
//
// -----------------------------------------------------------------------------
static void XMLCALL sCharacterDataHandler(void * userData, const XML_Char * s, int len) {
    ParseTracer * tracer = (ParseTracer *) userData;

    GN_ASSERT(tracer->parent);

    if (tracer->parent->type == GN::XML_CDATA) {
        GN::XmlCdata * t = tracer->parent->toCdata();
        t->text.append(s, len);
    } else {
        GN::StrA text = sMangleText(s, len);
        if (!text.empty() && tracer->parent->type == GN::XML_ELEMENT) {
            GN::StrA & currentText = tracer->parent->toElement()->text;

            if (!currentText.empty()) currentText += ' ';

            currentText += text;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
static void XMLCALL sCommentHandler(void * userData, const XML_Char * data) {
    ParseTracer * tracer = (ParseTracer *) userData;

    // create new node
    GN::XmlNode * n = sNewNode(tracer, GN::XML_COMMENT);
    if (0 == n) return;
    GN::XmlComment * c = n->toComment();
    c->text.assign(data);

    // update tracer
    GN_ASSERT(c == tracer->parent);
    tracer->prev   = tracer->parent;
    tracer->parent = tracer->parent->parent;
}

//
//
// -----------------------------------------------------------------------------
static int XMLCALL sDummyConvert(void *, const char *) { return ' '; }

//
//
// -----------------------------------------------------------------------------
static int XMLCALL sEncodingHandler(void * encodingHandlerData, const XML_Char * name, XML_Encoding * info) {
    using namespace GN;

    GN_UNUSED_PARAM(encodingHandlerData);

    if (0 == str::compareI("gbk", name) || 0 == str::compareI("gb2312", name)) {
        info->convert = &sDummyConvert;
        info->release = NULL;

        for (int i = 0; i < 128; ++i) { info->map[i] = (char) i; }
        for (int i = 129; i < 255; ++i) { info->map[i] = -2; }

        info->map[255] = 0;

        return 1;
    } else {
        GN_ERROR(sLogger)("Unknown encoding: %s", name);
        return 0;
    }
}
#endif

// *****************************************************************************
// XmlAttrib class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API void GN::XmlAttrib::setOwner(XmlElement * newOwner, XmlAttrib * newPrev) {
    if (node == newOwner) return;

    if (newOwner && &newOwner->doc != &this->doc) {
        GN_ERROR(sLogger)("Cannot attach attribute to node that belongs to different XML document.");
        return;
    }

    if (newPrev && newPrev->node != newOwner) {
        GN_ERROR(sLogger)("New previous attribute does not belong to the new element.");
        return;
    }

    // detach from old owner
    if (node) {
        if (node->firsta == this) node->firsta = this->next;
        if (node->lasta == this) node->lasta = this->prev;
    }
    if (this->next) this->next->prev = this->prev;
    if (this->prev) this->prev->next = this->next;
    this->node = NULL;
    this->prev = NULL;
    this->next = NULL;

    // attach to new node
    if (newOwner) {
        this->node = newOwner;

        if (newPrev) {
            this->prev = newPrev;
            this->next = newPrev->next;
            if (newPrev->next) { newPrev->next->prev = this; }
            newPrev->next = this;
            if (newPrev == newOwner->lasta) { newOwner->lasta = this; }
        } else {
            this->next = newOwner->firsta;
            if (newOwner->firsta) { newOwner->firsta->prev = this; }
            newOwner->firsta = this;
            if (NULL == newOwner->lasta) { newOwner->lasta = this; }
        }
    }
}

// *****************************************************************************
// XmlNode class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API void GN::XmlNode::setParent(XmlNode * newParent, XmlNode * newPrev) {
    if (parent == newParent) return;

    if (newParent && &newParent->doc != &this->doc) {
        GN_ERROR(sLogger)("Can not link nodes belong to different document.");
        return;
    }

    if (newPrev && newPrev->parent != newParent) {
        GN_ERROR(sLogger)("New previous node does not belong to the new parent node.");
        return;
    }

    // detach from old parent
    if (parent) {
        if (parent->firstc == this) parent->firstc = this->nexts;
        if (parent->lastc == this) parent->lastc = this->prevs;
    }
    if (this->nexts) this->nexts->prevs = this->prevs;
    if (this->prevs) this->prevs->nexts = this->nexts;
    this->parent = NULL;
    this->prevs  = NULL;
    this->nexts  = NULL;

    // attach to new parent
    if (newParent) {
        this->parent = newParent;

        if (newPrev) {
            this->prevs = newPrev;
            this->nexts = newPrev->nexts;
            if (newPrev->nexts) { newPrev->nexts->prevs = this; }
            newPrev->nexts = this;
            if (newPrev == newParent->lastc) { newParent->lastc = this; }
        } else {
            this->nexts = newParent->firstc;
            if (newParent->firstc) { newParent->firstc->prevs = this; }
            newParent->firstc = this;
            if (NULL == newParent->lastc) { newParent->lastc = this; }
        }
    }
}

// *****************************************************************************
// XmlDocument class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::XmlDocument::parse(XmlParseResult & result, const char * content, size_t length) {
    GN_GUARD;

    result.root = NULL;
    result.errInfo.clear();
    result.errLine   = 0;
    result.errColumn = 0;

#if USE_RAPIDXML

    using namespace rapidxml;

    xml_document<> doc;
    try {
        if (0 == length) length = str::length(content);
        char * buf = doc.allocate_string(NULL, length + 1);
        memcpy(buf, content, length);
        buf[length] = 0;
        doc.parse<parse_trim_whitespace>(buf);
    } catch (parse_error & e) {
        result.root    = NULL;
        result.errInfo = e.what();
        return false;
    }

    result.root = sCreateXmlSubTree(*this, NULL, doc.first_node());
    return true;

#else

    // create parser
    XML_Memory_Handling_Suite mm     = {&sXmlMalloc, &sXmlRealloc, &sXmlFree};
    XML_Parser                parser = XML_ParserCreate_MM(NULL, &mm, 0);
    if (0 == parser) {
        result.errInfo = "Fail to create parser.";
        return false;
    }
    AutoFree af(parser); // free the parser automatically when go out of this function.

    // setup user data
    ParseTracer userData = {parser, this, &result, NULL, NULL};
    XML_SetUserData(parser, &userData);

    // setup handlers
    XML_SetUnknownEncodingHandler(parser, &sEncodingHandler, NULL);
    XML_SetElementHandler(parser, &sStartElementHandler, &sEndElementHandler);
    XML_SetCdataSectionHandler(parser, &sStartCdataSectionHandler, &sEndCdataSectionHandler);
    XML_SetCharacterDataHandler(parser, &sCharacterDataHandler);
    XML_SetCommentHandler(parser, &sCommentHandler);

    // start parse
    XML_Status status = XML_Parse(parser, content, (int) length, XML_TRUE);
    if (XML_STATUS_OK != status) {
        if (result.errInfo.empty()) {
            result.errInfo   = "XML_Parse() failed.";
            result.errLine   = XML_GetCurrentLineNumber(parser);
            result.errColumn = XML_GetCurrentColumnNumber(parser);
        }
        return false;
    }

    GN_ASSERT(userData.prev);
    result.root = userData.prev;

    // success
    return true;

#endif

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::XmlDocument::parse(XmlParseResult & result, File & fp) {
    GN_GUARD;

    result.errInfo.clear();
    result.errLine   = 0;
    result.errColumn = 0;

    if (0 == fp.size()) {
        result.errInfo = "File is empty!";
        return false;
    }

    DynaArray<char> buf(fp.size());

    size_t rd = fp.read(buf.data(), buf.size());
    if (0 == rd) return false;

    return parse(result, buf.data(), rd);

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::XmlDocument::writeToFile(File & file, const XmlNode & root, bool compact) {
    GN_GUARD;

    // static const uint8 bom[3] = { 0xEF, 0xBB, 0xBF };
    // if( sizeof(bom) != file.write( bom, sizeof(bom) ) ) return false;

    file.output() << "<?xml version=\"1.0\"?>";

    if (compact) {
        return sCompactNodes(file, &root);
    } else {
        file.output() << "\n";
        return sFormatNodes(file, &root, 0);
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::XmlNode * GN::XmlDocument::createNode(XmlNodeType type, XmlNode * parent) {
    XmlNode * p;
    switch (type) {
    case XML_CDATA:
        p = new PooledNode<XmlCdata>(*this);
        break;
    case XML_COMMENT:
        p = new PooledNode<XmlComment>(*this);
        break;
    case XML_ELEMENT:
        p = new PooledNode<XmlElement>(*this);
        break;
    default:
        GN_ERROR(sLogger)("invalid node type : %d", type);
        return NULL;
    }
    mNodes.append(p);
    p->setParent(parent, parent ? parent->lastc : NULL);
    return p;
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::XmlAttrib * GN::XmlDocument::createAttrib(XmlElement * owner) {
    PooledAttrib * a = new PooledAttrib(*this);

    mAttribs.append(a);

    a->setOwner(owner, owner ? owner->lasta : NULL);

    return a;
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::XmlDocument::releaseAllNodesAndAttribs() {
    GN_GUARD;

    for (size_t i = 0; i < mNodes.size(); ++i) delete mNodes[i];
    mNodes.clear();

    for (size_t i = 0; i < mAttribs.size(); ++i) delete mAttribs[i];
    mAttribs.clear();

    GN_UNGUARD;
}
