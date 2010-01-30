#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.d3d10.ResourcePool");

static const GUID POOL_PTR_GUID = { 0xc6f87514, 0x48b1, 0x4e3c, { 0x97, 0x6a, 0x20, 0xd4, 0xb4, 0x74, 0xe4, 0x34 } };

// *****************************************************************************
// Local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// GN::d3d10::ResourcePool::PooledResourceDesc
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline bool GN::d3d10::ResourcePool::PooledResourceDesc::isImmutable() const
{
    switch( dim )
    {
        case D3D10_RESOURCE_DIMENSION_BUFFER    : return D3D10_USAGE_IMMUTABLE & buf.Usage;
        case D3D10_RESOURCE_DIMENSION_TEXTURE1D : return D3D10_USAGE_IMMUTABLE & tex1d.Usage;
        case D3D10_RESOURCE_DIMENSION_TEXTURE2D : return D3D10_USAGE_IMMUTABLE & tex2d.Usage;
        case D3D10_RESOURCE_DIMENSION_TEXTURE3D : return D3D10_USAGE_IMMUTABLE & tex3d.Usage;
        default                                 : GN_UNEXPECTED(); return false;
    }
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::d3d10::ResourcePool::init( ID3D10Device * dev )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::d3d10::ResourcePool, () );

    if( NULL == dev )
    {
        GN_ERROR(sLogger)( "NULL device pointer!" );
        return failure();
    }

    m_device = dev;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::ResourcePool::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
ID3D10Buffer *
GN::d3d10::ResourcePool::createBuffer( const D3D10_BUFFER_DESC & desc, const D3D10_SUBRESOURCE_DATA * data )
{
    return (ID3D10Buffer*)findOrCreateResource( desc, data );
}

//
//
// -----------------------------------------------------------------------------
void
GN::d3d10::ResourcePool::returnResource( ID3D10Resource * resource )
{
    // Silently ignore NULL pointer
    if( NULL == resource ) return;

    // check pool pointer
    UINT   size;
    void * ptr;
    if( S_OK != resource->GetPrivateData( POOL_PTR_GUID, &size, &ptr ) || ptr != this )
    {
        GN_ERROR(sLogger)( "Resource does NOT belong to the pool!" );
        return;
    }

    PooledResourceDesc desc;
    ::memset( &desc, 0, sizeof(desc) );

    // get resource type
    resource->GetType( &desc.dim );

    // get resource description
    bool immutable = false;
    switch( desc.dim )
    {
        case D3D10_RESOURCE_DIMENSION_BUFFER:
        {
            ((ID3D10Buffer*)resource)->GetDesc( &desc.buf );
            immutable = D3D10_USAGE_IMMUTABLE & desc.buf.Usage;
            break;
        }

        case D3D10_RESOURCE_DIMENSION_TEXTURE1D:
        {
            ((ID3D10Texture1D*)resource)->GetDesc( &desc.tex1d );
            immutable = D3D10_USAGE_IMMUTABLE & desc.tex1d.Usage;
            break;
        }

        case D3D10_RESOURCE_DIMENSION_TEXTURE2D:
        {
            ((ID3D10Texture2D*)resource)->GetDesc( &desc.tex2d );
            immutable = D3D10_USAGE_IMMUTABLE & desc.tex2d.Usage;
            break;
        }

        case D3D10_RESOURCE_DIMENSION_TEXTURE3D:
        {
            ((ID3D10Texture3D*)resource)->GetDesc( &desc.tex3d );
            immutable = D3D10_USAGE_IMMUTABLE & desc.tex3d.Usage;
            break;
        }

        default:
            GN_UNEXPECTED_EX( "invalid resource dimension" );
    }

    // Release immutable resource, instead of returning it to pool
    if( immutable )
    {
        resource->Release();
    }
    else
    {
        m_resources[desc] = resource;
    }
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
ID3D10Resource *
GN::d3d10::ResourcePool::findOrCreateResource( const PooledResourceDesc & desc, const D3D10_SUBRESOURCE_DATA * data )
{
    GN_GUARD;

    // Note: there's no way to reuse immutable resource.
    if( !desc.isImmutable() )
    {
        ResourceMap::iterator it = m_resources.find( desc );

        if( m_resources.end() != it )
        {
            // found!
            ID3D10Resource * res = it->second;
            GN_ASSERT( res );

            // update resource data
            if( data )
            {
                UInt32 numSubresources = 0;
                switch( desc.dim )
                {
                    case D3D10_RESOURCE_DIMENSION_BUFFER :
                        numSubresources = 1;
                        break;

                    case D3D10_RESOURCE_DIMENSION_TEXTURE1D :
                    case D3D10_RESOURCE_DIMENSION_TEXTURE2D :
                    case D3D10_RESOURCE_DIMENSION_TEXTURE3D :
                        GN_TODO( "calculate number of subresources for texture" );
                        numSubresources = 1;
                        break;

                    default:
                        GN_UNEXPECTED_EX( "invalid resource dimension" );
                        break;
                };

                for( UINT i = 0; i < numSubresources; ++i )
                {
                    m_device->UpdateSubresource( res, i, NULL, data[i].pSysMem, data[i].SysMemPitch, data[i].SysMemSlicePitch );
                }
            }

            return res;
        }
    }

    // try create new one
    ID3D10Resource * newResource = createResource( desc, data );
    if( NULL == newResource )
    {
        GN_ERROR(sLogger)( "Fail to create new resource." );
        return NULL;
    }

    m_resources[desc] = newResource;

    return newResource;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
ID3D10Resource *
GN::d3d10::ResourcePool::createResource( const PooledResourceDesc & desc, const D3D10_SUBRESOURCE_DATA * data )
{
    GN_GUARD;

    ID3D10Resource * res;

    switch( desc.dim )
    {
        case D3D10_RESOURCE_DIMENSION_BUFFER:
        {
            ID3D10Buffer * buf;
            GN_DX_CHECK_RETURN( m_device->CreateBuffer( &desc.buf, data, &buf ), NULL );
            res = buf;
            break;
        }

        case D3D10_RESOURCE_DIMENSION_TEXTURE1D:
        {
            ID3D10Texture1D * tex;
            GN_DX_CHECK_RETURN( m_device->CreateTexture1D( &desc.tex1d, data, &tex ), NULL );
            res = tex;
            break;
        }

        case D3D10_RESOURCE_DIMENSION_TEXTURE2D:
        {
            ID3D10Texture2D * tex;
            GN_DX_CHECK_RETURN( m_device->CreateTexture2D( &desc.tex2d, data, &tex ), NULL );
            res = tex;
            break;
        }

        case D3D10_RESOURCE_DIMENSION_TEXTURE3D:
        {
            ID3D10Texture3D * tex;
            GN_DX_CHECK_RETURN( m_device->CreateTexture3D( &desc.tex3d, data, &tex ), NULL );
            res = tex;
            break;
        }

        default:
            GN_UNEXPECTED_EX( "invalid resource dimension!" );
            return NULL;
    }

    // attach pool pointer to the resource
    void * thisPtr = this;
    res->SetPrivateData( POOL_PTR_GUID, sizeof(void*), &thisPtr );

    // success
    return res;

    GN_UNGUARD;
}
