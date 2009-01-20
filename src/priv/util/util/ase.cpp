#include "pch.h"
#include "garnet/GNutil.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.util.ase");

///
/// ASE file structure
///
struct AseFile
{
    DynaArray<char> buf;
    char *          str;
    int             line;

    bool open( File & file )
    {
        // read ASE file
        buf.resize( file.size() + 1 );
        size_t readen;
        buf.resize( file.size() );
        if( !file.read( buf.cptr(), file.size(), &readen ) )
        {
            return false;
        }
        GN_ASSERT( readen <= file.size() );
        buf[readen] = 0;

        str = buf.cptr();
        line = 0;

        // success
        return true;
    }

    void err( const StrA & msg ) { GN_ERROR(sLogger)( "line %d : %s", line, msg.cptr() ); }
    void warn( const StrA & msg ) { GN_WARN(sLogger)( "line %d : %s", line, msg.cptr() ); }
    void info( const StrA & msg ) { GN_INFO(sLogger)( "line %d : %s", line, msg.cptr() ); }

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

    ///
    /// skip white spaces
    ///
    void skipWhiteSpaces()
    {
        GN_ASSERT( str );
        while( ' ' == *str || '\t' == *str || '\n' == *str )
        {
            if( '\n' == *str ) ++line;
            ++str;
        }
    }

    ///
    /// get next token
    ///
    const char * next( const char * expectedValue = 0, ScanOption option = 0 )
    {
        GN_ASSERT( str );

        if( 0 == str || 0 == *str )
        {
            if( !option.silence ) info( "EOF" );
            return 0;
        }

        skipWhiteSpaces();

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

            if( expectedValue && 0 != strCmp( expectedValue, r ) )
            {
                if( !option.silence )
                {
                    err( strFormat( "expect '%s', but found '%s'.", expectedValue, r ) );
                }
                return 0;
            }

            // success
            return r;

        }
        else
        {
            const char * r = str;

            while( ' ' != *str && '\t' != *str && '\n' != *str && 0 != *str )
            {
                ++str;
            }

            if( '\n' == *str ) ++line;

            if( 0 != *str ) *str = 0, ++str; // point to start of next token

            if( expectedValue && 0 != strCmp( expectedValue, r ) )
            {
                if( !option.silence )
                {
                    err( strFormat( "expect '%s', but found '%s'.", expectedValue, r ) );
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
        GN_ASSERT( !strEmpty( endtoken ) );

        const char * token;
        int level = 0;
        for(;;)
        {
            token = next();

            if( 0 == strCmp( "{", token ) ) ++level;
            else if( 0 == strCmp( "}", token ) ) --level;

            if( IN_CURRENT_BLOCK == option.scope && level > 0 ) continue; // skip sub levels

            if( IN_CURRENT_AND_CHILD_BLOCKS == option.scope && level < 0 )
            {
                if( !option.silence ) err( strFormat( "token '%s' not found inside current block!", endtoken ) );
                return false;
            }

            if( 0 == token )
            {
                if( !option.silence ) err( strFormat( "token '%s' not found!", endtoken ) );
                return false;
            }

            if( 0 == strCmp( endtoken, token ) ) return true;
        }
    }

    //
    // True, if next token is a node (begin with '*')
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
            if( '*' == *str || '}' == *str ) return true; // found next node
        } while( next( 0, option ) );
        return false;
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool readBlockStart( ScanOption option = 0 ) { return 0 != next( "{", option ); }

    //
    //
    // -----------------------------------------------------------------------------
    bool readBlockEnd( ScanOption option = 0 ) { return 0 != next( "}", option ); }

    //
    //
    // -----------------------------------------------------------------------------
    const char * readNode( ScanOption option = 0  )
    {
        const char * token = next( 0, option );
        if( !token ) return false;
        GN_ASSERT( !strEmpty( token ) );

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
        char * token = const_cast<char*>( next( 0, option ) );
        if( !token ) return false;
        GN_ASSERT( !strEmpty( token ) );

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
    // Note: symbol is a word w/o quotes and spaces.
    // -----------------------------------------------------------------------------
    bool readSymbol( StrA & result, ScanOption option = 0  )
    {
        const char * s = next( 0, option );
        if( 0 == s ) return false;
        if( 'a' <= *s && *s <= 'z' || 'A' <= *s && *s <= 'Z' || '_' == *s )
        {
            result = s;
            return true;
        }
        else
        {
            err( strFormat( "Expect a symbol (start with [_a-zA-Z]), but met: %s", s ) );
            return false;
        }
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool readFloat( float & result, ScanOption option = 0  )
    {
        const char * token = next( 0, option );

        if( str2Float( result, token ) ) return true;
        else if( 0 == strCmp( "1.#QNB", token ) ) { result = 1.0f; return true; }
        else if( 0 == strCmp( "-1.#QNB", token ) ) { result = -1.0f; return true; }
        else
        {
            if( !option.silence ) err( strFormat( "Not valid float : %s", token ) );
            return false;
        }
    }

    //
    //
    // -----------------------------------------------------------------------------
    template<typename INT_TYPE>
    bool readInt( INT_TYPE & result, ScanOption option = 0  )
    {
        const char * token = next( 0, option );
        if( 0 == token ) return false;

        if( !str2Int<INT_TYPE>( result, token ) )
        {
            if( !option.silence ) err( strFormat( "Not valid integer : %s", token ) );
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
        GN_ASSERT( !strEmpty(nodename) );
        return next( nodename, option )
            && next( strFormat( "%d", index ).cptr(), option )
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

    while( 0 != ( token = ase.next() ) )
    {
        if( 0 == strCmp( token, "*MAP_NAME" ) )
        {
            if( !ase.readString( m.name ) ) return false;
        }
        else if( 0 == strCmp( token, "*MAP_CLASS" ) )
        {
            if( !ase.readString( m.class_ ) ) return false;
        }
        else if( 0 == strCmp( token, "*MAP_SUBNO" ) )
        {
            if( !ase.readInt( m.subno ) ) return false;
        }
        else if( 0 == strCmp( token, "*MAP_AMOUNT" ) )
        {
            if( !ase.readFloat( m.amount ) ) return false;
        }
        else if( 0 == strCmp( token, "*BITMAP" ) )
        {
            if( !ase.readString( m.bitmap ) ) return false;
        }
        else if( 0 == strCmp( token, "*MAP_TYPE" ) )
        {
            if( !ase.readSymbol( m.type ) ) return false;
        }
        else if( 0 == strCmp( token, "*UVW_U_OFFSET" ) )
        {
            if( !ase.readFloat( m.offset.u ) ) return false;
        }
        else if( 0 == strCmp( token, "*UVW_V_OFFSET" ) )
        {
            if( !ase.readFloat( m.offset.v ) ) return false;
        }
        else if( 0 == strCmp( token, "*UVW_U_TILING" ) )
        {
            if( !ase.readFloat( m.tiling.u ) ) return false;
        }
        else if( 0 == strCmp( token, "*UVW_V_TILING" ) )
        {
            if( !ase.readFloat( m.tiling.v ) ) return false;
        }
        else if( 0 == strCmp( token, "*UVW_ANGLE" ) )
        {
            if( !ase.readFloat( m.angle ) ) return false;
        }
        else if( 0 == strCmp( token, "*UVW_BLUR" ) )
        {
            if( !ase.readFloat( m.blur ) ) return false;
        }
        else if( 0 == strCmp( token, "*UVW_BLUR_OFFSET" ) )
        {
            if( !ase.readFloat( m.blur_offset ) ) return false;
        }
        // Note: this is 3dsmax ASE exporter bug. Should be MAP_NOISE_AMT
        else if( 0 == strCmp( token, "*UVW_NOUSE_AMT" ) )
        {
            if( !ase.readFloat( m.noise_amt ) ) return false;
        }
        else if( 0 == strCmp( token, "*UVW_NOISE_SIZE" ) )
        {
            if( !ase.readFloat( m.noise_size ) ) return false;
        }
        else if( 0 == strCmp( token, "*UVW_NOISE_LEVEL" ) )
        {
            if( !ase.readInt( m.noise_level ) ) return false;
        }
        else if( 0 == strCmp( token, "*UVW_NOISE_PHASE" ) )
        {
            if( !ase.readFloat( m.noise_phase ) ) return false;
        }
        else if( 0 == strCmp( token, "*BITMAP_FILTER" ) )
        {
            if( !ase.readSymbol( m.filter ) ) return false;
        }
        else if( '*' == *token )
        {
            ase.info( strFormat( "skip node %s", token ) );
            if( !ase.skipNode() ) return false;
        }
        else if( 0 == strCmp( token, "}" ) )
        {
            // end of the block
            return true;
        }
        else
        {
            ase.err( strFormat( "expecting node or close-brace, but met '%s'!", token ).cptr() );
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
static bool sReadMaterial( AseMaterial & m, AseFile & ase )
{
    GN_GUARD;

    if( !ase.readBlockStart() ) return false;

    const char * token;

    while( 0 != ( token = ase.next() ) )
    {
        GN_ASSERT( !strEmpty( token ) );

        if( 0 == strCmp( token, "*MATERIAL_NAME" ) )
        {
            if( !ase.readString( m.name ) ) return false;
        }
        else if( 0 == strCmp( token, "*MATERIAL_CLASS" ) )
        {
            if( !ase.readString( m.class_ ) ) return false;
        }
        else if( 0 == strCmp( token, "*MATERIAL_AMBIENT" ) )
        {
            if( !ase.readVector3( m.ambient ) ) return false;
        }
        else if( 0 == strCmp( token, "*MATERIAL_DIFFUSE" ) )
        {
            if( !ase.readVector3( m.diffuse ) ) return false;
        }
        else if( 0 == strCmp( token, "*MATERIAL_SPECULAR" ) )
        {
            if( !ase.readVector3( m.specular ) ) return false;
        }
        else if( 0 == strCmp( token, "*MATERIAL_SHINE" ) )
        {
            if( !ase.readFloat( m.shine ) ) return false;
        }
        else if( 0 == strCmp( token, "*MATERIAL_SHINESTRENGTH" ) )
        {
            if( !ase.readFloat( m.shinestrength ) ) return false;
        }
        else if( 0 == strCmp( token, "*MATERIAL_TRANSPARENCY" ) )
        {
            if( !ase.readFloat( m.transparency ) ) return false;
        }
        else if( 0 == strCmp( token, "*MATERIAL_WIRESIZE" ) )
        {
            if( !ase.readFloat( m.wiresize ) ) return false;
        }
        else if( 0 == strCmp( token, "*MATERIAL_SHADING" ) )
        {
            if( !ase.readSymbol( m.shading ) ) return false;
        }
        else if( 0 == strCmp( token, "*MATERIAL_XP_FALLOFF" ) )
        {
            if( !ase.readFloat( m.xp_falloff ) ) return false;
        }
        else if( 0 == strCmp( token, "*MATERIAL_SELFILLUM" ) )
        {
            if( !ase.readFloat( m.selfillum ) ) return false;
        }
        else if( 0 == strCmp( token, "*MATERIAL_FALLOFF" ) )
        {
            if( !ase.readSymbol( m.falloff ) ) return false;
        }
        else if( 0 == strCmp( token, "*MATERIAL_XP_TYPE" ) )
        {
            if( !ase.readSymbol( m.xp_type ) ) return false;
        }
        else if( 0 == strCmp( token, "*MAP_", 5 ) )
        {
            const char * map = token + 5;
            if( 0 == strCmp( map, "DIFFUSE" ) )
            {
                if( !sReadMap( m.mapdiff, ase ) ) return false;
            }
            else if( 0 == strCmp( map, "BUMP" ) )
            {
                if( !sReadMap( m.mapbump, ase ) ) return false;
            }
            else
            {
                ase.info( strFormat( "skip unsupport map %s", token ) );
                if( !ase.skipNode() ) return false;
            }
        }
        else if( 0 == strCmp( token, "*NUMSUBMTLS" ) )
        {
            UInt32 count;
            if( !ase.readInt( count ) ) return false;
            m.allocSubMaterials( count );

            // read sub-materials one by one
            for( UInt32 i = 0; i < count; ++i )
            {
                if( !ase.next( "*SUBMATERIAL" ) ) return false;
                if( !ase.next( strFormat("%d",i).cptr() ) ) return false;
                if( !sReadMaterial( m.submaterials[i], ase ) ) return false;
            }
        }
        else if( '*' == *token )
        {
            ase.info( strFormat( "skip node %s", token ) );
            if( !ase.skipNode() ) return false;
        }
        else if( 0 == strCmp( token, "}" ) )
        {
            // end of the block
            return true;
        }
        else
        {
            ase.err( strFormat( "expecting node or close-brace, but met '%s'!", token ).cptr() );
            return false;
        }
    }

    return false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadMaterials( AseScene & scene, AseFile & ase )
{
    GN_GUARD;

    ase.info( "Read materials ..." );

    if( !ase.readBlockStart() ) return false;

    // read material count
    if( !ase.next( "*MATERIAL_COUNT" ) ) return false;
    UInt32 matcount;
    if( !ase.readInt( matcount ) ) return false;

    scene.materials.resize( matcount );

    // read materials one by one
    for( UInt32 i = 0; i < matcount; ++i )
    {
        if( !ase.next( "*MATERIAL" ) ) return false;
        if( !ase.next( strFormat("%d",i).cptr() ) ) return false;
        if( !sReadMaterial( scene.materials[i], ase ) ) return false;
    }

    // done
    return ase.readBlockEnd();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadMesh( AseMesh & m, const Matrix44f & transform, AseFile & ase )
{
    GN_GUARD;

    if( !ase.readBlockStart() ) return false;

    if( !ase.next( "*TIMEVALUE" ) || !ase.readInt( m.timevalue ) ) return false;

    UInt32 numvert, numface;
    if( !ase.next( "*MESH_NUMVERTEX" ) || !ase.readInt( numvert ) ) return false;
    if( !ase.next( "*MESH_NUMFACES" ) || !ase.readInt( numface ) ) return false;

    m.vertices.resize( numvert );
    m.faces.resize( numface );

    // read vertices
    if( !ase.next( "*MESH_VERTEX_LIST" ) || !ase.readBlockStart() ) return false;
    for( UInt32 i = 0; i < numvert; ++i )
    {
        // Note: vertex position in ASE file is post-transformed.
        if( !ase.readIndexedVector3Node( "*MESH_VERTEX", i, m.vertices[i].p ) ) return false;
    }
    if( !ase.readBlockEnd() ) return false;

    // calculate mesh bounding box
    if( m.vertices.size() > 0 )
    {
        calcBoundingBox( m.bbox, &m.vertices[0].p, m.vertices.size(), sizeof(AseVertex) );
    }
    else
    {
        m.bbox.x = m.bbox.y = m.bbox.z = m.bbox.w = m.bbox.h = m.bbox.d = .0f;
    }

    // read faces
    if( !ase.next( "*MESH_FACE_LIST" ) || !ase.readBlockStart() ) return false;
    for( UInt32 i = 0; i < numface; ++i )
    {
        AseFace & f = m.faces[i];
        int dummy;
        if( !ase.next( "*MESH_FACE" ) ) return false;
        if( !ase.next( strFormat( "%d:", i ).cptr() ) ) return false;
        if( !ase.next( "A:" ) || !ase.readInt( f.v[0] ) ) return false;
        if( !ase.next( "B:" ) || !ase.readInt( f.v[1] ) ) return false;
        if( !ase.next( "C:" ) || !ase.readInt( f.v[2] ) ) return false;
        if( !ase.next( "AB:" ) || !ase.readInt( dummy ) ) return false;
        if( !ase.next( "BC:" ) || !ase.readInt( dummy ) ) return false;
        if( !ase.next( "CA:" ) || !ase.readInt( dummy ) ) return false;
        if( !ase.next( "*MESH_SMOOTHING" ) ) return false;
        if( !ase.nextIsNode() )
        {
            // then next must be smooth group ID
            if( !ase.readInt( f.smooth ) ) return false;
        }
        else
        {
            f.smooth = 0;
        }
        if( !ase.next( "*MESH_MTLID" ) || !ase.readInt( f.submat ) ) return false;
    }
    if( !ase.readBlockEnd() ) return false;

    // read texcoords
    UInt numtexcoord;
    if( !ase.next( "*MESH_NUMTVERTEX" ) || !ase.readInt( numtexcoord ) ) return false;
    if( numtexcoord > 0 )
    {
        DynaArray<Vector3f> texcoords( numtexcoord );

        if( !ase.next( "*MESH_TVERTLIST" ) || !ase.readBlockStart() ) return false;
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
        if( !ase.next( "*MESH_NUMTVFACES" ) || !ase.next( strFormat( "%d", numface ).cptr() ) ) return false;
        if( !ase.next( "*MESH_TFACELIST" ) || !ase.readBlockStart() ) return false;
        for( UInt32 i = 0; i < numface; ++i )
        {
            AseFace & f = m.faces[i];

            if( !ase.next( "*MESH_TFACE" ) ) return false;
            if( !ase.next( strFormat( "%d", i ).cptr() ) ) return false;

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

        for( UInt32 i = 0; i < m.vertices.size(); ++i )
        {
            m.vertices[i].addTexcoord( zero );
        }

        for( UInt32 i = 0; i < m.faces.size(); ++i )
        {
            AseFace & f = m.faces[i];
            f.t[0] = 0;
            f.t[1] = 0;
            f.t[2] = 0;
        }
    }

    // skip vertex colors
    UInt numcolor;
    if( !ase.next( "*MESH_NUMCVERTEX" ) || !ase.readInt( numcolor ) ) return false;
    if( numcolor > 0 )
    {
        if( !ase.next( "*MESH_CVERTLIST" ) ) return false;
        if( !ase.skipNode() ) return false;
    }

    // read normals
    if( !ase.next( "*MESH_NORMALS" ) || !ase.readBlockStart() ) return false;
    Matrix44f it = Matrix44f::sInvtrans( transform ); // use to transform normal
    for( UInt32 i = 0; i < numface; ++i )
    {
        AseFace & f = m.faces[i];
        if( !ase.readIndexedVector3Node( "*MESH_FACENORMAL", i, f.fn ) ) return false;

        for( UInt32 i = 0; i < 3; ++i )
        {
            if( !ase.next( "*MESH_VERTEXNORMAL" ) ) return false;

            UInt32 vi;
            if( !ase.readInt( vi ) ) return false;

            AseVertex & v = m.vertices[vi];

            Vector3f n;
            if( !ase.readVector3( n ) ) return false;

            f.vn[i] = v.addNormal( it.transformVector( n ) );
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

    n.transform.identity();

    const char * token;
    while( 0 != ( token = ase.next() ) )
    {
        if( 0 ) {}
        else if( 0 == strCmp( "*TM_ROW0", token ) )
        {
            Vector3f v;
            if( !ase.readVector3( v ) ) return false;
            n.transform[0][0] = v.x;
            n.transform[1][0] = v.y;
            n.transform[2][0] = v.z;
        }
        else if( 0 == strCmp( "*TM_ROW1", token ) )
        {
            Vector3f v;
            if( !ase.readVector3( v ) ) return false;
            n.transform[0][1] = v.x;
            n.transform[1][1] = v.y;
            n.transform[2][1] = v.z;
        }
        else if( 0 == strCmp( "*TM_ROW2", token ) )
        {
            Vector3f v;
            if( !ase.readVector3( v ) ) return false;
            n.transform[0][2] = v.x;
            n.transform[1][2] = v.y;
            n.transform[2][2] = v.z;
        }
        else if( 0 == strCmp( "*TM_ROW3", token ) )
        {
            Vector3f v;
            if( !ase.readVector3( v ) ) return false;
            n.transform[0][3] = v.x;
            n.transform[1][3] = v.y;
            n.transform[2][3] = v.z;
        }
        else if( 0 == strCmp( "*TM_POS", token ) )
        {
            if( !ase.readVector3( n.pos ) ) return false;
        }
        else if( 0 == strCmp( "*TM_ROTAXIS", token ) )
        {
            if( !ase.readVector3( n.rotaxis ) ) return false;
        }
        else if( 0 == strCmp( "*TM_ROTANGLE", token ) )
        {
            if( !ase.readFloat( n.rotangle ) ) return false;
        }
        else if( 0 == strCmp( "*TM_SCALE", token ) )
        {
            if( !ase.readVector3( n.scale ) ) return false;
        }
        else if( '*' == *token )
        {
            ase.info( strFormat( "skip node %s", token ) );
            if( !ase.skipNode() ) return false;
        }
        else if( 0 == strCmp( token, "}" ) )
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
static bool sReadGeomObject( AseScene & scene, AseFile & ase )
{
    GN_GUARD;

    scene.objects.resize( scene.objects.size() + 1 );
    AseGeoObject & o = scene.objects.back();

    if( !ase.readBlockStart() ) return false;

    bool hasMaterial = false;

    const char * token;
    while( 0 != ( token = ase.next() ) )
    {
        if( 0 == strCmp( token, "*NODE_NAME" ) )
        {
            o.node.name = ase.readString();
            if( o.node.name.empty() )
            {
                ase.err( "Node name can't be empty!" );
                return false;
            }
            ase.info( strFormat( "read geometry object '%s' ...", o.node.name ) );
        }
        else if( 0 == strCmp( token, "*NODE_PARENT" ) )
        {
            o.node.parent = ase.readString();
        }
        else if( 0 == strCmp( token, "*NODE_TM" ) )
        {
            if( !sReadNode( o.node, ase ) ) return false;
        }
        else if( 0 == strCmp( token, "*MESH" ) )
        {
            if( !sReadMesh( o.mesh, o.node.transform, ase ) ) return false;
        }
        else if( 0 == strCmp( token, "*MATERIAL_REF" ) )
        {
            if( !ase.readInt( o.matid ) ) return false;
            if( o.matid >= scene.materials.size() )
            {
                ase.err( "material ID is out of range!" );
                return false;
            }
            hasMaterial = true;
        }
        else if( '*' == *token )
        {
            ase.info( strFormat( "skip node %s", token ) );
            if( !ase.skipNode() ) return false;
        }
        else if( 0 == strCmp( token, "}" ) )
        {
            // end of the block. do some post processing.

            AseMesh & m = o.mesh;

            if( !hasMaterial )
            {
                ase.warn( strFormat( "object '%s' has no material. Using default one.", o.node.name.cptr() ) );
                o.matid = 0;
            }

            AseMaterial & mtl = scene.materials[o.matid];

            // build face chunk array
            if( "Multi/Sub-Object" == mtl.class_ )
            {
                m.chunks.reserve( mtl.numsub );

                for( UInt32 i = 0; i < m.faces.size(); ++i )
                {
                    const AseFace & f = m.faces[i];

                    UInt32 cid;
                    for( cid = 0; cid < m.chunks.size(); ++cid )
                    {
                        AseFaceChunk & c = m.chunks[cid];

                        if( c.submat == f.submat ) break;
                    }
                    GN_ASSERT( cid <= m.chunks.size() );

                    if( cid == m.chunks.size() )
                    {
                        m.chunks.resize( m.chunks.size() + 1 );
                        m.chunks.back().submat = f.submat;
                    }

                    m.chunks[cid].faces.append( i );


                }
            }
            else
            {
                m.chunks.resize( 1 );
                m.chunks[0].submat = 0;
                m.chunks[0].faces.resize( m.faces.size() );
                for( UInt32 i = 0; i < m.faces.size(); ++i )
                {
                    m.chunks[0].faces[i] = i;
                }
            }

            // success
            return true;
        }
        else
        {
            ase.err( strFormat( "expecting node or close-brace, but met '%s'!", token ).cptr() );
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
static bool sReadGroup( AseScene & scene, AseFile & ase )
{
    GN_GUARD;

    if( !ase.readBlockStart() ) return false;

    const char * token;

    while( 0 != ( token = ase.next() ) )
    {
        if( 0 ) {}
        //else if( 0 == strCmp( token, "*SCENE" ) )
        //{
        //    ...
        //}
        else if( 0 == strCmp( token, "*GEOMOBJECT" ) )
        {
            if( !sReadGeomObject( scene, ase ) ) return false;
        }
        else if( '*' == *token )
        {
            ase.info( strFormat( "skip node %s", token ) );
            if( !ase.skipNode() ) return false;
        }
        else if( 0 == strCmp( token, "}" ) )
        {
            // end of the block.
            return true;
        }
        else
        {
            ase.err( strFormat( "expecting node token, but met '%s'.", token ) );
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
static bool sReadAse( AseScene & scene, File & file )
{
    GN_GUARD;

    // open ase file
    AseFile ase;
    if( !ase.open( file ) ) return false;

    // read ASE version
    const char * token = ase.next( "*3DSMAX_ASCIIEXPORT" );
    if( !token ) return false;
    if( 200 != ase.readOptionalInt( 0 ) )
    {
        GN_ERROR(sLogger)( "Not support ASE version other than 2.00!" );
        return false;
    }

    while( 0 != ( token = ase.next() ) )
    {
        if( 0 ) {}
        //else if( 0 == strCmp( token, "*SCENE" ) )
        //{
        //    ...
        //}
        else if( 0 == strCmp( token, "*MATERIAL_LIST" ) )
        {
            if( !sReadMaterials( scene, ase ) ) return false;
        }
        else if( 0 == strCmp( token, "*GROUP" ) )
        {
            if( !ase.readString() ) return false; // skip group name
            if( !sReadGroup( scene, ase ) ) return false;
        }
        else if( 0 == strCmp( token, "*GEOMOBJECT" ) )
        {
            if( !sReadGeomObject( scene, ase ) ) return false;
        }
        else if( '*' == *token )
        {
            ase.info( strFormat( "skip node %s", token ) );
            if( !ase.skipNode() ) return false;
        }
        else
        {
            ase.err( strFormat( "expecting node token, but met '%s'.", token ) );
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
static AseGeoObject * sFindGeoObject( AseScene & scene, const StrA & name )
{
    if( name.empty() ) return &scene.root;
    for( AseGeoObject * o = scene.objects.begin(); o != scene.objects.end(); ++o )
    {
        if( name == o->node.name ) return o;
    }
    return 0;
}

//
//
// -----------------------------------------------------------------------------
static bool sBuildNodeTree( AseScene & scene )
{
    GN_INFO(sLogger)( "\nBuild node tree..." );

    // setup root node
    scene.root.node.name = "root";
    scene.root.node.pos.set( 0, 0, 0 );
    scene.root.node.rotaxis.set( 0, 0, 1 );
    scene.root.node.rotangle = 0;
    scene.root.node.scale.set( 1, 1, 1 );
    scene.root.node.transform.identity();
    scene.root.mesh.bbox.size().set( 0, 0, 0 );

    // build node tree
    for( size_t i = 0; i < scene.objects.size(); ++i )
    {
        AseGeoObject & o = scene.objects[i];

        AseGeoObject * p = sFindGeoObject( scene, o.node.parent );

        if( 0 == p )
        {
            GN_ERROR(sLogger)( "Object %s has invalid parent: %s. Replace it with \"root\".",
                o.node.name.cptr(), o.node.parent.cptr() );
            p = &scene.root;
        }

        o.setParent( p, 0 );
    }

    // make sure all objects are linked into the tree.
    GN_ASSERT_EX(
        scene.root.calcChildrenCount() == scene.objects.size(),
        strFormat( "numchildren=%d, numobjects=%d",
            scene.root.calcChildrenCount(), scene.objects.size() ).cptr() );

    // calculate bounding box for each node, in post order
    TreeTraversePostOrder<AseGeoObject> ttpost( &scene.root );

    AseGeoObject * n = ttpost.first();
    GN_ASSERT( 0 == n->getFirstChild() );

    while( n )
    {
        // copy mesh bbox to node
        n->node.bbox = n->mesh.bbox;

        // then merge with all childrens' bbox
        AseGeoObject * c = safeCastPtr<AseGeoObject>( n->getFirstChild() );
        while( c )
        {
            Boxf::sGetUnion( n->node.bbox, n->node.bbox, c->node.bbox );

            c = safeCastPtr<AseGeoObject>( c->getNextSibling() );
        }

        // next node
        n = ttpost.next( n );
    }

    // print node tree
    TreeTraversePreOrder<AseGeoObject> ttpre( &scene.root );
    n = ttpre.first();
    int level = 0;
    while( n )
    {
        StrA s;

        for( int i = 0; i < level; ++i ) s += '-';
        s += strFormat(
            "%s : bbox_pos(%f,%f,%f), bbox_size(%f,%f,%f)",
            n->node.name.cptr(),
            n->node.bbox.pos().x,
            n->node.bbox.pos().y,
            n->node.bbox.pos().z,
            n->node.bbox.size().x,
            n->node.bbox.size().y,
            n->node.bbox.size().z );

        GN_INFO(sLogger)( s.cptr() );

        // next node
        n = ttpre.next( n, &level );
    }

    GN_INFO(sLogger)( "" );

    // success
    return true;
}

// *****************************************************************************
// public function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::util::loadAseSceneFromFile( AseScene & scene, File & file )
{
    if( !sReadAse( scene, file ) ) return false;
    if( !sBuildNodeTree( scene ) ) return false;
    return true;
}
