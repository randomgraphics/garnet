#include "pch.h"
#include "loader.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::scene;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.tool.meshViewer");

// *****************************************************************************
// Local utilities
// *****************************************************************************

struct MeshContainer
{
    DynaArray<Mesh*> meshes;

    ~MeshContainer()
    {
        for( size_t i = 0; i < meshes.size(); ++i )
        {
            delete meshes[i];
        }
    }
};

struct EffectType
{
    enum ENUM
    {
        NONE,
        WIREFRAME,
        DIFFUSE,
        NORMAL_MAP,
        NUM_EFFECT_TYPES,
    };
};

static bool sHasSemantic( const Mesh & m, const char * binding, size_t index )
{
    const VertexFormat & vf = m.getDesc().vtxfmt;

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

static bool sHasPosition( const Mesh & m )
{
    return sHasSemantic( m, "position", 0 )
        || sHasSemantic( m, "pos", 0 );
}

static bool sHasNormal( const Mesh & m )
{
    return sHasSemantic( m, "normal", 0 );
}

static bool sHasTex0( const Mesh & m )
{
    return sHasSemantic( m, "texcoord", 0 );
}

static bool sHasTangent( const Mesh & m )
{
    return sHasSemantic( m, "tangent", 0 );
}

///
/// Determine the best effect that can show the mesh
///
static EffectType::ENUM sDetermineBestEffect( const Mesh & m )
{
    // position is required
    if( !sHasPosition( m ) )
    {
        GN_ERROR(sLogger)( "The mesh has no position, which is required by the mesh viewer." );
        return EffectType::NONE;
    }

    if( !sHasNormal( m ) )
    {
        GN_WARN(sLogger)( "The mesh has no normal." );
        return EffectType::WIREFRAME;
    }

    if( !sHasTex0( m ) )
    {
        GN_WARN(sLogger)( "The mesh has no texture coordinate." );
        return EffectType::WIREFRAME;
    }

    // use normal map, if the mesh has both normal and tangent.
    if( sHasTangent( m ) )
    {
        return EffectType::NORMAL_MAP;
    }
    else
    {
        return EffectType::DIFFUSE;
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
    DynaArray<gfx::MeshDesc> meshes;
    DynaArray<XPRTex2DDesc*> texDescs;
    DynaArray<XPRVBufDesc*>  vbDescs;
    DynaArray<XPRIBufDesc*>  ibDescs;
    DynaArray<void*>         sceneData; ///< store all texture and mesh data
    Boxf                     bbox;      ///< bounding box
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
static GN::scene::GeometryNode *
loadGeometryFromXpr( Scene & sc, File & file )
{
    // load XPR file
    XPRScene xpr;
    if( !loadXprSceneFromFile( xpr, file ) ) return NULL;

    // create mesh list
    MeshContainer mc;
    for( size_t i = 0; i < xpr.meshes.size(); ++i )
    {
        AutoObjPtr<Mesh> m( new Mesh(sc.getRenderer()) );
        if( !m || !m->init(xpr.meshes[i]) ) return false;
        mc.meshes.append( m );
        m.detach();
    }

    // create model
    GeometryNode * model = new GeometryNode(sc);
    GN_UNIMPL_WARNING();

    // success
    return model;
}

// *****************************************************************************
// ASE loader
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static GN::scene::GeometryNode *
loadGeometryFromAse( Scene & sc, File & file )
{
    // load ASE scene
    AseScene ase;
    if( !loadAseSceneFromFile(ase, file) ) return false;

    // create mesh list
    MeshContainer mc;
    for( size_t i = 0; i < ase.meshes.size(); ++i )
    {
        AutoObjPtr<Mesh> m( new Mesh(sc.getRenderer()) );
        if( !m || !m->init(ase.meshes[i]) ) return false;
        mc.meshes.append( m );
        m.detach();
    }

    // initialize effects
    SimpleWireframeEffect wireframeEffect;
    if( !wireframeEffect.init( sc.getRenderer() ) ) return false;
    SimpleDiffuseEffect diffuseEffect;
    if( !diffuseEffect.init( sc.getRenderer() ) ) return false;
    SimpleNormalMapEffect normalMapEffect;
    if( !normalMapEffect.init( sc.getRenderer() ) ) return false;

    // create model
    GeometryNode * model = new GeometryNode(sc);
    for( size_t i = 0; i < ase.subsets.size(); ++i )
    {
        const AseMeshSubset & s = ase.subsets[i];

        Mesh * m = mc.meshes[s.meshid];

        // determine effect
        EffectType::ENUM et = sDetermineBestEffect( *m );
        Effect * e;
        switch( et )
        {
            case EffectType::WIREFRAME:
                e = wireframeEffect.getEffect();
                break;

            case EffectType::DIFFUSE:
                e = diffuseEffect.getEffect();
                break;

            case EffectType::NORMAL_MAP:
                e = normalMapEffect.getEffect();
                break;

            default:
                e = NULL;
                break;
        }

        // skip the mesh, if there's no appropriate effect for it.
        if( !e ) continue;

        // bind textures to effect
        const AseMaterial & am = ase.materials[s.matid];
        if( e->textures.contains( "ALBEDO_TEXTURE" ) && !am.mapdiff.bitmap.empty() )
        {
            e->textures["ALBEDO_TEXTURE"].attach( GN::gfx::loadTextureFromFile( sc.getRenderer(), am.mapdiff.bitmap ) );
        }
        if( e->textures.contains( "NORMAL_TEXTURE" ) && !am.mapbump.bitmap.empty() )
        {
            e->textures["NORMAL_TEXTURE"].attach( GN::gfx::loadTextureFromFile( sc.getRenderer(), am.mapbump.bitmap ) );
        }

        model->addGeometryBlock( e, m, &s );
    }

    // calculate bounding sphere
    const Boxf & bbox = ase.bbox;
    Spheref bs;
    bs.center = bbox.center();
    bs.radius = (float)sqrt( bbox.w * bbox.w + bbox.h * bbox.h + bbox.d * bbox.d );

    // set model properties
    model->setPivot( Vector3f(0,0,0) );
    model->setBoundingSphere( bs );

    return model;
}

// *****************************************************************************
// global functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::GeometryNode *
loadGeometryFromFile( Scene & sc, const char * filename )
{
    // open file
    DiskFile file;
    if( !file.open( filename, "rb" ) ) return false;

    // get file extension
    StrA ext = fs::extName( filename );

    // do loading
    if( 0 == strCmpI( ".ase", ext.cptr() ) )
    {
        return loadGeometryFromAse( sc, file );
    }
    else if( 0 == strCmpI( ".xpr", ext.cptr() ) ||
             0 == strCmpI( ".tpr", ext.cptr() ))
    {
        return loadGeometryFromXpr( sc, file );
    }
    else
    {
        GN_ERROR(sLogger)( "Unknown file extension: %s", ext.cptr() );
        return NULL;
    }
}
