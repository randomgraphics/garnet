#include "pch.h"
#include "garnet/gfx/fatMesh.h"

// ***************************************************************************************
// local types and functions
// ***************************************************************************************

#pragma pack(push,1)
struct FatMeshDesc
{
    UInt32 numvtx;        // vertex count
    UInt32 numFace;       // face count
    UInt64 vtxfmt;        // vertex format
    UInt8  hasFaceNormal; // as is
    UInt8  reserved[3];   // reserved, must be zero
};
#pragma pack(pop)

static const char * BIN_TAG = "FATMESHB";
static const char * TXT_TAG = "FATMESHT";
static const size_t TAG_LEN = 8;
static const char * VERTEX_HEADER =
    "pos.x,pos.y,pos.z,pos.w,"
    "weight.x,weight.y,weight.z,weight.w,"
    "normal.x,normal.y,normal.z,"
    "diffuse.r,diffuse.g,diffuse.b,diffuse.a,"
    "specular.r,specular.g,specular.b,specular.a,"
    "fog,"
    "tangent.x,tangent.y,tangent.z,"
    "binormal.x,binormal.y,binormal.z,"
    "texcoord[0].x,texcoord[0].y,texcoord[0].z,texcoord[0].w,"
    "texcoord[1].x,texcoord[1].y,texcoord[1].z,texcoord[1].w,"
    "texcoord[2].x,texcoord[2].y,texcoord[2].z,texcoord[2].w,"
    "texcoord[3].x,texcoord[3].y,texcoord[3].z,texcoord[3].w,"
    "texcoord[4].x,texcoord[4].y,texcoord[4].z,texcoord[4].w,"
    "texcoord[5].x,texcoord[5].y,texcoord[5].z,texcoord[5].w,"
    "texcoord[6].x,texcoord[6].y,texcoord[6].z,texcoord[6].w,"
    "texcoord[7].x,texcoord[7].y,texcoord[7].z,texcoord[7].w";
static const char * FACE_HEADER = "i0,i1,i2,normal.x,normal.y,normal.z,material";

static bool sReadLn( GN::StrA & s, GN::File & fp )
{
    if( fp.eof() ) return false;
    s.clear();
    char ch;
    while( !fp.eof() )
    {
        if( !fp.read( &ch, 1, 0 ) ) return false;
        if( 13 == ch ) continue;
        if( 10 == ch ) return true;
        s.append( ch );
    }
    return true;
}

// ***************************************************************************************
// public functions
// ***************************************************************************************

//
//
// ---------------------------------------------------------------------------------------
bool GN::gfx::FatMesh::readFromFile( File & fp )
{
    GN_GUARD;

    // clear old mesh content.
    clear();

    // read file header
    char header[TAG_LEN];
    if( !fp.read( header, sizeof(header), 0 ) )
    {
        GN_ERROR(sLogger)( "Fail to read file header." );
        return false;
    }

    // check header
    if( 0 != memcmp( BIN_TAG, header, TAG_LEN ) &&
        0 != memcmp( TXT_TAG, header, TAG_LEN ) )
    {
        GN_ERROR(sLogger)( "FatMesh file must begin with \"FATMESHB\" or \"FATMESHT\"." );
        return false;
    }

    if( 'B' == header[TAG_LEN-1] )
    {
        FatMeshDesc desc;
        size_t readen;

        // read mesh descriptor
        if( !fp.read( &desc, sizeof(desc), &readen ) || sizeof(desc) != readen )
        { GN_ERROR(sLogger)( "fail to read mesh descriptor." ); return false; }
        mFatFmt.u64 = desc.vtxfmt;
        mHasFaceNormal = !!desc.hasFaceNormal;
        if( !mFatFmt.valid() ) { GN_ERROR(sLogger)( "invalid vertex format." ); return false; }

        // read vertices
        mVertices.resize( desc.numvtx );
        if( !fp.read( mVertices.cptr(), sizeof(FatVtx)*mVertices.size(), &readen ) ||
            readen != sizeof(FatVtx)*mVertices.size() )
        { GN_ERROR(sLogger)( "fail to read vertices." ); return false; }

        // read faces
        mFaces.resize( desc.numFace );
        if( !fp.read( mFaces.cptr(), sizeof(FatFace)*mFaces.size(), &readen ) ||
            readen != sizeof(FatFace)*mFaces.size() )
        { GN_ERROR(sLogger)( "fail to read faces." ); return false; }
    }
    else if( 'T' == header[TAG_LEN-1] )
    {
        StrA s;

#if GN_MSVC8
#define sscanf sscanf_s
#endif

        // read file header
        if( !sReadLn( s, fp ) ) { GN_ERROR(sLogger)( "fail to read file header." ); return false; }
        UInt32 numVerts, numFaces, faceNormal;
        UInt64 vtxfmt;
        if( 4 != sscanf(
            s.cptr(),
            " NumVertices=%lu NumFaces=%lu VertexFormat=%llu FaceNormal=%lu",
            (unsigned long*)&numVerts,
            (unsigned long*)&numFaces,
            (unsigned long long*)&vtxfmt,
            (unsigned long*)&faceNormal ) )
        {
            GN_ERROR(sLogger)( "invalid file header: %s", s.cptr() );
            return false;
        }
        mFatFmt.u64 = vtxfmt;
        mHasFaceNormal = !!faceNormal;

        // read vertex header
        if( !sReadLn( s, fp ) ) { GN_ERROR(sLogger)( "fail to read vertex header." ); return false; }
        if( VERTEX_HEADER != s ) { GN_ERROR(sLogger)( "invalid vertex header: %s", s.cptr() ); return false; }

        // read vertices
        mVertices.resize( numVerts );
        for( UInt32 i = 0; i < numVerts; ++i )
        {
            FatVtx & v = mVertices[i];

            if( !sReadLn( s, fp ) ) { GN_ERROR(sLogger)( "fail to read vertex #%u", i ); return false; }

            if( (12*4+3*3+1) != sscanf(
                s.cptr(),
                "%f,%f,%f,%f,"  // pos
                "%f,%f,%f,%f,"  // weight
                "%f,%f,%f,"     // normal
                "%f,%f,%f,%f,"  // diffuse
                "%f,%f,%f,%f,"  // specular
                "%f,"           // fog
                "%f,%f,%f,"     // tangent
                "%f,%f,%f,"     // binormal
                "%f,%f,%f,%f,"  // t0
                "%f,%f,%f,%f,"  // t1
                "%f,%f,%f,%f,"  // t2
                "%f,%f,%f,%f,"  // t3
                "%f,%f,%f,%f,"  // t4
                "%f,%f,%f,%f,"  // t5
                "%f,%f,%f,%f,"  // t6
                "%f,%f,%f,%f,", // t7
                &v.pos.x, &v.pos.y, &v.pos.z, &v.pos.w,
                &v.weight.x, &v.weight.y, &v.weight.z, &v.weight.w,
                &v.normal.x, &v.normal.y, &v.normal.z,
                &v.diffuse.x, &v.diffuse.y, &v.diffuse.z, &v.diffuse.w,
                &v.specular.x, &v.specular.y, &v.specular.z, &v.specular.w,
                &v.fog,
                &v.tangent.x, &v.tangent.y, &v.tangent.z,
                &v.binormal.x, &v.binormal.y, &v.binormal.z,
                &v.texcoord[0].x, &v.texcoord[0].y, &v.texcoord[0].z, &v.texcoord[0].w,
                &v.texcoord[1].x, &v.texcoord[1].y, &v.texcoord[1].z, &v.texcoord[1].w,
                &v.texcoord[2].x, &v.texcoord[2].y, &v.texcoord[2].z, &v.texcoord[2].w,
                &v.texcoord[3].x, &v.texcoord[3].y, &v.texcoord[3].z, &v.texcoord[3].w,
                &v.texcoord[4].x, &v.texcoord[4].y, &v.texcoord[4].z, &v.texcoord[4].w,
                &v.texcoord[5].x, &v.texcoord[5].y, &v.texcoord[5].z, &v.texcoord[5].w,
                &v.texcoord[6].x, &v.texcoord[6].y, &v.texcoord[6].z, &v.texcoord[6].w,
                &v.texcoord[7].x, &v.texcoord[7].y, &v.texcoord[7].z, &v.texcoord[7].w ) )
            {
                GN_ERROR(sLogger)( "fail to parse vertex #%u", i );
                return false;
            }
        }

        // read face heder
        if( !sReadLn( s, fp ) || FACE_HEADER != s ) { GN_ERROR(sLogger)( "fail to read face header." ); return false; }

        // read faces
        UInt32 i0, i1, i2;
        mFaces.resize( numFaces );
        for( UInt32 i = 0; i < numFaces; ++i )
        {
            FatFace & f = mFaces[i];

            if( !sReadLn( s, fp ) ) { GN_ERROR(sLogger)( "fail to read face #%u", i ); return false; }

            if( 7 != sscanf(
                s.cptr(),
                "%lu,%lu,%lu,%f,%f,%f,%d,",
                (unsigned long*)&i0,
                (unsigned long*)&i1,
                (unsigned long*)&i2,
                &f.normal.x, &f.normal.y, &f.normal.z, &f.material ) )
            {
                GN_ERROR(sLogger)( "fail to parse face #%u", i );
                return false;
            }

            f.i0 = i0;
            f.i1 = i1;
            f.i2 = i2;
        }
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ---------------------------------------------------------------------------------------
bool GN::gfx::FatMesh::writeToFile( File & fp, char mode ) const
{
    GN_GUARD;

    if( 'B' == mode )
    {
        // write file tag
        if( !fp.write( BIN_TAG, TAG_LEN, 0 ) ) { GN_ERROR(sLogger)( "fail to write file tag." ); return false; }

        // write mesh descriptor
        FatMeshDesc desc;
        desc.numvtx = (UInt32)mVertices.size();
        desc.numFace = (UInt32)mFaces.size();
        desc.vtxfmt = mFatFmt.u64;
        desc.hasFaceNormal = (UInt8)mHasFaceNormal;
        desc.reserved[0] = desc.reserved[1] = desc.reserved[2] = 0;
        if( !fp.write( &desc, sizeof(desc), 0 ) ) { GN_ERROR(sLogger)( "fail to write mesh descriptor." ); return false; }

        // write vertices
        if( !fp.write( mVertices.cptr(), sizeof(FatVtx)*mVertices.size(), 0 ) )
        { GN_ERROR(sLogger)( "fail to write vertices." ); return false; }

        // write faces
        if( !fp.write( mFaces.cptr(), sizeof(FatFace)*mFaces.size(), 0 ) )
        { GN_ERROR(sLogger)( "fail to write faces." ); return false; }
    }
    else if( 'T' == mode )
    {
        StrA s;

        // write header
        s.format(
            "%s NumVertices=%lu NumFaces=%lu VertexFormat=%u FaceNormal=%u\n",
            TXT_TAG,
            mVertices.size(),
            mFaces.size(),
            mFatFmt.u64,
            mHasFaceNormal ? 1 : 0 );
        if( !fp.write( s.cptr(), s.size(), 0 ) )
        {
            GN_ERROR(sLogger)( "fail to write mesh header." );
            return false;
        }

        // write vertex header
        s.format( "%s\n", VERTEX_HEADER );
        if( !fp.write( s.cptr(), s.size(), 0 ) )
        {
            GN_ERROR(sLogger)( "fail to write vertex header." );
            return false;
        }

        // write vertices
        for( size_t i = 0; i < mVertices.size(); ++i )
        {
            const FatVtx & v = mVertices[i];

            // compose vertex string
            s.format(
                "%f,%f,%f,%f,"  // pos
                "%f,%f,%f,%f,"  // weight
                "%f,%f,%f,"     // normal
                "%f,%f,%f,%f,"  // diffuse
                "%f,%f,%f,%f,"  // specular
                "%f,"           // fog
                "%f,%f,%f,"     // tangent
                "%f,%f,%f,"     // binormal
                "%f,%f,%f,%f,"  // t0
                "%f,%f,%f,%f,"  // t1
                "%f,%f,%f,%f,"  // t2
                "%f,%f,%f,%f,"  // t3
                "%f,%f,%f,%f,"  // t4
                "%f,%f,%f,%f,"  // t5
                "%f,%f,%f,%f,"  // t6
                "%f,%f,%f,%f,"  // t7
                "\n",
                v.pos.x, v.pos.y, v.pos.z, v.pos.w,
                v.weight.x, v.weight.y, v.weight.z, v.weight.w,
                v.normal.x, v.normal.y, v.normal.z,
                v.diffuse.x, v.diffuse.y, v.diffuse.z, v.diffuse.w,
                v.specular.x, v.specular.y, v.specular.z, v.specular.w,
                v.fog,
                v.tangent.x, v.tangent.y, v.tangent.z,
                v.binormal.x, v.binormal.y, v.binormal.z,
                v.texcoord[0].x, v.texcoord[0].y, v.texcoord[0].z, v.texcoord[0].w,
                v.texcoord[1].x, v.texcoord[1].y, v.texcoord[1].z, v.texcoord[1].w,
                v.texcoord[2].x, v.texcoord[2].y, v.texcoord[2].z, v.texcoord[2].w,
                v.texcoord[3].x, v.texcoord[3].y, v.texcoord[3].z, v.texcoord[3].w,
                v.texcoord[4].x, v.texcoord[4].y, v.texcoord[4].z, v.texcoord[4].w,
                v.texcoord[5].x, v.texcoord[5].y, v.texcoord[5].z, v.texcoord[5].w,
                v.texcoord[6].x, v.texcoord[6].y, v.texcoord[6].z, v.texcoord[6].w,
                v.texcoord[7].x, v.texcoord[7].y, v.texcoord[7].z, v.texcoord[7].w );

            // write vertex string to file
            if( !fp.write( s.cptr(), s.size(), 0 ) )
            {
                GN_ERROR(sLogger)( "fail to write vertex #%lu", i );
                return false;
            }
        }

        // write face header
        s.format( "%s\n", FACE_HEADER );
        if( !fp.write( s.cptr(), s.size(), 0 ) )
        {
            GN_ERROR(sLogger)( "fail to write face header." );
            return false;
        }

        // write faces
        for( size_t i = 0; i < mFaces.size(); ++i )
        {
            const FatFace & f = mFaces[i];

            // compose face string
            s.format( "%lu,%lu,%lu,%f,%f,%f,%d,\n",
                f.i0, f.i1, f.i2,
                f.normal.x, f.normal.y, f.normal.z,
                f.material );

            // write face string to file
            if( !fp.write( s.cptr(), s.size(), 0 ) )
            {
                GN_ERROR(sLogger)( "fail to write face #%lu", i );
                return false;
            }
        }
    }
    else
    {
        GN_ERROR(sLogger)( "invalid mode. Must be 'B' or 'T'." );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ---------------------------------------------------------------------------------------
bool GN::gfx::fatMeshFromX( FatMesh &, File & )
{
    GN_UNIMPL_WARNING();
    return false;
}
