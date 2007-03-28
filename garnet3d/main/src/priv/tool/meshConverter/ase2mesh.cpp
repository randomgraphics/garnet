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
    StrA      name;
    StrA      class_;
    Vector3f  ambient, diffuse, specular;
    float     shine;
    float     shinestrength;
    float     transparency;
    AseMap    mapdiff;
    AseMap    mapbump;
};

struct AseFaceVtx
{
    UInt32 p;   ///< index into position array
    UInt32 t;   ///< index into texcoord array
};

struct AseFace
{
    UInt32   v[3]; ///< vertices (index into vertex array)
    UInt32   t[3]; ///< texcoords (index into texcoord array)
    Vector3f n[3]; ///< vertex normals
};

///
/// faces with same material
struct AseFaceChunk
{
    UInt32             m; ///< material ID into material array
    DynaArray<AseFace> f; ///< face array
};

struct AseMesh
{
    ///
    /// this group is loaded directly from ASE file.
    //@{
    DynaArray<Vector3f>     p; ///< position array
    DynaArray<Vector2f>     t; ///< texcoord array
    DynaArray<AseFaceChunk> c; ///< face chunk array
    //@}

    ///
    /// This group is calculated out of above.
    //@{
    DynaArray<AseFaceVtx> v; ///< vertice array
    //@}
};

struct AseGeoObject
{
    StrA    name;
    AseMesh mesh;
};

struct AseScene
{
    DynaArray<AseMaterial>  materials;
    DynaArray<AseGeoObject> objects;
};

enum AseTokenType
{
    ASE_NULL,  ///< indicate invalid token
    ASE_INTEGER,
    ASE_FLOAT,
    ASE_STRING, ///< quoated string
    ASE_SYMBOL,
    ASE_NODE,
};

struct AseToken
{
    AseTokenType type;
    const char * text;

    static const AseToken NULL_TOKEN;
};
const AseToken AseToken::NULL_TOKEN = { ASE_NULL, 0 };

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
            if( '*' == *str ) return true; // found next node
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
        //else if( 0 == strCmp( token, "*GEOMOBJECT*" ) )
        //{
        //    ...
        //}
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
