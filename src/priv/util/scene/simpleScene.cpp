#include "pch.h"
#include "garnet/GNutil.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static GN::Logger * sLogger = GN::GetLogger("GN.util");

// *****************************************************************************
// Local stuff
// *****************************************************************************

static bool sHasSemantic( const VertexFormat & vf, const char * binding, size_t index )
{
    for( size_t i = 0; i < vf.numElements; ++i )
    {
        if( 0 == StringCompareI( vf.elements[i].binding, binding ) &&
            index == vf.elements[i].bindingIndex )
        {
            return true;
        }
    }

    return false;
}

static bool sHasPosition( const VertexFormat & vf )
{
    return sHasSemantic( vf, "position", 0 )
        || sHasSemantic( vf, "pos", 0 );
}

static bool sHasNormal( const VertexFormat & vf )
{
    return sHasSemantic( vf, "normal", 0 );
}

static bool sHasTex0( const VertexFormat & vf )
{
    return sHasSemantic( vf, "texcoord", 0 );
}

static bool sHasTangent( const VertexFormat & vf )
{
    return sHasSemantic( vf, "tangent", 0 );
}

///
/// Determine the best model template that can show the mesh, return NULL for failure
///
static const ModelResourceDesc * sDetermineBestModelTemplate( const MeshResourceDesc & m )
{
    const VertexFormat & vf = m.vtxfmt;

    // position is required
    if( !sHasPosition( vf ) )
    {
        GN_ERROR(sLogger)( "The mesh has no position, which is required by the mesh viewer." );
        return NULL;
    }

    if( !sHasNormal( vf ) )
    {
        GN_WARN(sLogger)( "The mesh has no normal." );
        return &SimpleWireframeModel::DESC;
    }

    if( !sHasTex0( vf ) )
    {
        GN_WARN(sLogger)( "The mesh has no texture coordinate." );
        return &SimpleDiffuseModel::DESC;
    }

    // Program reaches here, means that the mesh has position, norml and texcoord.

    if( sHasTangent( vf ) )
    {
        return &SimpleNormalMapModel::DESC;
    }
    else
    {
        return &SimpleDiffuseModel::DESC;
    }
}

// *****************************************************************************
// XPR loader
//
// NOTE: XPR2 format is 3D model format used by various games from Tecomo,
//       like DOA.
//
// The file is structured like this:
//
//     <XPRFileHeader> (16 bytes)
//     <ObjectTable>   (16 bytes * number_of_objects)
//     <Unknown>       (12 bytes)
//     <Data>          (data)
//
// *****************************************************************************

struct XPRFileHeader
{
    FOURCC tag;        ///< must be XPR2
    UInt32 size1;      ///< size tag 1
    UInt32 size2;      ///< size tag 2 (file size = size1+size2+12)
    UInt32 numObjects; ///< number of objects in this file
};

struct XPRObjectHeader
{
    FOURCC type;    ///< object type, could be "USER", "TX2D", "VBUF", "IBUF".
    UInt32 offset;  ///< object offset in bytes. The actual offset is this value + 12.
    UInt32 size;    ///< object size in bytes
    UInt32 unknown; ///< I don't know what this is for.
};

// XPR texture descriptor, 0x28 bytes
struct XPRTex2DDesc
{
    // 10 dwords
    UInt32 dwords[10];
};
GN_CASSERT( 0x28 == sizeof(XPRTex2DDesc) );

/// XPR vertex buffer descriptor, 0x14 bytes
struct XPRVBufDesc
{
    UInt32 dwords[5];
};
GN_CASSERT( 0x14 == sizeof(XPRVBufDesc) );

/// XPR index buffer descriptor, 0x14 bytes
struct XPRIBufDesc
{
    UInt32 dwords[5];
};
GN_CASSERT( 0x14 == sizeof(XPRIBufDesc) );

struct XPRScene
{
    DynaArray<gfx::MeshResourceDesc> meshes;
    DynaArray<XPRTex2DDesc*>         texDescs;
    DynaArray<XPRVBufDesc*>          vbDescs;
    DynaArray<XPRIBufDesc*>          ibDescs;
    DynaArray<void*>                 sceneData; ///< store all texture and mesh data
    Boxf                             bbox;      ///< bounding box
};

static bool
loadXprSceneFromFile( XPRScene & xpr, File & file )
{
    size_t readen;

    // read file header
    XPRFileHeader header;
    if( !file.read( &header, sizeof(header), &readen ) || sizeof(header) != readen )
    {
        GN_ERROR(sLogger)( "Fail to read file header." );
        return false;
    }

    // swap header to little endian
    header.size1 = SwapEndian8In32( header.size1 );
    header.size2 = SwapEndian8In32( header.size2 );
    header.numObjects = SwapEndian8In32( header.numObjects );

    // read scene data
    size_t dataSize = header.size1 + header.size2 + 12 - sizeof(header);
    xpr.sceneData.resize( dataSize );
    if( !file.read( xpr.sceneData.GetRawPtr(), dataSize, &readen ) || dataSize != readen )
    {
        GN_ERROR(sLogger)( "Fail to read XPR data." );
        return false;
    }

    // iterate all objects
    XPRObjectHeader * objects = (XPRObjectHeader *)xpr.sceneData.GetRawPtr();
    for( size_t i = 0; i < header.numObjects; ++i )
    {
        XPRObjectHeader & o = objects[i];

        // do endian swap
        o.offset = SwapEndian8In32( o.offset );
        o.size   = SwapEndian8In32( o.size );
        o.unknown = SwapEndian8In32( o.unknown );

        size_t offset = o.offset - sizeof(header) + 12;
        void * desc   = &xpr.sceneData[offset];

        switch( o.type.u32 )
        {
            case GN_MAKE_FOURCC( 'V','B','U','F' ):
            {
                XPRVBufDesc * vbdesc = (XPRVBufDesc*)desc;
                if( sizeof(XPRVBufDesc) != o.size )
                {
                    GN_ERROR(sLogger)( "object size is invalid." );
                    return false;
                }
                SwapEndian8In32( vbdesc->dwords, vbdesc->dwords, sizeof(*vbdesc)/4 );
                xpr.vbDescs.append( vbdesc );
                break;
            }

            case GN_MAKE_FOURCC( 'I','B','U','F' ):
            {
                XPRIBufDesc * ibdesc = (XPRIBufDesc*)desc;
                if( sizeof(XPRIBufDesc) != o.size )
                {
                    GN_ERROR(sLogger)( "object size is invalid." );
                    return false;
                }
                SwapEndian8In32( ibdesc->dwords, ibdesc->dwords, sizeof(*ibdesc)/4 );
                xpr.ibDescs.append( ibdesc );
                break;
            }

            case GN_MAKE_FOURCC( 'T','X','2','D' ):
            {
                XPRTex2DDesc * texdesc = (XPRTex2DDesc*)desc;
                if( sizeof(XPRTex2DDesc) == o.size )
                {
                    GN_ERROR(sLogger)( "object size is invalid." );
                    return false;
                }
                SwapEndian8In32( texdesc->dwords, texdesc->dwords, sizeof(*texdesc)/4 );
                xpr.texDescs.append( texdesc );
                break;
            }

            case GN_MAKE_FOURCC( 'U','S','E','R' ):
                // user data block is silently ignored.
                break;

            default:
                GN_WARN(sLogger)( "unknown object type." );
                return false;
        }
    }

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool
sLoadFromXPR( SimpleWorldDesc & desc, File & file )
{
    // load XPR file
    XPRScene xpr;
    if( !loadXprSceneFromFile( xpr, file ) ) return NULL;

    GN_UNUSED_PARAM( desc );
    GN_UNIMPL_WARNING();
    return false;
}

// *****************************************************************************
//
// ASE loader
//
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static bool
sLoadFromASE( SimpleWorldDesc & desc, File & file )
{
    // load ASE scene
    AseScene ase;
    if( !loadAseSceneFromFile( ase, file) ) return false;

    StrA filename = file.name();
    if( filename.Empty() )
    {
        GN_WARN(sLogger)( "Can not get filename" );
        return false;
    }
    filename = fs::resolvePath( fs::getCurrentDir(), filename );

#define FULL_MESH_NAME( n ) (n) // StringFormat("%s.%s",filename.GetRawPtr(),n.GetRawPtr())

    // copy meshes. create entities as well, since in ASE scene, one mesh is one node.
    for( size_t i = 0; i < ase.meshes.size(); ++i )
    {
        const AseMesh & src = ase.meshes[i];

        const StrA    & meshname = FULL_MESH_NAME(src.name);

        // copy mesh
        desc.meshes[meshname] = src;

        // create the entity
        SimpleWorldDesc::EntityDesc & entityDesc = desc.entities[meshname];
        entityDesc.spatial.parent = src.parent.Empty() ? "" : FULL_MESH_NAME(src.parent);
        entityDesc.spatial.position = src.pos;
        entityDesc.spatial.orientation.FromRotation( src.rotaxis, src.rotangle );
        entityDesc.spatial.bbox = src.selfbbox;
    }

    // copy mesh data
    desc.meshdata = ase.meshdata;

    // create models
    for( size_t i = 0; i < ase.subsets.size(); ++i )
    {
        const AseMeshSubset & subset = ase.subsets[i];

        const AseMesh & asemesh = ase.meshes[subset.meshid];

        // determine the model template
        const ModelResourceDesc * modelTemplate = sDetermineBestModelTemplate( asemesh );
        if( NULL == modelTemplate ) continue;

        // initialize the model descriptor based on the template
        ModelResourceDesc model = *modelTemplate;
        model.mesh = FULL_MESH_NAME(asemesh.name);
        model.subset = subset;

        // associate texture to the model
        const AseMaterial & am = ase.materials[subset.matid];
        if( model.hasTexture("ALBEDO_TEXTURE") && !am.mapdiff.bitmap.Empty() )
        {
            model.textures["ALBEDO_TEXTURE"].resourceName = am.mapdiff.bitmap;
        }
        if( model.hasTexture("NORMAL_TEXTURE") && !am.mapbump.bitmap.Empty() )
        {
            model.textures["NORMAL_TEXTURE"].resourceName = am.mapbump.bitmap;
        }

        // add the model to model list
        StrA modelname = StringFormat( "%s.%u", asemesh.name.GetRawPtr(), i );
        GN_ASSERT( desc.models.find( modelname ) == desc.models.end() );
        desc.models[modelname] = model;

        // add the model to appropriate entity
        GN_ASSERT( desc.entities.end() != desc.entities.find(model.mesh) );
        desc.entities[model.mesh].models.append( modelname );
    }

    // setup bounding box of the whole scene
    desc.bbox = ase.bbox;

    return true;
}

// *****************************************************************************
// XML loader
// *****************************************************************************

//
// post error message
// -----------------------------------------------------------------------------
static void sPostXMLError( const XmlNode & node, const StrA & msg )
{
    GN_UNUSED_PARAM( node );
    GN_ERROR(sLogger)( "%s", msg.GetRawPtr() );
}

//
//
// -----------------------------------------------------------------------------
static bool
sParseModel( SimpleWorldDesc & desc, XmlElement & root, const StrA & basedir )
{
    ModelResourceDesc md;

    if( !md.loadFromXml( root, basedir ) ) return false;

    XmlAttrib * modelName = root.findAttrib( "name" );
    if( !modelName || modelName->value.Empty() )
    {
        GN_ERROR(sLogger)( "Model name attribute is missing." );
        return false;
    }

    if( desc.meshes.end() == desc.meshes.find( md.mesh ) )
    {
        MeshResourceDesc mesh;
        AutoRef<Blob> blob = mesh.loadFromFile( fs::resolvePath( basedir, md.mesh ) );
        if( !blob ) return false;

        desc.meshes[md.mesh] = mesh;
        desc.meshdata.append( blob );
    }

    desc.models[modelName->value] = md;

    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool
sParseEntity( SimpleWorldDesc & desc, XmlElement & root )
{
    GN_ASSERT( root.name == "entity" );

    XmlAttrib * entityName = root.findAttrib( "name" );
    if( NULL == entityName )
    {
        sPostXMLError( root, "Entity name attribute is missing." );
        return false;
    }

    SimpleWorldDesc::EntityDesc entity;

    // parse spatial
    XmlElement * spatialNode = root.findChildElement( "spatial" );
    if( !spatialNode )
    {
        sPostXMLError( root, "<spatial> element is missing." );
        return false;
    }
    else
    {
        XmlAttrib * a = spatialNode->findAttrib( "parent" );
        if( a ) entity.spatial.parent = a->value;

        a = spatialNode->findAttrib( "position" );
        if( !a || 3 != String2FloatArray( (float*)&entity.spatial.position, 3, a->value ) )
        {
            sPostXMLError( *spatialNode, "Invalid position" );
            entity.spatial.position.Set( 0, 0, 0 );
        }

        a = spatialNode->findAttrib( "orientation" );
        if( !a || 4 != String2FloatArray( (float*)&entity.spatial.orientation, 4, a->value ) )
        {
            sPostXMLError( *spatialNode, "Invalid orientation" );
            entity.spatial.orientation.Set( 0, 0, 0, 1 );
        }

        a = spatialNode->findAttrib( "bbox" );
        if( !a || 6 != String2FloatArray( (float*)&entity.spatial.bbox, 6, a->value ) )
        {
            sPostXMLError( *spatialNode, "Invalid bounding box" );
            entity.spatial.bbox = Boxf( 0, 0, 0, 0, 0, 0 );
        }
    }

    // parse visual
    XmlElement * visualNode = root.findChildElement( "visual" );
    if( visualNode )
    {
        for( XmlNode * n = visualNode->child; n != NULL; n = n->next )
        {
            XmlElement * e = n->toElement();
            if( !e ) continue;

            if( "model" == e->name )
            {
                XmlAttrib * a = e->findAttrib( "ref" );
                if( !a )
                {
                    sPostXMLError( *e, "ref attribute is missing." );
                    return false;
                }

                entity.models.append( a->value );
            }
            else
            {
                sPostXMLError( *e, StringFormat( "Unknown element: <%s>", e->name.GetRawPtr() ) );
            }
        }
    }

    // done
    desc.entities[entityName->value] = entity;
    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool
sLoadFromXML( SimpleWorldDesc & desc, File & file )
{
    XmlDocument doc;
    XmlParseResult xpr;
    if( !doc.parse( xpr, file ) )
    {
        static Logger * sLogger = GetLogger( "GN.base.xml" );
        GN_ERROR(sLogger)(
            "Fail to parse XML file (%s):\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            file.name(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.GetRawPtr() );
        return false;
    }
    GN_ASSERT( xpr.root );

    StrA basedir = fs::dirName( file.name() );

    XmlElement * root = xpr.root->toElement();
    if( !root || "simpleWorld" != root->name )
    {
        sPostXMLError( *root, "Root element name must be \"<simpleWorld>\"." );
        return false;
    }

    XmlAttrib * bboxAttr = root->findAttrib( "bbox" );
    if( !bboxAttr || 6 != String2FloatArray( (float*)&desc.bbox, 6, bboxAttr->value ) )
    {
        sPostXMLError( *root, "Invalid bbox attribute." );
        return false;
    }

    // parse models
    XmlElement * modelsNode = root->findChildElement( "models" );
    if( NULL == modelsNode )
    {
        sPostXMLError( *root, "Element <models> is missing." );
        return false;
    }
    for( XmlNode * n = modelsNode->child; n != NULL; n = n->next )
    {
        XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "model" == e->name )
        {
            if( !sParseModel( desc, *e, basedir ) ) return false;
        }
        else
        {
            sPostXMLError( *e, StringFormat( "Ignore unknowned element: <%s>", e->name.GetRawPtr() ) );
        }
    }

    // parse entities
    XmlElement * entitiesNode = root->findChildElement( "entities" );
    if( NULL == entitiesNode )
    {
        sPostXMLError( *root, "Element <entities> is missing." );
        return false;
    }
    for( XmlNode * n = entitiesNode->child; n != NULL; n = n->next )
    {
        XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "entity" == e->name )
        {
            if( !sParseEntity( desc, *e ) ) return false;
        }
        else
        {
            sPostXMLError( *e, StringFormat( "Ignore unknowned element: <%s>", e->name.GetRawPtr() ) );
        }
    }

    // done
    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool
sSaveToXML( const SimpleWorldDesc & desc, const char * filename )
{
    // check dirname
    if( NULL == filename )
    {
        GN_ERROR(sLogger)( "NULL directory name" );
        return false;
    }

    // convert to full path
    StrA fullpath = fs::resolvePath( fs::getCurrentDir(), filename );
    filename = fullpath;
    StrA dirname = fs::dirName( fullpath );
    StrA basename = fs::baseName( fullpath );

    if( !fs::isDir( dirname ) )
    {
        GN_ERROR(sLogger)( "%s is not a directory", dirname.GetRawPtr() );
        return false;
    }

    // write meshes
    int meshindex = 0;
    std::map<StrA,StrA> meshNameMapping;
    for( std::map<StrA,MeshResourceDesc>::const_iterator i = desc.meshes.begin();
        i != desc.meshes.end();
        ++i )
    {
        const StrA & oldMeshName = i->first;
        const MeshResourceDesc & mesh = i->second;

        StrA newMeshName = StringFormat( "%s.%d.mesh.bin", basename.GetRawPtr(), meshindex );

        if( !mesh.saveToFile( dirname + "\\" + newMeshName ) ) return false;

        meshNameMapping[oldMeshName] = newMeshName;

        ++meshindex;
    }

    // create a new XML document
    XmlDocument xmldoc;
    XmlElement * root = xmldoc.createElement(NULL);
    root->name = "simpleWorld";

    // write models
    XmlElement * models = xmldoc.createElement( root );
    models->name = "models";
    for( std::map<StrA,gfx::ModelResourceDesc>::const_iterator i = desc.models.begin();
         i != desc.models.end();
         ++i )
    {
        const StrA & modelName  = i->first;
        ModelResourceDesc model = i->second;

        std::map<StrA,StrA>::iterator iter = meshNameMapping.find( model.mesh );
        if( iter != meshNameMapping.end() )
        {
            model.mesh = iter->second;
        }

        XmlElement * modelNode = model.saveToXml( *models, dirname );
        if( !modelNode ) return false;

        XmlAttrib * a = xmldoc.createAttrib( modelNode );
        a->name = "name";
        a->value = modelName;
    }

    // rename entities
    int entityIndex = 0;
    std::map<StrA,StrA> entityNameMap;
    for( std::map<StrA,SimpleWorldDesc::EntityDesc>::const_iterator i = desc.entities.begin();
        i != desc.entities.end();
        ++i )
    {
        const StrA & entityName = i->first;

        entityNameMap[entityName] = StringFormat( "%d", ++entityIndex );
    }

    // write entities
    XmlElement * entities = xmldoc.createElement( root );
    entities->name = "entities";
    for( std::map<StrA,SimpleWorldDesc::EntityDesc>::const_iterator i = desc.entities.begin();
        i != desc.entities.end();
        ++i )
    {
        const StrA                        & entityName = entityNameMap.find(i->first)->second;
        const SimpleWorldDesc::EntityDesc & entityDesc = i->second;

        XmlElement * entity = xmldoc.createElement( entities );
        entity->name = "entity";

        XmlAttrib * a = xmldoc.createAttrib( entity );
        a->name  = "name";
        a->value = entityName;

        XmlElement * spatial = xmldoc.createElement( entity );
        spatial->name = "spatial";

        a = xmldoc.createAttrib( spatial );
        a->name  = "parent";
        std::map<StrA,StrA>::iterator parentIter = entityNameMap.find(entityDesc.spatial.parent);
        if( entityNameMap.end() != parentIter )
        {
            a->value = parentIter->second;
        }
        else if( !entityDesc.spatial.parent.Empty() )
        {
            GN_WARN(sLogger)( "Entity %s has invalid parent: %s", i->first.GetRawPtr(), entityDesc.spatial.parent.GetRawPtr() );
        }

        a = xmldoc.createAttrib( spatial );
        a->name  = "position";
        a->value = StringFormat( "%f,%f,%f",
            entityDesc.spatial.position.x,
            entityDesc.spatial.position.y,
            entityDesc.spatial.position.z );

        a = xmldoc.createAttrib( spatial );
        a->name  = "orientation";
        a->value = StringFormat( "%f,%f,%f,%f",
            entityDesc.spatial.orientation.v.x,
            entityDesc.spatial.orientation.v.y,
            entityDesc.spatial.orientation.v.x,
            entityDesc.spatial.orientation.w );

        a = xmldoc.createAttrib( spatial );
        a->name  = "bbox";
        a->value = StringFormat( "%f,%f,%f,%f,%f,%f",
            entityDesc.spatial.bbox.x,
            entityDesc.spatial.bbox.y,
            entityDesc.spatial.bbox.z,
            entityDesc.spatial.bbox.w,
            entityDesc.spatial.bbox.h,
            entityDesc.spatial.bbox.d );

        XmlElement * visual = xmldoc.createElement( entity );
        visual->name  = "visual";
        for( size_t i = 0; i < entityDesc.models.size(); ++i )
        {
            XmlElement * modelref = xmldoc.createElement( visual );
            modelref->name = "model";

            XmlAttrib * a = xmldoc.createAttrib( modelref );
            a->name = "ref";
            a->value = entityDesc.models[i];
        }
    }

    // write scene bounding box
    XmlAttrib * a = xmldoc.createAttrib( root->toElement() );
    a->name  = "bbox";
    a->value = StringFormat( "%f,%f,%f,%f,%f,%f",
            desc.bbox.x,
            desc.bbox.y,
            desc.bbox.z,
            desc.bbox.w,
            desc.bbox.h,
            desc.bbox.d );

    // write XML document
    AutoObjPtr<File> fp( fs::openFile( filename, "wt" ) );
    if( !fp ) return false;
    return xmldoc.writeToFile( *fp, *root, false );
}

// *****************************************************************************
// Load from garnet mesh binary
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool sLoadFromMeshBinary( SimpleWorldDesc & desc, File & fp )
{
    desc.Clear();

    const StrA & meshname = fp.name();

    MeshResourceDesc mesh;
    AutoRef<Blob> blob = mesh.loadFromFile( fp );
    if( !blob ) return false;

    // determine the model template
    const ModelResourceDesc * modelTemplate = sDetermineBestModelTemplate( mesh );
    if( NULL == modelTemplate ) return false;

    // initialize the model descriptor based on the template
    ModelResourceDesc model = *modelTemplate;
    model.mesh = meshname;

    // add mesh and model to scene
    desc.meshes[meshname] = mesh;
    desc.meshdata.append( blob );
    desc.models[meshname] = model;

    // create a entity for the model
    SimpleWorldDesc::EntityDesc & ed = desc.entities[meshname];
    ed.spatial.position.Set( 0, 0, 0 );
    ed.spatial.orientation.Set( 0, 0, 0, 1 );
    mesh.CalculateBoundingBox( ed.spatial.bbox );
    ed.models.append( meshname );

    // done
    desc.bbox = ed.spatial.bbox;
    return true;
}

// *****************************************************************************
// Common local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static Entity * sPopulateEntity( World & world, Entity * root, const SimpleWorldDesc & desc, const StrA & entityName )
{
    GN_ASSERT( desc.entities.end() != desc.entities.find( entityName ) );

    const SimpleWorldDesc::EntityDesc & entityDesc = desc.entities.find(entityName)->second;

    // recursively populate parent entities
    Entity * parent = NULL;
    if( !entityDesc.spatial.parent.Empty() )
    {
        if( desc.entities.end() == desc.entities.find( entityDesc.spatial.parent ) )
        {
            GN_ERROR(sLogger)( "Entity '%s' has a invalid parent: '%s'", entityName.GetRawPtr(), entityDesc.spatial.parent.GetRawPtr() );
        }
        else
        {
            parent = sPopulateEntity( world, root, desc, entityDesc.spatial.parent );
        }
    }

    // create a new entity instance
    Entity * e = entityDesc.models.empty() ? world.createSpatialEntity( entityName ) : world.createVisualEntity( entityName );;
    if( !e ) return NULL;

    // attach the entity to parent node or root node
    e->getNode<SpatialNode>()->setParent( parent ? parent->getNode<SpatialNode>() : root->getNode<SpatialNode>() );

    // calculate bounding sphere
    const Boxf & bbox = entityDesc.spatial.bbox;
    Spheref bs;
    CalculateBoundingSphereFromBoundingBox( bs, bbox );
    e->getNode<SpatialNode>()->setBoundingSphere( bs );

    for( size_t i = 0; i < entityDesc.models.size(); ++i )
    {
        const StrA & modelName = entityDesc.models[i];

        std::map<StrA,gfx::ModelResourceDesc>::const_iterator modelIter;
        modelIter = desc.models.find( modelName );
        if( modelIter == desc.models.end() )
        {
            GN_ERROR(sLogger)(
                "Entity %s references invalid model named \"%s\".",
                entityName.GetRawPtr(),
                modelName.GetRawPtr() );
            continue;
        }

        const ModelResourceDesc & modelDesc = modelIter->second;

        // this variable is used to keep a reference to mesh resource,
        // to prevent it from being deleted, until the model is created.
        AutoRef<MeshResource> mesh;

        if( !modelDesc.mesh.Empty() )
        {
            mesh = world.gdb().findResource<MeshResource>( modelDesc.mesh );
            if( !mesh )
            {
                std::map<StrA,gfx::MeshResourceDesc>::const_iterator meshIter = desc.meshes.find(modelDesc.mesh);

                if( desc.meshes.end() == meshIter )
                {
                    GN_ERROR(sLogger)(
                        "Model \"%s\" references a mesh \"%s\" that does not belong to this scene.",
                        modelName.GetRawPtr(),
                        modelDesc.mesh.GetRawPtr() );
                    continue; // ignore the model
                }

                const MeshResourceDesc & meshDesc = meshIter->second;

                // create new mesh
                mesh = world.gdb().createResource<MeshResource>( modelDesc.mesh );
                if( !mesh || !mesh->reset( &meshDesc ) ) continue;
            }
        }

        AutoRef<ModelResource> model = world.gdb().createResource<ModelResource>( NULL );
        if( !model->reset( &modelDesc ) ) continue;

        e->getNode<VisualNode>()->addModel( model );
    }

    return e;
}

//
// Check if the string is end with specific suffix
// -----------------------------------------------------------------------------
static bool sStrEndWithI( const char * string, const char * suffix )
{
    if( NULL == suffix ) return true;
    if( NULL == string ) return false;

    size_t n1 = strlen( string );
    size_t n2 = strlen( suffix );

    if( n1 < n2 ) return false;

    string = string + n1 - n2;

    return 0 == StringCompareI( string, suffix );
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleWorldDesc::Clear()
{
    meshes.clear();
    meshdata.Clear();
    models.clear();
    entities.clear();
}

//
//
// -----------------------------------------------------------------------------
bool GN::util::SimpleWorldDesc::loadFromFile( const char * filename )
{
    GN_SCOPE_PROFILER( loadWorldFromFile, "Load simple world from file" );

    GN_INFO(sLogger)( "Load scene from file: %s", filename?filename:"<NULL>" );

    Clear();

    // open file
    AutoObjPtr<File> fp( fs::openFile( filename, "rb" ) );
    if( !fp ) return false;

    // get file extension
    StrA ext = fs::extName( filename );

    // do loading
    if( sStrEndWithI( filename, ".xml" ) )
    {
        return sLoadFromXML( *this, *fp );
    }
    else if( sStrEndWithI( filename, ".ase" ) )
    {
        return sLoadFromASE( *this, *fp );
    }
    else if( sStrEndWithI( filename, ".xpr" ) ||
             sStrEndWithI( filename, ".tpr" ) )
    {
        return sLoadFromXPR( *this, *fp );
    }
    else if( sStrEndWithI( filename, ".mesh.bin" ) )
    {
        return sLoadFromMeshBinary( *this, *fp );
    }
    else
    {
        GN_ERROR(sLogger)( "Unknown file extension: %s", ext.GetRawPtr() );
        return false;
    }
}

///
/// write world description to file
// -----------------------------------------------------------------------------
bool GN::util::SimpleWorldDesc::saveToFile( const char * filename )
{
    GN_SCOPE_PROFILER( profiler, "Save simple world to file" );

    GN_INFO(sLogger)( "Write scene to : %s", filename );

    return sSaveToXML( *this, filename );
}

//
//
// -----------------------------------------------------------------------------
Entity * GN::util::SimpleWorldDesc::populateTheWorld( World & world ) const
{
    // create root spatial entity of the whole world
    Entity * root = world.createSpatialEntity( NULL );
    if( NULL == root ) return NULL;

    for( std::map<StrA,EntityDesc>::const_iterator i = entities.begin();
         i != entities.end();
         ++i )
    {
        const StrA & entityName = i->first;

        if( !world.findEntity( entityName ) )
        {
            sPopulateEntity( world, root, *this, entityName );
        }
        else
        {
            // The entity has been initialized, just skip it
        }
    }

    return root;
}
