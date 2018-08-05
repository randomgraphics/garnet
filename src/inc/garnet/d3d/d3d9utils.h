#ifndef __GN_D3D_D3D9UTILS_H__
#define __GN_D3D_D3D9UTILS_H__
// *****************************************************************************
/// \file
/// \brief   d3d9 utils
/// \author  chen@@CHENLI-HOMEPC (2007.4.16)
// *****************************************************************************

#include "../GNinput.h"

#if GN_BUILD_DEBUG_ENABLED
#define D3D_DEBUG_INFO // Enable "Enhanced D3DDebugging"
#endif

#if GN_XBOX2
#include <xtl.h>
#include <xgraphics.h>
#include <d3dx9.h>
#include <xnamath.h>
#elif GN_WINPC
#ifndef NOMINMAX
#define NOMINMAX ///< This is to disable windows min/max macros
#endif
#include <windows.h>
#include <d3d9.h>
#include <directxmath.h>
using namespace DirectX;
typedef ID3DBlob   ID3DXBuffer;
typedef ID3DBlob * LPD3DXBUFFER;
typedef void *     LPD3DXCONSTANTTABLE;
#endif

// Check d3d version
#if DIRECT3D_VERSION < 0x0900
#error "directx 9.0 or higher is required!"
#endif


// Define constants and types that are missing on Xenon platform.
#if GN_XBOX2

#define D3DLOCK_DISCARD 0
#define D3DUSAGE_DYNAMIC 0
#endif

namespace GN { /* namespace for D3D9 utils */ namespace d3d9
{
    ///
    /// scoped PIX event
    ///
    struct PixPerfScopeEvent
    {
        //@{
        PixPerfScopeEvent( D3DCOLOR color, const char * name )
        {
#if GN_XBOX2
            PIXBeginNamedEvent( color, name );
#else
            D3DPERF_BeginEvent( color, mbs2wcs(name).rawptr() );
#endif
        }
        ~PixPerfScopeEvent()
        {
#if GN_XBOX2
            PIXEndNamedEvent();
#else
            D3DPERF_EndEvent();
#endif
        }
        //@}
    };

    ///
    /// Compile vertex shader from string
    ///
    GN_API LPDIRECT3DVERTEXSHADER9 compileAndCreateVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, uint32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0, LPD3DXBUFFER * binary = 0 );

    ///
    /// Compile pixel shader from string
    ///
    GN_API LPDIRECT3DPIXELSHADER9 compileAndCreatePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, uint32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0 );

#if GN_BUILD_HAS_D3DX9
    ///
    /// Assemble vertex shader from string
    ///
    GN_API LPDIRECT3DVERTEXSHADER9 assembleAndCreateVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, uint32 flags = 0, LPD3DXBUFFER * binary = 0 );

    ///
    /// Assemble pixel shader from string
    ///
    GN_API LPDIRECT3DPIXELSHADER9 assembleAndCreatePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, uint32 flags = 0 );

#endif

    //@{

    ///
    /// D3D9 shader compiler template
    ///
    template<class SHADER_CLASS>
    struct ShaderCompiler
    {
    };

    ///
    /// D3D9 vertex shader compiler
    ///
    template<>
    struct ShaderCompiler<IDirect3DVertexShader9>
    {
        //@{

#if GN_PLATFORM_HAS_D3D9X
        static inline IDirect3DVertexShader9 *
        assembleAndCreate( IDirect3DDevice9 & dev, const char * code, size_t len = 0, uint32 flags = 0, LPD3DXBUFFER * binary = 0 )
        {
            return assembleAndCreateVS( &dev, code, len, flags, binary );
        }
#endif

        static inline IDirect3DVertexShader9 *
        compileAndCreate( IDirect3DDevice9 & dev, const char * code, size_t len = 0, uint32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0, LPD3DXBUFFER * binary = 0 )
        {
            return compileAndCreateVS( &dev, code, len, flags, entryFunc, profile, constTable, binary );
        }

        //@}
    };

    ///
    /// D3D9 pixel shader compiler
    ///
    template<>
    struct ShaderCompiler<IDirect3DPixelShader9>
    {
        //@{

#if GN_PLATFORM_HAS_D3D9X
        static inline IDirect3DPixelShader9 *
        assembleAndCreate( IDirect3DDevice9 & dev, const char * code, size_t len = 0, uint32 flags = 0 )
        {
            return assembleAndCreatePS( &dev, code, len, flags );
        }
#endif

        static inline IDirect3DPixelShader9 *
        compileAndCreate( IDirect3DDevice9 & dev, const char * code, size_t len = 0, uint32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0 )
        {
            return compileAndCreatePS( &dev, code, len, flags, entryFunc, profile, constTable );
        }

        //@}
    };

    ///
    /// Get backbuffer descriptor
    ///
    inline bool getBackBufferDesc( LPDIRECT3DDEVICE9 dev, D3DSURFACE_DESC & desc )
    {
        GN_GUARD_SLOW;
        GN_ASSERT( dev );
        AutoComPtr<IDirect3DSurface9> surf;
        GN_DX_CHECK_RETURN( dev->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &surf ), false );
        GN_DX_CHECK_RETURN( surf->GetDesc( &desc ), false );
		return true;
        GN_UNGUARD_SLOW;
    }

    ///
    /// Get backbuffer size
    ///
    inline Vector2<uint32> getBackBufferSize( LPDIRECT3DDEVICE9 dev )
    {
        GN_GUARD_SLOW;
        D3DSURFACE_DESC desc;
        return getBackBufferDesc( dev, desc )
            ? Vector2<uint32>( desc.Width, desc.Height )
            : Vector2<uint32>( 0, 0 );
        GN_UNGUARD_SLOW;
    }

    ///
    /// Get depth buffer descriptor
    ///
    inline bool getDepthStencilDesc( LPDIRECT3DDEVICE9 dev, D3DSURFACE_DESC & desc )
    {
        GN_GUARD_SLOW;
        GN_ASSERT( dev );
        AutoComPtr<IDirect3DSurface9> surf;
        GN_DX_CHECK_RETURN( dev->GetDepthStencilSurface( &surf ), false );
        GN_DX_CHECK_RETURN( surf->GetDesc( &desc ), false );
		return true;
        GN_UNGUARD_SLOW;
    }

    ///
    /// Get depth buffer size
    ///
    inline Vector2<uint32> getDepthStencilSize( LPDIRECT3DDEVICE9 dev )
    {
        GN_GUARD_SLOW;
        D3DSURFACE_DESC desc;
        return getDepthStencilDesc( dev, desc )
            ? Vector2<uint32>( desc.Width, desc.Height )
            : Vector2<uint32>( 0, 0 );
        GN_UNGUARD_SLOW;
    }

    class GN_API D3D9RenderStateSaver
    {
        struct RenderStateItem
        {
            DWORD oldValue;
            DWORD currentValue;
            bool  got;
        };

        static const UINT MAX_RS = 256;
#if !GN_XBOX2
        static const UINT MAX_TSS = 64;
#endif
        static const UINT MAX_SS = 16;
        static const UINT MAX_STAGES = 16;

        Logger           * m_Logger;
        IDirect3DDevice9 * m_Device;
        RenderStateItem    m_Rs[MAX_RS];
#if !GN_XBOX2
        RenderStateItem    m_Tss[MAX_STAGES][MAX_TSS];
#endif
        RenderStateItem    m_Ss[MAX_STAGES][MAX_SS];

    public:

        D3D9RenderStateSaver( IDirect3DDevice9 * dev )
            : m_Logger(getLogger("GN.d3d9"))
        {
            ::memset( this, 0, sizeof(*this) );
            m_Device = dev;
        }

        ~D3D9RenderStateSaver()
        {
            RestoreAll();
        }

        IDirect3DDevice9 * GetDevice() const { return m_Device; }

        void SetRS( D3DRENDERSTATETYPE type, DWORD value )
        {
            if( NULL == m_Device ) return;

            if( (UINT)type >= MAX_RS )
            {
                // Must be an invalid D3D9 render state type.
                GN_ERROR(m_Logger)( "Invalid D3D render state: %d", type );
                return;
            }

            if( !m_Rs[type].got )
            {
                // This is an new state that has never been set.
                DWORD current;
                if( SUCCEEDED( m_Device->GetRenderState( type, &current ) ) )
                {
                    m_Rs[type].oldValue     = current;
                    m_Rs[type].currentValue = current;
                    m_Rs[type].got          = true;
                }
                else
                {
                    GN_ERROR(m_Logger)( "Fail to get value of D3D render state: %d", type );
                }
            }

            if( !m_Rs[type].got || value != m_Rs[type].currentValue )
            {
                m_Device->SetRenderState( type, value );
                m_Rs[type].currentValue = value;
            }
        }

#if !GN_XBOX2
        void SetTSS( DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD value )
        {
            if( NULL == m_Device ) return;

            if( stage >= MAX_STAGES || (UINT)type >= MAX_TSS )
            {
                GN_ERROR(m_Logger)( "Invalid D3D texture stage state: stage(%d), type(%d)", stage, type );
                return;
            }

            if( !m_Tss[stage][type].got )
            {
                // This is an new state that has never been set.
                DWORD current;
                if( SUCCEEDED( m_Device->GetTextureStageState( stage, type, &current ) ) )
                {
                    m_Tss[stage][type].oldValue     = current;
                    m_Tss[stage][type].currentValue = current;
                    m_Tss[stage][type].got          = true;
                }
                else
                {
                    GN_ERROR(m_Logger)( "Fail to get value of D3D render state: %d", type );
                }
            }

            if( !m_Tss[stage][type].got || value != m_Tss[stage][type].currentValue )
            {
                m_Device->SetTextureStageState( stage, type, value );
                m_Tss[stage][type].currentValue = value;
            }
        }
#endif

        void SetSS( DWORD stage, D3DSAMPLERSTATETYPE type, DWORD value )
        {
            if( NULL == m_Device ) return;

            if( stage >= MAX_STAGES || (UINT)type >= MAX_SS )
            {
                GN_ERROR(m_Logger)( "Invalid D3D sampler state: stage(%d), type(%d)", stage, type );
                return;
            }

            if( !m_Ss[stage][type].got )
            {
                // This is an new state that has never been set.
                DWORD current;
                if( SUCCEEDED( m_Device->GetSamplerState( stage, type, &current ) ) )
                {
                    m_Ss[stage][type].oldValue     = current;
                    m_Ss[stage][type].currentValue = current;
                    m_Ss[stage][type].got         = true;
                }
                else
                {
                    GN_ERROR(m_Logger)( "Fail to get value of D3D render state: %d", type );
                }
            }

            if( !m_Ss[stage][type].got || value != m_Ss[stage][type].currentValue )
            {
                m_Device->SetSamplerState( stage, type, value );
                m_Ss[stage][type].currentValue = value;
            }
        }

        void RestoreAll()
        {
            if( NULL == m_Device ) return;

            // restore RS
            for( UINT i = 0; i < MAX_RS; ++i )
            {
                if( m_Rs[i].got && m_Rs[i].currentValue != m_Rs[i].oldValue )
                {

                    m_Device->SetRenderState( (D3DRENDERSTATETYPE)i, m_Rs[i].oldValue );
                }
                m_Rs[i].got = false;
            }

            // restore TSS and SS
            for( UINT s = 0; s < MAX_STAGES; ++s )
            {
#if !GN_XBOX2
                for( UINT i = 0; i < MAX_TSS; ++i )
                {
                    if( m_Tss[s][i].got && m_Tss[s][i].currentValue != m_Tss[s][i].oldValue )
                    {
                        m_Device->SetTextureStageState( s, (D3DTEXTURESTAGESTATETYPE)i, m_Tss[s][i].oldValue );
                    }
                    m_Tss[s][i].got = false;
                }
#endif
                for( UINT i = 0; i < MAX_SS; ++i )
                {
                    if( m_Ss[s][i].got && m_Ss[s][i].currentValue != m_Ss[s][i].oldValue )
                    {
                        m_Device->SetSamplerState( s, (D3DSAMPLERSTATETYPE)i, m_Ss[s][i].oldValue );
                    }
                    m_Ss[s][i].got = false;
                }
            }
        }
    };

    struct ThickLineVertex
    {
        float    x, y, z;
        float    u, v;
        D3DCOLOR color;
    };

    struct ThickLineParameters
    {
        XMMATRIX worldview;          // matrix that transform vertex from object space to view space
        XMMATRIX proj;               // matrix that transform vertex from view space to clip space
        float    width;              // line width
        bool     widthInScreenSpace; // true  : line width is in pixels in screen space
                                     // false : line width is in view space
    };

    class GN_API D3D9ThickLineRenderer
    {

    public:

        D3D9ThickLineRenderer();
        ~D3D9ThickLineRenderer() { onDeviceDispose(); onDeviceDelete(); }

        bool onDeviceCreate( IDirect3DDevice9 * dev );
        bool onDeviceRestore();
        void onDeviceDispose();
        void onDeviceDelete();

        bool drawBegin( const ThickLineParameters & parameters );
        void drawEnd();

        void line( const ThickLineVertex & v0, const ThickLineVertex & v1 );
        void line( float x1, float y1, float z1, float x2, float y2, float z2, D3DCOLOR color );
        void lineList( const ThickLineVertex * vertices, size_t numverts );

    private:

        struct PrivateParameters : public ThickLineParameters
        {
            XMMATRIX wvp; // world * view * proj
            float screenWidth, screenHeight; // screen size in pixels
            float endPointHalfWidth, endPointHalfHeight; // size of line end point in clip space.
        };

        struct PrivateVertex
        {
            XMVECTOR position; // position in clip space
            D3DCOLOR color;
            float    u, v;
            uint32   _; // padding
        };

        struct EndPoint
        {
            float posl, posr, post, posb, posz, posw;
            float texl, texr, text, texb;
            D3DCOLOR color;

            PrivateVertex * topLeft( PrivateVertex * v );
            PrivateVertex * topRight( PrivateVertex * v );
            PrivateVertex * bottomLeft( PrivateVertex * v );
            PrivateVertex * bottomRight( PrivateVertex * v );
        };

    private:

        IDirect3DDevice9 * m_Device;
        IDirect3DVertexShader9 * m_Vs;
        IDirect3DPixelShader9 * m_Ps;
        IDirect3DVertexDeclaration9 * m_Decl;
        IDirect3DVertexBuffer9 * m_Vb;
        IDirect3DIndexBuffer9 * m_Ib;

        bool m_Drawing;
        PrivateParameters m_Parameters;

        static const size_t MAX_VERTICES = 1024;
        PrivateVertex m_Vertices[MAX_VERTICES];
        uint16        m_Indices[MAX_VERTICES/6*12]; // 12 indices (4 triangles ) per 6 verices
        uint32        m_NumVertices;


    private:

        void clear();

        void calcEndPoint(
            EndPoint              & endpoint,
            const ThickLineVertex & vertex );

        PrivateVertex * newPolygon6();

        void flush();
    };

    ///
    /// D3D9 application framework
    ///
    struct D3D9AppOption
    {
        ///
        /// ctor
        ///
        D3D9AppOption()
            : parent(0)
            , monitor(0)
            , refdev(false)
            , vsync(false)
            , fullscreen(false)
            , fsWidth(0)
            , fsHeight(0)
            , fsDepth(0)
            , fsRefrate(0)
            , windowedWidth(640)
            , windowedHeight(480)
        {
        }

        ///
        /// Handle of parent window. Default is zero, means a top-level window.
        ///
        HWND parent;

        ///
        /// Monitor handle.
        ///
        /// - 0 means using the monitor where parent window stays in.
        ///   If monitor and parent are both zero, primary monitor will be used.
        /// - Default is 0.
        ///
        HMONITOR monitor;

        ///
        /// use ref device. Default is false.
        ///
        bool refdev;

        ///
        /// V-SYNC enable or not. Default is false.
        ///
        bool vsync;

        ///
        /// fullscreen or windowed mode.
        /// Default is false.
        ///
        bool fullscreen;

        ///
        /// full screen display mode
        ///
        //@{
        uint32 fsWidth;   ///< Screen width.
        uint32 fsHeight;  ///< Screen height.
        uint32 fsDepth;   ///< Color depth. Ignored for windowed mode.
        uint32 fsRefrate; ///< Referesh rate. Ignored for windowed mode.
        //@}

        ///
        /// Backbuffer width for windowed mode. Ignored in fullscreen mode.
        /// Zero means using client width of render window. If render window
        /// is also not avaiable, default width 640 will be used.
        /// Default is 0.
        ///
        uint32 windowedWidth;

        ///
        /// Backbuffer height for windowed mode. Ignored in fullscreen mode.
        /// Zero means using client height of render window. If render window
        /// is also not avaiable, default height 480 will be used.
        /// Default is 0.
        ///
        uint32 windowedHeight;
    };

    ///
    /// D3D9 application framework
    ///
    class GN_API D3D9Application : public SlotBase
    {
    public:

        //@{

        D3D9Application();
        ~D3D9Application();

        HWND               window() const { return mWindow; }
        IDirect3DDevice9 & d3d9dev() const { GN_ASSERT( mDevice ); return *mDevice; }

        int run( const D3D9AppOption * = 0 );

        bool changeOption( const D3D9AppOption & );

        //@}

    protected:

        //@{

        virtual bool onInit( D3D9AppOption & ) { return true; }
        virtual bool onCreate() { return true; }
        virtual bool onRestore() { return true; }
        virtual void onDispose() {}
        virtual void onDestroy() {}
        virtual void onQuit() {}

        virtual void onDraw() {}
        virtual void onUpdate() {}
        virtual void onKeyPress( input::KeyEvent );
        virtual void onCharPress( wchar_t ) {}
        virtual void onAxisMove( input::Axis, int ) {}

        //@}

    private:

        bool init();
        void quit();

        bool createDevice();
        bool restoreDevice();
        void disposeDevice();
        void destroyDevice();

    private:

        D3D9AppOption         mOption;
        HWND                  mWindow;
        UINT                  mAdapter;
        D3DDEVTYPE            mDeviceType;
        UINT                  mBehavior;
        D3DPRESENT_PARAMETERS mPresentParameters;
        IDirect3D9          * mD3D;
        IDirect3DDevice9    * mDevice;
        bool                  mRunning;
        bool                  mShutdownInputSystem;
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_D3D_D3D9UTILS_H__
