#include "pch.h"
#include "orientationBox.h"

// *****************************************************************************
// utilities
// *****************************************************************************

static GN::Logger * sLogger = GN::getLogger("GN.gfx.orientationbox");

//
//
// -----------------------------------------------------------------------------
static void LOG_ERROR(const char * format, ...)
{
    GN::StrA s;
    va_list arglist;
    va_start( arglist, format );
    s.formatv( format, arglist );
    va_end( arglist );
    GN_ERROR(sLogger)( "%s\n", s.cptr() );
}

#define SAFE_RELEASE( x ) if(x) { (x)->Release(); (x) = NULL; } else void(0)

// *****************************************************************************
// local code
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
D3D9OrientationBox::D3D9OrientationBox( float boxSizeInScreenSpace = 32.0f )
    : m_BoxSize( boxSizeInScreenSpace )
    , m_Device(0)
{
    memset( m_Textures, 0, sizeof(m_Textures) );

    // build box vertices (right handed)

    // +x
    m_BoxFaces[0].vertices[0].Set( +1, -1, +1, 0, 0, 0xFFFF0000 );
    m_BoxFaces[0].vertices[1].Set( +1, -1, -1, 0, 1, 0xFFFF0000 );
    m_BoxFaces[0].vertices[2].Set( +1, +1, +1, 1, 0, 0xFFFF0000 );
    m_BoxFaces[0].vertices[3].Set( +1, +1, -1, 1, 1, 0xFFFF0000 );

    // -x
    m_BoxFaces[1].vertices[0].Set( -1, +1, +1, 0, 0, 0xFF800000 );
    m_BoxFaces[1].vertices[1].Set( -1, +1, -1, 0, 1, 0xFF800000 );
    m_BoxFaces[1].vertices[2].Set( -1, -1, +1, 1, 0, 0xFF800000 );
    m_BoxFaces[1].vertices[3].Set( -1, -1, -1, 1, 1, 0xFF800000 );

    // +y
    m_BoxFaces[2].vertices[0].Set( +1, +1, -1, 0, 0, 0xFF00FF00 );
    m_BoxFaces[2].vertices[1].Set( -1, +1, -1, 0, 1, 0xFF00FF00 );
    m_BoxFaces[2].vertices[2].Set( +1, +1, +1, 1, 0, 0xFF00FF00 );
    m_BoxFaces[2].vertices[3].Set( -1, +1, +1, 1, 1, 0xFF00FF00 );

    // -y
    m_BoxFaces[3].vertices[0].Set( +1, -1, +1, 0, 0, 0xFF008000 );
    m_BoxFaces[3].vertices[1].Set( -1, -1, +1, 0, 1, 0xFF008000 );
    m_BoxFaces[3].vertices[2].Set( +1, -1, -1, 1, 0, 0xFF008000 );
    m_BoxFaces[3].vertices[3].Set( -1, -1, -1, 1, 1, 0xFF008000 );

    // +z
    m_BoxFaces[4].vertices[0].Set( -1, +1, +1, 0, 0, 0xFF0000FF );
    m_BoxFaces[4].vertices[1].Set( -1, -1, +1, 0, 1, 0xFF0000FF );
    m_BoxFaces[4].vertices[2].Set( +1, +1, +1, 1, 0, 0xFF0000FF );
    m_BoxFaces[4].vertices[3].Set( +1, -1, +1, 1, 1, 0xFF0000FF );

    // -z
    m_BoxFaces[5].vertices[0].Set( +1, +1, -1, 0, 0, 0xFF0000FF );
    m_BoxFaces[5].vertices[1].Set( +1, -1, -1, 0, 1, 0xFF0000FF );
    m_BoxFaces[5].vertices[2].Set( -1, +1, -1, 1, 0, 0xFF0000FF );
    m_BoxFaces[5].vertices[3].Set( -1, -1, -1, 1, 1, 0xFF0000FF );
}

//
//
// -----------------------------------------------------------------------------
bool D3D9OrientationBox::OnDeviceCreate( IDirect3DDevice9 * dev )
{
    if( NULL == dev )
    {
        LOG_ERROR( "Null device pointer!" );
        return false;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool D3D9OrientationBox::OnDeviceRestore()
{
    if( NULL == m_Device )
    {
        LOG_ERROR( "Null device pointer!" );
        return false;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
void D3D9OrientationBox::OnDeviceDispose()
{
}

//
//
// -----------------------------------------------------------------------------
void D3D9OrientationBox::OnDeviceDelete()
{
    for( int i = 0; i < 6; ++i )
    {
        SAFE_RELEASE( m_Textures[i] );
    }
    m_Device = 0;
}

//
//
// -----------------------------------------------------------------------------
void D3D9OrientationBox::Draw( float x, float y, float z, const XMMATRIX & view )
{
}

// *****************************************************************************
// private method
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------


