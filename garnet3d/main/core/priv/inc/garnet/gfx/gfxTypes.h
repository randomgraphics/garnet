#ifndef __GN_GFX_GFXTYPES_H__
#define __GN_GFX_GFXTYPES_H__
// *****************************************************************************
//! \file    gfxTypes.h
//! \brief   Define public types of GFX module
//! \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN
{
    namespace gfx
    {
        //!
        //! Device setting structure
        //!
        struct DeviceSettings
        {
            //!
            //! fullscreen or windowed mode.
            //!
            bool fullscreen;

            //!
            //! Width of fullscreen mode. Ignored in windowed mode.
            //! 0 means using current display mode's width.
            //!
            uint32_t fullscreen_width;

            //!
            //! Height of fullscreen mode. Ignored in windowed mode.
            //! 0 means using current display mode's height.
            //!
            uint32_t fullscreen_height;

            //!
            //! Color depth of fullscreen mode. Ignored in windowed mode.
            //! 0 means using current display mode's color depth.
            //!
            uint32_t fullscreen_depth;

            //!
            //! Refresh rate of fullscreen mode. Ignored in windowed mode.
            //! 0 means using current display mode's color depth.
            //!
            uint32_t fullscreen_refrate;

            //!
            //! Width of windowed mode. Ignored in fullscreen mode.
            //!
            uint32_t windowed_width;

            //!
            //! Width of windowed mode. Ignored in fullscreen mode.
            //!
            uint32_t windowed_height;

            bool vsync;      //!< 是否同步刷新

            //! \name D3D only parameters
            //@{
            bool software;   //!< using software device
            bool reference;  //!< using reference device
            //@}

            //! \name OGL only parameters
            //@{

            //! Restore display mode while render window is deactivated.
            //!
            //! Note that this is a OGL only parameter. For D3D, you may use
            //! "Enable Multi-mon Debugging" option in DirectX control panel,
            //! and startup your application through an debugger to make that
            //! option effective.
            //!
            bool auto_restore;

            //@}

            //!
            //! Equality operator
            //!
            bool operator == ( const DeviceSettings & rhs ) const
            {
                return 0 == ::memcmp( this, &rhs, sizeof(rhs) );
            }

            //!
            //! un-equality operator
            //!
            bool operator != ( const DeviceSettings & rhs ) const
            {
                return 0 != ::memcmp( this, &rhs, sizeof(rhs) );
            }
        };

        //!
        //! 渲染屏幕描述 (screen descriptor)
        //!
        struct ScreenDesc
        {
            bool fullscreen;  //!< fullscreen or windowed mode
            uint32_t width;   //!< screen width
            uint32_t height;  //!< screen height
            uint32_t depth;   //!< screen depth
            uint32_t refrate; //!< refresh rate
        };

        //! \def GNGFX_CAPS
        //! Define renderer caps

        //!
        //! renderer caps
        //!
        enum Caps
        {
            #define GNGFX_CAPS(X) CAPS_##X,
            #include "gfxCapsMeta.h"
            #undef GNGFX_CAPS
            NUM_CAPS,
            CAPS_INVALID
        };

        //!
        //! Vertex shader capability flags
        //!
        enum VSCaps
        {
            VSCAPS_D3D_1_1   = 1<<0, //!< DirectX vs.1.1
            VSCAPS_D3D_2_0   = 1<<1, //!< DirectX vs.2.0
            VSCAPS_D3D_2_X   = 1<<2, //!< DirectX vs.2.x
            VSCAPS_D3D_3_0   = 1<<3, //!< DirectX vs.3.0
            VSCAPS_OGL_ARB1  = 1<<4, //!< OpenGL ARB vertex program 1.0
            VSCAPS_OGL_GLSL  = 1<<5, //!< OpenGL shading language

            //!
            //! alias for all D3D vertex shader caps
            //!
            VSCAPS_D3D_ALL = VSCAPS_D3D_1_1 |
                             VSCAPS_D3D_2_0 |
                             VSCAPS_D3D_2_X |
                             VSCAPS_D3D_3_0
        };

        //!
        //! Pixel shader capability flags
        //!
        enum PSCaps
        {
            PSCAPS_D3D_1_1   = 1<<0, //!< DirectX ps.1.1
            PSCAPS_D3D_1_2   = 1<<1, //!< DirectX ps.1.2
            PSCAPS_D3D_1_3   = 1<<2, //!< DirectX ps.1.3
            PSCAPS_D3D_1_4   = 1<<3, //!< DirectX ps.1.4
            PSCAPS_D3D_2_0   = 1<<4, //!< DirectX ps.2.0
            PSCAPS_D3D_2_X   = 1<<5, //!< DirectX ps.2.x
            PSCAPS_D3D_3_0   = 1<<6, //!< DirectX ps.3.0
            PSCAPS_OGL_ARB1  = 1<<8, //!< OpenGL ARB pixel program 1.0
            PSCAPS_OGL_GLSL  = 1<<9, //!< OpenGL shading language

            //!
            //! alias for all D3D pixel shader caps
            //!
            PSCAPS_D3D_ALL = PSCAPS_D3D_1_1 |
                             PSCAPS_D3D_1_2 |
                             PSCAPS_D3D_1_3 |
                             PSCAPS_D3D_1_4 |
                             PSCAPS_D3D_2_0 |
                             PSCAPS_D3D_2_X |
                             PSCAPS_D3D_3_0
        };

        //!
        //! shader usage type
        //!
        enum ShaderUsage
        {
            SUSAGE_VERTEX,      //!< vertex shader
            SUSAGE_PIXEL,       //!< pixel shader
            NUM_SUSAGES         //!< number of available shader usages
        };

        //!
        //! shader language type
        //!
        enum ShadingLanguage
        {
            SLANG_FFP,          //!< fixed function pipeline
            SLANG_OGL_ARB,      //!< OpenGL ARB shader
            SLANG_OGL_GLSL,     //!< OpenGL shading language
            SLANG_D3D_ASM,      //!< DirectX ASM shader
            SLANG_D3D_HLSL,     //!< DirectX high level shading language
            SLANG_CG,           //!< Nvidia Cg
            NUM_SLANGS          //!< number of available languages
        };

        //!
        //! 贴图类型
        //!
        enum TexType
        {
            TEXTYPE_1D,
            TEXTYPE_2D,
            TEXTYPE_3D,
            TEXTYPE_CUBE,
            NUM_TEXTYPES
        };

        //!
        //! 贴图过滤器
        //!
        enum TexFilter
        {
            TEXFILTER_NEAREST,      //!< nearest
            TEXFILTER_LINEAR,       //!< linear
            TEXFILTER_N_MIP_N,      //!< NEAREST_MIPMAP_NEAREST
            TEXFILTER_N_MIP_L,      //!< NEAREST_MIPMAP_LINEAR
            TEXFILTER_L_MIP_N,      //!< LINEAR_MIPMAP_NEAREST
            TEXFILTER_L_MIP_L,      //!< LINEAR_MIPMAP_LINEAR
            NUM_TEXFILTERS
        };

        //!
        //! 贴图Wrap Mode
        //!
        enum TexWrap
        {
            TEXWRAP_REPEAT,
            TEXWRAP_CLAMP,
            TEXWRAP_CLAMP_TO_EDGE,
            NUM_TEXWRAPS
        };

        //!
        //! cube texture face
        //!
        enum TexFace
        {
            TEXFACE_PX, //!< positive X
            TEXFACE_NX, //!< negative X
            TEXFACE_PY, //!< positive Y
            TEXFACE_NY, //!< negative Y
            TEXFACE_PZ, //!< positive Z
            TEXFACE_NZ, //!< negative Z
            NUM_TEXFACES
        };

        //!
        //! Resource usage.
        //!
        enum ResourceUsage
        {
            USAGE_STATIC           = 0,    //!< normal static resource
            USAGE_DYNAMIC          = 1<<0, //!< dynamic resource
            USAGE_AUTOGEN_MIPMAP   = 1<<1, //!< texture that can auto-generate mipmaps
            USAGE_RENDERTARGET     = 1<<2, //!< render target texture
            USAGE_DEPTH            = 1<<3, //!< depth texture
            USAGE_NORMAL           = USAGE_STATIC
        };

        //!
        //! 锁定标志
        //!
        enum LockFlag
        {
            LOCK_RO = 1,                 //!< 只读锁定
            LOCK_WO = 1 << 1,            //!< 只写锁定
            LOCK_DISCARD = 1 << 2,       //!< Discard old content
            LOCK_NO_OVERWRITE = 1 << 3,  //!< no overwrite to existing content
            LOCK_RW = LOCK_RO | LOCK_WO, //!< 读写锁定
        };

        //!
        //! 2D锁定的返回结果
        //!
        struct LockedRect
        {
            size_t  pitch; //!< bytes per line
            void *  data;  //!< 指向被锁定图象的第一个字节
        };

        //!
        //! 3D锁定的返回结果
        //!
        struct LockedBox
        {
            size_t rowBytes;    //!< bytes per line
            size_t sliceBygtes; //!< bytes per slice
            void * data;        //!< 指向被锁定图象的第一个字节
        };

        //!
        //! 清屏标志
        //!
        enum ClearFlag
        {
            C_BUFFER       = 1,     //!< clear color buffer
            Z_BUFFER       = 2,     //!< clear z buffer
            S_BUFFER       = 4,     //!< clear stencil buffer
            ALL_BUFFERS    = 7,     //!< clear all buffers
        };
    }
}

// *****************************************************************************
//                           End of gfxTypes.h
// *****************************************************************************
#endif // __GN_GFX_GFXTYPES_H__
