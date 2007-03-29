#include "pch.h"
#include "convert.h"

using namespace GN;

// *****************************************************************************
// local functions
// *****************************************************************************

struct AseMap
{
    StrA  name;
    StrA  class_;
    StrA  bitmap;
};

struct AseMaterial
{
    StrA          name;
    StrA          class_;
    Vector3f      ambient, diffuse, specular;
    float         shine;
    float         shinestrength;
    float         transparency;
    AseMap        mapdiff;
    AseMap        mapbump;
    UInt32        numsub;
    AseMaterial * submaterials;

    AseMaterial() : numsub(0), submaterials(0) {}

    ~AseMaterial() { safeDeleteArray(submaterials); }

    void allocSubMaterials( UInt32 count )
    {
        GN_ASSERT( 0 == numsub && 0 == submaterials );
        if( 0 == count ) return;
        submaterials = new AseMaterial[count];
        numsub = count;
    }
};

struct AseFace
{
    UInt32   v[3];  ///< vertices (index into vertex array)
    UInt32   t[3];  ///< texcoords (index into texcoord array)
    Vector3f fn;    ///< face normal
    Vector3f vn[3]; ///< vertex normals
    UInt32   s;     ///< smooth group ID
    UInt32   m;     ///< sub material ID
};

struct AseMesh
{
    ///
    /// this group is loaded directly from ASE file.
    //@{
    UInt32                  timevalue;
    DynaArray<Vector3f>     positions; ///< position array
    DynaArray<Vector3f>     texcoords; ///< texcoord array
    DynaArray<AseFace>      faces;     ///< face array
    //@}
};

struct AseGeoObject
{
    StrA    name;
    AseMesh mesh;
    UInt32  matid; ///< material ID into global material array
};

struct AseScene
{
    DynaArray<AseMaterial>  materials;
    DynaArray<AseGeoObject> objects;
};

struct AseFile
{
    DynaArray<char> buf;
    char *          str;
    int             line;

    bool open( const StrA & filename )
    {
        // read ASE file
        AutoObjPtr<File> fp( core::openFile( filename, "rt" ) );
        if( !fp ) return false;
        buf.resize( fp->size() + 1 );
        size_t readen;
        buf.resize( fp->size() );
        if( !fp->read( buf.cptr(), fp->size(), &readen ) )
        {
            return false;
        }
        GN_ASSERT( readen <= fp->size() );
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
        GN_ASSERT( (token+3) <= str );
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
static bool sReadGeomObject( AseScene & scene, AseFile & ase )
{
    GN_GUARD;

    scene.objects.resize( scene.objects.size() + 1 );
    AseGeoObject & o = scene.objects.back();

    if( !ase.readBlockStart() ) return false;

    // node name
    if( !ase.next( "*NODE_NAME" ) ) return false;
    o.name = ase.readString();
    if( o.name.empty() ) return false;

    // skip *NODE_TM by now
    if( !ase.next( "*NODE_TM" ) ) return false;
    if( !ase.skipNode() ) return false;

    AseMesh & m = o.mesh;

    // read *MESH
    if( !ase.next( "*MESH" ) ) return false;
    if( !ase.readBlockStart() ) return false;

    if( !ase.next( "*TIMEVALUE" ) || !ase.readInt( m.timevalue ) ) return false;

    UInt32 numvert, numface;
    if( !ase.next( "*MESH_NUMVERTEX" ) || !ase.readInt( numvert ) ) return false;
    if( !ase.next( "*MESH_NUMFACES" ) || !ase.readInt( numface ) ) return false;

    m.positions.resize( numvert );
    m.faces.resize( numface );

    // read vertices
    if( !ase.next( "*MESH_VERTEX_LIST" ) || !ase.readBlockStart() ) return false;
    for( UInt32 i = 0; i < numvert; ++i )
    {
        if( !ase.readIndexedVector3Node( "*MESH_VERTEX", i, m.positions[i] ) ) return false;
    }
    if( !ase.readBlockEnd() ) return false;

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
        if( !ase.next( "*MESH_SMOOTHING" ) || !ase.readInt( f.s ) ) return false;
        if( !ase.next( "*MESH_MTLID" ) || !ase.readInt( f.m ) ) return false;
    }
    if( !ase.readBlockEnd() ) return false;

    // read texcoords
    UInt numtexcoord;
    if( !ase.next( "*MESH_NUMTVERTEX" ) || !ase.readInt( numtexcoord ) ) return false;
    m.texcoords.resize( numtexcoord );

    if( !ase.next( "*MESH_TVERTLIST" ) || !ase.readBlockStart() ) return false;
    for( UInt32 i = 0; i < numtexcoord; ++i )
    {
        if( !ase.readIndexedVector3Node( "*MESH_TVERT", i, m.texcoords[i] ) ) return false;
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
        if( !ase.readInt( f.t[0] ) ) return false;
        if( !ase.readInt( f.t[1] ) ) return false;
        if( !ase.readInt( f.t[2] ) ) return false;
    }
    if( !ase.readBlockEnd() ) return false;

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
    for( UInt32 i = 0; i < numface; ++i )
    {
        AseFace & f = m.faces[i];
        if( !ase.readIndexedVector3Node( "*MESH_FACENORMAL", i, f.fn ) ) return false;
        if( !ase.readIndexedVector3Node( "*MESH_VERTEXNORMAL", f.v[0], f.vn[0] ) ) return false;
        if( !ase.readIndexedVector3Node( "*MESH_VERTEXNORMAL", f.v[1], f.vn[1] ) ) return false;
        if( !ase.readIndexedVector3Node( "*MESH_VERTEXNORMAL", f.v[2], f.vn[2] ) ) return false;
    }
    if( !ase.readBlockEnd() ) return false;

    // end of mesh block
    if( !ase.readBlockEnd() ) return false;

    // read remaining nodes in geomobject block
    const char * token;
    while( 0 != (token = ase.next() ) )
    {
        if( 0 == strCmp( token, "*MATERIAL_REF" ) )
        {
            if( !ase.readInt( o.matid ) ) return false;
            if( o.matid >= scene.materials.size() )
            {
                ase.err( "material ID is out of range!" );
                return false;
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

    // done
    return ase.readBlockEnd();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadAse( AseScene & scene, const StrA & filename )
{
    GN_GUARD;

    // open ase file
    AseFile ase;
    if( !ase.open( filename ) ) return false;

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

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::ase2mesh( const ConvertOptions & co )
{
    GN_GUARD;

    GN_ASSERT( !co.inputFileName.empty() );
    GN_ASSERT( !co.outputFileName.empty() );

    AseScene s;

    if( !sReadAse( s, co.inputFileName ) ) return false;

    //if( !sWriteScene( s, co.outputFileName ) ) return false;

    // success
    return true;

    GN_UNGUARD;
}
