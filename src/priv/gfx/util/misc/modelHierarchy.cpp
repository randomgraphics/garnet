#include "pch.h"

#if GN_MSVC
#pragma warning(disable:4100) // unreferenced formal parameter
#endif
#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>       // Output data structure
#include <assimp/aiPostProcess.h> // Post processing flags
#include <assimp/IOStream.h>
#include <assimp/IOSystem.h>

#ifdef HAS_FBX
#include <fbxsdk.h>
#include <fbxfilesdk/kfbxio/kfbximporter.h>
#include <fbxfilesdk/fbxfilesdk_nsuse.h>
#include <fbxfilesdk/kfbxplugins/kfbxgeometryconverter.h>
#endif

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.misc");

#define ROOT_BBOX 1

// *****************************************************************************
// Local stuff
// *****************************************************************************

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

    return 0 == stringCompareI( string, suffix );
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

namespace xpr
{

struct XPRFileHeader
{
    FOURCC tag;        ///< must be XPR2
    uint32 size1;      ///< size tag 1
    uint32 size2;      ///< size tag 2 (file size = size1+size2+12)
    uint32 numObjects; ///< number of objects in this file
};

struct XPRObjectHeader
{
    FOURCC type;    ///< object type, could be "USER", "TX2D", "VBUF", "IBUF".
    uint32 offset;  ///< object offset in bytes. The actual offset is this value + 12.
    uint32 size;    ///< object size in bytes
    uint32 unknown; ///< I don't know what this is for.
};

// XPR texture descriptor, 0x28 bytes
struct XPRTex2DDesc
{
    // 10 dwords
    uint32 dwords[10];
};
GN_CASSERT( 0x28 == sizeof(XPRTex2DDesc) );

/// XPR vertex buffer descriptor, 0x14 bytes
struct XPRVBufDesc
{
    uint32 dwords[5];
};
GN_CASSERT( 0x14 == sizeof(XPRVBufDesc) );

/// XPR index buffer descriptor, 0x14 bytes
struct XPRIBufDesc
{
    uint32 dwords[5];
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
sLoadXprSceneFromFile( XPRScene & xpr, File & file )
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
sLoadModelHierarchyFromXPR( ModelHierarchyDesc & desc, File & file )
{
    // load XPR file
    XPRScene xpr;
    if( !sLoadXprSceneFromFile( xpr, file ) ) return NULL;

    GN_UNUSED_PARAM( desc );
    GN_UNIMPL_WARNING();
    return false;
}

}

// *****************************************************************************
//
// ASE loader
//
// *****************************************************************************

namespace ase
{

//
//
// -----------------------------------------------------------------------------
static bool sHasPosition( const MeshVertexFormat & vf )
{
    return vf.hasSemantic( "position" )
        || vf.hasSemantic( "pos" );
}

//
//
// -----------------------------------------------------------------------------
static bool sHasNormal( const MeshVertexFormat & vf )
{
    return vf.hasSemantic( "normal" );
}

//
//
// -----------------------------------------------------------------------------
static bool sHasTex0( const MeshVertexFormat & vf )
{
    return vf.hasSemantic( "texcoord" );
}

//
//
// -----------------------------------------------------------------------------
static bool sHasTangent( const MeshVertexFormat & vf )
{
    return vf.hasSemantic( "tangent" );
}

///
/// Determine the best model template that can show the mesh, return NULL for failure
// -----------------------------------------------------------------------------
static const ModelResourceDesc * sDetermineBestModelTemplate( const MeshResourceDesc & m )
{
    const MeshVertexFormat & vf = m.vtxfmt;

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

//
//
// -----------------------------------------------------------------------------
static bool
sLoadModelHierarchyFromASE( ModelHierarchyDesc & desc, File & file )
{
    // load ASE scene
    AseScene ase;
    if( !ase.loadFromFile(file) ) return false;

    StrA filename = file.name();
    if( filename.empty() )
    {
        GN_WARN(sLogger)( "Can not get filename" );
        return false;
    }
    filename = fs::resolvePath( fs::getCurrentDir(), filename );

#define FULL_MESH_NAME( n ) stringFormat("%s.%s",filename.cptr(),n.cptr())

    // copy meshes. create nodes as well, since in ASE scene, one mesh is one node.
    for( size_t i = 0; i < ase.meshes.size(); ++i )
    {
        const AseMesh & src = ase.meshes[i];

        const StrA    & meshname = FULL_MESH_NAME(src.name);

        // copy mesh
        desc.meshes[meshname] = src;

        // create the node
        ModelHierarchyDesc::NodeDesc & nodeDesc = desc.nodes[meshname];
#if 0
        nodeDesc.parent = src.parent.empty() ? "" : FULL_MESH_NAME(src.parent);
        nodeDesc.position = src.pos;
        nodeDesc.orientation.fromRotation( src.rotaxis, src.rotangle );
#else
        // Note: Ingore hierarchy properties in ASE, since models in ASE have already in "world" space.
        nodeDesc.parent = "";
        nodeDesc.position.set( 0, 0, 0 );
        nodeDesc.orientation.identity();
        nodeDesc.scaling.set( 1, 1, 1 );
#endif
        nodeDesc.bbox = src.selfbbox;
    }

    // copy mesh data
    desc.meshdata = ase.meshdata;

    size_t totalVerts = 0;
    size_t totalFaces = 0;

    // create models
    for( size_t i = 0; i < ase.subsets.size(); ++i )
    {
        const AseMeshSubset & subset = ase.subsets[i];

        totalVerts += subset.numvtx;
        totalFaces += subset.numidx / 3;

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
        if( model.hasTexture("ALBEDO_TEXTURE") && !am.mapdiff.bitmap.empty() )
        {
            model.textures["ALBEDO_TEXTURE"].resourceName = am.mapdiff.bitmap;
        }
        if( model.hasTexture("NORMAL_TEXTURE") && !am.mapbump.bitmap.empty() )
        {
            model.textures["NORMAL_TEXTURE"].resourceName = am.mapbump.bitmap;
        }

        // add the model to model list
        StrA modelname = stringFormat( "%s.%u", asemesh.name.cptr(), i );
        GN_ASSERT( NULL == desc.models.find( modelname ) );
        desc.models[modelname] = model;

        // add the model to appropriate node
        GN_ASSERT( NULL != desc.nodes.find( model.mesh ) );
        desc.nodes[model.mesh].models.append( modelname );
    }

#if ROOT_BBOX
    // setup bounding box of the whole scene
    desc.bbox = ase.bbox;
#endif

    GN_INFO(sLogger)( "Total vertices: %d, faces: %d", totalVerts, totalFaces );

    return true;
}

}

// *****************************************************************************
//
// FBX loader
//
// *****************************************************************************

namespace fbx
{
#ifdef HAS_FBX

class FbxSdkWrapper
{
public:

    KFbxSdkManager * manager;
    KFbxGeometryConverter * converter;

    FbxSdkWrapper() : manager(NULL), converter(NULL)
    {

    }

    ~FbxSdkWrapper()
    {
        delete converter;
        converter = NULL;

        // Delete the FBX SDK manager. All the objects that have been allocated
        // using the FBX SDK manager and that haven't been explicitly destroyed
        // are automatically destroyed at the same time.
        if (manager) manager->Destroy();
        manager = NULL;
    }

    bool init()
    {
        manager = KFbxSdkManager::Create();
        if( NULL == manager ) return false;

    	// create an IOSettings object
    	KFbxIOSettings * ios = KFbxIOSettings::Create( manager, IOSROOT );
    	manager->SetIOSettings(ios);

        // create a converter
        converter = new KFbxGeometryConverter(manager);

    	// Load plugins from the executable directory
    	KString lPath = KFbxGetApplicationDirectory();
#if defined(KARCH_ENV_WIN)
    	KString lExtension = "dll";
#elif defined(KARCH_ENV_MACOSX)
    	KString lExtension = "dylib";
#elif defined(KARCH_ENV_LINUX)
    	KString lExtension = "so";
#endif
    	manager->LoadPluginsDirectory(lPath.Buffer(), lExtension.Buffer());

        return true;
    }

};

//
//
// -----------------------------------------------------------------------------
static void
sLoadFbxMesh(
    ModelHierarchyDesc           & desc,
    ModelHierarchyDesc::NodeDesc & gnnode,
    FbxSdkWrapper                & sdk,
    KFbxMesh                     * mesh,
    const char *                   meshName )
{
    if( !mesh->IsTriangleMesh() )
    {
        mesh = sdk.converter->TriangulateMesh( mesh );
        if( NULL == mesh )
        {
            GN_ERROR(sLogger)( "Fail to triangulate mesh node: %s", meshName );
            return;
        }
    }

    // TODO: prefix mesh meshName with file meshName

    // load mesh
    MeshResourceDesc  & gnmesh  = desc.meshes[meshName];
    gnmesh.prim = PrimitiveType::TRIANGLE_LIST;
    gnmesh.numvtx = (size_t)mesh->GetControlPointsCount();
    gnmesh.numidx = (size_t)mesh->GetPolygonCount() * 3;
    gnmesh.idx32  = gnmesh.numidx > 0x10000;
    gnmesh.vtxfmt = MeshVertexFormat::XYZ();
    gnmesh.strides[0] = sizeof(Vector4f);
    gnmesh.offsets[0] = 0;

    // read vertices
    AutoRef<Blob> blob( new SimpleBlob( gnmesh.numvtx * gnmesh.strides[0] ) );
    KFbxVector4 * fbxverts = mesh->GetControlPoints();
    Vector4f * vertices = (Vector4f*)blob->data();
    for( size_t i = 0; i < gnmesh.numvtx; ++i )
    {
        const KFbxVector4 & v = fbxverts[i];
        vertices[i].set( (float)v[0], (float)v[1], (float)v[2], 0 );
    }
    gnmesh.vertices[0] = vertices;
    desc.meshdata.append( blob );

    // read polygons
    if( gnmesh.idx32 )
    {
        blob.attach( new SimpleBlob( 4 * gnmesh.numidx ) );
        uint32 * indices = (uint32*)blob->data();
        int * fbxindices = mesh->GetPolygonVertices();
        memcpy( indices, fbxindices, blob->size() );
    }
    else
    {
        blob.attach( new SimpleBlob( 2 * gnmesh.numidx ) );
        uint16 * indices = (uint16*)blob->data();
        int * fbxindices = mesh->GetPolygonVertices();
        for( size_t i = 0; i < gnmesh.numidx; ++i )
        {
            indices[i] = (uint16)fbxindices[i];
        }
    }
    gnmesh.indices = blob->data();
    desc.meshdata.append( blob );

    ModelResourceDesc & gnmodel = desc.models[meshName];
    gnmodel = SimpleWireframeModel::DESC;
    gnmodel.mesh = meshName;
    gnmodel.subset.clear();

    // update node
    vertices = (Vector4f*)gnmesh.vertices[0];
    calculateBoundingBox(
        gnnode.bbox,
        &vertices->x, sizeof(Vector4f),
        &vertices->y, sizeof(Vector4f),
        &vertices->z, sizeof(Vector4f),
        gnmesh.numvtx );
    gnnode.models.append( meshName );
}

//
//
// -----------------------------------------------------------------------------
static bool
sLoadFbxNodeRecursivly(
    ModelHierarchyDesc & desc,
    FbxSdkWrapper      & sdk,
    KFbxNode           * node,
    KFbxNode           * parent )
{
    if( NULL == node ) return true;

    // the node name should be unique
    // TODO: if the name is not unique, make it unique.
    const char * name = node->GetName();
    if( desc.nodes.find(name) )
    {
        GN_VERBOSE(sLogger)( "Node named %s exists already.", name );
        return true;
    }

    KFbxVector4 t = node->LclTranslation.Get();
    KFbxVector4 r = node->LclRotation.Get();
    KFbxVector4 s = node->LclScaling.Get();

    ModelHierarchyDesc::NodeDesc & gnnode = desc.nodes[name];
    gnnode.parent = parent ? parent->GetName() : "";
    gnnode.position.set( (float)t[0], (float)t[1], (float)t[2] );
    gnnode.orientation.identity(); // TODO: setup rotation.
    gnnode.scaling.set( (float)s[0], (float)s[1], (float)s[2] );
    gnnode.bbox.set( 0, 0, 0, 0, 0, 0 );

    KFbxNodeAttribute* attrib = node->GetNodeAttribute();
    KFbxNodeAttribute::EAttributeType type = attrib ? attrib->GetAttributeType() : KFbxNodeAttribute::eUNIDENTIFIED;
    if( KFbxNodeAttribute::eMESH == type )
    {
        // load mesh node
        sLoadFbxMesh( desc, gnnode, sdk, (KFbxMesh*)attrib, name );
    }
    else
    {
        // create an empty node.
        GN_VERBOSE(sLogger)( "Ignore unsupported node: type=%d, name=%s", type, name );
    }

    // load children
    for( int i = 0; i < node->GetChildCount(); ++i )
    {
        if( !sLoadFbxNodeRecursivly( desc, sdk, node->GetChild( i ), node ) )
        {
            return false;
        }
    }

    // done
    return true;
}

#endif

//
//
// -----------------------------------------------------------------------------
static bool
sLoadModelHierarchyFromFBX( ModelHierarchyDesc & desc, File & file )
{
#ifdef HAS_FBX

    FbxSdkWrapper sdk;
    if( !sdk.init() ) return false;
    KFbxSdkManager * gSdkManager = sdk.manager;

    // TODO: setup file system.

    // detect file format
    StrA filename = fs::toNativeDiskFilePath( file.name() );
	int lFileFormat = -1;
    if (!gSdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(filename, lFileFormat) )
    {
        // Unrecognizable file format. Try to fall back to KFbxImporter::eFBX_BINARY
        lFileFormat = gSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription( "FBX binary (*.fbx)" );;
    }

    // Create the importer.
    KFbxImporter* gImporter = KFbxImporter::Create(gSdkManager,"");
    if( NULL == gImporter ) return false;
    if(!gImporter->Initialize(filename, lFileFormat))
    {
        GN_ERROR(sLogger)( gImporter->GetLastErrorString() );
        return false;
    }

    // Import the scene
    KFbxScene * gScene = KFbxScene::Create( gSdkManager, "" );
    if( NULL == gScene ) return false;
    if(!gImporter->Import(gScene))
    {
        GN_ERROR(sLogger)( gImporter->GetLastErrorString() );
        return false;
    }

    return sLoadFbxNodeRecursivly( desc, sdk, gScene->GetRootNode(), NULL );

#else

    desc.clear();
    GN_ERROR(sLogger)( "Fail to load file %s: FBX is not supported.", file.name() );
    return false;

#endif
}

}

// *****************************************************************************
//
// Assimp loader
//
// *****************************************************************************
namespace ai
{

// My own implementation of IOStream
class MyIOStream : public Assimp::IOStream
{
    friend class MyIOSystem;

    AutoObjPtr<File> mFile;

protected:

    // Constructor protected for private usage by MyIOSystem
    MyIOStream( const std::string & filename, const std::string & mode )
    {
        mFile.attach( fs::openFile( filename.c_str(), mode.c_str() ) );
    }

public:

    ~MyIOStream()
    {
        mFile.clear();
    }

    size_t Read( void* pvBuffer, size_t pSize, size_t pCount)
    {
        size_t readen;
        if( mFile && mFile->write( pvBuffer, pSize * pCount, &readen ) )
        {
            return readen;
        }
        else
        {
            return 0;
        }
    }

    size_t Write( const void* pvBuffer, size_t pSize, size_t pCount)
    {
        size_t written;
        if( mFile && mFile->write( pvBuffer, pSize * pCount, &written ) )
        {
            return written;
        }
        else
        {
            return 0;
        }
    }

    aiReturn Seek( size_t pOffset, aiOrigin pOrigin)
    {
        if( mFile )
        {
            FileSeek fs;
            switch( pOrigin )
            {
                case aiOrigin_SET : fs = FileSeek::SET;
                case aiOrigin_CUR : fs = FileSeek::CUR;
                case aiOrigin_END : fs = FileSeek::END;
                default           : return aiReturn_FAILURE;
            }
            return mFile->seek( pOffset, fs ) ? aiReturn_SUCCESS : aiReturn_FAILURE;
        }
        else
        {
            return aiReturn_FAILURE;
        }
    }

    size_t Tell() const
    {
        return mFile ? mFile->tell() : 0;
    }

    size_t FileSize() const
    {
        return mFile ? mFile->size() : 0;
    }

    void Flush ()
    {
    }
};

// Fisher Price - My First Filesystem
class MyIOSystem : public Assimp::IOSystem
{
    MyIOSystem()
    {
    }

    ~MyIOSystem()
    {
    }

    // Check whether a specific file exists
    bool Exists( const std::string & filename ) const
    {
        return GN::fs::pathExist( filename.c_str() );
    }

    // Get the path delimiter character we'd like to see
    char GetOsSeparator() const
    {
        return '/';
    }

    // ... and finally a method to open a custom stream
    Assimp::IOStream * Open( const std::string & file, const std::string& mode )
    {
        return new MyIOStream( file, mode );
    }

    void Close( Assimp::IOStream* pFile)
    {
        delete pFile;
    }
};

}

// *****************************************************************************
//
// Garnet's homebew XML loader
//
// *****************************************************************************

namespace xml
{
//
// post error message
// -----------------------------------------------------------------------------
static void sPostXMLError( const XmlNode & node, const StrA & msg )
{
    GN_UNUSED_PARAM( node );
    const XmlElement * e = node.toElement();
    if( e )
    {
        GN_ERROR(sLogger)( "<%s>: %s", e->name.cptr(), msg.cptr() );
    }
    else
    {
        GN_ERROR(sLogger)( "%s", msg.cptr() );
    }
}

//
//
// -----------------------------------------------------------------------------
static bool
sParseModel( ModelHierarchyDesc & desc, XmlElement & root, const StrA & basedir )
{
    ModelResourceDesc md;

    if( !md.loadFromXml( root, basedir ) ) return false;

    XmlAttrib * modelName = root.findAttrib( "name" );
    if( !modelName || modelName->value.empty() )
    {
        GN_ERROR(sLogger)( "Model name attribute is missing." );
        return false;
    }

    if( NULL == desc.meshes.find( md.mesh ) )
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
sParseNode( ModelHierarchyDesc & desc, XmlElement & root )
{
    GN_ASSERT( root.name == "node" );

    XmlAttrib * nodeName = root.findAttrib( "name" );
    if( NULL == nodeName )
    {
        sPostXMLError( root, "Entity name attribute is missing." );
        return false;
    }

    ModelHierarchyDesc::NodeDesc node;

    // parse spatial
    XmlAttrib * a = root.findAttrib( "parent" );
    if( a ) node.parent = a->value;

    a = root.findAttrib( "position" );
    if( !a || 3 != string2FloatArray( (float*)&node.position, 3, a->value ) )
    {
        sPostXMLError( root, "Invalid position" );
        node.position.set( 0, 0, 0 );
    }

    a = root.findAttrib( "orientation" );
    if( !a || 4 != string2FloatArray( (float*)&node.orientation, 4, a->value ) )
    {
        sPostXMLError( root, "Invalid orientation" );
        node.orientation.set( 0, 0, 0, 1 );
    }

    a = root.findAttrib( "bbox" );
    if( !a || 6 != string2FloatArray( (float*)&node.bbox, 6, a->value ) )
    {
        sPostXMLError( root, "Invalid bounding box" );
        node.bbox = Boxf( 0, 0, 0, 0, 0, 0 );
    }

    // parse visual
    XmlElement * visualNode = root.findChildElement( "visual" );
    if( visualNode )
    {
        for( XmlNode * n = visualNode->firstc; n != NULL; n = n->nexts )
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

                node.models.append( a->value );
            }
            else
            {
                sPostXMLError( *e, stringFormat( "Unknown element: <%s>", e->name.cptr() ) );
            }
        }
    }

    // done
    desc.nodes[nodeName->value] = node;
    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool
sLoadModelHierarchyFromXML( ModelHierarchyDesc & desc, File & file )
{
    XmlDocument doc;
    XmlParseResult xpr;
    if( !doc.parse( xpr, file ) )
    {
        static Logger * sLogger = getLogger( "GN.base.xml" );
        GN_ERROR(sLogger)(
            "Fail to parse XML file (%s):\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            file.name(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.cptr() );
        return false;
    }
    GN_ASSERT( xpr.root );

    StrA basedir = fs::dirName( file.name() );

    XmlElement * root = xpr.root->toElement();
    if( !root || "modelHierarchy" != root->name )
    {
        sPostXMLError( *root, "Root element name must be \"<modelHierarchy>\"." );
        return false;
    }

#if ROOT_BBOX
    XmlAttrib * bboxAttr = root->findAttrib( "bbox" );
    if( !bboxAttr || 6 != string2FloatArray( (float*)&desc.bbox, 6, bboxAttr->value ) )
    {
        sPostXMLError( *root, "Invalid bbox attribute." );
        return false;
    }
#endif

    // parse models
    XmlElement * modelsNode = root->findChildElement( "models" );
    if( NULL == modelsNode )
    {
        sPostXMLError( *root, "Element <models> is missing." );
        return false;
    }
    for( XmlNode * n = modelsNode->firstc; n != NULL; n = n->nexts )
    {
        XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "model" == e->name )
        {
            if( !sParseModel( desc, *e, basedir ) ) return false;
        }
        else
        {
            sPostXMLError( *e, stringFormat( "Ignore unknowned element: <%s>", e->name.cptr() ) );
        }
    }

    // parse nodes
    XmlElement * entitiesNode = root->findChildElement( "nodes" );
    if( NULL == entitiesNode )
    {
        sPostXMLError( *root, "Element <nodes> is missing." );
        return false;
    }
    for( XmlNode * n = entitiesNode->firstc; n != NULL; n = n->nexts )
    {
        XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "node" == e->name )
        {
            if( !sParseNode( desc, *e ) ) return false;
        }
        else
        {
            sPostXMLError( *e, stringFormat( "Ignore unknowned element: <%s>", e->name.cptr() ) );
        }
    }

    // done
    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool
sSaveModelHierarchyToXML( const ModelHierarchyDesc & desc, const char * filename )
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
        GN_ERROR(sLogger)( "%s is not a directory", dirname.cptr() );
        return false;
    }

    // write meshes
    int meshindex = 0;
    StringMap<char,StrA> meshNameMapping;
    for( const StringMap<char,MeshResourceDesc>::KeyValuePair * i = desc.meshes.first();
        i != NULL;
        i = desc.meshes.next( i ) )
    {
        const StrA & oldMeshName = i->key;
        const MeshResourceDesc & mesh = i->value;

        StrA newMeshName = stringFormat( "%s.%d.mesh.bin", basename.cptr(), meshindex );

        if( !mesh.saveToFile( dirname + "\\" + newMeshName ) ) return false;

        meshNameMapping[oldMeshName] = newMeshName;

        ++meshindex;
    }

    // create a new XML document
    XmlDocument xmldoc;
    XmlElement * root = xmldoc.createElement(NULL);
    root->name = "modelHierarchy";

    // write models
    XmlElement * models = xmldoc.createElement( root );
    models->name = "models";
    for( const StringMap<char,gfx::ModelResourceDesc>::KeyValuePair * i = desc.models.first();
         i != NULL;
         i = desc.models.next( i ) )
    {
        const StrA & modelName  = i->key;
        ModelResourceDesc model = i->value;

        StrA * pNewMeshName = meshNameMapping.find( model.mesh );
        if( NULL != pNewMeshName )
        {
            model.mesh = *pNewMeshName;
        }

        XmlElement * modelNode = model.saveToXml( *models, dirname );
        if( !modelNode ) return false;

        XmlAttrib * a = xmldoc.createAttrib( modelNode );
        a->name = "name";
        a->value = modelName;
    }

    // rename nodes
    int entityIndex = 0;
    StringMap<char,StrA> entityNameMap;
    for( const StringMap<char,ModelHierarchyDesc::NodeDesc>::KeyValuePair * i = desc.nodes.first();
        i != NULL;
        i = desc.nodes.next( i ) )
    {
        const StrA & nodeName = i->key;

        entityNameMap[nodeName] = stringFormat( "%d", ++entityIndex );
    }

    // write nodes
    XmlElement * nodes = xmldoc.createElement( root );
    nodes->name = "nodes";
    for( const StringMap<char,ModelHierarchyDesc::NodeDesc>::KeyValuePair * i = desc.nodes.first();
        i != NULL;
        i = desc.nodes.next( i ) )
    {
        const StrA                         & nodeName = *entityNameMap.find(i->key);
        const ModelHierarchyDesc::NodeDesc & nodeDesc = i->value;

        XmlElement * node = xmldoc.createElement( nodes );
        node->name = "node";

        XmlAttrib * a = xmldoc.createAttrib( node );
        a->name  = "name";
        a->value = nodeName;

        a = xmldoc.createAttrib( node );
        a->name  = "parent";
        StrA * pParentEntityName = entityNameMap.find(nodeDesc.parent);
        if( NULL != pParentEntityName )
        {
            a->value = *pParentEntityName;
        }
        else if( !nodeDesc.parent.empty() )
        {
            GN_WARN(sLogger)( "Entity %s has invalid parent: %s", i->key, nodeDesc.parent.cptr() );
        }

        a = xmldoc.createAttrib( node );
        a->name  = "position";
        a->value = stringFormat( "%f,%f,%f",
            nodeDesc.position.x,
            nodeDesc.position.y,
            nodeDesc.position.z );

        a = xmldoc.createAttrib( node );
        a->name  = "orientation";
        a->value = stringFormat( "%f,%f,%f,%f",
            nodeDesc.orientation.v.x,
            nodeDesc.orientation.v.y,
            nodeDesc.orientation.v.x,
            nodeDesc.orientation.w );

        a = xmldoc.createAttrib( node );
        a->name  = "bbox";
        a->value = stringFormat( "%f,%f,%f,%f,%f,%f",
            nodeDesc.bbox.x,
            nodeDesc.bbox.y,
            nodeDesc.bbox.z,
            nodeDesc.bbox.w,
            nodeDesc.bbox.h,
            nodeDesc.bbox.d );

        XmlElement * visual = xmldoc.createElement( node );
        visual->name  = "visual";
        for( size_t i = 0; i < nodeDesc.models.size(); ++i )
        {
            XmlElement * modelref = xmldoc.createElement( visual );
            modelref->name = "model";

            XmlAttrib * a = xmldoc.createAttrib( modelref );
            a->name = "ref";
            a->value = nodeDesc.models[i];
        }
    }

#if ROOT_BBOX
    // write scene bounding box
    XmlAttrib * a = xmldoc.createAttrib( root->toElement() );
    a->name  = "bbox";
    a->value = stringFormat( "%f,%f,%f,%f,%f,%f",
            desc.bbox.x,
            desc.bbox.y,
            desc.bbox.z,
            desc.bbox.w,
            desc.bbox.h,
            desc.bbox.d );
#endif

    // write XML document
    AutoObjPtr<File> fp( fs::openFile( filename, "wt" ) );
    if( !fp ) return false;
    return xmldoc.writeToFile( *fp, *root, false );
}

}

// *****************************************************************************
// Load from garnet mesh binary
// *****************************************************************************

namespace bin
{

//
//
// -----------------------------------------------------------------------------
bool sLoadModelHierarchyFromMeshBinary( ModelHierarchyDesc & desc, File & fp )
{
    desc.clear();

    const StrA & meshname = fp.name();

    MeshResourceDesc mesh;
    AutoRef<Blob> blob = mesh.loadFromFile( fp );
    if( !blob ) return false;

    // determine the model template
    const ModelResourceDesc * modelTemplate = ase::sDetermineBestModelTemplate( mesh );
    if( NULL == modelTemplate ) return false;

    // initialize the model descriptor based on the template
    ModelResourceDesc model = *modelTemplate;
    model.mesh = meshname;

    // add mesh and model to scene
    desc.meshes[meshname] = mesh;
    desc.meshdata.append( blob );
    desc.models[meshname] = model;

    // create a node for the model
    ModelHierarchyDesc::NodeDesc & ed = desc.nodes[meshname];
    ed.position.set( 0, 0, 0 );
    ed.orientation.set( 0, 0, 0, 1 );
    mesh.calculateBoundingBox( ed.bbox );
    ed.models.append( meshname );

#if ROOT_BBOX
    desc.bbox = ed.bbox;
#endif

    // done

    return true;
}

}
// *****************************************************************************
// ModelHierarchyDesc
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelHierarchyDesc::clear()
{
    meshes.clear();
    meshdata.clear();
    models.clear();
    nodes.clear();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::ModelHierarchyDesc::loadFromFile( const char * filename )
{
    GN_SCOPE_PROFILER( ModelHierarchyDesc_loadFromFile, "Load models hierarchy from file" );

    GN_INFO(sLogger)( "Load models from file: %s", filename?filename:"<NULL>" );

    clear();

    // open file
    AutoObjPtr<File> fp( fs::openFile( filename, "rb" ) );
    if( !fp ) return false;

    // get file extension
    StrA ext = fs::extName( filename );

    // do loading
    if( sStrEndWithI( filename, ".xml" ) )
    {
        return xml::sLoadModelHierarchyFromXML( *this, *fp );
    }
    else if( sStrEndWithI( filename, ".ase" ) )
    {
        return ase::sLoadModelHierarchyFromASE( *this, *fp );
    }
    else if( sStrEndWithI( filename, ".fbx" ) )
    {
        return fbx::sLoadModelHierarchyFromFBX( *this, *fp );
    }
    else if( sStrEndWithI( filename, ".xpr" ) ||
             sStrEndWithI( filename, ".tpr" ) )
    {
        return xpr::sLoadModelHierarchyFromXPR( *this, *fp );
    }
    else if( sStrEndWithI( filename, ".mesh.bin" ) )
    {
        return bin::sLoadModelHierarchyFromMeshBinary( *this, *fp );
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
bool GN::gfx::ModelHierarchyDesc::saveToFile( const char * filename ) const
{
    GN_SCOPE_PROFILER( ModelHierarchyDesc_saveToFile, "Save models hierarchy to file" );

    GN_INFO(sLogger)( "Write scene to : %s", filename );

    return xml::sSaveModelHierarchyToXML( *this, filename );
}
