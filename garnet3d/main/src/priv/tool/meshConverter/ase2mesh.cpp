#include "pch.h"
#include "convert.h"

using namespace GN;

// *****************************************************************************
// local functions
// *****************************************************************************

struct AseBitmap
{
    StrA texname;
};

struct AseStandardMaterial
{
    StrA      name;
    Vector3f  ambient, diffuse, specular;
    float     shininess;
    float     alpha;
    AseBitmap mapdiff;
    AseBitmap mapbump;
};

struct AseFaceVtx
{
    UInt32 p;   ///< index into position array
    UInt32 t;   ///< index into texcoord array
};

struct AseFace
{
    UInt32   v[3]; ///< vertices (index into vertex array)
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
    DynaArray<AseStandardMaterial> materials;
    DynaArray<AseGeoObject>        objects;
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

    const char * next()
    {
        if( 0 == str || 0 == *str ) return 0;

        while( ' ' == *str || '\t' == *str || '\n' == *str )
        {
            if( '\n' == *str ) ++line;
            ++str;
        }

        if ( '"' == *str )
        {
            // quoted string

            ++str;

            const char * r = str;

            while( '"' != *str && 0 != *str )
            {
                if( '\n' == *str ) ++line;
                ++str;
            }

            if( 0 != *str ) *str = 0, ++str; // point to start of next token

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

            return r;
        }
    }

    ///
    /// skip until meet specific token
    ///
    bool skipUntil( const char * endtoken )
    {
        const char * token;
        for(;;)
        {
            token = next();

            if( 0 == strCmp( endtoken, token ) ) return true;

            if( 0 == token )
            {
                GN_ERROR(sLogger)( "token '%s' not found!", endtoken );
                return false;
            }
        }
    }

    //
    //
    // -----------------------------------------------------------------------------
    bool getf( float & result )
    {
        const char * token = next();
        return str2Float( result, token );
    }

    //
    //
    // -----------------------------------------------------------------------------
    template<typename INT_TYPE>
    bool geti( INT_TYPE & result )
    {
        const char * token = next();
        return str2Int<INT_TYPE>( result, token );
    }

    //
    //
    // -----------------------------------------------------------------------------
    template<typename INT_TYPE>
    INT_TYPE getid( INT_TYPE defval )
    {
        INT_TYPE result;
        return geti( result ) ? result : defval;
    }
};

//
//
// -----------------------------------------------------------------------------

//
//
// -----------------------------------------------------------------------------
static bool sReadMaterials( AseScene & scene, AseFile & ase )
{
    GN_GUARD;

    // looking for material list block
    if( !ase.skipUntil( "*MATERIAL_LIST" ) ) return false;

    // read material count
    if( !ase.skipUntil( "*MATERIAL_COUNT" ) ) return false;
    UInt32 matcount;
    if( !ase.geti( matcount ) ) return false;

    GN_UNIMPL();
    GN_UNUSED_PARAM( scene );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sReadScene( AseScene & scene, const StrA & filename )
{
    GN_GUARD;

    // open ase file
    AseFile ase;
    if( !ase.open( filename ) ) return false;

    // read ASE version
    const char * token = ase.next();
    if( 0 != strCmp( "*3DSMAX_ASCIIEXPORT", token ) )
    {
        GN_ERROR(sLogger)( "invalid ASE version tag!" );
        return false;
    }
    if( 200 != ase.getid( 0 ) )
    {
        GN_ERROR(sLogger)( "Not support ASE version other than 2.00!" );
        return false;
    }

    // read materials
    if( !sReadMaterials( scene, ase ) ) return false;

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

    if( !sReadScene( s, co.inputFileName ) ) return false;

    //if( !sWriteScene( s, co.outputFileName ) ) return false;

    // success
    return true;

    GN_UNGUARD;
}
