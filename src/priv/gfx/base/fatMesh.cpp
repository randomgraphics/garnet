#include "pch.h"
#include "garnet/gfx/fatMesh.h"

// ***************************************************************************************
// local types and functions
// ***************************************************************************************

#pragma pack(push,1)
struct FatMeshDesc
{
    uint32_t numVtx;  // vertex count
    uint32_t numFace; // face count
    uint32_t vtxFmt;  // vertex format
    uint8_t  hasFaceNormal; // as is
    uint8_t  reserved[3]; // reserved, must be zero
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
bool GN::gfx::FatVertexFormat::fromStr( const char * str, size_t len )
{
    if( 0 == str ) { GN_ERROR( "null string" ); return false; }
    if( 0 == len ) len = strLen( str );
    GN_UNIMPL_WARNING();
    return false;
}

//
//
// ---------------------------------------------------------------------------------------
void GN::gfx::FatMesh::optimize( const OptimizeOptions & oo )
{
    GN_GUARD;

    // build vertex segments, to make sure index wouldn't be too large
    if( mVertices.size() > 0xFFFF )
    {
        GN_WARN( "do not support mesh with more than 0xFFFF vertices" );
        return;
    }
    DynaArray<DynaArray<size_t> > faces; // indices of faces that belongs to each segments.
    mVtxSegments.resize( 1 );
    mVtxSegments[0].start = 0;
    mVtxSegments[0].count = mVertices.size();
    faces.resize( 1 );
    faces[0].resize( mFaces.size() );
    for( size_t i = 0; i < mFaces.size(); ++i ) faces[0][i] = i;

    // for each vertex segment
    for( size_t vtxSegIdx = 0; vtxSegIdx < mVtxSegments.size(); ++vtxSegIdx )
    {
        // sort by material, generate face segments as well.
        sortByMaterial( vtxSegIdx, faces[vtxSegIdx] );
    }

    // for each face segment
    for( size_t faceSegIdx = 0; faceSegIdx < mFaceSegments.size(); ++faceSegIdx )
    {
        // TODO: do strip and vcache optimization here (try nvtristrip)

        // move indices to 16bit index buffer, if required.
        if( !oo.use32BitIndex )
        {
            FaceSegment & fs = mFaceSegments[faceSegIdx];
            fs.indices16.resize( fs.indices32.size() );
            for( size_t i = 0; i < fs.indices32.size(); ++i )
            {
                GN_ASSERT( fs.indices32[i] <= 0xFFFF );
                fs.indices16[i] = (uint16_t)fs.indices32[i];
            }
            fs.indices32.clear();
        }
    }

    mUse32BitIndex = oo.use32BitIndex;
    mUseTriStrip = oo.strip;

    GN_UNGUARD;
}

//
//
// ---------------------------------------------------------------------------------------
void GN::gfx::FatMesh::draw( int material )
{
    GN_GUARD_SLOW;

    if( mFaceSegments.empty() )
    {
        OptimizeOptions oo;
        oo.maxPrimitivesInSingleDraw = 0xFFFF;
        oo.vcache = false;
        oo.strip = true;
        oo.useResetIndex = false;
        oo.use32BitIndex = false;
        optimize( oo );
    }

    // setup vertex format
    VtxFmtDesc vfd;
    vfd.addAttrib( 0, 0, VTXSEM_COORD, FMT_FLOAT3 );
    gRenderer.setVtxFmt( gRenderer.createVtxFmt( vfd ) );

    for( size_t idx = 0; idx < mFaceSegments.size(); ++idx )
    {
        if( mFaceSegments[idx].material != material ) continue;
        drawFaceSegment( idx );
        break;
    }

    GN_UNGUARD_SLOW;
}

//
//
// ---------------------------------------------------------------------------------------
bool GN::gfx::FatMesh::readFrom( File & fp )
{
    GN_GUARD;

    // clear old mesh content.
    clear();

    // read file header
    char header[TAG_LEN];
    if( !fp.read( header, sizeof(header), 0 ) )
    {
        GN_ERROR( "Fail to read file header." );
        return false;
    }

    // check header
    if( 0 != memcmp( BIN_TAG, header, TAG_LEN ) &&
        0 != memcmp( TXT_TAG, header, TAG_LEN ) )
    {
        GN_ERROR( "FatMesh file must begin with \"FATMESHB\" or \"FATMESHT\"." );
        return false;
    }

    if( 'B' == header[TAG_LEN-1] )
    {
        FatMeshDesc desc;
        size_t readen;

        // read mesh descriptor
        if( !fp.read( &desc, sizeof(desc), &readen ) || sizeof(desc) != readen )
        { GN_ERROR( "fail to read mesh descriptor." ); return false; }
        mVertexFormat.u32 = desc.vtxFmt;
        mHasFaceNormal = !!desc.hasFaceNormal;
        if( !mVertexFormat.valid() ) { GN_ERROR( "invalid vertex format." ); return false; }

        // read vertices
        mVertices.resize( desc.numVtx );
        if( !fp.read( mVertices.cptr(), sizeof(FatVertex)*mVertices.size(), &readen ) ||
            readen != sizeof(FatVertex)*mVertices.size() )
        { GN_ERROR( "fail to read vertices." ); return false; }

        // read faces
        mFaces.resize( desc.numFace );
        if( !fp.read( mFaces.cptr(), sizeof(Face)*mFaces.size(), &readen ) ||
            readen != sizeof(Face)*mFaces.size() )
        { GN_ERROR( "fail to read faces." ); return false; }
    }
    else if( 'T' == header[TAG_LEN-1] )
    {
        StrA s;

#if GN_MSVC8
#define sscanf sscanf_s
#endif

        // read file header
        if( !sReadLn( s, fp ) ) { GN_ERROR( "fail to read file header." ); return false; }
        uint32_t numVerts, numFaces, vtxFmt, faceNormal;
        if( 4 != sscanf(
            s.cptr(),
            " NumVertices=%lu NumFaces=%lu VertexFormat=%lu FaceNormal=%lu",
            &numVerts, &numFaces, &vtxFmt, &faceNormal ) )
        {
            GN_ERROR( "invalid file header: %s", s.cptr() );
            return false;
        }
        mVertexFormat.u32 = vtxFmt;
        mHasFaceNormal = !!faceNormal;

        // read vertex header
        if( !sReadLn( s, fp ) ) { GN_ERROR( "fail to read vertex header." ); return false; }
        if( VERTEX_HEADER != s ) { GN_ERROR( "invalid vertex header: %s", s.cptr() ); return false; }

        // read vertices
        mVertices.resize( numVerts );
        for( uint32_t i = 0; i < numVerts; ++i )
        {
            FatVertex & v = mVertices[i];

            if( !sReadLn( s, fp ) ) { GN_ERROR( "fail to read vertex #%u", i ); return false; }

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
                GN_ERROR( "fail to parse vertex #%u", i );
                return false;
            }
        }

        // read face heder
        if( !sReadLn( s, fp ) || FACE_HEADER != s ) { GN_ERROR( "fail to read face header." ); return false; }

        // read faces
        uint32_t i0, i1, i2;
        mFaces.resize( numFaces );
        for( uint32_t i = 0; i < numFaces; ++i )
        {
            Face & f = mFaces[i];

            if( !sReadLn( s, fp ) ) { GN_ERROR( "fail to read face #%u", i ); return false; }

            if( 7 != sscanf(
                s.cptr(),
                "%lu,%lu,%lu,%f,%f,%f,%d,",
                &i0, &i1, &i2, &f.normal.x, &f.normal.y, &f.normal.z, &f.material ) )
            {
                GN_ERROR( "fail to parse face #%u", i );
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
bool GN::gfx::FatMesh::readFromX( File & )
{
    GN_UNIMPL_WARNING();
    return false;
}

//
//
// ---------------------------------------------------------------------------------------
bool GN::gfx::FatMesh::writeTo( File & fp, char mode ) const
{
    GN_GUARD;

    if( 'B' == mode )
    {
        // write file tag
        if( !fp.write( BIN_TAG, TAG_LEN, 0 ) ) { GN_ERROR( "fail to write file tag." ); return false; }

        // write mesh descriptor
        FatMeshDesc desc;
        desc.numVtx = (uint32_t)mVertices.size();
        desc.numFace = (uint32_t)mFaces.size();
        desc.vtxFmt = mVertexFormat.u32;
        desc.hasFaceNormal = (uint8_t)mHasFaceNormal;
        desc.reserved[0] = desc.reserved[1] = desc.reserved[2] = 0;
        if( !fp.write( &desc, sizeof(desc), 0 ) ) { GN_ERROR( "fail to write mesh descriptor." ); return false; }

        // write vertices
        if( !fp.write( mVertices.cptr(), sizeof(FatVertex)*mVertices.size(), 0 ) )
        { GN_ERROR( "fail to write vertices." ); return false; }

        // write faces
        if( !fp.write( mFaces.cptr(), sizeof(Face)*mFaces.size(), 0 ) )
        { GN_ERROR( "fail to write faces." ); return false; }
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
            mVertexFormat.u32,
            mHasFaceNormal ? 1 : 0 );
        if( !fp.write( s.cptr(), s.size(), 0 ) )
        {
            GN_ERROR( "fail to write mesh header." );
            return false;
        }

        // write vertex header
        s.format( "%s\n", VERTEX_HEADER );
        if( !fp.write( s.cptr(), s.size(), 0 ) )
        {
            GN_ERROR( "fail to write vertex header." );
            return false;
        }

        // write vertices
        for( size_t i = 0; i < mVertices.size(); ++i )
        {
            const FatVertex & v = mVertices[i];

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
                GN_ERROR( "fail to write vertex #%lu", i );
                return false;
            }
        }

        // write face header
        s.format( "%s\n", FACE_HEADER );
        if( !fp.write( s.cptr(), s.size(), 0 ) )
        {
            GN_ERROR( "fail to write face header." );
            return false;
        }

        // write faces
        for( size_t i = 0; i < mFaces.size(); ++i )
        {
            const Face & f = mFaces[i];

            // compose face string
            s.format( "%lu,%lu,%lu,%f,%f,%f,%d,\n",
                f.i0, f.i1, f.i2,
                f.normal.x, f.normal.y, f.normal.z,
                f.material );

            // write face string to file
            if( !fp.write( s.cptr(), s.size(), 0 ) )
            {
                GN_ERROR( "fail to write face #%lu", i );
                return false;
            }
        }
    }
    else
    {
        GN_ERROR( "invalid mode. Must be 'B' or 'T'." );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}


// ***************************************************************************************
// private functions
// ***************************************************************************************

//
//
// ---------------------------------------------------------------------------------------
void GN::gfx::FatMesh::sortByMaterial( size_t vtxSegIdx, const DynaArray<size_t> & faces )
{
    GN_GUARD;

    const VtxSegment & vs = mVtxSegments[vtxSegIdx];

    std::map< int,DynaArray<size_t> > matTable;
    for( size_t i = 0; i < faces.size(); ++i )
    {
        size_t faceIdx = faces[i];
        matTable[mFaces[faceIdx].material].append( faceIdx );
    }

    for( std::map< int,DynaArray<size_t> >::const_iterator matIter = matTable.begin(); matIter != matTable.end(); ++matIter )
    {
        // create new face segment
        mFaceSegments.resize( mFaceSegments.size() + 1 );
        FaceSegment & fs = mFaceSegments.back();

        fs.material = matIter->first;
        fs.vtxSegIdx = vtxSegIdx;

        // fill 32-bit index buffer
        const DynaArray<size_t> & m = matIter->second;
        fs.indices32.resize( m.size() * 3 );
        for( size_t faceIdx = 0; faceIdx < m.size(); ++faceIdx )
        {
            const Face & f = mFaces[m[faceIdx]];

            GN_ASSERT( matIter->first == f.material );
            GN_ASSERT( vs.start <= f.i0 && f.i0 < (vs.start+vs.count) );
            GN_ASSERT( vs.start <= f.i1 && f.i1 < (vs.start+vs.count) );
            GN_ASSERT( vs.start <= f.i2 && f.i2 < (vs.start+vs.count) );

#if GN_X64
            GN_ASSERT( f.i0 < 0xFFFFFFFF );
            GN_ASSERT( f.i1 < 0xFFFFFFFF );
            GN_ASSERT( f.i2 < 0xFFFFFFFF );
#endif
            fs.indices32[faceIdx*3+0] = (uint32_t)( f.i0 - vs.start );
            fs.indices32[faceIdx*3+1] = (uint32_t)( f.i1 - vs.start );
            fs.indices32[faceIdx*3+2] = (uint32_t)( f.i2 - vs.start );
        }
    }

    GN_UNGUARD;
}

//
//
// ---------------------------------------------------------------------------------------
inline void GN::gfx::FatMesh::drawFaceSegment( size_t idx )
{
    GN_GUARD_SLOW;

    GN_ASSERT( idx <= mFaceSegments.size() );

    if( mUse32BitIndex )
    {
        GN_WARN( "current renderer does not support 32bit index buffer" );
        return;
    }

    const FaceSegment & s = mFaceSegments[idx];
    if( s.indices16.size() < 3 )
    {
        GN_WARN( "no enough indices (at least 3)" );
        return;
    }

    GN_ASSERT( s.vtxSegIdx < mVtxSegments.size() );
    const VtxSegment & v = mVtxSegments[s.vtxSegIdx];
    GN_ASSERT( v.start < mVertices.size() && (v.start+v.count) <= mVertices.size() );

    gRenderer.drawIndexedUp(
        mUseTriStrip ? TRIANGLE_STRIP : TRIANGLE_LIST,
        mUseTriStrip ? s.indices16.size() - 2 : s.indices16.size() / 3,
        v.count,
        mVertices.cptr() + v.start,
        sizeof(FatVertex),
        s.indices16.cptr() );

    GN_UNGUARD_SLOW;
}
