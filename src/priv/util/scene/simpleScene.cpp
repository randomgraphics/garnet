#include "pch.h"
#include "garnet/GNutil.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.util");

// *****************************************************************************
// Local stuff
// *****************************************************************************

static bool sHasSemantic( const VertexFormat & vf, const char * binding, size_t index )
{
    for( size_t i = 0; i < vf.numElements; ++i )
    {
        if( 0 == strCmpI( vf.elements[i].binding, binding ) &&
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
    header.size1 = swap8in32( header.size1 );
    header.size2 = swap8in32( header.size2 );
    header.numObjects = swap8in32( header.numObjects );

    // read scene data
    size_t dataSize = header.size1 + header.size2 + 12 - sizeof(header);
    xpr.sceneData.resize( dataSize );
    if( !file.read( xpr.sceneData.cptr(), dataSize, &readen ) || dataSize != readen )
    {
        GN_ERROR(sLogger)( "Fail to read XPR data." );
        return false;
    }

    // iterate all objects
    XPRObjectHeader * objects = (XPRObjectHeader *)xpr.sceneData.cptr();
    for( size_t i = 0; i < header.numObjects; ++i )
    {
        XPRObjectHeader & o = objects[i];

        // do endian swap
        o.offset = swap8in32( o.offset );
        o.size   = swap8in32( o.size );
        o.unknown = swap8in32( o.unknown );

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
                swap8in32( vbdesc->dwords, vbdesc->dwords, sizeof(*vbdesc)/4 );
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
                swap8in32( ibdesc->dwords, ibdesc->dwords, sizeof(*ibdesc)/4 );
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
                swap8in32( texdesc->dwords, texdesc->dwords, sizeof(*texdesc)/4 );
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
    if( filename.empty() )
    {
        GN_WARN(sLogger)( "Can not get filename" );
        return false;
    }
    filename = fs::resolvePath( fs::getCurrentDir(), filename );

#define FULL_MESH_NAME( n ) strFormat("%s.%s",filename.cptr(),n.cptr())

    // copy meshes. create entities as well, since in ASE scene, one mesh is one node.
    for( size_t i = 0; i < ase.meshes.size(); ++i )
    {
        const AseMesh    & src = ase.meshes[i];

        const StrA       & meshname = FULL_MESH_NAME(src.name);

        MeshResourceDesc & dst = desc.meshes[meshname];

        dst = src;

        for( size_t i = 0; i < GpuContext::MAX_VERTEX_BUFFERS; ++i )
        {
            size_t vbsize = src.getVtxBufSize( i );
            if( vbsize > 0 )
            {
                desc.meshdata.resize( desc.meshdata.size() + 1 );
                desc.meshdata.back().size = vbsize;
                desc.meshdata.back().data = heapAlloc( vbsize );
                if( !desc.meshdata.back().data )
                {
                    GN_ERROR(sLogger)( "Out of memory." );
                    return false;
                }
                memcpy( desc.meshdata.back().data, src.vertices[i], vbsize );
                dst.vertices[i] = desc.meshdata.back().data;
            }
            else
            {
                dst.vertices[i] = 0;
            }
        }

        size_t ibsize = src.getIdxBufSize();
        if( ibsize > 0 )
        {
            desc.meshdata.resize( desc.meshdata.size() + 1 );
            desc.meshdata.back().size = ibsize;
            desc.meshdata.back().data = heapAlloc( ibsize );
            if( !desc.meshdata.back().data )
            {
                GN_ERROR(sLogger)( "Out of memory." );
                return false;
            }
            memcpy( desc.meshdata.back().data, src.indices, ibsize );
            dst.indices = desc.meshdata.back().data;
        }
        else
        {
            dst.indices = 0;
        }

        // create the entity
        SimpleWorldDesc::EntityDesc & entityDesc = desc.entities[meshname];
        entityDesc.spatial.parent = src.parent.empty() ? "" : FULL_MESH_NAME(src.parent);
        entityDesc.spatial.position = src.pos;
        entityDesc.spatial.orientation.fromRotation( src.rotaxis, src.rotangle );
        entityDesc.spatial.bbox = src.selfbbox;
    }

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
        model.meshResourceName = FULL_MESH_NAME(asemesh.name);
        model.subset = subset;

        // associate texture to the model
        const AseMaterial & am = ase.materials[subset.matid];
        if( model.hasTexture("ALBEDO_TEXTURE") && !am.mapdiff.bitmap.empty() )
        {
            model.textures["ALBEDO_TEXTURE"].resourceName = am.mapdiff.bitmap;
        }
        if( model.hasTexture("NORMAL_TEXTURE") && !am.mapbump.bitmap.empty() )
        {
            model.textures["NORMAL_TEXTURE"].resourceName = am.mapbump.bitmap;
        }

        // add the model to model list
        desc.models.append( model );

        // add the model to appropriate entity
        GN_ASSERT( desc.entities.end() != desc.entities.find(model.meshResourceName) );
        desc.entities[model.meshResourceName].models.append( desc.models.size() - 1 );
    }

    // setup bounding box of the whole scene
    desc.bbox = ase.bbox;

    return true;
}

// *****************************************************************************
// XML loader
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static bool
sLoadFromXML( SimpleWorldDesc & desc, File & file )
{
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
    return t.loadFromXmlNode( *xpr.root, basedir );
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

    if( !fs::isDir( dirname ) )
    {
        GN_ERROR(sLogger)( "%s is not a directory", dirname.cptr() );
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

        StrA newMeshName = strFormat( "%d.mesh.bin", meshindex );

        if( !mesh.saveToFile( dirname + "\\" + newMeshName ) ) return false;

        meshNameMapping[oldMeshName] = newMeshName;

        ++meshindex;
    }

    // create a new XML document
    XmlDocument xmldoc;
    XmlNode * root = xmldoc.createNode(XML_ELEMENT, NULL);
    root->toElement()->name = "simpleWorld";

    // write models
    XmlElement * models = xmldoc.createNode( XML_ELEMENT, root )->toElement();
    models->name = "models";
    for( size_t i = 0; i < desc.models.size(); ++i )
    {
        ModelResourceDesc model = desc.models[i];

        std::map<StrA,StrA>::iterator iter = meshNameMapping.find( model.meshResourceName );
        if( iter != meshNameMapping.end() )
        {
            model.meshResourceName = iter->second;
        }

        if( !model.saveToXmlNode( *models, dirname ) ) return false;
    }

    // rename entities
    int entityIndex = 0;
    std::map<StrA,StrA> entityNameMap;
    for( std::map<StrA,EntityDesc>::const_iterator i = desc.entities.begin();
        i != desc.entities.end();
        ++i )
    {
        const StrA & entityName = i->first;

        entityNameMap[entityName] = strFormat( "%d", ++entityIndex );
    }

    // write entities
    XmlElement * entities = xmldoc.createNode( XML_ELEMENT, root )->toElement();
    entities->name = "entities";
    for( std::map<StrA,EntityDesc>::const_iterator i = desc.entities.begin();
        i != desc.entities.end();
        ++i )
    {
        const StrA       & entityName = entityNameMap.find(i->first)->second;
        const EntityDesc & entityDesc = i->second;

        XmlElement * entity = xmldoc.createNode( XML_ELEMENT, entities )->toElement();
        entity->name = "entity";

        XmlAttrib * a = xmldoc.createAttrib( entity );
        a->name  = "name";
        a->value = entityName;

        XmlElement * spatial = xmldoc.createNode( XML_ELEMENT, entity )->toElement();
        spatial->name = "spatial";

        a = xmldoc.createAttrib( spatial );
        a->name  = "parent";
        std::map<StrA,StrA>::iterator parentIter = entityNameMap.find(entityDesc.spatial.parent);
        if( entityNameMap.end() != parentIter )
        {
            a->value = parentIter->second;
        }
        else if( !entityDesc.spatial.parent.empty() )
        {
            GN_WARN(sLogger)( "Entity %s has invalid parent: %s", i->first.cptr(), entityDesc.spatial.parent.cptr() );
        }

        a = xmldoc.createAttrib( spatial );
        a->name  = "position";
        a->value = strFormat( "%f,%f,%f",
            entityDesc.spatial.position.x,
            entityDesc.spatial.position.y,
            entityDesc.spatial.position.z );

        a = xmldoc.createAttrib( spatial );
        a->name  = "orientation";
        a->value = strFormat( "%f,%f,%f,%f",
            entityDesc.spatial.orientation.v.x,
            entityDesc.spatial.orientation.v.y,
            entityDesc.spatial.orientation.v.x,
            entityDesc.spatial.orientation.w );

        a = xmldoc.createAttrib( spatial );
        a->name  = "bbox";
        a->value = strFormat( "%f,%f,%f,%f,%f,%f",
            entityDesc.spatial.bbox.x,
            entityDesc.spatial.bbox.y,
            entityDesc.spatial.bbox.z,
            entityDesc.spatial.bbox.w,
            entityDesc.spatial.bbox.h,
            entityDesc.spatial.bbox.d );

        XmlElement * visual = xmldoc.createNode( XML_ELEMENT, entity )->toElement();
        visual->name  = "visual";
        for( size_t i = 0; i < entityDesc.models.size(); ++i )
        {
            XmlElement * modelref = xmldoc.createNode( XML_ELEMENT, visual )->toElement();
            modelref->name = "model";

            XmlAttrib * a = xmldoc.createAttrib( modelref );
            a->name = "ref";
            a->value = strFormat( "%u", entityDesc.models[i] );
        }
    }

    // write XML document
    AutoObjPtr<File> fp( fs::openFile( filename, "wt" ) );
    if( !fp ) return false;
    return xmldoc.writeToFile( *fp, *root, false );
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
    if( !entityDesc.spatial.parent.empty() )
    {
        if( desc.entities.end() == desc.entities.find( entityDesc.spatial.parent ) )
        {
            GN_ERROR(sLogger)( "Entity '%s' has a invalid parent: '%s'", entityName.cptr(), entityDesc.spatial.parent.cptr() );
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
    calculateBoundingSphereFromBoundingBox( bs, bbox );
    e->getNode<SpatialNode>()->setBoundingSphere( bs );

    if( !entityDesc.models.empty() )
    {
        for( size_t i = 0; i < entityDesc.models.size(); ++i )
        {
            const ModelResourceDesc & modelDesc = desc.models[entityDesc.models[i]];

            // this variable is used to keep a reference to mesh resource,
            // to prevent it from being deleted, until the model is created.
            AutoRef<MeshResource> mesh;

            if( !modelDesc.meshResourceName.empty() )
            {
                mesh = world.gdb().findResource<MeshResource>( modelDesc.meshResourceName );
                if( !mesh )
                {
                    std::map<StrA,gfx::MeshResourceDesc>::const_iterator meshIter = desc.meshes.find(modelDesc.meshResourceName);

                    if( desc.meshes.end() == meshIter )
                    {
                        GN_ERROR(sLogger)(
                            "Model %d references a mesh '%s' that does not belong to this scene.",
                            i,
                            modelDesc.meshResourceName.cptr() );
                        continue; // ignore the model
                    }

                    const MeshResourceDesc & meshDesc = meshIter->second;

                    // create new mesh
                    mesh = world.gdb().createResource<MeshResource>( modelDesc.meshResourceName );
                    if( !mesh || !mesh->reset( &meshDesc ) ) continue;
                }
            }

            AutoRef<ModelResource> model = world.gdb().createResource<ModelResource>( NULL );
            if( !model->reset( &modelDesc ) ) continue;

            e->getNode<VisualNode>()->addModel( model );
        }
    }

    return e;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::util::SimpleWorldDesc::clear()
{
    for( size_t i = 0; i < meshdata.size(); ++i )
    {
        safeHeapFree( meshdata[i].data );
    }
    meshdata.clear();

    meshes.clear();
    models.clear();
    entities.clear();
}

//
//
// -----------------------------------------------------------------------------
bool GN::util::SimpleWorldDesc::loadFromFile( const char * filename )
{
    GN_SCOPE_PROFILER( loadWorldFromFile, "Load simple world from file" );

    clear();

    // open file
    AutoObjPtr<File> fp( fs::openFile( filename, "rb" ) );
    if( !fp ) return false;

    // get file extension
    StrA ext = fs::extName( filename );

    // do loading
    if( 0 == strCmpI( ".xml", ext.cptr() ) )
    {
        return sLoadFromXML( *this, *fp );
    }
    else if( 0 == strCmpI( ".ase", ext.cptr() ) )
    {
        return sLoadFromASE( *this, *fp );
    }
    else if( 0 == strCmpI( ".xpr", ext.cptr() ) ||
             0 == strCmpI( ".tpr", ext.cptr() ))
    {
        return sLoadFromXPR( *this, *fp );
    }
    else
    {
        GN_ERROR(sLogger)( "Unknown file extension: %s", ext.cptr() );
        return false;
    }
}

///
/// write world description to file
// -----------------------------------------------------------------------------
bool GN::util::SimpleWorldDesc::saveToFile( const char * filename )
{
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
