#include "pch.h"
#include "garnet/GNutil.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static GN::Logger * sLogger = GN::GetLogger("GN.util.ase");

///
/// raw ASE material (has sub material information)
///
struct AseMaterialInternal : public AseMaterial
{
    DynaArray<AseMaterialInternal> submaterials; ///< sub material array. could be empty.
};

///
/// ASE vertex
///
struct AseVertex
{
    Vector3f            p; ///< position
    DynaArray<Vector3f> t; ///< texcoord
    DynaArray<Vector3f> n; ///< normal

    UInt32 addTexcoord( const Vector3f & v )
    {
        for( UInt32 i = 0; i < t.Size(); ++i )
        {
            if( t[i] == v ) return i;
        }
        t.Append( v );
        return (UInt32)( t.Size() - 1 );
    }

    UInt32 addNormal( const Vector3f & v )
    {
        for( UInt32 i = 0; i < n.Size(); ++i )
        {
            if( n[i] == v ) return i;
        }
        n.Append( v );
        return (UInt32)( n.Size() - 1 );
    }
};

///
/// ASE triangle face
///
struct AseFace
{
    UInt32   v[3];   ///< vertices (index into AseMeshInternal.vertices)
    UInt32   t[3];   ///< texcoords (index into AseVertex.t)
    UInt32   vn[3];  ///< normal (index into AseVertex.n)
    Vector3f fn;     ///< face normal
    UInt32   smooth; ///< smooth group ID
    UInt32   submat; ///< sub material ID
};

///
/// ASE face chunks (faces with same sub-material)
///
struct AseFaceChunk
{
    UInt32            submat; ///< submaterial ID
    DynaArray<UInt32> faces;  ///< indices into AseMeshInternal.faces
};

///
/// ASE mesh object
///
struct AseMeshInternal
{
    ///
    /// this group is loaded directly from ASE file.
    //@{
    UInt32                  timevalue;
    DynaArray<AseVertex>    vertices;  ///< vertex array
    DynaArray<AseFace>      faces;     ///< face array
    //@}

    //@{
    DynaArray<AseFaceChunk> chunks; ///< faces sorted by material
    Boxf                    bbox;   ///< bounding box of the mesh itself
    //@}
};

///
/// ASE node (elemnet for mesh hierachy)
///
struct AseNode
{
    StrA      parent;
    StrA      name;
    Matrix44f transform;
    Vector3f  pos;
    Vector3f  rotaxis;
    float     rotangle; // rotation angle in radian
    Vector3f  scale;
    Boxf      selfbbox;  ///< bounding box of myself
    Boxf      treebbox;  ///< bounding box of myself and my children.
};

///
/// An complete ASE geometry object that includes a mesh, a node and a meterial
///
struct AseGeoObject : public GN::TreeNode<AseGeoObject>
{
    AseNode         node;
    AseMeshInternal mesh;
    UInt32          matid; ///< material ID into global material array
};

///
/// Internal ASE scene structure, stores raw ASE information.
///
struct AseSceneInternal
{
    DynaArray<AseMaterialInternal> materials;
    DynaArray<AseGeoObject>        objects;
    AseGeoObject                   root; ///< root object
};

///
/// ASE file structure
///
struct AseFile
{
    DynaArray<char> buf;
    char *          str;
    int             line;
    StrA            filedir; // directory of the ASE file

    bool open( File & file )
    {
        // read ASE file
        buf.Resize( file.Size() + 1 );
        size_t readen;
        if( !file.Read( buf.ToRawPtr(), file.Size(), &readen ) )
        {
            return false;
        }
        GN_ASSERT( readen <= file.Size() );
        buf[readen] = 0;

        str = buf.ToRawPtr();
        line = 0;

        // get file dir
        filedir = GN::fs::DirName( file.Name() );

        // success
        return true;
    }

    void err( const StrA & msg )     const { GN_ERROR(sLogger)  ( "ASEFILE: line %d : %s", line, msg.ToRawPtr() ); }
    void warn( const StrA & msg )    const { GN_WARN(sLogger)   ( "ASEFILE: line %d : %s", line, msg.ToRawPtr() ); }
    void verbose( const StrA & msg ) const { GN_VERBOSE(sLogger)( "ASEFILE: line %d : %s", line, msg.ToRawPtr() ); }

    enum ScanOptionEnum
    {
        IN_CURRENT_BLOCK            = 0,
        IN_CURRENT_AND_CHILD_BLOCKS = 1,
        IN_ALL_BLOCKS               = 2,
        SILENCE                     = 4,
    };

    union ScanOption
    {
        UInt32 u32;

        struct
        {
            unsigned int scope   : 2;
            unsigned int silence : 1;
            unsigned int _       : 29; ///< reserved
        };

        ScanOption( BitFields bits ) : u32(bits) {}
    };

    static inline bool isWhiteSpace( char ch )
    {
        return ' ' == ch || '\t' == ch || '\n' == ch || '\r' == ch;
    }

    ///
    /// skip white spaces
    ///
    void skipWhiteSpaces()
    {
        GN_ASSERT( str );
        while( isWhiteSpace( *str ) )
        {
            if( '\n' == *str ) ++line;
            ++str;
        }
    }

    ///
    /// get Next token
    ///
    const char * Next( const char * expectedValue = 0, ScanOption option = 0 )
    {
        GN_ASSERT( str );

        skipWhiteSpaces();

        if( 0 == str || 0 == *str )
        {
            if( !option.silence ) verbose( "EOF" );
            return 0;
        }

        if ( '"' == *str )
        {
            // quoted string

            const char * r = str;

            ++str;
            while( '"' != *str && 0 != *str )
            {
                if( '\n' == *str ) ++line;
                ++str;
            }

            if( 0 != *str )
            {
                GN_ASSERT( '"' == *str );
                ++str;
                if( '\n' == *str ) ++line;
                *str = 0;
                ++str;
            }

            if( expectedValue && 0 != StringCompare( expectedValue, r ) )
            {
                if( !option.silence )
                {
                    err( StringFormat( "expect '%s', but found '%s'.", expectedValue, r ) );
                }
                return 0;
            }

            // success
            return r;

        }
        else
        {
            const char * r = str;

            while( !isWhiteSpace( *str ) )
            {
                ++str;
            }

            if( '\n' == *str ) ++line;

            if( 0 != *str ) *str = 0, ++str; // point to start of Next token

            if( expectedValue && 0 != StringCompare( expectedValue, r ) )
            {
                if( !option.silence )
                {
                    err( StringFormat( "expect '%s', but found '%s'.", expectedValue, r ) );
                }
                return 0;
            }

            return r;
        }
    }

    ///
    /// skip until meet specific token
    ///
    bool skipUntil( const char * endtoken, ScanOption option = 0 )
    {
        GN_ASSERT( !IsStringEmpty( endtoken ) );

        const char * token;
        int level = 0;
        for(;;)
        {
            token = Next();

            if( 0 == StringCompare( "{", token ) ) ++level;
            else if( 0 == StringCompare( "}", token ) ) --level;

            if( IN_CURRENT_BLOCK == option.scope && level > 0 ) continue; // skip sub levels

            if( IN_CURRENT_AND_CHILD_BLOCKS == option.scope && level < 0 )
            {
                if( !option.silence ) err( StringFormat( "token '%s' not found inside current block!", endtoken ) );
                return false;
            }

            if( 0 == token )
            {
                if( !option.silence ) err( StringFormat( "token '%s' not found!", endtoken ) );
                return false;
            }

            if( 0 == StringCompare( endtoken, token ) ) return true;
        }
    }

    //
    // True, if Next token is a node (begin with '*')
    // -----------------------------------------------------------------------------
    bool nextIsNode()
    {
        skipWhiteSpaces();
        return '*' == *str;
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool skipNode( ScanOption option = 0 )
    {
        do {
            skipWhiteSpaces();
            if( '{' == *str ) return skipUntil( "}", option );
            if( '*' == *str || '}' == *str ) return true; // found Next node
        } while( Next( 0, option ) );
        return false;
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool readBlockStart( ScanOption option = 0 ) { return 0 != Next( "{", option ); }

    //
    //
    // -----------------------------------------------------------------------------
    bool readBlockEnd( ScanOption option = 0 ) { return 0 != Next( "}", option ); }

    //
    //
    // -----------------------------------------------------------------------------
    const char * readNode( ScanOption option = 0  )
    {
        const char * token = Next( 0, option );
        if( !token ) return false;
        GN_ASSERT( !IsStringEmpty( token ) );

        if( '*' != *token )
        {
            if( !option.silence ) err( "Not a node token" );
            return 0;
        }

        // success
        return token;
    }

    //
    //
    // -----------------------------------------------------------------------------
    const char * readString( ScanOption option = 0  )
    {
        char * token = const_cast<char*>( Next( 0, option ) );
        if( !token ) return false;
        GN_ASSERT( !IsStringEmpty( token ) );

        if( '"' != *token )
        {
            if( !option.silence ) err( "Not a string token" );
            return 0;
        }

        // trim quota
        ++token;
        GN_ASSERT( (token+2) <= str );
        GN_ASSERT( '"' == token[str-token-2] );
        token[str-token-2] = 0;

        // success
        return token;
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool readString( StrA & result, ScanOption option = 0  )
    {
        const char * s = readString( option );
        if( 0 == s ) return false;
        result = s;
        return true;
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool readAndResolveRelativePath( StrA & result, ScanOption option = 0  )
    {
        StrA relpath;
        if( !readString( relpath, option ) ) return false;
        GN::fs::ResolvePath( result, filedir, relpath );
        return true;
    }

    //
    // Note: symbol is a word w/o quotes and spaces.
    // -----------------------------------------------------------------------------
    bool readSymbol( StrA & result, ScanOption option = 0  )
    {
        const char * s = Next( 0, option );
        if( 0 == s ) return false;
        if( ( 'a' <= *s && *s <= 'z' ) || ( 'A' <= *s && *s <= 'Z' ) || '_' == *s )
        {
            result = s;
            return true;
        }
        else
        {
            err( StringFormat( "Expect a symbol (start with [_a-zA-Z]), but met: %s", s ) );
            return false;
        }
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool readFloat( float & result, ScanOption option = 0  )
    {
        const char * token = Next( 0, option );

        if( String2Float( result, token ) ) return true;
        else if( 0 == StringCompare( "1.#QNB", token ) ) { result = 1.0f; return true; }
        else if( 0 == StringCompare( "-1.#QNB", token ) ) { result = -1.0f; return true; }
        else
        {
            if( !option.silence ) err( StringFormat( "Not valid float : %s", token ) );
            return false;
        }
    }

    //
    //
    // -----------------------------------------------------------------------------
    template<typename INT_TYPE>
    bool readInt( INT_TYPE & result, ScanOption option = 0  )
    {
        const char * token = Next( 0, option );
        if( 0 == token ) return false;

        if( 0 == String2Integer<INT_TYPE>( result, token ) )
        {
            if( !option.silence ) err( StringFormat( "Not valid integer : %s", token ) );
            return false;
        }

        return true;
    }

    //
    //
    // -----------------------------------------------------------------------------
    template<typename INT_TYPE>
    INT_TYPE readOptionalInt( INT_TYPE defval, ScanOption option = 0  )
    {
        INT_TYPE result;
        return readInt( result, option ) ? result : defval;
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool readVector3( Vector3f & result, ScanOption option = 0  )
    {
        return readFloat( result.x, option )
            && readFloat( result.y, option )
            && readFloat( result.z, option );
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool readIndexedVector3Node( const char * nodename, UInt32 index, Vector3f & result, ScanOption option = 0  )
    {
        GN_ASSERT( !IsStringEmpty(nodename) );
        return Next( nodename, option )
            && Next( StringFormat( "%d", index ).ToRawPtr(), option )
            && readVector3( result, option );
    }
};

//
//
// -----------------------------------------------------------------------------
static bool sReadMap( AseMap & m, AseFile & ase )
{
    GN_GUARD;

    if( !ase.readBlockStart() ) return false;

    const char * token;

    while( 0 != ( token = ase.Next() ) )
    {
        if( 0 == StringCompare( token, "*MAP_NAME" ) )
        {
            if( !ase.readString( m.name ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MAP_CLASS" ) )
        {
            if( !ase.readString( m.class_ ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MAP_SUBNO" ) )
        {
            if( !ase.readInt( m.subno ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MAP_AMOUNT" ) )
        {
            if( !ase.readFloat( m.amount ) ) return false;
        }
        else if( 0 == StringCompare( token, "*BITMAP" ) )
        {
            if( !ase.readAndResolveRelativePath( m.bitmap ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MAP_TYPE" ) )
        {
            if( !ase.readSymbol( m.type ) ) return false;
        }
        else if( 0 == StringCompare( token, "*UVW_U_OFFSET" ) )
        {
            if( !ase.readFloat( m.offset.u ) ) return false;
        }
        else if( 0 == StringCompare( token, "*UVW_V_OFFSET" ) )
        {
            if( !ase.readFloat( m.offset.v ) ) return false;
        }
        else if( 0 == StringCompare( token, "*UVW_U_TILING" ) )
        {
            if( !ase.readFloat( m.tiling.u ) ) return false;
        }
        else if( 0 == StringCompare( token, "*UVW_V_TILING" ) )
        {
            if( !ase.readFloat( m.tiling.v ) ) return false;
        }
        else if( 0 == StringCompare( token, "*UVW_ANGLE" ) )
        {
            if( !ase.readFloat( m.angle ) ) return false;
        }
        else if( 0 == StringCompare( token, "*UVW_BLUR" ) )
        {
            if( !ase.readFloat( m.blur ) ) return false;
        }
        else if( 0 == StringCompare( token, "*UVW_BLUR_OFFSET" ) )
        {
            if( !ase.readFloat( m.blur_offset ) ) return false;
        }
        // Note: this is 3dsmax ASE exporter bug. Should be MAP_NOISE_AMT
        else if( 0 == StringCompare( token, "*UVW_NOUSE_AMT" ) )
        {
            if( !ase.readFloat( m.noise_amt ) ) return false;
        }
        else if( 0 == StringCompare( token, "*UVW_NOISE_SIZE" ) )
        {
            if( !ase.readFloat( m.noise_size ) ) return false;
        }
        else if( 0 == StringCompare( token, "*UVW_NOISE_LEVEL" ) )
        {
            if( !ase.readInt( m.noise_level ) ) return false;
        }
        else if( 0 == StringCompare( token, "*UVW_NOISE_PHASE" ) )
        {
            if( !ase.readFloat( m.noise_phase ) ) return false;
        }
        else if( 0 == StringCompare( token, "*BITMAP_FILTER" ) )
        {
            if( !ase.readSymbol( m.filter ) ) return false;
        }
        else if( '*' == *token )
        {
            ase.verbose( StringFormat( "skip node %s", token ) );
            if( !ase.skipNode() ) return false;
        }
        else if( 0 == StringCompare( token, "}" ) )
        {
            // end of the block
            return true;
        }
        else
        {
            ase.err( StringFormat( "expecting node or close-brace, but met '%s'!", token ).ToRawPtr() );
            return false;
        }
    }

    // something wrong
    ase.err( "fail to get next token!" );
    return false;

    GN_UNGUARD;
}
//
//
// -----------------------------------------------------------------------------
static bool sReadMaterial( AseMaterialInternal & m, AseFile & ase )
{
    GN_GUARD;

    if( !ase.readBlockStart() ) return false;

    const char * token;

    while( 0 != ( token = ase.Next() ) )
    {
        GN_ASSERT( !IsStringEmpty( token ) );

        if( 0 == StringCompare( token, "*MATERIAL_NAME" ) )
        {
            if( !ase.readString( m.name ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MATERIAL_CLASS" ) )
        {
            if( !ase.readString( m.class_ ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MATERIAL_AMBIENT" ) )
        {
            if( !ase.readVector3( m.ambient ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MATERIAL_DIFFUSE" ) )
        {
            if( !ase.readVector3( m.diffuse ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MATERIAL_SPECULAR" ) )
        {
            if( !ase.readVector3( m.specular ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MATERIAL_SHINE" ) )
        {
            if( !ase.readFloat( m.shine ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MATERIAL_SHINESTRENGTH" ) )
        {
            if( !ase.readFloat( m.shinestrength ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MATERIAL_TRANSPARENCY" ) )
        {
            if( !ase.readFloat( m.transparency ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MATERIAL_WIRESIZE" ) )
        {
            if( !ase.readFloat( m.wiresize ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MATERIAL_SHADING" ) )
        {
            if( !ase.readSymbol( m.shading ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MATERIAL_XP_FALLOFF" ) )
        {
            if( !ase.readFloat( m.xp_falloff ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MATERIAL_SELFILLUM" ) )
        {
            if( !ase.readFloat( m.selfillum ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MATERIAL_FALLOFF" ) )
        {
            if( !ase.readSymbol( m.falloff ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MATERIAL_XP_TYPE" ) )
        {
            if( !ase.readSymbol( m.xp_type ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MAP_", 5 ) )
        {
            const char * map = token + 5;
            if( 0 == StringCompare( map, "DIFFUSE" ) )
            {
                if( !sReadMap( m.mapdiff, ase ) ) return false;
            }
            else if( 0 == StringCompare( map, "BUMP" ) )
            {
                if( !sReadMap( m.mapbump, ase ) ) return false;
            }
            else
            {
                ase.verbose( StringFormat( "skip unsupport map %s", token ) );
                if( !ase.skipNode() ) return false;
            }
        }
        else if( 0 == StringCompare( token, "*NUMSUBMTLS" ) )
        {
            UInt32 count;
            if( !ase.readInt( count ) ) return false;
            m.submaterials.Resize( count );

            // read sub-materials one by one
            for( UInt32 i = 0; i < count; ++i )
            {
                if( !ase.Next( "*SUBMATERIAL" ) ) return false;
                if( !ase.Next( StringFormat("%d",i).ToRawPtr() ) ) return false;
                if( !sReadMaterial( m.submaterials[i], ase ) ) return false;
            }
        }
        else if( '*' == *token )
        {
            ase.verbose( StringFormat( "skip node %s", token ) );
            if( !ase.skipNode() ) return false;
        }
        else if( 0 == StringCompare( token, "}" ) )
        {
            // end of the block
            return true;
        }
        else
        {
            ase.err( StringFormat( "expecting node or close-brace, but met '%s'!", token ).ToRawPtr() );
            return false;
        }
    }

    return false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadMaterials( AseSceneInternal & scene, AseFile & ase )
{
    GN_GUARD;

    ase.verbose( "Read materials ..." );

    if( !ase.readBlockStart() ) return false;

    // read material count
    if( !ase.Next( "*MATERIAL_COUNT" ) ) return false;
    UInt32 matcount;
    if( !ase.readInt( matcount ) ) return false;

    scene.materials.Resize( matcount );

    // read materials one by one
    for( UInt32 i = 0; i < matcount; ++i )
    {
        if( !ase.Next( "*MATERIAL" ) ) return false;
        if( !ase.Next( StringFormat("%d",i).ToRawPtr() ) ) return false;
        if( !sReadMaterial( scene.materials[i], ase ) ) return false;
    }

    // done
    return ase.readBlockEnd();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadMesh( AseMeshInternal & m, const Matrix44f & transform, AseFile & ase )
{
    GN_GUARD;

    if( !ase.readBlockStart() ) return false;

    if( !ase.Next( "*TIMEVALUE" ) || !ase.readInt( m.timevalue ) ) return false;

    UInt32 numvert, numface;
    if( !ase.Next( "*MESH_NUMVERTEX" ) || !ase.readInt( numvert ) ) return false;
    if( !ase.Next( "*MESH_NUMFACES" ) || !ase.readInt( numface ) ) return false;

    m.vertices.Resize( numvert );
    m.faces.Resize( numface );

    // read vertices
    if( !ase.Next( "*MESH_VERTEX_LIST" ) || !ase.readBlockStart() ) return false;
    for( UInt32 i = 0; i < numvert; ++i )
    {
        // Note: vertex position in ASE file is post-transformed.
        if( !ase.readIndexedVector3Node( "*MESH_VERTEX", i, m.vertices[i].p ) ) return false;
    }
    if( !ase.readBlockEnd() ) return false;

    // calculate mesh bounding box
    if( m.vertices.Size() > 0 )
    {
        CalculateBoundingBox( m.bbox, &m.vertices[0].p, sizeof(AseVertex), m.vertices.Size() );
    }
    else
    {
        m.bbox.x = m.bbox.y = m.bbox.z = m.bbox.w = m.bbox.h = m.bbox.d = .0f;
    }

    // read faces
    if( !ase.Next( "*MESH_FACE_LIST" ) || !ase.readBlockStart() ) return false;
    for( UInt32 i = 0; i < numface; ++i )
    {
        AseFace & f = m.faces[i];
        int dummy;
        if( !ase.Next( "*MESH_FACE" ) ) return false;
        if( !ase.Next( StringFormat( "%d:", i ).ToRawPtr() ) ) return false;
        if( !ase.Next( "A:" ) || !ase.readInt( f.v[0] ) ) return false;
        if( !ase.Next( "B:" ) || !ase.readInt( f.v[1] ) ) return false;
        if( !ase.Next( "C:" ) || !ase.readInt( f.v[2] ) ) return false;
        if( !ase.Next( "AB:" ) || !ase.readInt( dummy ) ) return false;
        if( !ase.Next( "BC:" ) || !ase.readInt( dummy ) ) return false;
        if( !ase.Next( "CA:" ) || !ase.readInt( dummy ) ) return false;
        if( !ase.Next( "*MESH_SMOOTHING" ) ) return false;
        if( !ase.nextIsNode() )
        {
            // then next must be smooth group ID
            if( !ase.readInt( f.smooth ) ) return false;
        }
        else
        {
            f.smooth = 0;
        }
        if( !ase.Next( "*MESH_MTLID" ) || !ase.readInt( f.submat ) ) return false;
    }
    if( !ase.readBlockEnd() ) return false;

    // read texcoords
    UInt numtexcoord;
    if( !ase.Next( "*MESH_NUMTVERTEX" ) || !ase.readInt( numtexcoord ) ) return false;
    if( numtexcoord > 0 )
    {
        DynaArray<Vector3f> texcoords( numtexcoord );

        if( !ase.Next( "*MESH_TVERTLIST" ) || !ase.readBlockStart() ) return false;
        for( UInt32 i = 0; i < numtexcoord; ++i )
        {
            if( !ase.readIndexedVector3Node( "*MESH_TVERT", i, texcoords[i] ) ) return false;

            // Y axis has be to reversed.
            texcoords[i].y = 1.0f - texcoords[i].y;

            // NOTE: currently, we only supports 2D texcoord
            texcoords[i].z = .0f;
        }
        if( !ase.readBlockEnd() ) return false;

        // read tface list
        if( !ase.Next( "*MESH_NUMTVFACES" ) || !ase.Next( StringFormat( "%d", numface ).ToRawPtr() ) ) return false;
        if( !ase.Next( "*MESH_TFACELIST" ) || !ase.readBlockStart() ) return false;
        for( UInt32 i = 0; i < numface; ++i )
        {
            AseFace & f = m.faces[i];

            if( !ase.Next( "*MESH_TFACE" ) ) return false;
            if( !ase.Next( StringFormat( "%d", i ).ToRawPtr() ) ) return false;

            // for each vertex in the face
            for( UInt32 i = 0; i < 3; ++i )
            {
                AseVertex & v = m.vertices[ f.v[i] ];

                // get the index into texcoord array
                UInt32 t;
                if( !ase.readInt( t ) ) return false;

                // add to vertex's texcoord array, store index in the face.
                f.t[i] = v.addTexcoord( texcoords[t] );
            }
        }
        if( !ase.readBlockEnd() ) return false;
    }
    else
    {
        GN_WARN(sLogger)( "Current mesh has no texcoord information. We'll have to fake it..." );

        Vector3f zero(0,0,0);

        for( UInt32 i = 0; i < m.vertices.Size(); ++i )
        {
            m.vertices[i].addTexcoord( zero );
        }

        for( UInt32 i = 0; i < m.faces.Size(); ++i )
        {
            AseFace & f = m.faces[i];
            f.t[0] = 0;
            f.t[1] = 0;
            f.t[2] = 0;
        }
    }

    // skip vertex colors
    UInt numcolor;
    if( !ase.Next( "*MESH_NUMCVERTEX" ) || !ase.readInt( numcolor ) ) return false;
    if( numcolor > 0 )
    {
        if( !ase.Next( "*MESH_CVERTLIST" ) ) return false;
        if( !ase.skipNode() ) return false;
    }

    // read normals
    if( !ase.Next( "*MESH_NORMALS" ) || !ase.readBlockStart() ) return false;
    Matrix44f it = Matrix44f::sInvTrans( transform ); // use to transform normal
    for( UInt32 i = 0; i < numface; ++i )
    {
        AseFace & f = m.faces[i];
        if( !ase.readIndexedVector3Node( "*MESH_FACENORMAL", i, f.fn ) ) return false;

        for( UInt32 i = 0; i < 3; ++i )
        {
            if( !ase.Next( "*MESH_VERTEXNORMAL" ) ) return false;

            UInt32 vi;
            if( !ase.readInt( vi ) ) return false;

            AseVertex & v = m.vertices[vi];

            Vector3f n;
            if( !ase.readVector3( n ) ) return false;

            f.vn[i] = v.addNormal( it.TransformVector( n ) );
        }
    }
    if( !ase.readBlockEnd() ) return false;

    // end of mesh block
    return ase.readBlockEnd();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadNode( AseNode & n, AseFile & ase )
{
    GN_GUARD;

    if( !ase.readBlockStart() ) return false;

    n.transform.Identity();

    const char * token;
    while( 0 != ( token = ase.Next() ) )
    {
        if( 0 ) {}
        else if( 0 == StringCompare( "*TM_ROW0", token ) )
        {
            Vector3f v;
            if( !ase.readVector3( v ) ) return false;
            n.transform[0][0] = v.x;
            n.transform[1][0] = v.y;
            n.transform[2][0] = v.z;
        }
        else if( 0 == StringCompare( "*TM_ROW1", token ) )
        {
            Vector3f v;
            if( !ase.readVector3( v ) ) return false;
            n.transform[0][1] = v.x;
            n.transform[1][1] = v.y;
            n.transform[2][1] = v.z;
        }
        else if( 0 == StringCompare( "*TM_ROW2", token ) )
        {
            Vector3f v;
            if( !ase.readVector3( v ) ) return false;
            n.transform[0][2] = v.x;
            n.transform[1][2] = v.y;
            n.transform[2][2] = v.z;
        }
        else if( 0 == StringCompare( "*TM_ROW3", token ) )
        {
            Vector3f v;
            if( !ase.readVector3( v ) ) return false;
            n.transform[0][3] = v.x;
            n.transform[1][3] = v.y;
            n.transform[2][3] = v.z;
        }
        else if( 0 == StringCompare( "*TM_POS", token ) )
        {
            if( !ase.readVector3( n.pos ) ) return false;
        }
        else if( 0 == StringCompare( "*TM_ROTAXIS", token ) )
        {
            if( !ase.readVector3( n.rotaxis ) ) return false;
        }
        else if( 0 == StringCompare( "*TM_ROTANGLE", token ) )
        {
            if( !ase.readFloat( n.rotangle ) ) return false;
        }
        else if( 0 == StringCompare( "*TM_SCALE", token ) )
        {
            if( !ase.readVector3( n.scale ) ) return false;
        }
        else if( '*' == *token )
        {
            ase.verbose( StringFormat( "skip node %s", token ) );
            if( !ase.skipNode() ) return false;
        }
        else if( 0 == StringCompare( token, "}" ) )
        {
            // end of the block. done.
            return true;
        }
    }

    // something wrong!
    ase.err( "Fail to get next node!" );
    return false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadGeomObject( AseSceneInternal & scene, AseFile & ase )
{
    GN_GUARD;

    scene.objects.Resize( scene.objects.Size() + 1 );
    AseGeoObject & o = scene.objects.Back();

    if( !ase.readBlockStart() ) return false;

    bool hasMaterial = false;

    const char * token;
    while( 0 != ( token = ase.Next() ) )
    {
        if( 0 == StringCompare( token, "*NODE_NAME" ) )
        {
            o.node.name = ase.readString();
            if( o.node.name.Empty() )
            {
                ase.err( "Node name can't be empty!" );
                return false;
            }
            ase.verbose( StringFormat( "read geometry object '%s' ...", o.node.name.ToRawPtr() ) );
        }
        else if( 0 == StringCompare( token, "*NODE_PARENT" ) )
        {
            o.node.parent = ase.readString();
        }
        else if( 0 == StringCompare( token, "*NODE_TM" ) )
        {
            if( !sReadNode( o.node, ase ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MESH" ) )
        {
            if( !sReadMesh( o.mesh, o.node.transform, ase ) ) return false;
        }
        else if( 0 == StringCompare( token, "*MATERIAL_REF" ) )
        {
            if( !ase.readInt( o.matid ) ) return false;
            if( o.matid >= scene.materials.Size() )
            {
                ase.err( "material ID is out of range!" );
                return false;
            }
            hasMaterial = true;
        }
        else if( '*' == *token )
        {
            ase.verbose( StringFormat( "skip node %s", token ) );
            if( !ase.skipNode() ) return false;
        }
        else if( 0 == StringCompare( token, "}" ) )
        {
            // end of the block. do some post processing.

            AseMeshInternal & m = o.mesh;

            if( !hasMaterial )
            {
                ase.warn( StringFormat( "object '%s' has no material. Using default one.", o.node.name.ToRawPtr() ) );
                o.matid = 0;
            }

            AseMaterialInternal & mtl = scene.materials[o.matid];

            // build face chunk array
            if( "Multi/Sub-Object" == mtl.class_ )
            {
                m.chunks.Reserve( mtl.submaterials.Size() );

                for( UInt32 i = 0; i < m.faces.Size(); ++i )
                {
                    const AseFace & f = m.faces[i];

                    UInt32 cid;
                    for( cid = 0; cid < m.chunks.Size(); ++cid )
                    {
                        AseFaceChunk & c = m.chunks[cid];

                        if( c.submat == f.submat ) break;
                    }
                    GN_ASSERT( cid <= m.chunks.Size() );

                    if( cid == m.chunks.Size() )
                    {
                        m.chunks.Resize( m.chunks.Size() + 1 );
                        m.chunks.Back().submat = f.submat;
                    }

                    m.chunks[cid].faces.Append( i );


                }
            }
            else
            {
                m.chunks.Resize( 1 );
                m.chunks[0].submat = 0;
                m.chunks[0].faces.Resize( m.faces.Size() );
                for( UInt32 i = 0; i < m.faces.Size(); ++i )
                {
                    m.chunks[0].faces[i] = i;
                }
            }

            // success
            return true;
        }
        else
        {
            ase.err( StringFormat( "expecting node or close-brace, but met '%s'!", token ).ToRawPtr() );
            return false;
        }
    }

    // something wrong!
    ase.err( "Fail to get next node!" );
    return false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadGroup( AseSceneInternal & scene, AseFile & ase )
{
    GN_GUARD;

    if( !ase.readBlockStart() ) return false;

    const char * token;

    while( 0 != ( token = ase.Next() ) )
    {
        if( 0 ) {}
        //else if( 0 == StringCompare( token, "*SCENE" ) )
        //{
        //    ...
        //}
        else if( 0 == StringCompare( token, "*GEOMOBJECT" ) )
        {
            if( !sReadGeomObject( scene, ase ) ) return false;
        }
        else if( '*' == *token )
        {
            ase.verbose( StringFormat( "skip node %s", token ) );
            if( !ase.skipNode() ) return false;
        }
        else if( 0 == StringCompare( token, "}" ) )
        {
            // end of the block.
            return true;
        }
        else
        {
            ase.err( StringFormat( "expecting node token, but met '%s'.", token ) );
            return false;
        }
    }

    // something wrong!
    ase.err( "Fail to get next node!" );
    return false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadAse( AseSceneInternal & scene, File & file )
{
    GN_SCOPE_PROFILER( sReadAse, "Read ASE" );

    GN_GUARD;

    // open ase file
    AseFile ase;
    if( !ase.open( file ) ) return false;

    // read ASE version
    const char * token = ase.Next( "*3DSMAX_ASCIIEXPORT" );
    if( !token ) return false;
    if( 200 != ase.readOptionalInt( 0 ) )
    {
        GN_ERROR(sLogger)( "Not support ASE version other than 2.00!" );
        return false;
    }

    while( 0 != ( token = ase.Next() ) )
    {
        if( 0 ) {}
        //else if( 0 == StringCompare( token, "*SCENE" ) )
        //{
        //    ...
        //}
        else if( 0 == StringCompare( token, "*MATERIAL_LIST" ) )
        {
            if( !sReadMaterials( scene, ase ) ) return false;
        }
        else if( 0 == StringCompare( token, "*GROUP" ) )
        {
            if( !ase.readString() ) return false; // skip group name
            if( !sReadGroup( scene, ase ) ) return false;
        }
        else if( 0 == StringCompare( token, "*GEOMOBJECT" ) )
        {
            if( !sReadGeomObject( scene, ase ) ) return false;
        }
        else if( '*' == *token )
        {
            ase.verbose( StringFormat( "skip node %s", token ) );
            if( !ase.skipNode() ) return false;
        }
        else
        {
            ase.err( StringFormat( "expecting node token, but met '%s'.", token ) );
            return false;
        }
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static AseGeoObject * sFindGeoObject( AseSceneInternal & scene, const StrA & name )
{
    if( name.Empty() ) return &scene.root;
    for( AseGeoObject * o = scene.objects.Begin(); o != scene.objects.End(); ++o )
    {
        if( name == o->node.name ) return o;
    }
    return 0;
}

//
//
// -----------------------------------------------------------------------------
static bool sBuildNodeTree( AseSceneInternal & scene )
{
    GN_SCOPE_PROFILER( sBuildNodeTree, "Build ASE node tree" );

    GN_VERBOSE(sLogger)( "\nASE: Build node tree:" );

    // setup root node
    scene.root.node.name = "root";
    scene.root.node.pos.Set( 0, 0, 0 );
    scene.root.node.rotaxis.Set( 0, 0, 1 );
    scene.root.node.rotangle = 0;
    scene.root.node.scale.Set( 1, 1, 1 );
    scene.root.node.transform.Identity();
    scene.root.mesh.bbox.Size().Set( 0, 0, 0 );

    // build node tree
    for( size_t i = 0; i < scene.objects.Size(); ++i )
    {
        AseGeoObject & o = scene.objects[i];

        AseGeoObject * p = sFindGeoObject( scene, o.node.parent );

        if( 0 == p )
        {
            GN_ERROR(sLogger)( "Object %s has invalid parent: %s. Replace it with \"root\".",
                o.node.name.ToRawPtr(), o.node.parent.ToRawPtr() );
            p = &scene.root;
        }

        o.SetParent( p, 0 );
    }

    // make sure all objects are linked into the tree.
    GN_ASSERT_EX(
        scene.root.CalcChildrenCount() == scene.objects.Size(),
        StringFormat( "numchildren=%d, numobjects=%d",
            scene.root.CalcChildrenCount(), scene.objects.Size() ).ToRawPtr() );

    // calculate bounding box for each node, in post order
    TreeTraversePostOrder<AseGeoObject> ttpost( &scene.root );

    AseGeoObject * n = ttpost.First();
    GN_ASSERT( 0 == n->GetFirstChild() );

    while( n )
    {
        // copy mesh bbox to node
        n->node.selfbbox = n->mesh.bbox;
        n->node.treebbox = n->mesh.bbox;

        // then merge with all childrens' bbox
        AseGeoObject * c = SafeCastPtr<AseGeoObject>( n->GetFirstChild() );
        while( c )
        {
            Boxf::sGetUnion( n->node.treebbox, n->node.treebbox, c->node.treebbox );

            c = SafeCastPtr<AseGeoObject>( c->GetNextSibling() );
        }

        // next node
        n = ttpost.Next( n );
    }

    // print node tree
    TreeTraversePreOrder<AseGeoObject> ttpre( &scene.root );
    n = ttpre.First();
    int level = 0;
    while( n )
    {
        StrA s( "    " );

        for( int i = 0; i < level; ++i ) s += "- ";
        s += StringFormat(
            "%s : bbox_pos(%f,%f,%f), bbox_size(%f,%f,%f)",
            n->node.name.ToRawPtr(),
            n->node.selfbbox.Pos().x,
            n->node.selfbbox.Pos().y,
            n->node.selfbbox.Pos().z,
            n->node.selfbbox.Size().x,
            n->node.selfbbox.Size().y,
            n->node.selfbbox.Size().z );

        GN_VERBOSE(sLogger)( s.ToRawPtr() );

        // next node
        n = ttpre.Next( n, &level );
    }

    // Output an empty line
    GN_VERBOSE(sLogger)( "" );

    // success
    return true;
}

/// unique vertex selector
struct VertexSelector
{
    UInt32 p; ///< position (index into AseMeshInternal.vertices)
    UInt32 t; ///< texcoord (index into AseVertex.t)
    UInt32 n; ///< normal   (index into AseVertex.n)

    friend bool operator==( const VertexSelector & a, const VertexSelector & b )
    {
        return a.p == b.p && a.t == b.t && a.n == b.n;
    }

    struct Hash
    {
        UInt64 operator()( const VertexSelector & vs ) const
        {
            UInt64 h = ( ( ((UInt64)vs.p) << 32 ) | vs.t ) ^ ( ((UInt64)vs.n) << 16 );
            return h;
        }
    };
};

/// collection of unique items
template<typename T>
class ElementCollection
{
    typedef GN::HashMap<T,UInt32,typename T::Hash> TypeMap;

    TypeMap      mMap;
    DynaArray<T> mBuffer;

public:

    ///
    /// add element into buffer, ignore redundant element.
    ///
    UInt32 add( const T & element )
    {
        TypeMap::KeyValuePair * p = mMap.Insert( element, 0xbad );

        if( p )
        {
            // this is a new element
            GN_ASSERT( 0xbad == p->value );
            GN_ASSERT( mBuffer.Size() + 1 == mMap.Size() );

            p->value = (UInt32)( mBuffer.Size() );

            mBuffer.Append( element );

            return p->value;
        }
        else
        {
            return mMap[element];
        }
    }

    ///
    /// get number of vertices in buffer
    ///
    size_t Size() const { GN_ASSERT( mMap.Size() == mBuffer.Size() ); return mBuffer.Size(); }

    ///
    /// return specific element
    ///
    const T & operator[]( size_t idx ) const
    {
        GN_ASSERT( idx < mBuffer.Size() );
        return mBuffer[idx];
    }

    ///
    /// return specific element
    ///
    T & operator[]( size_t idx )
    {
        GN_ASSERT( idx < mBuffer.Size() );
        return mBuffer[idx];
    }
};
typedef ElementCollection<VertexSelector> VertexCollection;

//
//
// -----------------------------------------------------------------------------
static bool operator==( const AseMaterial & a, const AseMaterial & b )
{
    // TODO: check all fields.
    return a.name == b.name;
}

//
//
// -----------------------------------------------------------------------------
static UInt32
sGetFaceChunkMatID(
    AseScene               & dst,
    const AseSceneInternal & src,
    UInt32                   matid,
    UInt32                   submat )
{
    const AseMaterial * mat;

    if( "Multi/Sub-Object" == src.materials[matid].class_ )
    {
        mat = &src.materials[matid].submaterials[submat];
    }
    else
    {
        mat = &src.materials[matid];
    }

    for( UInt32 i = 0; i < dst.materials.Size(); ++i )
    {
        if( dst.materials[i] == *mat )
        {
            // this is a existing material
            return i;
        }
    }

    // this is a new material
    UInt32 newidx = (UInt32)dst.materials.Size();
    dst.materials.Resize( dst.materials.Size() + 1 );
    dst.materials.Back() = *mat;
    return newidx;
}

struct OutputVertex
{
    Vector3f position;
    Vector3f normal;
    Vector2f texcoord;
};

//
//
// -----------------------------------------------------------------------------
static bool sWriteGeoObject( AseScene & dst, const AseSceneInternal & src, const AseGeoObject & obj )
{
    GN_GUARD;

    dst.meshes.Resize( dst.meshes.Size() + 1 );

    AseMesh & dstmesh = dst.meshes.Back();

    // setup hierarchy and transformation properties
    dstmesh.parent = obj.node.parent;
    dstmesh.name = obj.node.name;
    dstmesh.transform = obj.node.transform;
    dstmesh.pos = obj.node.pos;
    dstmesh.rotaxis = obj.node.rotaxis;
    dstmesh.rotangle = obj.node.rotangle;
    dstmesh.scale = obj.node.scale;
    dstmesh.selfbbox = obj.node.selfbbox;
    dstmesh.treebbox = obj.node.treebbox;

    // setup constant mesh properties
    dstmesh.vtxfmt.numElements = 3;
    dstmesh.vtxfmt.elements[0].format = ColorFormat::FLOAT3;
    dstmesh.vtxfmt.elements[0].offset = 0;
    dstmesh.vtxfmt.elements[0].BindTo( "position", 0 );
    dstmesh.vtxfmt.elements[1].format = ColorFormat::FLOAT3;
    dstmesh.vtxfmt.elements[1].offset = 12;
    dstmesh.vtxfmt.elements[1].BindTo( "normal", 0 );
    dstmesh.vtxfmt.elements[2].format = ColorFormat::FLOAT2;
    dstmesh.vtxfmt.elements[2].offset = 24;
    dstmesh.vtxfmt.elements[2].BindTo( "texcoord", 0 );
    dstmesh.prim = PrimitiveType::TRIANGLE_LIST;
    dstmesh.strides[0] = sizeof(OutputVertex);

    // generate mesh
    VertexCollection  vc;
    VertexSelector    vs;
    DynaArray<UInt32> ib; // index into vertex collection
    for( size_t i = 0; i < obj.mesh.chunks.Size(); ++i )
    {
        const AseFaceChunk & c = obj.mesh.chunks[i];

        dst.subsets.Resize( dst.subsets.Size() + 1 );
        AseMeshSubset & subset = dst.subsets.Back();

        subset.matid    = sGetFaceChunkMatID( dst, src, obj.matid, c.submat );
        subset.meshid   = dst.meshes.Size() - 1;
        subset.startidx = ib.Size();
        subset.numidx   = c.faces.Size() * 3;

        UInt32 minidx = 0xFFFFFFFF;
        UInt32 maxidx = 0;

        for( size_t i = 0; i < c.faces.Size(); ++i )
        {
            const AseFace & f = obj.mesh.faces[c.faces[i]];

            for( size_t i = 0; i < 3; ++i )
            {
                vs.p = f.v[i];
                vs.t = f.t[i];
                vs.n = f.vn[i];

                UInt32 idx = vc.add( vs );
                ib.Append( idx );

                if( idx < minidx ) minidx = idx;
                if( idx > maxidx ) maxidx = idx;
            }
        }

        subset.basevtx = 0;
        subset.numvtx  = maxidx + 1;
    }

    // copy vertices into destination scene
    AutoRef<Blob> blob( new SimpleBlob(sizeof(OutputVertex) * vc.Size()) );
    OutputVertex * vertices = (OutputVertex*)blob->Data();
    if( NULL == vertices ) return false;
    for( size_t i = 0; i < vc.Size(); ++i )
    {
        const VertexSelector & vs = vc[i];

        const AseVertex & srcvert = obj.mesh.vertices[vs.p];

        const Vector3f & srctexcoord = srcvert.t[vs.t];

        OutputVertex & o = vertices[i];

        o.position = srcvert.p;
        o.normal   = srcvert.n[vs.n];
        o.texcoord = Vector2f( srctexcoord.x, srctexcoord.y );
    }
    dstmesh.numvtx = vc.Size();
    dstmesh.vertices[0] = vertices;
    dst.meshdata.Append( blob );

    // copy index data into destination scene
    dstmesh.numidx = ib.Size();
    if( vc.Size() > 0x10000 )
    {
        // 32bit index buffer
        blob.Attach( new SimpleBlob(sizeof(UInt32) * ib.Size()) );
        memcpy( blob->Data(), ib.ToRawPtr(), blob->Size() );
        dstmesh.idx32 = true;
        dstmesh.indices = blob->Data();
        dst.meshdata.Append( blob );
    }
    else
    {
        // 16bit index buffer
        blob.Attach( new SimpleBlob(sizeof(UInt16) * ib.Size()) );
        UInt16 * idx16 = (UInt16*)blob->Data();
        for( size_t i = 0; i < ib.Size(); ++i )
        {
            GN_ASSERT( ib[i] < 0x10000 );
            idx16[i] = (UInt16)ib[i];
        }
        dstmesh.idx32 = false;
        dstmesh.indices = idx16;
        dst.meshdata.Append( blob );
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sWriteScene( AseScene & dst, const AseSceneInternal & src )
{
    GN_SCOPE_PROFILER( sWriteScene, "Write ASE scene" );

    GN_GUARD;

    for( size_t i = 0; i < src.objects.Size(); ++i )
    {
        sWriteGeoObject( dst, src, src.objects[i] );
    }

    dst.bbox = src.root.node.treebbox;

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// public function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::util::AseScene::Clear()
{
    materials.Clear();
    meshes.Clear();
    subsets.Clear();
    meshdata.Clear();
}

//
//
// -----------------------------------------------------------------------------
bool GN::util::loadAseSceneFromFile( AseScene & scene, File & file )
{
    GN_SCOPE_PROFILER( loadAseSceneFromFile, "Load ASE scene from file" );
    AseSceneInternal internal;
    if( !sReadAse( internal, file ) ) return false;
    if( !sBuildNodeTree( internal ) ) return false;
    if( !sWriteScene( scene, internal ) ) return false;
    return true;
}
