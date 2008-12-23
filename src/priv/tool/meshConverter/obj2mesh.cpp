#include "pch.h"
#include "convert.h"

using namespace GN;

// *****************************************************************************
// local functions
// *****************************************************************************

struct ObjFaceVertex
{
    UInt32 pos;
    UInt32 tex;
    UInt32 nrm;

    bool operator < ( const ObjFaceVertex & rhs ) const
    {
        if( pos != rhs.pos ) return pos < rhs.pos;
        if( tex != rhs.tex ) return tex < rhs.tex;
        return nrm < rhs.nrm;
    }
};

struct ObjMaterial
{
    StrA     name;
    Vector3f ambient;
    Vector3f diffuse;
    Vector3f specular;
    float    alpha;
    float    shininess;
    StrA     texdiff; ///< diffuse texture
    StrA     texbump; ///< bump texture

    bool operator==( const ObjMaterial & rhs ) const
    {
        return name == rhs.name;
    }
};

struct ObjFaceChunk
{
    UInt32                     material; ///< index into ObjScene's material list.
    std::vector<ObjFaceVertex> indices;
};

struct ObjGroup
{
    StrA                      name;
    std::vector<GN::Vector3f> positions;
    std::vector<GN::Vector2f> texcoords;
    std::vector<GN::Vector3f> normals;
    std::vector<ObjFaceChunk> chunks;
};

struct ObjScene
{
    std::vector<ObjMaterial> materials;
    std::vector<ObjGroup>    groups;

    ///
    /// lookup specific material. Return -1, if material not found
    ///
    UInt32 lookupMaterial( const StrA & name ) const
    {
        for( UInt32 i = 0; i < materials.size(); ++i )
        {
            if( materials[i].name == name ) return i;
        }
        GN_ERROR(sLogger)( "invalid material name : %s", name.cptr() );
        return (UInt32)-1;
    }
};

class VertexMap
{
    typedef std::map<ObjFaceVertex,UInt16> VtxMap;
    VtxMap mVtxMap;

public:

    ///
    /// return true means a new vertex
    ///
    bool getVertexIndex( UInt16 & index, const ObjFaceVertex & vtx )
    {
        std::pair<VtxMap::iterator,bool> i = mVtxMap.insert( VtxMap::value_type(vtx,0xbad) );

        if( i.second )
        {
            // this is a new vertex
            GN_ASSERT( 0xbad == i.first->second );
            i.first->second = (UInt16)( mVtxMap.size() - 1 );
        }

        index = i.first->second;

        return i.second;
    }
};

//
//
// -----------------------------------------------------------------------------
static bool sReadLine( File & fp, StrA & line )
{
    char ch;
    size_t readen;
    while( fp.read( &ch, 1, &readen ) && readen > 0 && '\n' != ch )
    {
        line.append( ch );
    }
    size_t k = fp.tell();
    size_t n = fp.size();
    printf( "\r%%%d", k <= n ? ( k * 100 / n ) : 0 );
    return !line.empty() || !fp.eof();
}

//
//
// -----------------------------------------------------------------------------
template<UInt32 COUNT>
static bool sReadFloats( float * v, const StrA & s, size_t offset )
{
    if( s.size() <= offset ) return false;
    if( !str2Floats( v, COUNT, s.cptr() + offset, s.size() - offset ) ) return false;
    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadMaterials( File & fp, ObjScene & scene )
{
    // read materials
    StrA line;
    ObjMaterial mat;
    int linecount = 0;
    bool readmat = false;
    while( sReadLine( fp, line ) )
    {
        if( !line.empty() && '#' != line[0] )
        {
            if( "newmtl" == line.subString( 0, 6 ) )
            {
                if( readmat )
                {
                    if( scene.materials.end() == std::find( scene.materials.begin(), scene.materials.end(), mat ) )
                    {
                        scene.materials.push_back( mat );
                    }
                    else
                    {
                        GN_VERBOSE(sLogger)( "ignore redundent material %s.", mat.name.cptr() );
                    }
                    readmat = false;
                }
                else
                {
                    readmat = true;
                    mat.name = line.subString( 7, 0 );
                    mat.ambient.set( 0.2f, 0.2f, 0.2f );
                    mat.diffuse.set( 0.8f, 0.8f, 0.8f );
                    mat.specular.set( 1.0f, 1.0f, 1.0f );
                    mat.alpha = 1.0f;
                    mat.shininess = 0.0f;
                    mat.texdiff.clear();
                    mat.texbump.clear();
                }
            }

            if( readmat )
            {
                if( 'K' == line[0] && 'a' == line[1] )
                {
                    if( !sReadFloats<3>( mat.ambient, line, 3 ) )
                    {
                        GN_ERROR(sLogger)( "line %d : invalid ambient!", linecount );
                        return false;
                    }
                }
                else if ( 'K' == line[0] && 'd' == line[1] )
                {
                    if( !sReadFloats<3>( mat.diffuse, line, 3 ) )
                    {
                        GN_ERROR(sLogger)( "line %d : invalid diffuse!", linecount );
                        return false;
                    }
                }
                else if ( 'K' == line[0] && 's' == line[1] )
                {
                    if( !sReadFloats<3>( mat.specular, line, 3 ) )
                    {
                        GN_ERROR(sLogger)( "line %d : invalid specular!", linecount );
                        return false;
                    }
                }
                else if( 'd' == line[0] )
                {
                    if( !str2Float( mat.alpha, line.cptr() + 2 ) )
                    {
                        GN_ERROR(sLogger)( "line %d : invalid alpha!", linecount );
                        return false;
                    }
                }
                else if ( 'N' == line[0] && 's' == line[1] )
                {
                    if( !str2Float( mat.shininess, line.cptr() + 3 ) )
                    {
                        GN_ERROR(sLogger)( "line %d : invalid shininess!", linecount );
                        return false;
                    }
                }
                else if( "illum" == line.subString( 0, 5 ) )
                {
                    // do nothing here
                }
                else if( "map_Ka" == line.subString( 0, 6 ) )
                {
                    mat.texdiff = line.subString( 7, 0 );
                }
                else if( "bump" == line.subString( 0, 4 ) )
                {
                    mat.texbump = line.subString( 5, 0 );
                }
                else
                {
                    GN_VERBOSE(sLogger)( "line %d : ignored.", linecount );
                }
            }
        }

        // next line
        ++linecount;
        line.clear();
    }
    if( readmat )
    {
        // handle last mat
        if( scene.materials.end() == std::find( scene.materials.begin(), scene.materials.end(), mat ) )
        {
            scene.materials.push_back( mat );
        }
        else
        {
            GN_VERBOSE(sLogger)( "ignore redundent material %s.", mat.name.cptr() );
        }
    }

    // success
    return true;
}

#if GN_MSVC8
#define sscanf sscanf_s
#endif

//
//
// -----------------------------------------------------------------------------
static bool sReadScene( ObjScene & scene, const StrA & objFileName )
{
    GN_GUARD;

    // compose material library file name
    StrA mtlFileName = joinPath( parentPath(objFileName), baseName(objFileName) ) + ".mtl";

    // read materials
    AutoObjPtr<File> mtlfile( core::openFile( mtlFileName, "rt" ) );
    if( !mtlfile ) return false;
    GN_INFO(sLogger)( "\nRead materials ..." );
    if( !sReadMaterials( *mtlfile, scene ) ) return false;

    // open obj file
    AutoObjPtr<File> objfile( core::openFile( objFileName, "rt" ) );
    if( !objfile ) return false;

    GN_INFO(sLogger)( "\nRead objects ..." );

    scene.groups.reserve( 100 );

    StrA line;
    int linecount = 0;
    ObjGroup *     group = 0;
    ObjFaceChunk * chunk = 0;
    while( sReadLine( *objfile, line ) )
    {
        if( !line.empty() && '#' != line[0] )
        {
            if( 'v' == line[0] )
            {
                if( 0 == group )
                {
                    GN_ERROR(sLogger)( "line %d : vertex definition out of group." );
                    return false;
                }
                if( ' ' == line[1] )
                {
                    group->positions.resize( group->positions.size() + 1 );
                    if( !sReadFloats<3>( group->positions.back(), line, 2 ) )
                    {
                        GN_ERROR(sLogger)( "line %d : invalid vertex position.", linecount );
                        return false;
                    }
                }
                else if( 't' == line[1] )
                {
                    group->texcoords.resize( group->texcoords.size() + 1 );
                    if( !sReadFloats<2>( group->texcoords.back(), line, 3 ) )
                    {
                        GN_ERROR(sLogger)( "line %d : invalid vertex position.", linecount );
                        return false;
                    }
                }
                else if( 'n' == line[1] )
                {
                    group->normals.resize( group->normals.size() + 1 );
                    if( !sReadFloats<3>( group->normals.back(), line, 3 ) )
                    {
                        GN_ERROR(sLogger)( "line %d : invalid vertex position.", linecount );
                        return false;
                    }
                }
            }
            else if( 'f' == line[0] )
            {
                if( 0 == chunk )
                {
                    GN_ERROR(sLogger)( "line %d : missing usemtl before the first face definition!", linecount );
                    return false;
                }

                if( chunk->indices.empty() ) chunk->indices.reserve( 0x40000 );

                chunk->indices.resize( chunk->indices.size() + 3 );

                ObjFaceVertex & v1 = chunk->indices[ chunk->indices.size() - 3 ];
                ObjFaceVertex & v2 = chunk->indices[ chunk->indices.size() - 2 ];
                ObjFaceVertex & v3 = chunk->indices[ chunk->indices.size() - 1 ];

                if( 9 != sscanf( line.cptr() + 2, "%u/%u/%u %u/%u/%u %u/%u/%u",
                    &v1.pos, &v1.tex, &v1.nrm,
                    &v2.pos, &v2.tex, &v2.nrm,
                    &v3.pos, &v3.tex, &v3.nrm ) )
                {
                    GN_ERROR(sLogger)( "line %d : invalid face definition.", linecount );
                    return false;
                }
            }
            else if( "usemtl" == line.subString( 0, 6 ) )
            {
                // create new chunk
                if( 0 == group )
                {
                    GN_ERROR(sLogger)( "line %d : usemtl out of be used in a group." );
                    return false;
                }
                group->chunks.resize( group->chunks.size() + 1 );
                chunk = &group->chunks.back();
                chunk->material = scene.lookupMaterial( line.subString( 7, 0 ) );
                if( (UInt32)-1 == chunk->material ) return false;
                chunk->indices.reserve( 0x10000 );
            }
            else if( 'g' == line[0] )
            {
                if( 1 == line.size() )
                {
                    // create new group
                    scene.groups.resize( scene.groups.size() + 1 );
                    group = &scene.groups.back();
                    group->positions.resize( 0x10000 );
                    group->texcoords.resize( 0x10000 );
                    group->normals.resize( 0x10000 );
                    group->chunks.reserve( 0x100 );
                    chunk = 0;
                }
                else
                {
                    // define group name
                    group->name = line.subString( 2, 0 );
                }
            }
        }

        // next line
        ++linecount;
        line.clear();
    }

    // remove pending empty group
    if( scene.groups.back().chunks.empty() ) scene.groups.pop_back();

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sWriteScene( const ObjScene & scene, const StrA & name )
{
    GN_GUARD;

    // for each group
    for( size_t i = 0; i < scene.groups.size(); ++i )
    {
        const ObjGroup & group = scene.groups[i];

        // for each chunk
        for( size_t i = 0; i < group.chunks.size(); ++i )
        {
            const ObjFaceChunk & chunk = group.chunks[i];

            GN_ASSERT( chunk.material < scene.materials.size() );

            const ObjMaterial & material = scene.materials[chunk.material];

            StrA basename = name + "_" + group.name + "_" + material.name;
            StrA drawname = basename + ".drawable.xml";
            StrA meshname = basename + ".mesh.xml";
            StrA vbname   = basename + ".vb";
            StrA ibname   = basename + ".ib";

            // create IB and VB file
            AutoObjPtr<File> vb( core::openFile( vbname, "wb" ) );
            AutoObjPtr<File> ib( core::openFile( ibname, "wb" ) );
            if( !vb || !ib ) return false;

            VertexMap vtxmap;

            struct MeshVertex
            {
                float pos[3];
                float tex[2];
                float nml[3];
            };

            // for each vertex
            UInt32 vtxcount = 0;
            for( size_t i = 0; i < chunk.indices.size(); ++i )
            {
                const ObjFaceVertex & v = chunk.indices[i];

                UInt16 idx;
                if( vtxmap.getVertexIndex( idx, v ) )
                {
                    // this is a new vertex, write to VB file
                    if( !vb->write( group.positions[v.pos], sizeof(float)*3, 0 ) ||
                        !vb->write( group.normals  [v.nrm], sizeof(float)*3, 0 ) ||
                        !vb->write( group.texcoords[v.tex], sizeof(float)*2, 0 ) )
                    {
                        GN_ERROR(sLogger)( "fail to write vertex %d", idx );
                        return false;
                    }

                    ++vtxcount;
                }

                // write to IB file
                if( !ib->write( &idx, sizeof(idx), 0 ) )
                {
                    GN_ERROR(sLogger)( "fail to write index %d", i );
                    return false;
                }
            }

            // write mesh xml
            StrA meshxml;
            meshxml.format(
                "<?xml version=\"1.0\" standalone=\"yes\"?>\n"
                "<mesh\n"
                "	primtype  = \"TRIANGLE_LIST\"\n"
                "	numprim   = \"%d\"\n"
                "	startvtx  = \"0\"\n"
                "	minvtxidx = \"0\"\n"
                "	numvtx    = \"%d\"\n"
                "	startidx  = \"0\"\n"
                "	>\n"
                "\n"
                "	<vtxfmt>\n"
                "		<attrib stream = \"0\" offset =  \"0\" semantic = \"POS0\" format = \"FMT_FLOAT3\"/>\n"
                "		<attrib stream = \"0\" offset = \"12\" semantic = \"NML0\" format = \"FMT_FLOAT3\"/>\n"
                "		<attrib stream = \"0\" offset = \"24\" semantic = \"TEX0\" format = \"FMT_FLOAT2\"/>\n"
                "	</vtxfmt>\n"
                "	\n"
                "	<vtxbuf stream = \"0\" offset = \"0\" stride = \"32\" ref=\"%s\"/>\n"
                "\n"
                "	<idxbuf ref=\"%s\"/>\n"
                "</mesh>\n",
                chunk.indices.size() / 3,
                vtxcount,
                vbname.cptr(),
                ibname.cptr() );
            AutoObjPtr<File> mesh( core::openFile( meshname, "wt" ) );
            if( !mesh || !mesh->write( meshxml.cptr(), meshxml.size(), 0 ) ) return false;

            // write drawable xml
            StrA drawxml;
            drawxml.format(
                "<?xml version=\"1.0\" standalone=\"yes\"?>\n"
                "<drawable>\n"
                "	<effect ref=\"media::\\effect\\colored_texed_bump.xml\"/>\n"
                "	<mesh ref=\"%s\"/>\n"
                "	<texture binding=\"texdiff\" ref=\"%s\"/>\n"
                "	<uniform binding=\"texbump\" ref=\"%s\"/>\n"
                "	<uniform binding=\"pvw\"/>\n"
                "</drawable>\n",
                meshname.cptr(),
                material.texdiff.cptr(),
                material.texbump.cptr() );
            AutoObjPtr<File> draw( core::openFile( drawname, "wt" ) );
            if( !draw || !draw->write( drawxml.cptr(), drawxml.size(), 0 ) ) return false;
        }
    }

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::obj2mesh( const ConvertOptions & co )
{
    GN_GUARD;

    GN_ASSERT( !co.inputFileName.empty() );
    GN_ASSERT( !co.outputFileName.empty() );

    ObjScene s;

    if( !sReadScene( s, co.inputFileName ) ) return false;

    if( !sWriteScene( s, co.outputFileName ) ) return false;

    // success
    return true;

    GN_UNGUARD;
}
