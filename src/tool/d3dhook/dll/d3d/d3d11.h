/*-------------------------------------------------------------------------------------
 *
 * Copyright (c) Microsoft Corporation
 *
 *-------------------------------------------------------------------------------------*/

/* this ALWAYS GENERATED file contains the definitions for the interfaces */

/* File created by MIDL compiler version 8.00.0595 */
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning(disable : 4049) /* more than 64k source lines */

/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
    #define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

/* verify that the <rpcsal.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCSAL_H_VERSION__
    #define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
    #error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
    #include "windows.h"
    #include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __d3d11_h__
    #define __d3d11_h__

    #if defined(_MSC_VER) && (_MSC_VER >= 1020)
        #pragma once
    #endif

/* Forward Declarations */

    #ifndef __ID3D11DeviceChild_FWD_DEFINED__
        #define __ID3D11DeviceChild_FWD_DEFINED__
typedef interface ID3D11DeviceChild ID3D11DeviceChild;

    #endif /* __ID3D11DeviceChild_FWD_DEFINED__ */

    #ifndef __ID3D11DepthStencilState_FWD_DEFINED__
        #define __ID3D11DepthStencilState_FWD_DEFINED__
typedef interface ID3D11DepthStencilState ID3D11DepthStencilState;

    #endif /* __ID3D11DepthStencilState_FWD_DEFINED__ */

    #ifndef __ID3D11BlendState_FWD_DEFINED__
        #define __ID3D11BlendState_FWD_DEFINED__
typedef interface ID3D11BlendState ID3D11BlendState;

    #endif /* __ID3D11BlendState_FWD_DEFINED__ */

    #ifndef __ID3D11RasterizerState_FWD_DEFINED__
        #define __ID3D11RasterizerState_FWD_DEFINED__
typedef interface ID3D11RasterizerState ID3D11RasterizerState;

    #endif /* __ID3D11RasterizerState_FWD_DEFINED__ */

    #ifndef __ID3D11Resource_FWD_DEFINED__
        #define __ID3D11Resource_FWD_DEFINED__
typedef interface ID3D11Resource ID3D11Resource;

    #endif /* __ID3D11Resource_FWD_DEFINED__ */

    #ifndef __ID3D11Buffer_FWD_DEFINED__
        #define __ID3D11Buffer_FWD_DEFINED__
typedef interface ID3D11Buffer ID3D11Buffer;

    #endif /* __ID3D11Buffer_FWD_DEFINED__ */

    #ifndef __ID3D11Texture1D_FWD_DEFINED__
        #define __ID3D11Texture1D_FWD_DEFINED__
typedef interface ID3D11Texture1D ID3D11Texture1D;

    #endif /* __ID3D11Texture1D_FWD_DEFINED__ */

    #ifndef __ID3D11Texture2D_FWD_DEFINED__
        #define __ID3D11Texture2D_FWD_DEFINED__
typedef interface ID3D11Texture2D ID3D11Texture2D;

    #endif /* __ID3D11Texture2D_FWD_DEFINED__ */

    #ifndef __ID3D11Texture3D_FWD_DEFINED__
        #define __ID3D11Texture3D_FWD_DEFINED__
typedef interface ID3D11Texture3D ID3D11Texture3D;

    #endif /* __ID3D11Texture3D_FWD_DEFINED__ */

    #ifndef __ID3D11View_FWD_DEFINED__
        #define __ID3D11View_FWD_DEFINED__
typedef interface ID3D11View ID3D11View;

    #endif /* __ID3D11View_FWD_DEFINED__ */

    #ifndef __ID3D11ShaderResourceView_FWD_DEFINED__
        #define __ID3D11ShaderResourceView_FWD_DEFINED__
typedef interface ID3D11ShaderResourceView ID3D11ShaderResourceView;

    #endif /* __ID3D11ShaderResourceView_FWD_DEFINED__ */

    #ifndef __ID3D11RenderTargetView_FWD_DEFINED__
        #define __ID3D11RenderTargetView_FWD_DEFINED__
typedef interface ID3D11RenderTargetView ID3D11RenderTargetView;

    #endif /* __ID3D11RenderTargetView_FWD_DEFINED__ */

    #ifndef __ID3D11DepthStencilView_FWD_DEFINED__
        #define __ID3D11DepthStencilView_FWD_DEFINED__
typedef interface ID3D11DepthStencilView ID3D11DepthStencilView;

    #endif /* __ID3D11DepthStencilView_FWD_DEFINED__ */

    #ifndef __ID3D11UnorderedAccessView_FWD_DEFINED__
        #define __ID3D11UnorderedAccessView_FWD_DEFINED__
typedef interface ID3D11UnorderedAccessView ID3D11UnorderedAccessView;

    #endif /* __ID3D11UnorderedAccessView_FWD_DEFINED__ */

    #ifndef __ID3D11VertexShader_FWD_DEFINED__
        #define __ID3D11VertexShader_FWD_DEFINED__
typedef interface ID3D11VertexShader ID3D11VertexShader;

    #endif /* __ID3D11VertexShader_FWD_DEFINED__ */

    #ifndef __ID3D11HullShader_FWD_DEFINED__
        #define __ID3D11HullShader_FWD_DEFINED__
typedef interface ID3D11HullShader ID3D11HullShader;

    #endif /* __ID3D11HullShader_FWD_DEFINED__ */

    #ifndef __ID3D11DomainShader_FWD_DEFINED__
        #define __ID3D11DomainShader_FWD_DEFINED__
typedef interface ID3D11DomainShader ID3D11DomainShader;

    #endif /* __ID3D11DomainShader_FWD_DEFINED__ */

    #ifndef __ID3D11GeometryShader_FWD_DEFINED__
        #define __ID3D11GeometryShader_FWD_DEFINED__
typedef interface ID3D11GeometryShader ID3D11GeometryShader;

    #endif /* __ID3D11GeometryShader_FWD_DEFINED__ */

    #ifndef __ID3D11PixelShader_FWD_DEFINED__
        #define __ID3D11PixelShader_FWD_DEFINED__
typedef interface ID3D11PixelShader ID3D11PixelShader;

    #endif /* __ID3D11PixelShader_FWD_DEFINED__ */

    #ifndef __ID3D11ComputeShader_FWD_DEFINED__
        #define __ID3D11ComputeShader_FWD_DEFINED__
typedef interface ID3D11ComputeShader ID3D11ComputeShader;

    #endif /* __ID3D11ComputeShader_FWD_DEFINED__ */

    #ifndef __ID3D11InputLayout_FWD_DEFINED__
        #define __ID3D11InputLayout_FWD_DEFINED__
typedef interface ID3D11InputLayout ID3D11InputLayout;

    #endif /* __ID3D11InputLayout_FWD_DEFINED__ */

    #ifndef __ID3D11SamplerState_FWD_DEFINED__
        #define __ID3D11SamplerState_FWD_DEFINED__
typedef interface ID3D11SamplerState ID3D11SamplerState;

    #endif /* __ID3D11SamplerState_FWD_DEFINED__ */

    #ifndef __ID3D11Asynchronous_FWD_DEFINED__
        #define __ID3D11Asynchronous_FWD_DEFINED__
typedef interface ID3D11Asynchronous ID3D11Asynchronous;

    #endif /* __ID3D11Asynchronous_FWD_DEFINED__ */

    #ifndef __ID3D11Query_FWD_DEFINED__
        #define __ID3D11Query_FWD_DEFINED__
typedef interface ID3D11Query ID3D11Query;

    #endif /* __ID3D11Query_FWD_DEFINED__ */

    #ifndef __ID3D11Predicate_FWD_DEFINED__
        #define __ID3D11Predicate_FWD_DEFINED__
typedef interface ID3D11Predicate ID3D11Predicate;

    #endif /* __ID3D11Predicate_FWD_DEFINED__ */

    #ifndef __ID3D11Counter_FWD_DEFINED__
        #define __ID3D11Counter_FWD_DEFINED__
typedef interface ID3D11Counter ID3D11Counter;

    #endif /* __ID3D11Counter_FWD_DEFINED__ */

    #ifndef __ID3D11ClassInstance_FWD_DEFINED__
        #define __ID3D11ClassInstance_FWD_DEFINED__
typedef interface ID3D11ClassInstance ID3D11ClassInstance;

    #endif /* __ID3D11ClassInstance_FWD_DEFINED__ */

    #ifndef __ID3D11ClassLinkage_FWD_DEFINED__
        #define __ID3D11ClassLinkage_FWD_DEFINED__
typedef interface ID3D11ClassLinkage ID3D11ClassLinkage;

    #endif /* __ID3D11ClassLinkage_FWD_DEFINED__ */

    #ifndef __ID3D11CommandList_FWD_DEFINED__
        #define __ID3D11CommandList_FWD_DEFINED__
typedef interface ID3D11CommandList ID3D11CommandList;

    #endif /* __ID3D11CommandList_FWD_DEFINED__ */

    #ifndef __ID3D11DeviceContext_FWD_DEFINED__
        #define __ID3D11DeviceContext_FWD_DEFINED__
typedef interface ID3D11DeviceContext ID3D11DeviceContext;

    #endif /* __ID3D11DeviceContext_FWD_DEFINED__ */

    #ifndef __ID3D11VideoDecoder_FWD_DEFINED__
        #define __ID3D11VideoDecoder_FWD_DEFINED__
typedef interface ID3D11VideoDecoder ID3D11VideoDecoder;

    #endif /* __ID3D11VideoDecoder_FWD_DEFINED__ */

    #ifndef __ID3D11VideoProcessorEnumerator_FWD_DEFINED__
        #define __ID3D11VideoProcessorEnumerator_FWD_DEFINED__
typedef interface ID3D11VideoProcessorEnumerator ID3D11VideoProcessorEnumerator;

    #endif /* __ID3D11VideoProcessorEnumerator_FWD_DEFINED__ */

    #ifndef __ID3D11VideoProcessor_FWD_DEFINED__
        #define __ID3D11VideoProcessor_FWD_DEFINED__
typedef interface ID3D11VideoProcessor ID3D11VideoProcessor;

    #endif /* __ID3D11VideoProcessor_FWD_DEFINED__ */

    #ifndef __ID3D11AuthenticatedChannel_FWD_DEFINED__
        #define __ID3D11AuthenticatedChannel_FWD_DEFINED__
typedef interface ID3D11AuthenticatedChannel ID3D11AuthenticatedChannel;

    #endif /* __ID3D11AuthenticatedChannel_FWD_DEFINED__ */

    #ifndef __ID3D11CryptoSession_FWD_DEFINED__
        #define __ID3D11CryptoSession_FWD_DEFINED__
typedef interface ID3D11CryptoSession ID3D11CryptoSession;

    #endif /* __ID3D11CryptoSession_FWD_DEFINED__ */

    #ifndef __ID3D11VideoDecoderOutputView_FWD_DEFINED__
        #define __ID3D11VideoDecoderOutputView_FWD_DEFINED__
typedef interface ID3D11VideoDecoderOutputView ID3D11VideoDecoderOutputView;

    #endif /* __ID3D11VideoDecoderOutputView_FWD_DEFINED__ */

    #ifndef __ID3D11VideoProcessorInputView_FWD_DEFINED__
        #define __ID3D11VideoProcessorInputView_FWD_DEFINED__
typedef interface ID3D11VideoProcessorInputView ID3D11VideoProcessorInputView;

    #endif /* __ID3D11VideoProcessorInputView_FWD_DEFINED__ */

    #ifndef __ID3D11VideoProcessorOutputView_FWD_DEFINED__
        #define __ID3D11VideoProcessorOutputView_FWD_DEFINED__
typedef interface ID3D11VideoProcessorOutputView ID3D11VideoProcessorOutputView;

    #endif /* __ID3D11VideoProcessorOutputView_FWD_DEFINED__ */

    #ifndef __ID3D11VideoContext_FWD_DEFINED__
        #define __ID3D11VideoContext_FWD_DEFINED__
typedef interface ID3D11VideoContext ID3D11VideoContext;

    #endif /* __ID3D11VideoContext_FWD_DEFINED__ */

    #ifndef __ID3D11VideoDevice_FWD_DEFINED__
        #define __ID3D11VideoDevice_FWD_DEFINED__
typedef interface ID3D11VideoDevice ID3D11VideoDevice;

    #endif /* __ID3D11VideoDevice_FWD_DEFINED__ */

    #ifndef __ID3D11Device_FWD_DEFINED__
        #define __ID3D11Device_FWD_DEFINED__
typedef interface ID3D11Device ID3D11Device;

    #endif /* __ID3D11Device_FWD_DEFINED__ */

    /* header files for imported files */
    #include "oaidl.h"
    #include "ocidl.h"
    #include "dxgi.h"
    #include "d3dcommon.h"

    #ifdef __cplusplus
extern "C" {
    #endif

/* interface __MIDL_itf_d3d11_0000_0000 */
/* [local] */

    #ifndef _D3D11_CONSTANTS
        #define _D3D11_CONSTANTS
        #define D3D11_16BIT_INDEX_STRIP_CUT_VALUE (0xffff)

        #define D3D11_32BIT_INDEX_STRIP_CUT_VALUE (0xffffffff)

        #define D3D11_8BIT_INDEX_STRIP_CUT_VALUE (0xff)

        #define D3D11_ARRAY_AXIS_ADDRESS_RANGE_BIT_COUNT (9)

        #define D3D11_CLIP_OR_CULL_DISTANCE_COUNT (8)

        #define D3D11_CLIP_OR_CULL_DISTANCE_ELEMENT_COUNT (2)

        #define D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT (14)

        #define D3D11_COMMONSHADER_CONSTANT_BUFFER_COMPONENTS (4)

        #define D3D11_COMMONSHADER_CONSTANT_BUFFER_COMPONENT_BIT_COUNT (32)

        #define D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT (15)

        #define D3D11_COMMONSHADER_CONSTANT_BUFFER_PARTIAL_UPDATE_EXTENTS_BYTE_ALIGNMENT (16)

        #define D3D11_COMMONSHADER_CONSTANT_BUFFER_REGISTER_COMPONENTS (4)

        #define D3D11_COMMONSHADER_CONSTANT_BUFFER_REGISTER_COUNT (15)

        #define D3D11_COMMONSHADER_CONSTANT_BUFFER_REGISTER_READS_PER_INST (1)

        #define D3D11_COMMONSHADER_CONSTANT_BUFFER_REGISTER_READ_PORTS (1)

        #define D3D11_COMMONSHADER_FLOWCONTROL_NESTING_LIMIT (64)

        #define D3D11_COMMONSHADER_IMMEDIATE_CONSTANT_BUFFER_REGISTER_COMPONENTS (4)

        #define D3D11_COMMONSHADER_IMMEDIATE_CONSTANT_BUFFER_REGISTER_COUNT (1)

        #define D3D11_COMMONSHADER_IMMEDIATE_CONSTANT_BUFFER_REGISTER_READS_PER_INST (1)

        #define D3D11_COMMONSHADER_IMMEDIATE_CONSTANT_BUFFER_REGISTER_READ_PORTS (1)

        #define D3D11_COMMONSHADER_IMMEDIATE_VALUE_COMPONENT_BIT_COUNT (32)

        #define D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COMPONENTS (1)

        #define D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT (128)

        #define D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_READS_PER_INST (1)

        #define D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_READ_PORTS (1)

        #define D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT (128)

        #define D3D11_COMMONSHADER_SAMPLER_REGISTER_COMPONENTS (1)

        #define D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT (16)

        #define D3D11_COMMONSHADER_SAMPLER_REGISTER_READS_PER_INST (1)

        #define D3D11_COMMONSHADER_SAMPLER_REGISTER_READ_PORTS (1)

        #define D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT (16)

        #define D3D11_COMMONSHADER_SUBROUTINE_NESTING_LIMIT (32)

        #define D3D11_COMMONSHADER_TEMP_REGISTER_COMPONENTS (4)

        #define D3D11_COMMONSHADER_TEMP_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_COMMONSHADER_TEMP_REGISTER_COUNT (4096)

        #define D3D11_COMMONSHADER_TEMP_REGISTER_READS_PER_INST (3)

        #define D3D11_COMMONSHADER_TEMP_REGISTER_READ_PORTS (3)

        #define D3D11_COMMONSHADER_TEXCOORD_RANGE_REDUCTION_MAX (10)

        #define D3D11_COMMONSHADER_TEXCOORD_RANGE_REDUCTION_MIN (-10)

        #define D3D11_COMMONSHADER_TEXEL_OFFSET_MAX_NEGATIVE (-8)

        #define D3D11_COMMONSHADER_TEXEL_OFFSET_MAX_POSITIVE (7)

        #define D3D11_CS_4_X_BUCKET00_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (256)

        #define D3D11_CS_4_X_BUCKET00_MAX_NUM_THREADS_PER_GROUP (64)

        #define D3D11_CS_4_X_BUCKET01_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (240)

        #define D3D11_CS_4_X_BUCKET01_MAX_NUM_THREADS_PER_GROUP (68)

        #define D3D11_CS_4_X_BUCKET02_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (224)

        #define D3D11_CS_4_X_BUCKET02_MAX_NUM_THREADS_PER_GROUP (72)

        #define D3D11_CS_4_X_BUCKET03_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (208)

        #define D3D11_CS_4_X_BUCKET03_MAX_NUM_THREADS_PER_GROUP (76)

        #define D3D11_CS_4_X_BUCKET04_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (192)

        #define D3D11_CS_4_X_BUCKET04_MAX_NUM_THREADS_PER_GROUP (84)

        #define D3D11_CS_4_X_BUCKET05_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (176)

        #define D3D11_CS_4_X_BUCKET05_MAX_NUM_THREADS_PER_GROUP (92)

        #define D3D11_CS_4_X_BUCKET06_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (160)

        #define D3D11_CS_4_X_BUCKET06_MAX_NUM_THREADS_PER_GROUP (100)

        #define D3D11_CS_4_X_BUCKET07_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (144)

        #define D3D11_CS_4_X_BUCKET07_MAX_NUM_THREADS_PER_GROUP (112)

        #define D3D11_CS_4_X_BUCKET08_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (128)

        #define D3D11_CS_4_X_BUCKET08_MAX_NUM_THREADS_PER_GROUP (128)

        #define D3D11_CS_4_X_BUCKET09_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (112)

        #define D3D11_CS_4_X_BUCKET09_MAX_NUM_THREADS_PER_GROUP (144)

        #define D3D11_CS_4_X_BUCKET10_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (96)

        #define D3D11_CS_4_X_BUCKET10_MAX_NUM_THREADS_PER_GROUP (168)

        #define D3D11_CS_4_X_BUCKET11_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (80)

        #define D3D11_CS_4_X_BUCKET11_MAX_NUM_THREADS_PER_GROUP (204)

        #define D3D11_CS_4_X_BUCKET12_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (64)

        #define D3D11_CS_4_X_BUCKET12_MAX_NUM_THREADS_PER_GROUP (256)

        #define D3D11_CS_4_X_BUCKET13_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (48)

        #define D3D11_CS_4_X_BUCKET13_MAX_NUM_THREADS_PER_GROUP (340)

        #define D3D11_CS_4_X_BUCKET14_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (32)

        #define D3D11_CS_4_X_BUCKET14_MAX_NUM_THREADS_PER_GROUP (512)

        #define D3D11_CS_4_X_BUCKET15_MAX_BYTES_TGSM_WRITABLE_PER_THREAD (16)

        #define D3D11_CS_4_X_BUCKET15_MAX_NUM_THREADS_PER_GROUP (768)

        #define D3D11_CS_4_X_DISPATCH_MAX_THREAD_GROUPS_IN_Z_DIMENSION (1)

        #define D3D11_CS_4_X_RAW_UAV_BYTE_ALIGNMENT (256)

        #define D3D11_CS_4_X_THREAD_GROUP_MAX_THREADS_PER_GROUP (768)

        #define D3D11_CS_4_X_THREAD_GROUP_MAX_X (768)

        #define D3D11_CS_4_X_THREAD_GROUP_MAX_Y (768)

        #define D3D11_CS_4_X_UAV_REGISTER_COUNT (1)

        #define D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION (65535)

        #define D3D11_CS_TGSM_REGISTER_COUNT (8192)

        #define D3D11_CS_TGSM_REGISTER_READS_PER_INST (1)

        #define D3D11_CS_TGSM_RESOURCE_REGISTER_COMPONENTS (1)

        #define D3D11_CS_TGSM_RESOURCE_REGISTER_READ_PORTS (1)

        #define D3D11_CS_THREADGROUPID_REGISTER_COMPONENTS (3)

        #define D3D11_CS_THREADGROUPID_REGISTER_COUNT (1)

        #define D3D11_CS_THREADIDINGROUPFLATTENED_REGISTER_COMPONENTS (1)

        #define D3D11_CS_THREADIDINGROUPFLATTENED_REGISTER_COUNT (1)

        #define D3D11_CS_THREADIDINGROUP_REGISTER_COMPONENTS (3)

        #define D3D11_CS_THREADIDINGROUP_REGISTER_COUNT (1)

        #define D3D11_CS_THREADID_REGISTER_COMPONENTS (3)

        #define D3D11_CS_THREADID_REGISTER_COUNT (1)

        #define D3D11_CS_THREAD_GROUP_MAX_THREADS_PER_GROUP (1024)

        #define D3D11_CS_THREAD_GROUP_MAX_X (1024)

        #define D3D11_CS_THREAD_GROUP_MAX_Y (1024)

        #define D3D11_CS_THREAD_GROUP_MAX_Z (64)

        #define D3D11_CS_THREAD_GROUP_MIN_X (1)

        #define D3D11_CS_THREAD_GROUP_MIN_Y (1)

        #define D3D11_CS_THREAD_GROUP_MIN_Z (1)

        #define D3D11_CS_THREAD_LOCAL_TEMP_REGISTER_POOL (16384)

        #define D3D11_DEFAULT_BLEND_FACTOR_ALPHA     (1.0f)
        #define D3D11_DEFAULT_BLEND_FACTOR_BLUE      (1.0f)
        #define D3D11_DEFAULT_BLEND_FACTOR_GREEN     (1.0f)
        #define D3D11_DEFAULT_BLEND_FACTOR_RED       (1.0f)
        #define D3D11_DEFAULT_BORDER_COLOR_COMPONENT (0.0f)
        #define D3D11_DEFAULT_DEPTH_BIAS             (0)

        #define D3D11_DEFAULT_DEPTH_BIAS_CLAMP (0.0f)
        #define D3D11_DEFAULT_MAX_ANISOTROPY   (16)

        #define D3D11_DEFAULT_MIP_LOD_BIAS              (0.0f)
        #define D3D11_DEFAULT_RENDER_TARGET_ARRAY_INDEX (0)

        #define D3D11_DEFAULT_SAMPLE_MASK (0xffffffff)

        #define D3D11_DEFAULT_SCISSOR_ENDX (0)

        #define D3D11_DEFAULT_SCISSOR_ENDY (0)

        #define D3D11_DEFAULT_SCISSOR_STARTX (0)

        #define D3D11_DEFAULT_SCISSOR_STARTY (0)

        #define D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS (0.0f)
        #define D3D11_DEFAULT_STENCIL_READ_MASK       (0xff)

        #define D3D11_DEFAULT_STENCIL_REFERENCE (0)

        #define D3D11_DEFAULT_STENCIL_WRITE_MASK (0xff)

        #define D3D11_DEFAULT_VIEWPORT_AND_SCISSORRECT_INDEX (0)

        #define D3D11_DEFAULT_VIEWPORT_HEIGHT (0)

        #define D3D11_DEFAULT_VIEWPORT_MAX_DEPTH (0.0f)
        #define D3D11_DEFAULT_VIEWPORT_MIN_DEPTH (0.0f)
        #define D3D11_DEFAULT_VIEWPORT_TOPLEFTX  (0)

        #define D3D11_DEFAULT_VIEWPORT_TOPLEFTY (0)

        #define D3D11_DEFAULT_VIEWPORT_WIDTH (0)

        #define D3D11_DS_INPUT_CONTROL_POINTS_MAX_TOTAL_SCALARS (3968)

        #define D3D11_DS_INPUT_CONTROL_POINT_REGISTER_COMPONENTS (4)

        #define D3D11_DS_INPUT_CONTROL_POINT_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_DS_INPUT_CONTROL_POINT_REGISTER_COUNT (32)

        #define D3D11_DS_INPUT_CONTROL_POINT_REGISTER_READS_PER_INST (2)

        #define D3D11_DS_INPUT_CONTROL_POINT_REGISTER_READ_PORTS (1)

        #define D3D11_DS_INPUT_DOMAIN_POINT_REGISTER_COMPONENTS (3)

        #define D3D11_DS_INPUT_DOMAIN_POINT_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_DS_INPUT_DOMAIN_POINT_REGISTER_COUNT (1)

        #define D3D11_DS_INPUT_DOMAIN_POINT_REGISTER_READS_PER_INST (2)

        #define D3D11_DS_INPUT_DOMAIN_POINT_REGISTER_READ_PORTS (1)

        #define D3D11_DS_INPUT_PATCH_CONSTANT_REGISTER_COMPONENTS (4)

        #define D3D11_DS_INPUT_PATCH_CONSTANT_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_DS_INPUT_PATCH_CONSTANT_REGISTER_COUNT (32)

        #define D3D11_DS_INPUT_PATCH_CONSTANT_REGISTER_READS_PER_INST (2)

        #define D3D11_DS_INPUT_PATCH_CONSTANT_REGISTER_READ_PORTS (1)

        #define D3D11_DS_INPUT_PRIMITIVE_ID_REGISTER_COMPONENTS (1)

        #define D3D11_DS_INPUT_PRIMITIVE_ID_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_DS_INPUT_PRIMITIVE_ID_REGISTER_COUNT (1)

        #define D3D11_DS_INPUT_PRIMITIVE_ID_REGISTER_READS_PER_INST (2)

        #define D3D11_DS_INPUT_PRIMITIVE_ID_REGISTER_READ_PORTS (1)

        #define D3D11_DS_OUTPUT_REGISTER_COMPONENTS (4)

        #define D3D11_DS_OUTPUT_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_DS_OUTPUT_REGISTER_COUNT (32)

        #define D3D11_FLOAT16_FUSED_TOLERANCE_IN_ULP      (0.6)
        #define D3D11_FLOAT32_MAX                         (3.402823466e+38f)
        #define D3D11_FLOAT32_TO_INTEGER_TOLERANCE_IN_ULP (0.6f)
        #define D3D11_FLOAT_TO_SRGB_EXPONENT_DENOMINATOR  (2.4f)
        #define D3D11_FLOAT_TO_SRGB_EXPONENT_NUMERATOR    (1.0f)
        #define D3D11_FLOAT_TO_SRGB_OFFSET                (0.055f)
        #define D3D11_FLOAT_TO_SRGB_SCALE_1               (12.92f)
        #define D3D11_FLOAT_TO_SRGB_SCALE_2               (1.055f)
        #define D3D11_FLOAT_TO_SRGB_THRESHOLD             (0.0031308f)
        #define D3D11_FTOI_INSTRUCTION_MAX_INPUT          (2147483647.999f)
        #define D3D11_FTOI_INSTRUCTION_MIN_INPUT          (-2147483648.999f)
        #define D3D11_FTOU_INSTRUCTION_MAX_INPUT          (4294967295.999f)
        #define D3D11_FTOU_INSTRUCTION_MIN_INPUT          (0.0f)
        #define D3D11_GS_INPUT_INSTANCE_ID_READS_PER_INST (2)

        #define D3D11_GS_INPUT_INSTANCE_ID_READ_PORTS (1)

        #define D3D11_GS_INPUT_INSTANCE_ID_REGISTER_COMPONENTS (1)

        #define D3D11_GS_INPUT_INSTANCE_ID_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_GS_INPUT_INSTANCE_ID_REGISTER_COUNT (1)

        #define D3D11_GS_INPUT_PRIM_CONST_REGISTER_COMPONENTS (1)

        #define D3D11_GS_INPUT_PRIM_CONST_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_GS_INPUT_PRIM_CONST_REGISTER_COUNT (1)

        #define D3D11_GS_INPUT_PRIM_CONST_REGISTER_READS_PER_INST (2)

        #define D3D11_GS_INPUT_PRIM_CONST_REGISTER_READ_PORTS (1)

        #define D3D11_GS_INPUT_REGISTER_COMPONENTS (4)

        #define D3D11_GS_INPUT_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_GS_INPUT_REGISTER_COUNT (32)

        #define D3D11_GS_INPUT_REGISTER_READS_PER_INST (2)

        #define D3D11_GS_INPUT_REGISTER_READ_PORTS (1)

        #define D3D11_GS_INPUT_REGISTER_VERTICES (32)

        #define D3D11_GS_MAX_INSTANCE_COUNT (32)

        #define D3D11_GS_MAX_OUTPUT_VERTEX_COUNT_ACROSS_INSTANCES (1024)

        #define D3D11_GS_OUTPUT_ELEMENTS (32)

        #define D3D11_GS_OUTPUT_REGISTER_COMPONENTS (4)

        #define D3D11_GS_OUTPUT_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_GS_OUTPUT_REGISTER_COUNT (32)

        #define D3D11_HS_CONTROL_POINT_PHASE_INPUT_REGISTER_COUNT (32)

        #define D3D11_HS_CONTROL_POINT_PHASE_OUTPUT_REGISTER_COUNT (32)

        #define D3D11_HS_CONTROL_POINT_REGISTER_COMPONENTS (4)

        #define D3D11_HS_CONTROL_POINT_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_HS_CONTROL_POINT_REGISTER_READS_PER_INST (2)

        #define D3D11_HS_CONTROL_POINT_REGISTER_READ_PORTS (1)

        #define D3D11_HS_FORK_PHASE_INSTANCE_COUNT_UPPER_BOUND (0xffffffff)

        #define D3D11_HS_INPUT_FORK_INSTANCE_ID_REGISTER_COMPONENTS (1)

        #define D3D11_HS_INPUT_FORK_INSTANCE_ID_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_HS_INPUT_FORK_INSTANCE_ID_REGISTER_COUNT (1)

        #define D3D11_HS_INPUT_FORK_INSTANCE_ID_REGISTER_READS_PER_INST (2)

        #define D3D11_HS_INPUT_FORK_INSTANCE_ID_REGISTER_READ_PORTS (1)

        #define D3D11_HS_INPUT_JOIN_INSTANCE_ID_REGISTER_COMPONENTS (1)

        #define D3D11_HS_INPUT_JOIN_INSTANCE_ID_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_HS_INPUT_JOIN_INSTANCE_ID_REGISTER_COUNT (1)

        #define D3D11_HS_INPUT_JOIN_INSTANCE_ID_REGISTER_READS_PER_INST (2)

        #define D3D11_HS_INPUT_JOIN_INSTANCE_ID_REGISTER_READ_PORTS (1)

        #define D3D11_HS_INPUT_PRIMITIVE_ID_REGISTER_COMPONENTS (1)

        #define D3D11_HS_INPUT_PRIMITIVE_ID_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_HS_INPUT_PRIMITIVE_ID_REGISTER_COUNT (1)

        #define D3D11_HS_INPUT_PRIMITIVE_ID_REGISTER_READS_PER_INST (2)

        #define D3D11_HS_INPUT_PRIMITIVE_ID_REGISTER_READ_PORTS (1)

        #define D3D11_HS_JOIN_PHASE_INSTANCE_COUNT_UPPER_BOUND (0xffffffff)

        #define D3D11_HS_MAXTESSFACTOR_LOWER_BOUND               (1.0f)
        #define D3D11_HS_MAXTESSFACTOR_UPPER_BOUND               (64.0f)
        #define D3D11_HS_OUTPUT_CONTROL_POINTS_MAX_TOTAL_SCALARS (3968)

        #define D3D11_HS_OUTPUT_CONTROL_POINT_ID_REGISTER_COMPONENTS (1)

        #define D3D11_HS_OUTPUT_CONTROL_POINT_ID_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_HS_OUTPUT_CONTROL_POINT_ID_REGISTER_COUNT (1)

        #define D3D11_HS_OUTPUT_CONTROL_POINT_ID_REGISTER_READS_PER_INST (2)

        #define D3D11_HS_OUTPUT_CONTROL_POINT_ID_REGISTER_READ_PORTS (1)

        #define D3D11_HS_OUTPUT_PATCH_CONSTANT_REGISTER_COMPONENTS (4)

        #define D3D11_HS_OUTPUT_PATCH_CONSTANT_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_HS_OUTPUT_PATCH_CONSTANT_REGISTER_COUNT (32)

        #define D3D11_HS_OUTPUT_PATCH_CONSTANT_REGISTER_READS_PER_INST (2)

        #define D3D11_HS_OUTPUT_PATCH_CONSTANT_REGISTER_READ_PORTS (1)

        #define D3D11_HS_OUTPUT_PATCH_CONSTANT_REGISTER_SCALAR_COMPONENTS (128)

        #define D3D11_IA_DEFAULT_INDEX_BUFFER_OFFSET_IN_BYTES (0)

        #define D3D11_IA_DEFAULT_PRIMITIVE_TOPOLOGY (0)

        #define D3D11_IA_DEFAULT_VERTEX_BUFFER_OFFSET_IN_BYTES (0)

        #define D3D11_IA_INDEX_INPUT_RESOURCE_SLOT_COUNT (1)

        #define D3D11_IA_INSTANCE_ID_BIT_COUNT (32)

        #define D3D11_IA_INTEGER_ARITHMETIC_BIT_COUNT (32)

        #define D3D11_IA_PATCH_MAX_CONTROL_POINT_COUNT (32)

        #define D3D11_IA_PRIMITIVE_ID_BIT_COUNT (32)

        #define D3D11_IA_VERTEX_ID_BIT_COUNT (32)

        #define D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT (32)

        #define D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENTS_COMPONENTS (128)

        #define D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT (32)

        #define D3D11_INTEGER_DIVIDE_BY_ZERO_QUOTIENT (0xffffffff)

        #define D3D11_INTEGER_DIVIDE_BY_ZERO_REMAINDER (0xffffffff)

        #define D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL (0xffffffff)

        #define D3D11_KEEP_UNORDERED_ACCESS_VIEWS (0xffffffff)

        #define D3D11_LINEAR_GAMMA  (1.0f)
        #define D3D11_MAJOR_VERSION (11)

        #define D3D11_MAX_BORDER_COLOR_COMPONENT (1.0f)
        #define D3D11_MAX_DEPTH                  (1.0f)
        #define D3D11_MAX_MAXANISOTROPY          (16)

        #define D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT (32)

        #define D3D11_MAX_POSITION_VALUE             (3.402823466e+34f)
        #define D3D11_MAX_TEXTURE_DIMENSION_2_TO_EXP (17)

        #define D3D11_MINOR_VERSION (0)

        #define D3D11_MIN_BORDER_COLOR_COMPONENT (0.0f)
        #define D3D11_MIN_DEPTH                  (0.0f)
        #define D3D11_MIN_MAXANISOTROPY          (0)

        #define D3D11_MIP_LOD_BIAS_MAX             (15.99f)
        #define D3D11_MIP_LOD_BIAS_MIN             (-16.0f)
        #define D3D11_MIP_LOD_FRACTIONAL_BIT_COUNT (8)

        #define D3D11_MIP_LOD_RANGE_BIT_COUNT (8)

        #define D3D11_MULTISAMPLE_ANTIALIAS_LINE_WIDTH           (1.4f)
        #define D3D11_NONSAMPLE_FETCH_OUT_OF_RANGE_ACCESS_RESULT (0)

        #define D3D11_PIXEL_ADDRESS_RANGE_BIT_COUNT (15)

        #define D3D11_PRE_SCISSOR_PIXEL_ADDRESS_RANGE_BIT_COUNT (16)

        #define D3D11_PS_CS_UAV_REGISTER_COMPONENTS (1)

        #define D3D11_PS_CS_UAV_REGISTER_COUNT (8)

        #define D3D11_PS_CS_UAV_REGISTER_READS_PER_INST (1)

        #define D3D11_PS_CS_UAV_REGISTER_READ_PORTS (1)

        #define D3D11_PS_FRONTFACING_DEFAULT_VALUE (0xffffffff)

        #define D3D11_PS_FRONTFACING_FALSE_VALUE (0)

        #define D3D11_PS_FRONTFACING_TRUE_VALUE (0xffffffff)

        #define D3D11_PS_INPUT_REGISTER_COMPONENTS (4)

        #define D3D11_PS_INPUT_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_PS_INPUT_REGISTER_COUNT (32)

        #define D3D11_PS_INPUT_REGISTER_READS_PER_INST (2)

        #define D3D11_PS_INPUT_REGISTER_READ_PORTS (1)

        #define D3D11_PS_LEGACY_PIXEL_CENTER_FRACTIONAL_COMPONENT (0.0f)
        #define D3D11_PS_OUTPUT_DEPTH_REGISTER_COMPONENTS         (1)

        #define D3D11_PS_OUTPUT_DEPTH_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_PS_OUTPUT_DEPTH_REGISTER_COUNT (1)

        #define D3D11_PS_OUTPUT_MASK_REGISTER_COMPONENTS (1)

        #define D3D11_PS_OUTPUT_MASK_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_PS_OUTPUT_MASK_REGISTER_COUNT (1)

        #define D3D11_PS_OUTPUT_REGISTER_COMPONENTS (4)

        #define D3D11_PS_OUTPUT_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_PS_OUTPUT_REGISTER_COUNT (8)

        #define D3D11_PS_PIXEL_CENTER_FRACTIONAL_COMPONENT (0.5f)
        #define D3D11_RAW_UAV_SRV_BYTE_ALIGNMENT           (16)

        #define D3D11_REQ_BLEND_OBJECT_COUNT_PER_DEVICE (4096)

        #define D3D11_REQ_BUFFER_RESOURCE_TEXEL_COUNT_2_TO_EXP (27)

        #define D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT (4096)

        #define D3D11_REQ_DEPTH_STENCIL_OBJECT_COUNT_PER_DEVICE (4096)

        #define D3D11_REQ_DRAWINDEXED_INDEX_COUNT_2_TO_EXP (32)

        #define D3D11_REQ_DRAW_VERTEX_COUNT_2_TO_EXP (32)

        #define D3D11_REQ_FILTERING_HW_ADDRESSABLE_RESOURCE_DIMENSION (16384)

        #define D3D11_REQ_GS_INVOCATION_32BIT_OUTPUT_COMPONENT_LIMIT (1024)

        #define D3D11_REQ_IMMEDIATE_CONSTANT_BUFFER_ELEMENT_COUNT (4096)

        #define D3D11_REQ_MAXANISOTROPY (16)

        #define D3D11_REQ_MIP_LEVELS (15)

        #define D3D11_REQ_MULTI_ELEMENT_STRUCTURE_SIZE_IN_BYTES (2048)

        #define D3D11_REQ_RASTERIZER_OBJECT_COUNT_PER_DEVICE (4096)

        #define D3D11_REQ_RENDER_TO_BUFFER_WINDOW_WIDTH (16384)

        #define D3D11_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_A_TERM (128)

        #define D3D11_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_B_TERM (0.25f)
        #define D3D11_REQ_RESOURCE_SIZE_IN_MEGABYTES_EXPRESSION_C_TERM (2048)

        #define D3D11_REQ_RESOURCE_VIEW_COUNT_PER_DEVICE_2_TO_EXP (20)

        #define D3D11_REQ_SAMPLER_OBJECT_COUNT_PER_DEVICE (4096)

        #define D3D11_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION (2048)

        #define D3D11_REQ_TEXTURE1D_U_DIMENSION (16384)

        #define D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION (2048)

        #define D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION (16384)

        #define D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION (2048)

        #define D3D11_REQ_TEXTURECUBE_DIMENSION (16384)

        #define D3D11_RESINFO_INSTRUCTION_MISSING_COMPONENT_RETVAL (0)

        #define D3D11_SHADER_MAJOR_VERSION (5)

        #define D3D11_SHADER_MAX_INSTANCES (65535)

        #define D3D11_SHADER_MAX_INTERFACES (253)

        #define D3D11_SHADER_MAX_INTERFACE_CALL_SITES (4096)

        #define D3D11_SHADER_MAX_TYPES (65535)

        #define D3D11_SHADER_MINOR_VERSION (0)

        #define D3D11_SHIFT_INSTRUCTION_PAD_VALUE (0)

        #define D3D11_SHIFT_INSTRUCTION_SHIFT_VALUE_BIT_COUNT (5)

        #define D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT (8)

        #define D3D11_SO_BUFFER_MAX_STRIDE_IN_BYTES (2048)

        #define D3D11_SO_BUFFER_MAX_WRITE_WINDOW_IN_BYTES (512)

        #define D3D11_SO_BUFFER_SLOT_COUNT (4)

        #define D3D11_SO_DDI_REGISTER_INDEX_DENOTING_GAP (0xffffffff)

        #define D3D11_SO_NO_RASTERIZED_STREAM (0xffffffff)

        #define D3D11_SO_OUTPUT_COMPONENT_COUNT (128)

        #define D3D11_SO_STREAM_COUNT (4)

        #define D3D11_SPEC_DATE_DAY (16)

        #define D3D11_SPEC_DATE_MONTH (05)

        #define D3D11_SPEC_DATE_YEAR (2011)

        #define D3D11_SPEC_VERSION                   (1.07)
        #define D3D11_SRGB_GAMMA                     (2.2f)
        #define D3D11_SRGB_TO_FLOAT_DENOMINATOR_1    (12.92f)
        #define D3D11_SRGB_TO_FLOAT_DENOMINATOR_2    (1.055f)
        #define D3D11_SRGB_TO_FLOAT_EXPONENT         (2.4f)
        #define D3D11_SRGB_TO_FLOAT_OFFSET           (0.055f)
        #define D3D11_SRGB_TO_FLOAT_THRESHOLD        (0.04045f)
        #define D3D11_SRGB_TO_FLOAT_TOLERANCE_IN_ULP (0.5f)
        #define D3D11_STANDARD_COMPONENT_BIT_COUNT   (32)

        #define D3D11_STANDARD_COMPONENT_BIT_COUNT_DOUBLED (64)

        #define D3D11_STANDARD_MAXIMUM_ELEMENT_ALIGNMENT_BYTE_MULTIPLE (4)

        #define D3D11_STANDARD_PIXEL_COMPONENT_COUNT (128)

        #define D3D11_STANDARD_PIXEL_ELEMENT_COUNT (32)

        #define D3D11_STANDARD_VECTOR_SIZE (4)

        #define D3D11_STANDARD_VERTEX_ELEMENT_COUNT (32)

        #define D3D11_STANDARD_VERTEX_TOTAL_COMPONENT_COUNT (64)

        #define D3D11_SUBPIXEL_FRACTIONAL_BIT_COUNT (8)

        #define D3D11_SUBTEXEL_FRACTIONAL_BIT_COUNT (8)

        #define D3D11_TESSELLATOR_MAX_EVEN_TESSELLATION_FACTOR (64)

        #define D3D11_TESSELLATOR_MAX_ISOLINE_DENSITY_TESSELLATION_FACTOR (64)

        #define D3D11_TESSELLATOR_MAX_ODD_TESSELLATION_FACTOR (63)

        #define D3D11_TESSELLATOR_MAX_TESSELLATION_FACTOR (64)

        #define D3D11_TESSELLATOR_MIN_EVEN_TESSELLATION_FACTOR (2)

        #define D3D11_TESSELLATOR_MIN_ISOLINE_DENSITY_TESSELLATION_FACTOR (1)

        #define D3D11_TESSELLATOR_MIN_ODD_TESSELLATION_FACTOR (1)

        #define D3D11_TEXEL_ADDRESS_RANGE_BIT_COUNT (16)

        #define D3D11_UNBOUND_MEMORY_ACCESS_RESULT (0)

        #define D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX (15)

        #define D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE (16)

        #define D3D11_VIEWPORT_BOUNDS_MAX (32767)

        #define D3D11_VIEWPORT_BOUNDS_MIN (-32768)

        #define D3D11_VS_INPUT_REGISTER_COMPONENTS (4)

        #define D3D11_VS_INPUT_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_VS_INPUT_REGISTER_COUNT (32)

        #define D3D11_VS_INPUT_REGISTER_READS_PER_INST (2)

        #define D3D11_VS_INPUT_REGISTER_READ_PORTS (1)

        #define D3D11_VS_OUTPUT_REGISTER_COMPONENTS (4)

        #define D3D11_VS_OUTPUT_REGISTER_COMPONENT_BIT_COUNT (32)

        #define D3D11_VS_OUTPUT_REGISTER_COUNT (32)

        #define D3D11_WHQL_CONTEXT_COUNT_FOR_RESOURCE_LIMIT (10)

        #define D3D11_WHQL_DRAWINDEXED_INDEX_COUNT_2_TO_EXP (25)

        #define D3D11_WHQL_DRAW_VERTEX_COUNT_2_TO_EXP (25)

    #endif
    #ifndef _D3D11_1_CONSTANTS
        #define _D3D11_1_CONSTANTS
        #define D3D11_1_UAV_SLOT_COUNT (64)

    #endif
    #define _FACD3D11 (0x87c)

    #define _FACD3D11DEBUG ((_FACD3D11 + 1))

    /* Direct3D errors are now found in winerror.h */
    #define MAKE_D3D11_HRESULT(code) MAKE_HRESULT(1, _FACD3D11, code)
    #define MAKE_D3D11_STATUS(code)  MAKE_HRESULT(0, _FACD3D11, code)
    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_DEFAULT {};
extern const DECLSPEC_SELECTANY CD3D11_DEFAULT D3D11_DEFAULT;
extern "C" {
    #endif
typedef enum D3D11_INPUT_CLASSIFICATION { D3D11_INPUT_PER_VERTEX_DATA = 0, D3D11_INPUT_PER_INSTANCE_DATA = 1 } D3D11_INPUT_CLASSIFICATION;

    #define D3D11_APPEND_ALIGNED_ELEMENT (0xffffffff)

typedef struct D3D11_INPUT_ELEMENT_DESC {
    LPCSTR                     SemanticName;
    UINT                       SemanticIndex;
    DXGI_FORMAT                Format;
    UINT                       InputSlot;
    UINT                       AlignedByteOffset;
    D3D11_INPUT_CLASSIFICATION InputSlotClass;
    UINT                       InstanceDataStepRate;
} D3D11_INPUT_ELEMENT_DESC;

typedef enum D3D11_FILL_MODE { D3D11_FILL_WIREFRAME = 2, D3D11_FILL_SOLID = 3 } D3D11_FILL_MODE;

typedef D3D_PRIMITIVE_TOPOLOGY D3D11_PRIMITIVE_TOPOLOGY;

typedef D3D_PRIMITIVE D3D11_PRIMITIVE;

typedef enum D3D11_CULL_MODE { D3D11_CULL_NONE = 1, D3D11_CULL_FRONT = 2, D3D11_CULL_BACK = 3 } D3D11_CULL_MODE;

typedef struct D3D11_SO_DECLARATION_ENTRY {
    UINT   Stream;
    LPCSTR SemanticName;
    UINT   SemanticIndex;
    BYTE   StartComponent;
    BYTE   ComponentCount;
    BYTE   OutputSlot;
} D3D11_SO_DECLARATION_ENTRY;

typedef struct D3D11_VIEWPORT {
    FLOAT TopLeftX;
    FLOAT TopLeftY;
    FLOAT Width;
    FLOAT Height;
    FLOAT MinDepth;
    FLOAT MaxDepth;
} D3D11_VIEWPORT;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
inline bool operator==(const D3D11_VIEWPORT & l, const D3D11_VIEWPORT & r) {
    return l.TopLeftX == r.TopLeftX && l.TopLeftY == r.TopLeftY && l.Width == r.Width && l.Height == r.Height && l.MinDepth == r.MinDepth &&
           l.MaxDepth == r.MaxDepth;
}
inline bool operator!=(const D3D11_VIEWPORT & l, const D3D11_VIEWPORT & r) { return !(l == r); }
extern "C" {
    #endif
typedef enum D3D11_RESOURCE_DIMENSION {
    D3D11_RESOURCE_DIMENSION_UNKNOWN   = 0,
    D3D11_RESOURCE_DIMENSION_BUFFER    = 1,
    D3D11_RESOURCE_DIMENSION_TEXTURE1D = 2,
    D3D11_RESOURCE_DIMENSION_TEXTURE2D = 3,
    D3D11_RESOURCE_DIMENSION_TEXTURE3D = 4
} D3D11_RESOURCE_DIMENSION;

typedef D3D_SRV_DIMENSION D3D11_SRV_DIMENSION;

typedef enum D3D11_DSV_DIMENSION {
    D3D11_DSV_DIMENSION_UNKNOWN          = 0,
    D3D11_DSV_DIMENSION_TEXTURE1D        = 1,
    D3D11_DSV_DIMENSION_TEXTURE1DARRAY   = 2,
    D3D11_DSV_DIMENSION_TEXTURE2D        = 3,
    D3D11_DSV_DIMENSION_TEXTURE2DARRAY   = 4,
    D3D11_DSV_DIMENSION_TEXTURE2DMS      = 5,
    D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY = 6
} D3D11_DSV_DIMENSION;

typedef enum D3D11_RTV_DIMENSION {
    D3D11_RTV_DIMENSION_UNKNOWN          = 0,
    D3D11_RTV_DIMENSION_BUFFER           = 1,
    D3D11_RTV_DIMENSION_TEXTURE1D        = 2,
    D3D11_RTV_DIMENSION_TEXTURE1DARRAY   = 3,
    D3D11_RTV_DIMENSION_TEXTURE2D        = 4,
    D3D11_RTV_DIMENSION_TEXTURE2DARRAY   = 5,
    D3D11_RTV_DIMENSION_TEXTURE2DMS      = 6,
    D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY = 7,
    D3D11_RTV_DIMENSION_TEXTURE3D        = 8
} D3D11_RTV_DIMENSION;

typedef enum D3D11_UAV_DIMENSION {
    D3D11_UAV_DIMENSION_UNKNOWN        = 0,
    D3D11_UAV_DIMENSION_BUFFER         = 1,
    D3D11_UAV_DIMENSION_TEXTURE1D      = 2,
    D3D11_UAV_DIMENSION_TEXTURE1DARRAY = 3,
    D3D11_UAV_DIMENSION_TEXTURE2D      = 4,
    D3D11_UAV_DIMENSION_TEXTURE2DARRAY = 5,
    D3D11_UAV_DIMENSION_TEXTURE3D      = 8
} D3D11_UAV_DIMENSION;

typedef enum D3D11_USAGE { D3D11_USAGE_DEFAULT = 0, D3D11_USAGE_IMMUTABLE = 1, D3D11_USAGE_DYNAMIC = 2, D3D11_USAGE_STAGING = 3 } D3D11_USAGE;

typedef enum D3D11_BIND_FLAG {
    D3D11_BIND_VERTEX_BUFFER    = 0x1L,
    D3D11_BIND_INDEX_BUFFER     = 0x2L,
    D3D11_BIND_CONSTANT_BUFFER  = 0x4L,
    D3D11_BIND_SHADER_RESOURCE  = 0x8L,
    D3D11_BIND_STREAM_OUTPUT    = 0x10L,
    D3D11_BIND_RENDER_TARGET    = 0x20L,
    D3D11_BIND_DEPTH_STENCIL    = 0x40L,
    D3D11_BIND_UNORDERED_ACCESS = 0x80L,
    D3D11_BIND_DECODER          = 0x200L,
    D3D11_BIND_VIDEO_ENCODER    = 0x400L
} D3D11_BIND_FLAG;

typedef enum D3D11_CPU_ACCESS_FLAG { D3D11_CPU_ACCESS_WRITE = 0x10000L, D3D11_CPU_ACCESS_READ = 0x20000L } D3D11_CPU_ACCESS_FLAG;

typedef enum D3D11_RESOURCE_MISC_FLAG {
    D3D11_RESOURCE_MISC_GENERATE_MIPS                   = 0x1L,
    D3D11_RESOURCE_MISC_SHARED                          = 0x2L,
    D3D11_RESOURCE_MISC_TEXTURECUBE                     = 0x4L,
    D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS               = 0x10L,
    D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS          = 0x20L,
    D3D11_RESOURCE_MISC_BUFFER_STRUCTURED               = 0x40L,
    D3D11_RESOURCE_MISC_RESOURCE_CLAMP                  = 0x80L,
    D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX               = 0x100L,
    D3D11_RESOURCE_MISC_GDI_COMPATIBLE                  = 0x200L,
    D3D11_RESOURCE_MISC_SHARED_NTHANDLE                 = 0x800L,
    D3D11_RESOURCE_MISC_RESTRICTED_CONTENT              = 0x1000L,
    D3D11_RESOURCE_MISC_RESTRICT_SHARED_RESOURCE        = 0x2000L,
    D3D11_RESOURCE_MISC_RESTRICT_SHARED_RESOURCE_DRIVER = 0x4000L,
    D3D11_RESOURCE_MISC_GUARDED                         = 0x8000L
} D3D11_RESOURCE_MISC_FLAG;

typedef enum D3D11_MAP {
    D3D11_MAP_READ               = 1,
    D3D11_MAP_WRITE              = 2,
    D3D11_MAP_READ_WRITE         = 3,
    D3D11_MAP_WRITE_DISCARD      = 4,
    D3D11_MAP_WRITE_NO_OVERWRITE = 5
} D3D11_MAP;

typedef enum D3D11_MAP_FLAG { D3D11_MAP_FLAG_DO_NOT_WAIT = 0x100000L } D3D11_MAP_FLAG;

typedef enum D3D11_RAISE_FLAG { D3D11_RAISE_FLAG_DRIVER_INTERNAL_ERROR = 0x1L } D3D11_RAISE_FLAG;

typedef enum D3D11_CLEAR_FLAG { D3D11_CLEAR_DEPTH = 0x1L, D3D11_CLEAR_STENCIL = 0x2L } D3D11_CLEAR_FLAG;

typedef RECT D3D11_RECT;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_RECT : public D3D11_RECT {
    CD3D11_RECT() {}
    explicit CD3D11_RECT(const D3D11_RECT & o): D3D11_RECT(o) {}
    explicit CD3D11_RECT(LONG Left, LONG Top, LONG Right, LONG Bottom) {
        left   = Left;
        top    = Top;
        right  = Right;
        bottom = Bottom;
    }
    ~CD3D11_RECT() {}
    operator const D3D11_RECT &() const { return *this; }
};
inline bool operator==(const D3D11_RECT & l, const D3D11_RECT & r) { return l.left == r.left && l.top == r.top && l.right == r.right && l.bottom == r.bottom; }
inline bool operator!=(const D3D11_RECT & l, const D3D11_RECT & r) { return !(l == r); }
extern "C" {
    #endif
typedef struct D3D11_BOX {
    UINT left;
    UINT top;
    UINT front;
    UINT right;
    UINT bottom;
    UINT back;
} D3D11_BOX;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_BOX : public D3D11_BOX {
    CD3D11_BOX() {}
    explicit CD3D11_BOX(const D3D11_BOX & o): D3D11_BOX(o) {}
    explicit CD3D11_BOX(LONG Left, LONG Top, LONG Front, LONG Right, LONG Bottom, LONG Back) {
        left   = Left;
        top    = Top;
        front  = Front;
        right  = Right;
        bottom = Bottom;
        back   = Back;
    }
    ~CD3D11_BOX() {}
    operator const D3D11_BOX &() const { return *this; }
};
inline bool operator==(const D3D11_BOX & l, const D3D11_BOX & r) {
    return l.left == r.left && l.top == r.top && l.front == r.front && l.right == r.right && l.bottom == r.bottom && l.back == r.back;
}
inline bool operator!=(const D3D11_BOX & l, const D3D11_BOX & r) { return !(l == r); }
extern "C" {
    #endif

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0000_v0_0_s_ifspec;

    #ifndef __ID3D11DeviceChild_INTERFACE_DEFINED__
        #define __ID3D11DeviceChild_INTERFACE_DEFINED__

/* interface ID3D11DeviceChild */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11DeviceChild;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("1841e5c8-16b0-489b-bcc8-44cfb0d5deae")
ID3D11DeviceChild : public IUnknown {
public:
    virtual void STDMETHODCALLTYPE GetDevice(
        /* [annotation] */
        _Out_ ID3D11Device * *ppDevice) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
        /* [annotation] */
        _In_ REFGUID guid,
        /* [annotation] */
        _Inout_ UINT * pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_(*pDataSize) void * pData) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
        /* [annotation] */
        _In_ REFGUID guid,
        /* [annotation] */
        _In_ UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_(DataSize) const void * pData) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
        /* [annotation] */
        _In_ REFGUID guid,
        /* [annotation] */
        _In_opt_ const IUnknown * pData) = 0;
};

        #else /* C style interface */

typedef struct ID3D11DeviceChildVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11DeviceChild * This,
     /* [in] */ REFIID   riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11DeviceChild * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11DeviceChild * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11DeviceChild * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11DeviceChild * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11DeviceChild * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11DeviceChild * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    END_INTERFACE
} ID3D11DeviceChildVtbl;

interface ID3D11DeviceChild {
    CONST_VTBL struct ID3D11DeviceChildVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11DeviceChild_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11DeviceChild_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11DeviceChild_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11DeviceChild_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11DeviceChild_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11DeviceChild_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11DeviceChild_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11DeviceChild_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0001 */
/* [local] */

typedef enum D3D11_COMPARISON_FUNC {
    D3D11_COMPARISON_NEVER         = 1,
    D3D11_COMPARISON_LESS          = 2,
    D3D11_COMPARISON_EQUAL         = 3,
    D3D11_COMPARISON_LESS_EQUAL    = 4,
    D3D11_COMPARISON_GREATER       = 5,
    D3D11_COMPARISON_NOT_EQUAL     = 6,
    D3D11_COMPARISON_GREATER_EQUAL = 7,
    D3D11_COMPARISON_ALWAYS        = 8
} D3D11_COMPARISON_FUNC;

typedef enum D3D11_DEPTH_WRITE_MASK { D3D11_DEPTH_WRITE_MASK_ZERO = 0, D3D11_DEPTH_WRITE_MASK_ALL = 1 } D3D11_DEPTH_WRITE_MASK;

typedef enum D3D11_STENCIL_OP {
    D3D11_STENCIL_OP_KEEP     = 1,
    D3D11_STENCIL_OP_ZERO     = 2,
    D3D11_STENCIL_OP_REPLACE  = 3,
    D3D11_STENCIL_OP_INCR_SAT = 4,
    D3D11_STENCIL_OP_DECR_SAT = 5,
    D3D11_STENCIL_OP_INVERT   = 6,
    D3D11_STENCIL_OP_INCR     = 7,
    D3D11_STENCIL_OP_DECR     = 8
} D3D11_STENCIL_OP;

typedef struct D3D11_DEPTH_STENCILOP_DESC {
    D3D11_STENCIL_OP      StencilFailOp;
    D3D11_STENCIL_OP      StencilDepthFailOp;
    D3D11_STENCIL_OP      StencilPassOp;
    D3D11_COMPARISON_FUNC StencilFunc;
} D3D11_DEPTH_STENCILOP_DESC;

typedef struct D3D11_DEPTH_STENCIL_DESC {
    BOOL                       DepthEnable;
    D3D11_DEPTH_WRITE_MASK     DepthWriteMask;
    D3D11_COMPARISON_FUNC      DepthFunc;
    BOOL                       StencilEnable;
    UINT8                      StencilReadMask;
    UINT8                      StencilWriteMask;
    D3D11_DEPTH_STENCILOP_DESC FrontFace;
    D3D11_DEPTH_STENCILOP_DESC BackFace;
} D3D11_DEPTH_STENCIL_DESC;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_DEPTH_STENCIL_DESC : public D3D11_DEPTH_STENCIL_DESC {
    CD3D11_DEPTH_STENCIL_DESC() {}
    explicit CD3D11_DEPTH_STENCIL_DESC(const D3D11_DEPTH_STENCIL_DESC & o): D3D11_DEPTH_STENCIL_DESC(o) {}
    explicit CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT) {
        DepthEnable                                       = TRUE;
        DepthWriteMask                                    = D3D11_DEPTH_WRITE_MASK_ALL;
        DepthFunc                                         = D3D11_COMPARISON_LESS;
        StencilEnable                                     = FALSE;
        StencilReadMask                                   = D3D11_DEFAULT_STENCIL_READ_MASK;
        StencilWriteMask                                  = D3D11_DEFAULT_STENCIL_WRITE_MASK;
        const D3D11_DEPTH_STENCILOP_DESC defaultStencilOp = {D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS};
        FrontFace                                         = defaultStencilOp;
        BackFace                                          = defaultStencilOp;
    }
    explicit CD3D11_DEPTH_STENCIL_DESC(BOOL depthEnable, D3D11_DEPTH_WRITE_MASK depthWriteMask, D3D11_COMPARISON_FUNC depthFunc, BOOL stencilEnable,
                                       UINT8 stencilReadMask, UINT8 stencilWriteMask, D3D11_STENCIL_OP frontStencilFailOp,
                                       D3D11_STENCIL_OP frontStencilDepthFailOp, D3D11_STENCIL_OP frontStencilPassOp, D3D11_COMPARISON_FUNC frontStencilFunc,
                                       D3D11_STENCIL_OP backStencilFailOp, D3D11_STENCIL_OP backStencilDepthFailOp, D3D11_STENCIL_OP backStencilPassOp,
                                       D3D11_COMPARISON_FUNC backStencilFunc) {
        DepthEnable                  = depthEnable;
        DepthWriteMask               = depthWriteMask;
        DepthFunc                    = depthFunc;
        StencilEnable                = stencilEnable;
        StencilReadMask              = stencilReadMask;
        StencilWriteMask             = stencilWriteMask;
        FrontFace.StencilFailOp      = frontStencilFailOp;
        FrontFace.StencilDepthFailOp = frontStencilDepthFailOp;
        FrontFace.StencilPassOp      = frontStencilPassOp;
        FrontFace.StencilFunc        = frontStencilFunc;
        BackFace.StencilFailOp       = backStencilFailOp;
        BackFace.StencilDepthFailOp  = backStencilDepthFailOp;
        BackFace.StencilPassOp       = backStencilPassOp;
        BackFace.StencilFunc         = backStencilFunc;
    }
    ~CD3D11_DEPTH_STENCIL_DESC() {}
    operator const D3D11_DEPTH_STENCIL_DESC &() const { return *this; }
};
extern "C" {
    #endif

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0001_v0_0_s_ifspec;

    #ifndef __ID3D11DepthStencilState_INTERFACE_DEFINED__
        #define __ID3D11DepthStencilState_INTERFACE_DEFINED__

/* interface ID3D11DepthStencilState */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11DepthStencilState;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("03823efb-8d8f-4e1c-9aa2-f64bb2cbfdf1")
ID3D11DepthStencilState : public ID3D11DeviceChild {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_DEPTH_STENCIL_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11DepthStencilStateVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11DepthStencilState * This,
     /* [in] */ REFIID         riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11DepthStencilState * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11DepthStencilState * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11DepthStencilState * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11DepthStencilState * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11DepthStencilState * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11DepthStencilState * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11DepthStencilState * This,
                                      /* [annotation] */
                                      _Out_ D3D11_DEPTH_STENCIL_DESC * pDesc);

    END_INTERFACE
} ID3D11DepthStencilStateVtbl;

interface ID3D11DepthStencilState {
    CONST_VTBL struct ID3D11DepthStencilStateVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11DepthStencilState_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11DepthStencilState_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11DepthStencilState_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11DepthStencilState_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11DepthStencilState_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11DepthStencilState_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11DepthStencilState_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11DepthStencilState_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11DepthStencilState_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0002 */
/* [local] */

typedef enum D3D11_BLEND {
    D3D11_BLEND_ZERO             = 1,
    D3D11_BLEND_ONE              = 2,
    D3D11_BLEND_SRC_COLOR        = 3,
    D3D11_BLEND_INV_SRC_COLOR    = 4,
    D3D11_BLEND_SRC_ALPHA        = 5,
    D3D11_BLEND_INV_SRC_ALPHA    = 6,
    D3D11_BLEND_DEST_ALPHA       = 7,
    D3D11_BLEND_INV_DEST_ALPHA   = 8,
    D3D11_BLEND_DEST_COLOR       = 9,
    D3D11_BLEND_INV_DEST_COLOR   = 10,
    D3D11_BLEND_SRC_ALPHA_SAT    = 11,
    D3D11_BLEND_BLEND_FACTOR     = 14,
    D3D11_BLEND_INV_BLEND_FACTOR = 15,
    D3D11_BLEND_SRC1_COLOR       = 16,
    D3D11_BLEND_INV_SRC1_COLOR   = 17,
    D3D11_BLEND_SRC1_ALPHA       = 18,
    D3D11_BLEND_INV_SRC1_ALPHA   = 19
} D3D11_BLEND;

typedef enum D3D11_BLEND_OP {
    D3D11_BLEND_OP_ADD          = 1,
    D3D11_BLEND_OP_SUBTRACT     = 2,
    D3D11_BLEND_OP_REV_SUBTRACT = 3,
    D3D11_BLEND_OP_MIN          = 4,
    D3D11_BLEND_OP_MAX          = 5
} D3D11_BLEND_OP;

typedef enum D3D11_COLOR_WRITE_ENABLE {
    D3D11_COLOR_WRITE_ENABLE_RED   = 1,
    D3D11_COLOR_WRITE_ENABLE_GREEN = 2,
    D3D11_COLOR_WRITE_ENABLE_BLUE  = 4,
    D3D11_COLOR_WRITE_ENABLE_ALPHA = 8,
    D3D11_COLOR_WRITE_ENABLE_ALL =
        (((D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN) | D3D11_COLOR_WRITE_ENABLE_BLUE) | D3D11_COLOR_WRITE_ENABLE_ALPHA)
} D3D11_COLOR_WRITE_ENABLE;

typedef struct D3D11_RENDER_TARGET_BLEND_DESC {
    BOOL           BlendEnable;
    D3D11_BLEND    SrcBlend;
    D3D11_BLEND    DestBlend;
    D3D11_BLEND_OP BlendOp;
    D3D11_BLEND    SrcBlendAlpha;
    D3D11_BLEND    DestBlendAlpha;
    D3D11_BLEND_OP BlendOpAlpha;
    UINT8          RenderTargetWriteMask;
} D3D11_RENDER_TARGET_BLEND_DESC;

typedef struct D3D11_BLEND_DESC {
    BOOL                           AlphaToCoverageEnable;
    BOOL                           IndependentBlendEnable;
    D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[8];
} D3D11_BLEND_DESC;

    /* Note, the array size for RenderTarget[] above is D3D11_SIMULTANEOUS_RENDERTARGET_COUNT.
       IDL processing/generation of this header replaces the define; this comment is merely explaining what happened. */
    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_BLEND_DESC : public D3D11_BLEND_DESC {
    CD3D11_BLEND_DESC() {}
    explicit CD3D11_BLEND_DESC(const D3D11_BLEND_DESC & o): D3D11_BLEND_DESC(o) {}
    explicit CD3D11_BLEND_DESC(CD3D11_DEFAULT) {
        AlphaToCoverageEnable                                             = FALSE;
        IndependentBlendEnable                                            = FALSE;
        const D3D11_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc = {
            FALSE, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL,
        };
        for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) RenderTarget[i] = defaultRenderTargetBlendDesc;
    }
    ~CD3D11_BLEND_DESC() {}
    operator const D3D11_BLEND_DESC &() const { return *this; }
};
extern "C" {
    #endif

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0002_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0002_v0_0_s_ifspec;

    #ifndef __ID3D11BlendState_INTERFACE_DEFINED__
        #define __ID3D11BlendState_INTERFACE_DEFINED__

/* interface ID3D11BlendState */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11BlendState;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("75b68faa-347d-4159-8f45-a0640f01cd9a")
ID3D11BlendState : public ID3D11DeviceChild {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_BLEND_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11BlendStateVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11BlendState * This,
     /* [in] */ REFIID  riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11BlendState * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11BlendState * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11BlendState * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11BlendState * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11BlendState * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11BlendState * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11BlendState * This,
                                      /* [annotation] */
                                      _Out_ D3D11_BLEND_DESC * pDesc);

    END_INTERFACE
} ID3D11BlendStateVtbl;

interface ID3D11BlendState {
    CONST_VTBL struct ID3D11BlendStateVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11BlendState_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11BlendState_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11BlendState_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11BlendState_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11BlendState_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11BlendState_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11BlendState_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11BlendState_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11BlendState_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0003 */
/* [local] */

typedef struct D3D11_RASTERIZER_DESC {
    D3D11_FILL_MODE FillMode;
    D3D11_CULL_MODE CullMode;
    BOOL            FrontCounterClockwise;
    INT             DepthBias;
    FLOAT           DepthBiasClamp;
    FLOAT           SlopeScaledDepthBias;
    BOOL            DepthClipEnable;
    BOOL            ScissorEnable;
    BOOL            MultisampleEnable;
    BOOL            AntialiasedLineEnable;
} D3D11_RASTERIZER_DESC;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_RASTERIZER_DESC : public D3D11_RASTERIZER_DESC {
    CD3D11_RASTERIZER_DESC() {}
    explicit CD3D11_RASTERIZER_DESC(const D3D11_RASTERIZER_DESC & o): D3D11_RASTERIZER_DESC(o) {}
    explicit CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT) {
        FillMode              = D3D11_FILL_SOLID;
        CullMode              = D3D11_CULL_BACK;
        FrontCounterClockwise = FALSE;
        DepthBias             = D3D11_DEFAULT_DEPTH_BIAS;
        DepthBiasClamp        = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;
        SlopeScaledDepthBias  = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        DepthClipEnable       = TRUE;
        ScissorEnable         = FALSE;
        MultisampleEnable     = FALSE;
        AntialiasedLineEnable = FALSE;
    }
    explicit CD3D11_RASTERIZER_DESC(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode, BOOL frontCounterClockwise, INT depthBias, FLOAT depthBiasClamp,
                                    FLOAT slopeScaledDepthBias, BOOL depthClipEnable, BOOL scissorEnable, BOOL multisampleEnable, BOOL antialiasedLineEnable) {
        FillMode              = fillMode;
        CullMode              = cullMode;
        FrontCounterClockwise = frontCounterClockwise;
        DepthBias             = depthBias;
        DepthBiasClamp        = depthBiasClamp;
        SlopeScaledDepthBias  = slopeScaledDepthBias;
        DepthClipEnable       = depthClipEnable;
        ScissorEnable         = scissorEnable;
        MultisampleEnable     = multisampleEnable;
        AntialiasedLineEnable = antialiasedLineEnable;
    }
    ~CD3D11_RASTERIZER_DESC() {}
    operator const D3D11_RASTERIZER_DESC &() const { return *this; }
};
extern "C" {
    #endif

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0003_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0003_v0_0_s_ifspec;

    #ifndef __ID3D11RasterizerState_INTERFACE_DEFINED__
        #define __ID3D11RasterizerState_INTERFACE_DEFINED__

/* interface ID3D11RasterizerState */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11RasterizerState;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("9bb4ab81-ab1a-4d8f-b506-fc04200b6ee7")
ID3D11RasterizerState : public ID3D11DeviceChild {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_RASTERIZER_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11RasterizerStateVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11RasterizerState * This,
     /* [in] */ REFIID       riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11RasterizerState * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11RasterizerState * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11RasterizerState * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11RasterizerState * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11RasterizerState * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11RasterizerState * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11RasterizerState * This,
                                      /* [annotation] */
                                      _Out_ D3D11_RASTERIZER_DESC * pDesc);

    END_INTERFACE
} ID3D11RasterizerStateVtbl;

interface ID3D11RasterizerState {
    CONST_VTBL struct ID3D11RasterizerStateVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11RasterizerState_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11RasterizerState_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11RasterizerState_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11RasterizerState_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11RasterizerState_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11RasterizerState_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11RasterizerState_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11RasterizerState_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11RasterizerState_INTERFACE_DEFINED__ */

    /* interface __MIDL_itf_d3d11_0000_0004 */
    /* [local] */

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
inline UINT D3D11CalcSubresource(UINT MipSlice, UINT ArraySlice, UINT MipLevels) { return MipSlice + ArraySlice * MipLevels; }
extern "C" {
    #endif
typedef struct D3D11_SUBRESOURCE_DATA {
    const void * pSysMem;
    UINT         SysMemPitch;
    UINT         SysMemSlicePitch;
} D3D11_SUBRESOURCE_DATA;

typedef struct D3D11_MAPPED_SUBRESOURCE {
    void * pData;
    UINT   RowPitch;
    UINT   DepthPitch;
} D3D11_MAPPED_SUBRESOURCE;

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0004_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0004_v0_0_s_ifspec;

    #ifndef __ID3D11Resource_INTERFACE_DEFINED__
        #define __ID3D11Resource_INTERFACE_DEFINED__

/* interface ID3D11Resource */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11Resource;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("dc8e63f3-d12b-4952-b47b-5e45026a862d")
ID3D11Resource : public ID3D11DeviceChild {
public:
    virtual void STDMETHODCALLTYPE GetType(
        /* [annotation] */
        _Out_ D3D11_RESOURCE_DIMENSION * pResourceDimension) = 0;

    virtual void STDMETHODCALLTYPE SetEvictionPriority(
        /* [annotation] */
        _In_ UINT EvictionPriority) = 0;

    virtual UINT STDMETHODCALLTYPE GetEvictionPriority(void) = 0;
};

        #else /* C style interface */

typedef struct ID3D11ResourceVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11Resource *  This,
     /* [in] */ REFIID riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11Resource * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11Resource * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11Resource * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11Resource * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11Resource * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11Resource * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetType)(ID3D11Resource * This,
                                      /* [annotation] */
                                      _Out_ D3D11_RESOURCE_DIMENSION * pResourceDimension);

    void(STDMETHODCALLTYPE * SetEvictionPriority)(ID3D11Resource * This,
                                                  /* [annotation] */
                                                  _In_ UINT EvictionPriority);

    UINT(STDMETHODCALLTYPE * GetEvictionPriority)(ID3D11Resource * This);

    END_INTERFACE
} ID3D11ResourceVtbl;

interface ID3D11Resource {
    CONST_VTBL struct ID3D11ResourceVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11Resource_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11Resource_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11Resource_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11Resource_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11Resource_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11Resource_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11Resource_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11Resource_GetType(This, pResourceDimension) ((This)->lpVtbl->GetType(This, pResourceDimension))

                #define ID3D11Resource_SetEvictionPriority(This, EvictionPriority) ((This)->lpVtbl->SetEvictionPriority(This, EvictionPriority))

                #define ID3D11Resource_GetEvictionPriority(This) ((This)->lpVtbl->GetEvictionPriority(This))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11Resource_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0005 */
/* [local] */

typedef struct D3D11_BUFFER_DESC {
    UINT        ByteWidth;
    D3D11_USAGE Usage;
    UINT        BindFlags;
    UINT        CPUAccessFlags;
    UINT        MiscFlags;
    UINT        StructureByteStride;
} D3D11_BUFFER_DESC;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_BUFFER_DESC : public D3D11_BUFFER_DESC {
    CD3D11_BUFFER_DESC() {}
    explicit CD3D11_BUFFER_DESC(const D3D11_BUFFER_DESC & o): D3D11_BUFFER_DESC(o) {}
    explicit CD3D11_BUFFER_DESC(UINT byteWidth, UINT bindFlags, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuaccessFlags = 0, UINT miscFlags = 0,
                                UINT structureByteStride = 0) {
        ByteWidth           = byteWidth;
        Usage               = usage;
        BindFlags           = bindFlags;
        CPUAccessFlags      = cpuaccessFlags;
        MiscFlags           = miscFlags;
        StructureByteStride = structureByteStride;
    }
    ~CD3D11_BUFFER_DESC() {}
    operator const D3D11_BUFFER_DESC &() const { return *this; }
};
extern "C" {
    #endif

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0005_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0005_v0_0_s_ifspec;

    #ifndef __ID3D11Buffer_INTERFACE_DEFINED__
        #define __ID3D11Buffer_INTERFACE_DEFINED__

/* interface ID3D11Buffer */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11Buffer;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("48570b85-d1ee-4fcd-a250-eb350722b037")
ID3D11Buffer : public ID3D11Resource {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_BUFFER_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11BufferVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11Buffer *    This,
     /* [in] */ REFIID riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11Buffer * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11Buffer * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11Buffer * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11Buffer * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11Buffer * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11Buffer * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetType)(ID3D11Buffer * This,
                                      /* [annotation] */
                                      _Out_ D3D11_RESOURCE_DIMENSION * pResourceDimension);

    void(STDMETHODCALLTYPE * SetEvictionPriority)(ID3D11Buffer * This,
                                                  /* [annotation] */
                                                  _In_ UINT EvictionPriority);

    UINT(STDMETHODCALLTYPE * GetEvictionPriority)(ID3D11Buffer * This);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11Buffer * This,
                                      /* [annotation] */
                                      _Out_ D3D11_BUFFER_DESC * pDesc);

    END_INTERFACE
} ID3D11BufferVtbl;

interface ID3D11Buffer {
    CONST_VTBL struct ID3D11BufferVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11Buffer_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11Buffer_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11Buffer_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11Buffer_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11Buffer_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11Buffer_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11Buffer_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11Buffer_GetType(This, pResourceDimension) ((This)->lpVtbl->GetType(This, pResourceDimension))

                #define ID3D11Buffer_SetEvictionPriority(This, EvictionPriority) ((This)->lpVtbl->SetEvictionPriority(This, EvictionPriority))

                #define ID3D11Buffer_GetEvictionPriority(This) ((This)->lpVtbl->GetEvictionPriority(This))

                #define ID3D11Buffer_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11Buffer_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0006 */
/* [local] */

typedef struct D3D11_TEXTURE1D_DESC {
    UINT        Width;
    UINT        MipLevels;
    UINT        ArraySize;
    DXGI_FORMAT Format;
    D3D11_USAGE Usage;
    UINT        BindFlags;
    UINT        CPUAccessFlags;
    UINT        MiscFlags;
} D3D11_TEXTURE1D_DESC;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_TEXTURE1D_DESC : public D3D11_TEXTURE1D_DESC {
    CD3D11_TEXTURE1D_DESC() {}
    explicit CD3D11_TEXTURE1D_DESC(const D3D11_TEXTURE1D_DESC & o): D3D11_TEXTURE1D_DESC(o) {}
    explicit CD3D11_TEXTURE1D_DESC(DXGI_FORMAT format, UINT width, UINT arraySize = 1, UINT mipLevels = 0, UINT bindFlags = D3D11_BIND_SHADER_RESOURCE,
                                   D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuaccessFlags = 0, UINT miscFlags = 0) {
        Width          = width;
        MipLevels      = mipLevels;
        ArraySize      = arraySize;
        Format         = format;
        Usage          = usage;
        BindFlags      = bindFlags;
        CPUAccessFlags = cpuaccessFlags;
        MiscFlags      = miscFlags;
    }
    ~CD3D11_TEXTURE1D_DESC() {}
    operator const D3D11_TEXTURE1D_DESC &() const { return *this; }
};
extern "C" {
    #endif

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0006_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0006_v0_0_s_ifspec;

    #ifndef __ID3D11Texture1D_INTERFACE_DEFINED__
        #define __ID3D11Texture1D_INTERFACE_DEFINED__

/* interface ID3D11Texture1D */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11Texture1D;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("f8fb5c27-c6b3-4f75-a4c8-439af2ef564c")
ID3D11Texture1D : public ID3D11Resource {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_TEXTURE1D_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11Texture1DVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11Texture1D * This,
     /* [in] */ REFIID riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11Texture1D * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11Texture1D * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11Texture1D * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11Texture1D * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11Texture1D * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11Texture1D * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetType)(ID3D11Texture1D * This,
                                      /* [annotation] */
                                      _Out_ D3D11_RESOURCE_DIMENSION * pResourceDimension);

    void(STDMETHODCALLTYPE * SetEvictionPriority)(ID3D11Texture1D * This,
                                                  /* [annotation] */
                                                  _In_ UINT EvictionPriority);

    UINT(STDMETHODCALLTYPE * GetEvictionPriority)(ID3D11Texture1D * This);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11Texture1D * This,
                                      /* [annotation] */
                                      _Out_ D3D11_TEXTURE1D_DESC * pDesc);

    END_INTERFACE
} ID3D11Texture1DVtbl;

interface ID3D11Texture1D {
    CONST_VTBL struct ID3D11Texture1DVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11Texture1D_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11Texture1D_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11Texture1D_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11Texture1D_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11Texture1D_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11Texture1D_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11Texture1D_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11Texture1D_GetType(This, pResourceDimension) ((This)->lpVtbl->GetType(This, pResourceDimension))

                #define ID3D11Texture1D_SetEvictionPriority(This, EvictionPriority) ((This)->lpVtbl->SetEvictionPriority(This, EvictionPriority))

                #define ID3D11Texture1D_GetEvictionPriority(This) ((This)->lpVtbl->GetEvictionPriority(This))

                #define ID3D11Texture1D_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11Texture1D_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0007 */
/* [local] */

typedef struct D3D11_TEXTURE2D_DESC {
    UINT             Width;
    UINT             Height;
    UINT             MipLevels;
    UINT             ArraySize;
    DXGI_FORMAT      Format;
    DXGI_SAMPLE_DESC SampleDesc;
    D3D11_USAGE      Usage;
    UINT             BindFlags;
    UINT             CPUAccessFlags;
    UINT             MiscFlags;
} D3D11_TEXTURE2D_DESC;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_TEXTURE2D_DESC : public D3D11_TEXTURE2D_DESC {
    CD3D11_TEXTURE2D_DESC() {}
    explicit CD3D11_TEXTURE2D_DESC(const D3D11_TEXTURE2D_DESC & o): D3D11_TEXTURE2D_DESC(o) {}
    explicit CD3D11_TEXTURE2D_DESC(DXGI_FORMAT format, UINT width, UINT height, UINT arraySize = 1, UINT mipLevels = 0,
                                   UINT bindFlags = D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuaccessFlags = 0,
                                   UINT sampleCount = 1, UINT sampleQuality = 0, UINT miscFlags = 0) {
        Width              = width;
        Height             = height;
        MipLevels          = mipLevels;
        ArraySize          = arraySize;
        Format             = format;
        SampleDesc.Count   = sampleCount;
        SampleDesc.Quality = sampleQuality;
        Usage              = usage;
        BindFlags          = bindFlags;
        CPUAccessFlags     = cpuaccessFlags;
        MiscFlags          = miscFlags;
    }
    ~CD3D11_TEXTURE2D_DESC() {}
    operator const D3D11_TEXTURE2D_DESC &() const { return *this; }
};
extern "C" {
    #endif

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0007_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0007_v0_0_s_ifspec;

    #ifndef __ID3D11Texture2D_INTERFACE_DEFINED__
        #define __ID3D11Texture2D_INTERFACE_DEFINED__

/* interface ID3D11Texture2D */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11Texture2D;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("6f15aaf2-d208-4e89-9ab4-489535d34f9c")
ID3D11Texture2D : public ID3D11Resource {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_TEXTURE2D_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11Texture2DVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11Texture2D * This,
     /* [in] */ REFIID riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11Texture2D * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11Texture2D * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11Texture2D * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11Texture2D * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11Texture2D * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11Texture2D * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetType)(ID3D11Texture2D * This,
                                      /* [annotation] */
                                      _Out_ D3D11_RESOURCE_DIMENSION * pResourceDimension);

    void(STDMETHODCALLTYPE * SetEvictionPriority)(ID3D11Texture2D * This,
                                                  /* [annotation] */
                                                  _In_ UINT EvictionPriority);

    UINT(STDMETHODCALLTYPE * GetEvictionPriority)(ID3D11Texture2D * This);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11Texture2D * This,
                                      /* [annotation] */
                                      _Out_ D3D11_TEXTURE2D_DESC * pDesc);

    END_INTERFACE
} ID3D11Texture2DVtbl;

interface ID3D11Texture2D {
    CONST_VTBL struct ID3D11Texture2DVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11Texture2D_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11Texture2D_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11Texture2D_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11Texture2D_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11Texture2D_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11Texture2D_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11Texture2D_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11Texture2D_GetType(This, pResourceDimension) ((This)->lpVtbl->GetType(This, pResourceDimension))

                #define ID3D11Texture2D_SetEvictionPriority(This, EvictionPriority) ((This)->lpVtbl->SetEvictionPriority(This, EvictionPriority))

                #define ID3D11Texture2D_GetEvictionPriority(This) ((This)->lpVtbl->GetEvictionPriority(This))

                #define ID3D11Texture2D_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11Texture2D_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0008 */
/* [local] */

typedef struct D3D11_TEXTURE3D_DESC {
    UINT        Width;
    UINT        Height;
    UINT        Depth;
    UINT        MipLevels;
    DXGI_FORMAT Format;
    D3D11_USAGE Usage;
    UINT        BindFlags;
    UINT        CPUAccessFlags;
    UINT        MiscFlags;
} D3D11_TEXTURE3D_DESC;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_TEXTURE3D_DESC : public D3D11_TEXTURE3D_DESC {
    CD3D11_TEXTURE3D_DESC() {}
    explicit CD3D11_TEXTURE3D_DESC(const D3D11_TEXTURE3D_DESC & o): D3D11_TEXTURE3D_DESC(o) {}
    explicit CD3D11_TEXTURE3D_DESC(DXGI_FORMAT format, UINT width, UINT height, UINT depth, UINT mipLevels = 0, UINT bindFlags = D3D11_BIND_SHADER_RESOURCE,
                                   D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuaccessFlags = 0, UINT miscFlags = 0) {
        Width          = width;
        Height         = height;
        Depth          = depth;
        MipLevels      = mipLevels;
        Format         = format;
        Usage          = usage;
        BindFlags      = bindFlags;
        CPUAccessFlags = cpuaccessFlags;
        MiscFlags      = miscFlags;
    }
    ~CD3D11_TEXTURE3D_DESC() {}
    operator const D3D11_TEXTURE3D_DESC &() const { return *this; }
};
extern "C" {
    #endif

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0008_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0008_v0_0_s_ifspec;

    #ifndef __ID3D11Texture3D_INTERFACE_DEFINED__
        #define __ID3D11Texture3D_INTERFACE_DEFINED__

/* interface ID3D11Texture3D */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11Texture3D;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("037e866e-f56d-4357-a8af-9dabbe6e250e")
ID3D11Texture3D : public ID3D11Resource {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_TEXTURE3D_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11Texture3DVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11Texture3D * This,
     /* [in] */ REFIID riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11Texture3D * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11Texture3D * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11Texture3D * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11Texture3D * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11Texture3D * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11Texture3D * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetType)(ID3D11Texture3D * This,
                                      /* [annotation] */
                                      _Out_ D3D11_RESOURCE_DIMENSION * pResourceDimension);

    void(STDMETHODCALLTYPE * SetEvictionPriority)(ID3D11Texture3D * This,
                                                  /* [annotation] */
                                                  _In_ UINT EvictionPriority);

    UINT(STDMETHODCALLTYPE * GetEvictionPriority)(ID3D11Texture3D * This);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11Texture3D * This,
                                      /* [annotation] */
                                      _Out_ D3D11_TEXTURE3D_DESC * pDesc);

    END_INTERFACE
} ID3D11Texture3DVtbl;

interface ID3D11Texture3D {
    CONST_VTBL struct ID3D11Texture3DVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11Texture3D_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11Texture3D_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11Texture3D_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11Texture3D_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11Texture3D_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11Texture3D_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11Texture3D_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11Texture3D_GetType(This, pResourceDimension) ((This)->lpVtbl->GetType(This, pResourceDimension))

                #define ID3D11Texture3D_SetEvictionPriority(This, EvictionPriority) ((This)->lpVtbl->SetEvictionPriority(This, EvictionPriority))

                #define ID3D11Texture3D_GetEvictionPriority(This) ((This)->lpVtbl->GetEvictionPriority(This))

                #define ID3D11Texture3D_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11Texture3D_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0009 */
/* [local] */

typedef enum D3D11_TEXTURECUBE_FACE {
    D3D11_TEXTURECUBE_FACE_POSITIVE_X = 0,
    D3D11_TEXTURECUBE_FACE_NEGATIVE_X = 1,
    D3D11_TEXTURECUBE_FACE_POSITIVE_Y = 2,
    D3D11_TEXTURECUBE_FACE_NEGATIVE_Y = 3,
    D3D11_TEXTURECUBE_FACE_POSITIVE_Z = 4,
    D3D11_TEXTURECUBE_FACE_NEGATIVE_Z = 5
} D3D11_TEXTURECUBE_FACE;

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0009_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0009_v0_0_s_ifspec;

    #ifndef __ID3D11View_INTERFACE_DEFINED__
        #define __ID3D11View_INTERFACE_DEFINED__

/* interface ID3D11View */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11View;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("839d1216-bb2e-412b-b7f4-a9dbebe08ed1")
ID3D11View : public ID3D11DeviceChild {
public:
    virtual void STDMETHODCALLTYPE GetResource(
        /* [annotation] */
        _Out_ ID3D11Resource * *ppResource) = 0;
};

        #else /* C style interface */

typedef struct ID3D11ViewVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11View *      This,
     /* [in] */ REFIID riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11View * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11View * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11View * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11View * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11View * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11View * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetResource)(ID3D11View * This,
                                          /* [annotation] */
                                          _Out_ ID3D11Resource ** ppResource);

    END_INTERFACE
} ID3D11ViewVtbl;

interface ID3D11View {
    CONST_VTBL struct ID3D11ViewVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11View_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11View_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11View_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11View_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11View_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11View_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11View_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11View_GetResource(This, ppResource) ((This)->lpVtbl->GetResource(This, ppResource))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11View_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0010 */
/* [local] */

typedef struct D3D11_BUFFER_SRV {
    union {
        UINT FirstElement;
        UINT ElementOffset;
    };
    union {
        UINT NumElements;
        UINT ElementWidth;
    };
} D3D11_BUFFER_SRV;

typedef enum D3D11_BUFFEREX_SRV_FLAG { D3D11_BUFFEREX_SRV_FLAG_RAW = 0x1 } D3D11_BUFFEREX_SRV_FLAG;

typedef struct D3D11_BUFFEREX_SRV {
    UINT FirstElement;
    UINT NumElements;
    UINT Flags;
} D3D11_BUFFEREX_SRV;

typedef struct D3D11_TEX1D_SRV {
    UINT MostDetailedMip;
    UINT MipLevels;
} D3D11_TEX1D_SRV;

typedef struct D3D11_TEX1D_ARRAY_SRV {
    UINT MostDetailedMip;
    UINT MipLevels;
    UINT FirstArraySlice;
    UINT ArraySize;
} D3D11_TEX1D_ARRAY_SRV;

typedef struct D3D11_TEX2D_SRV {
    UINT MostDetailedMip;
    UINT MipLevels;
} D3D11_TEX2D_SRV;

typedef struct D3D11_TEX2D_ARRAY_SRV {
    UINT MostDetailedMip;
    UINT MipLevels;
    UINT FirstArraySlice;
    UINT ArraySize;
} D3D11_TEX2D_ARRAY_SRV;

typedef struct D3D11_TEX3D_SRV {
    UINT MostDetailedMip;
    UINT MipLevels;
} D3D11_TEX3D_SRV;

typedef struct D3D11_TEXCUBE_SRV {
    UINT MostDetailedMip;
    UINT MipLevels;
} D3D11_TEXCUBE_SRV;

typedef struct D3D11_TEXCUBE_ARRAY_SRV {
    UINT MostDetailedMip;
    UINT MipLevels;
    UINT First2DArrayFace;
    UINT NumCubes;
} D3D11_TEXCUBE_ARRAY_SRV;

typedef struct D3D11_TEX2DMS_SRV {
    UINT UnusedField_NothingToDefine;
} D3D11_TEX2DMS_SRV;

typedef struct D3D11_TEX2DMS_ARRAY_SRV {
    UINT FirstArraySlice;
    UINT ArraySize;
} D3D11_TEX2DMS_ARRAY_SRV;

typedef struct D3D11_SHADER_RESOURCE_VIEW_DESC {
    DXGI_FORMAT         Format;
    D3D11_SRV_DIMENSION ViewDimension;
    union {
        D3D11_BUFFER_SRV        Buffer;
        D3D11_TEX1D_SRV         Texture1D;
        D3D11_TEX1D_ARRAY_SRV   Texture1DArray;
        D3D11_TEX2D_SRV         Texture2D;
        D3D11_TEX2D_ARRAY_SRV   Texture2DArray;
        D3D11_TEX2DMS_SRV       Texture2DMS;
        D3D11_TEX2DMS_ARRAY_SRV Texture2DMSArray;
        D3D11_TEX3D_SRV         Texture3D;
        D3D11_TEXCUBE_SRV       TextureCube;
        D3D11_TEXCUBE_ARRAY_SRV TextureCubeArray;
        D3D11_BUFFEREX_SRV      BufferEx;
    };
} D3D11_SHADER_RESOURCE_VIEW_DESC;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_SHADER_RESOURCE_VIEW_DESC : public D3D11_SHADER_RESOURCE_VIEW_DESC {
    CD3D11_SHADER_RESOURCE_VIEW_DESC() {}
    explicit CD3D11_SHADER_RESOURCE_VIEW_DESC(const D3D11_SHADER_RESOURCE_VIEW_DESC & o): D3D11_SHADER_RESOURCE_VIEW_DESC(o) {}
    explicit CD3D11_SHADER_RESOURCE_VIEW_DESC(D3D11_SRV_DIMENSION viewDimension, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN,
                                              UINT mostDetailedMip = 0,  // FirstElement for BUFFER
                                              UINT mipLevels       = -1, // NumElements for BUFFER
                                              UINT firstArraySlice = 0,  // First2DArrayFace for TEXTURECUBEARRAY
                                              UINT arraySize       = -1, // NumCubes for TEXTURECUBEARRAY
                                              UINT flags           = 0)            // BUFFEREX only
    {
        Format        = format;
        ViewDimension = viewDimension;
        switch (viewDimension) {
        case D3D11_SRV_DIMENSION_BUFFER:
            Buffer.FirstElement = mostDetailedMip;
            Buffer.NumElements  = mipLevels;
            break;
        case D3D11_SRV_DIMENSION_TEXTURE1D:
            Texture1D.MostDetailedMip = mostDetailedMip;
            Texture1D.MipLevels       = mipLevels;
            break;
        case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:
            Texture1DArray.MostDetailedMip = mostDetailedMip;
            Texture1DArray.MipLevels       = mipLevels;
            Texture1DArray.FirstArraySlice = firstArraySlice;
            Texture1DArray.ArraySize       = arraySize;
            break;
        case D3D11_SRV_DIMENSION_TEXTURE2D:
            Texture2D.MostDetailedMip = mostDetailedMip;
            Texture2D.MipLevels       = mipLevels;
            break;
        case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
            Texture2DArray.MostDetailedMip = mostDetailedMip;
            Texture2DArray.MipLevels       = mipLevels;
            Texture2DArray.FirstArraySlice = firstArraySlice;
            Texture2DArray.ArraySize       = arraySize;
            break;
        case D3D11_SRV_DIMENSION_TEXTURE2DMS:
            break;
        case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY:
            Texture2DMSArray.FirstArraySlice = firstArraySlice;
            Texture2DMSArray.ArraySize       = arraySize;
            break;
        case D3D11_SRV_DIMENSION_TEXTURE3D:
            Texture3D.MostDetailedMip = mostDetailedMip;
            Texture3D.MipLevels       = mipLevels;
            break;
        case D3D11_SRV_DIMENSION_TEXTURECUBE:
            TextureCube.MostDetailedMip = mostDetailedMip;
            TextureCube.MipLevels       = mipLevels;
            break;
        case D3D11_SRV_DIMENSION_TEXTURECUBEARRAY:
            TextureCubeArray.MostDetailedMip  = mostDetailedMip;
            TextureCubeArray.MipLevels        = mipLevels;
            TextureCubeArray.First2DArrayFace = firstArraySlice;
            TextureCubeArray.NumCubes         = arraySize;
            break;
        case D3D11_SRV_DIMENSION_BUFFEREX:
            BufferEx.FirstElement = mostDetailedMip;
            BufferEx.NumElements  = mipLevels;
            BufferEx.Flags        = flags;
            break;
        default:
            break;
        }
    }
    explicit CD3D11_SHADER_RESOURCE_VIEW_DESC(_In_ ID3D11Buffer *, DXGI_FORMAT format, UINT firstElement, UINT numElements, UINT flags = 0) {
        Format                = format;
        ViewDimension         = D3D11_SRV_DIMENSION_BUFFEREX;
        BufferEx.FirstElement = firstElement;
        BufferEx.NumElements  = numElements;
        BufferEx.Flags        = flags;
    }
    explicit CD3D11_SHADER_RESOURCE_VIEW_DESC(_In_ ID3D11Texture1D * pTex1D, D3D11_SRV_DIMENSION viewDimension, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN,
                                              UINT mostDetailedMip = 0, UINT mipLevels = -1, UINT firstArraySlice = 0, UINT arraySize = -1) {
        ViewDimension = viewDimension;
        if (DXGI_FORMAT_UNKNOWN == format || -1 == mipLevels || (-1 == arraySize && D3D11_SRV_DIMENSION_TEXTURE1DARRAY == viewDimension)) {
            D3D11_TEXTURE1D_DESC TexDesc;
            pTex1D->GetDesc(&TexDesc);
            if (DXGI_FORMAT_UNKNOWN == format) format = TexDesc.Format;
            if (-1 == mipLevels) mipLevels = TexDesc.MipLevels - mostDetailedMip;
            if (-1 == arraySize) arraySize = TexDesc.ArraySize - firstArraySlice;
        }
        Format = format;
        switch (viewDimension) {
        case D3D11_SRV_DIMENSION_TEXTURE1D:
            Texture1D.MostDetailedMip = mostDetailedMip;
            Texture1D.MipLevels       = mipLevels;
            break;
        case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:
            Texture1DArray.MostDetailedMip = mostDetailedMip;
            Texture1DArray.MipLevels       = mipLevels;
            Texture1DArray.FirstArraySlice = firstArraySlice;
            Texture1DArray.ArraySize       = arraySize;
            break;
        default:
            break;
        }
    }
    explicit CD3D11_SHADER_RESOURCE_VIEW_DESC(_In_ ID3D11Texture2D * pTex2D, D3D11_SRV_DIMENSION viewDimension, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN,
                                              UINT mostDetailedMip = 0, UINT mipLevels = -1,
                                              UINT firstArraySlice = 0, // First2DArrayFace for TEXTURECUBEARRAY
                                              UINT arraySize       = -1)      // NumCubes for TEXTURECUBEARRAY
    {
        ViewDimension = viewDimension;
        if (DXGI_FORMAT_UNKNOWN == format ||
            (-1 == mipLevels && D3D11_SRV_DIMENSION_TEXTURE2DMS != viewDimension && D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY != viewDimension) ||
            (-1 == arraySize && (D3D11_SRV_DIMENSION_TEXTURE2DARRAY == viewDimension || D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY == viewDimension ||
                                 D3D11_SRV_DIMENSION_TEXTURECUBEARRAY == viewDimension))) {
            D3D11_TEXTURE2D_DESC TexDesc;
            pTex2D->GetDesc(&TexDesc);
            if (DXGI_FORMAT_UNKNOWN == format) format = TexDesc.Format;
            if (-1 == mipLevels) mipLevels = TexDesc.MipLevels - mostDetailedMip;
            if (-1 == arraySize) {
                arraySize = TexDesc.ArraySize - firstArraySlice;
                if (D3D11_SRV_DIMENSION_TEXTURECUBEARRAY == viewDimension) arraySize /= 6;
            }
        }
        Format = format;
        switch (viewDimension) {
        case D3D11_SRV_DIMENSION_TEXTURE2D:
            Texture2D.MostDetailedMip = mostDetailedMip;
            Texture2D.MipLevels       = mipLevels;
            break;
        case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
            Texture2DArray.MostDetailedMip = mostDetailedMip;
            Texture2DArray.MipLevels       = mipLevels;
            Texture2DArray.FirstArraySlice = firstArraySlice;
            Texture2DArray.ArraySize       = arraySize;
            break;
        case D3D11_SRV_DIMENSION_TEXTURE2DMS:
            break;
        case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY:
            Texture2DMSArray.FirstArraySlice = firstArraySlice;
            Texture2DMSArray.ArraySize       = arraySize;
            break;
        case D3D11_SRV_DIMENSION_TEXTURECUBE:
            TextureCube.MostDetailedMip = mostDetailedMip;
            TextureCube.MipLevels       = mipLevels;
            break;
        case D3D11_SRV_DIMENSION_TEXTURECUBEARRAY:
            TextureCubeArray.MostDetailedMip  = mostDetailedMip;
            TextureCubeArray.MipLevels        = mipLevels;
            TextureCubeArray.First2DArrayFace = firstArraySlice;
            TextureCubeArray.NumCubes         = arraySize;
            break;
        default:
            break;
        }
    }
    explicit CD3D11_SHADER_RESOURCE_VIEW_DESC(_In_ ID3D11Texture3D * pTex3D, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, UINT mostDetailedMip = 0,
                                              UINT mipLevels = -1) {
        ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
        if (DXGI_FORMAT_UNKNOWN == format || -1 == mipLevels) {
            D3D11_TEXTURE3D_DESC TexDesc;
            pTex3D->GetDesc(&TexDesc);
            if (DXGI_FORMAT_UNKNOWN == format) format = TexDesc.Format;
            if (-1 == mipLevels) mipLevels = TexDesc.MipLevels - mostDetailedMip;
        }
        Format                    = format;
        Texture3D.MostDetailedMip = mostDetailedMip;
        Texture3D.MipLevels       = mipLevels;
    }
    ~CD3D11_SHADER_RESOURCE_VIEW_DESC() {}
    operator const D3D11_SHADER_RESOURCE_VIEW_DESC &() const { return *this; }
};
extern "C" {
    #endif

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0010_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0010_v0_0_s_ifspec;

    #ifndef __ID3D11ShaderResourceView_INTERFACE_DEFINED__
        #define __ID3D11ShaderResourceView_INTERFACE_DEFINED__

/* interface ID3D11ShaderResourceView */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11ShaderResourceView;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("b0e06fe0-8192-4e1a-b1ca-36d7414710b2")
ID3D11ShaderResourceView : public ID3D11View {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11ShaderResourceViewVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11ShaderResourceView * This,
     /* [in] */ REFIID          riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11ShaderResourceView * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11ShaderResourceView * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11ShaderResourceView * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11ShaderResourceView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11ShaderResourceView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11ShaderResourceView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetResource)(ID3D11ShaderResourceView * This,
                                          /* [annotation] */
                                          _Out_ ID3D11Resource ** ppResource);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11ShaderResourceView * This,
                                      /* [annotation] */
                                      _Out_ D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc);

    END_INTERFACE
} ID3D11ShaderResourceViewVtbl;

interface ID3D11ShaderResourceView {
    CONST_VTBL struct ID3D11ShaderResourceViewVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11ShaderResourceView_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11ShaderResourceView_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11ShaderResourceView_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11ShaderResourceView_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11ShaderResourceView_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11ShaderResourceView_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11ShaderResourceView_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11ShaderResourceView_GetResource(This, ppResource) ((This)->lpVtbl->GetResource(This, ppResource))

                #define ID3D11ShaderResourceView_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11ShaderResourceView_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0011 */
/* [local] */

typedef struct D3D11_BUFFER_RTV {
    union {
        UINT FirstElement;
        UINT ElementOffset;
    };
    union {
        UINT NumElements;
        UINT ElementWidth;
    };
} D3D11_BUFFER_RTV;

typedef struct D3D11_TEX1D_RTV {
    UINT MipSlice;
} D3D11_TEX1D_RTV;

typedef struct D3D11_TEX1D_ARRAY_RTV {
    UINT MipSlice;
    UINT FirstArraySlice;
    UINT ArraySize;
} D3D11_TEX1D_ARRAY_RTV;

typedef struct D3D11_TEX2D_RTV {
    UINT MipSlice;
} D3D11_TEX2D_RTV;

typedef struct D3D11_TEX2DMS_RTV {
    UINT UnusedField_NothingToDefine;
} D3D11_TEX2DMS_RTV;

typedef struct D3D11_TEX2D_ARRAY_RTV {
    UINT MipSlice;
    UINT FirstArraySlice;
    UINT ArraySize;
} D3D11_TEX2D_ARRAY_RTV;

typedef struct D3D11_TEX2DMS_ARRAY_RTV {
    UINT FirstArraySlice;
    UINT ArraySize;
} D3D11_TEX2DMS_ARRAY_RTV;

typedef struct D3D11_TEX3D_RTV {
    UINT MipSlice;
    UINT FirstWSlice;
    UINT WSize;
} D3D11_TEX3D_RTV;

typedef struct D3D11_RENDER_TARGET_VIEW_DESC {
    DXGI_FORMAT         Format;
    D3D11_RTV_DIMENSION ViewDimension;
    union {
        D3D11_BUFFER_RTV        Buffer;
        D3D11_TEX1D_RTV         Texture1D;
        D3D11_TEX1D_ARRAY_RTV   Texture1DArray;
        D3D11_TEX2D_RTV         Texture2D;
        D3D11_TEX2D_ARRAY_RTV   Texture2DArray;
        D3D11_TEX2DMS_RTV       Texture2DMS;
        D3D11_TEX2DMS_ARRAY_RTV Texture2DMSArray;
        D3D11_TEX3D_RTV         Texture3D;
    };
} D3D11_RENDER_TARGET_VIEW_DESC;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_RENDER_TARGET_VIEW_DESC : public D3D11_RENDER_TARGET_VIEW_DESC {
    CD3D11_RENDER_TARGET_VIEW_DESC() {}
    explicit CD3D11_RENDER_TARGET_VIEW_DESC(const D3D11_RENDER_TARGET_VIEW_DESC & o): D3D11_RENDER_TARGET_VIEW_DESC(o) {}
    explicit CD3D11_RENDER_TARGET_VIEW_DESC(D3D11_RTV_DIMENSION viewDimension, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN,
                                            UINT mipSlice        = 0, // FirstElement for BUFFER
                                            UINT firstArraySlice = 0, // NumElements for BUFFER, FirstWSlice for TEXTURE3D
                                            UINT arraySize       = -1)      // WSize for TEXTURE3D
    {
        Format        = format;
        ViewDimension = viewDimension;
        switch (viewDimension) {
        case D3D11_RTV_DIMENSION_BUFFER:
            Buffer.FirstElement = mipSlice;
            Buffer.NumElements  = firstArraySlice;
            break;
        case D3D11_RTV_DIMENSION_TEXTURE1D:
            Texture1D.MipSlice = mipSlice;
            break;
        case D3D11_RTV_DIMENSION_TEXTURE1DARRAY:
            Texture1DArray.MipSlice        = mipSlice;
            Texture1DArray.FirstArraySlice = firstArraySlice;
            Texture1DArray.ArraySize       = arraySize;
            break;
        case D3D11_RTV_DIMENSION_TEXTURE2D:
            Texture2D.MipSlice = mipSlice;
            break;
        case D3D11_RTV_DIMENSION_TEXTURE2DARRAY:
            Texture2DArray.MipSlice        = mipSlice;
            Texture2DArray.FirstArraySlice = firstArraySlice;
            Texture2DArray.ArraySize       = arraySize;
            break;
        case D3D11_RTV_DIMENSION_TEXTURE2DMS:
            break;
        case D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY:
            Texture2DMSArray.FirstArraySlice = firstArraySlice;
            Texture2DMSArray.ArraySize       = arraySize;
            break;
        case D3D11_RTV_DIMENSION_TEXTURE3D:
            Texture3D.MipSlice    = mipSlice;
            Texture3D.FirstWSlice = firstArraySlice;
            Texture3D.WSize       = arraySize;
            break;
        default:
            break;
        }
    }
    explicit CD3D11_RENDER_TARGET_VIEW_DESC(_In_ ID3D11Buffer *, DXGI_FORMAT format, UINT firstElement, UINT numElements) {
        Format              = format;
        ViewDimension       = D3D11_RTV_DIMENSION_BUFFER;
        Buffer.FirstElement = firstElement;
        Buffer.NumElements  = numElements;
    }
    explicit CD3D11_RENDER_TARGET_VIEW_DESC(_In_ ID3D11Texture1D * pTex1D, D3D11_RTV_DIMENSION viewDimension, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN,
                                            UINT mipSlice = 0, UINT firstArraySlice = 0, UINT arraySize = -1) {
        ViewDimension = viewDimension;
        if (DXGI_FORMAT_UNKNOWN == format || (-1 == arraySize && D3D11_RTV_DIMENSION_TEXTURE1DARRAY == viewDimension)) {
            D3D11_TEXTURE1D_DESC TexDesc;
            pTex1D->GetDesc(&TexDesc);
            if (DXGI_FORMAT_UNKNOWN == format) format = TexDesc.Format;
            if (-1 == arraySize) arraySize = TexDesc.ArraySize - firstArraySlice;
        }
        Format = format;
        switch (viewDimension) {
        case D3D11_RTV_DIMENSION_TEXTURE1D:
            Texture1D.MipSlice = mipSlice;
            break;
        case D3D11_RTV_DIMENSION_TEXTURE1DARRAY:
            Texture1DArray.MipSlice        = mipSlice;
            Texture1DArray.FirstArraySlice = firstArraySlice;
            Texture1DArray.ArraySize       = arraySize;
            break;
        default:
            break;
        }
    }
    explicit CD3D11_RENDER_TARGET_VIEW_DESC(_In_ ID3D11Texture2D * pTex2D, D3D11_RTV_DIMENSION viewDimension, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN,
                                            UINT mipSlice = 0, UINT firstArraySlice = 0, UINT arraySize = -1) {
        ViewDimension = viewDimension;
        if (DXGI_FORMAT_UNKNOWN == format ||
            (-1 == arraySize && (D3D11_RTV_DIMENSION_TEXTURE2DARRAY == viewDimension || D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY == viewDimension))) {
            D3D11_TEXTURE2D_DESC TexDesc;
            pTex2D->GetDesc(&TexDesc);
            if (DXGI_FORMAT_UNKNOWN == format) format = TexDesc.Format;
            if (-1 == arraySize) arraySize = TexDesc.ArraySize - firstArraySlice;
        }
        Format = format;
        switch (viewDimension) {
        case D3D11_RTV_DIMENSION_TEXTURE2D:
            Texture2D.MipSlice = mipSlice;
            break;
        case D3D11_RTV_DIMENSION_TEXTURE2DARRAY:
            Texture2DArray.MipSlice        = mipSlice;
            Texture2DArray.FirstArraySlice = firstArraySlice;
            Texture2DArray.ArraySize       = arraySize;
            break;
        case D3D11_RTV_DIMENSION_TEXTURE2DMS:
            break;
        case D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY:
            Texture2DMSArray.FirstArraySlice = firstArraySlice;
            Texture2DMSArray.ArraySize       = arraySize;
            break;
        default:
            break;
        }
    }
    explicit CD3D11_RENDER_TARGET_VIEW_DESC(_In_ ID3D11Texture3D * pTex3D, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, UINT mipSlice = 0, UINT firstWSlice = 0,
                                            UINT wSize = -1) {
        ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
        if (DXGI_FORMAT_UNKNOWN == format || -1 == wSize) {
            D3D11_TEXTURE3D_DESC TexDesc;
            pTex3D->GetDesc(&TexDesc);
            if (DXGI_FORMAT_UNKNOWN == format) format = TexDesc.Format;
            if (-1 == wSize) wSize = TexDesc.Depth - firstWSlice;
        }
        Format                = format;
        Texture3D.MipSlice    = mipSlice;
        Texture3D.FirstWSlice = firstWSlice;
        Texture3D.WSize       = wSize;
    }
    ~CD3D11_RENDER_TARGET_VIEW_DESC() {}
    operator const D3D11_RENDER_TARGET_VIEW_DESC &() const { return *this; }
};
extern "C" {
    #endif

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0011_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0011_v0_0_s_ifspec;

    #ifndef __ID3D11RenderTargetView_INTERFACE_DEFINED__
        #define __ID3D11RenderTargetView_INTERFACE_DEFINED__

/* interface ID3D11RenderTargetView */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11RenderTargetView;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("dfdba067-0b8d-4865-875b-d7b4516cc164")
ID3D11RenderTargetView : public ID3D11View {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_RENDER_TARGET_VIEW_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11RenderTargetViewVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11RenderTargetView * This,
     /* [in] */ REFIID        riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11RenderTargetView * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11RenderTargetView * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11RenderTargetView * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11RenderTargetView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11RenderTargetView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11RenderTargetView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetResource)(ID3D11RenderTargetView * This,
                                          /* [annotation] */
                                          _Out_ ID3D11Resource ** ppResource);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11RenderTargetView * This,
                                      /* [annotation] */
                                      _Out_ D3D11_RENDER_TARGET_VIEW_DESC * pDesc);

    END_INTERFACE
} ID3D11RenderTargetViewVtbl;

interface ID3D11RenderTargetView {
    CONST_VTBL struct ID3D11RenderTargetViewVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11RenderTargetView_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11RenderTargetView_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11RenderTargetView_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11RenderTargetView_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11RenderTargetView_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11RenderTargetView_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11RenderTargetView_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11RenderTargetView_GetResource(This, ppResource) ((This)->lpVtbl->GetResource(This, ppResource))

                #define ID3D11RenderTargetView_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11RenderTargetView_INTERFACE_DEFINED__ */

    /* interface __MIDL_itf_d3d11_0000_0012 */
    /* [local] */

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_VIEWPORT : public D3D11_VIEWPORT {
    CD3D11_VIEWPORT() {}
    explicit CD3D11_VIEWPORT(const D3D11_VIEWPORT & o): D3D11_VIEWPORT(o) {}
    explicit CD3D11_VIEWPORT(FLOAT topLeftX, FLOAT topLeftY, FLOAT width, FLOAT height, FLOAT minDepth = D3D11_MIN_DEPTH, FLOAT maxDepth = D3D11_MAX_DEPTH) {
        TopLeftX = topLeftX;
        TopLeftY = topLeftY;
        Width    = width;
        Height   = height;
        MinDepth = minDepth;
        MaxDepth = maxDepth;
    }
    explicit CD3D11_VIEWPORT(_In_ ID3D11Buffer *, _In_ ID3D11RenderTargetView * pRTView, FLOAT topLeftX = 0.0f, FLOAT minDepth = D3D11_MIN_DEPTH,
                             FLOAT maxDepth = D3D11_MAX_DEPTH) {
        D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
        pRTView->GetDesc(&RTVDesc);
        UINT NumElements = 0;
        switch (RTVDesc.ViewDimension) {
        case D3D11_RTV_DIMENSION_BUFFER:
            NumElements = RTVDesc.Buffer.NumElements;
            break;
        default:
            break;
        }
        TopLeftX = topLeftX;
        TopLeftY = 0.0f;
        Width    = NumElements - topLeftX;
        Height   = 1.0f;
        MinDepth = minDepth;
        MaxDepth = maxDepth;
    }
    explicit CD3D11_VIEWPORT(_In_ ID3D11Texture1D * pTex1D, _In_ ID3D11RenderTargetView * pRTView, FLOAT topLeftX = 0.0f, FLOAT minDepth = D3D11_MIN_DEPTH,
                             FLOAT maxDepth = D3D11_MAX_DEPTH) {
        D3D11_TEXTURE1D_DESC TexDesc;
        pTex1D->GetDesc(&TexDesc);
        D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
        pRTView->GetDesc(&RTVDesc);
        UINT MipSlice = 0;
        switch (RTVDesc.ViewDimension) {
        case D3D11_RTV_DIMENSION_TEXTURE1D:
            MipSlice = RTVDesc.Texture1D.MipSlice;
            break;
        case D3D11_RTV_DIMENSION_TEXTURE1DARRAY:
            MipSlice = RTVDesc.Texture1DArray.MipSlice;
            break;
        default:
            break;
        }
        const UINT SubResourceWidth = TexDesc.Width / (UINT(1) << MipSlice);
        TopLeftX                    = topLeftX;
        TopLeftY                    = 0.0f;
        Width                       = (SubResourceWidth ? SubResourceWidth : 1) - topLeftX;
        Height                      = 1.0f;
        MinDepth                    = minDepth;
        MaxDepth                    = maxDepth;
    }
    explicit CD3D11_VIEWPORT(_In_ ID3D11Texture2D * pTex2D, _In_ ID3D11RenderTargetView * pRTView, FLOAT topLeftX = 0.0f, FLOAT topLeftY = 0.0f,
                             FLOAT minDepth = D3D11_MIN_DEPTH, FLOAT maxDepth = D3D11_MAX_DEPTH) {
        D3D11_TEXTURE2D_DESC TexDesc;
        pTex2D->GetDesc(&TexDesc);
        D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
        pRTView->GetDesc(&RTVDesc);
        UINT MipSlice = 0;
        switch (RTVDesc.ViewDimension) {
        case D3D11_RTV_DIMENSION_TEXTURE2D:
            MipSlice = RTVDesc.Texture2D.MipSlice;
            break;
        case D3D11_RTV_DIMENSION_TEXTURE2DARRAY:
            MipSlice = RTVDesc.Texture2DArray.MipSlice;
            break;
        case D3D11_RTV_DIMENSION_TEXTURE2DMS:
        case D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY:
            break;
        default:
            break;
        }
        const UINT SubResourceWidth  = TexDesc.Width / (UINT(1) << MipSlice);
        const UINT SubResourceHeight = TexDesc.Height / (UINT(1) << MipSlice);
        TopLeftX                     = topLeftX;
        TopLeftY                     = topLeftY;
        Width                        = (SubResourceWidth ? SubResourceWidth : 1) - topLeftX;
        Height                       = (SubResourceHeight ? SubResourceHeight : 1) - topLeftY;
        MinDepth                     = minDepth;
        MaxDepth                     = maxDepth;
    }
    explicit CD3D11_VIEWPORT(_In_ ID3D11Texture3D * pTex3D, _In_ ID3D11RenderTargetView * pRTView, FLOAT topLeftX = 0.0f, FLOAT topLeftY = 0.0f,
                             FLOAT minDepth = D3D11_MIN_DEPTH, FLOAT maxDepth = D3D11_MAX_DEPTH) {
        D3D11_TEXTURE3D_DESC TexDesc;
        pTex3D->GetDesc(&TexDesc);
        D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
        pRTView->GetDesc(&RTVDesc);
        UINT MipSlice = 0;
        switch (RTVDesc.ViewDimension) {
        case D3D11_RTV_DIMENSION_TEXTURE3D:
            MipSlice = RTVDesc.Texture3D.MipSlice;
            break;
        default:
            break;
        }
        const UINT SubResourceWidth  = TexDesc.Width / (UINT(1) << MipSlice);
        const UINT SubResourceHeight = TexDesc.Height / (UINT(1) << MipSlice);
        TopLeftX                     = topLeftX;
        TopLeftY                     = topLeftY;
        Width                        = (SubResourceWidth ? SubResourceWidth : 1) - topLeftX;
        Height                       = (SubResourceHeight ? SubResourceHeight : 1) - topLeftY;
        MinDepth                     = minDepth;
        MaxDepth                     = maxDepth;
    }
    ~CD3D11_VIEWPORT() {}
    operator const D3D11_VIEWPORT &() const { return *this; }
};
extern "C" {
    #endif
typedef struct D3D11_TEX1D_DSV {
    UINT MipSlice;
} D3D11_TEX1D_DSV;

typedef struct D3D11_TEX1D_ARRAY_DSV {
    UINT MipSlice;
    UINT FirstArraySlice;
    UINT ArraySize;
} D3D11_TEX1D_ARRAY_DSV;

typedef struct D3D11_TEX2D_DSV {
    UINT MipSlice;
} D3D11_TEX2D_DSV;

typedef struct D3D11_TEX2D_ARRAY_DSV {
    UINT MipSlice;
    UINT FirstArraySlice;
    UINT ArraySize;
} D3D11_TEX2D_ARRAY_DSV;

typedef struct D3D11_TEX2DMS_DSV {
    UINT UnusedField_NothingToDefine;
} D3D11_TEX2DMS_DSV;

typedef struct D3D11_TEX2DMS_ARRAY_DSV {
    UINT FirstArraySlice;
    UINT ArraySize;
} D3D11_TEX2DMS_ARRAY_DSV;

typedef enum D3D11_DSV_FLAG { D3D11_DSV_READ_ONLY_DEPTH = 0x1L, D3D11_DSV_READ_ONLY_STENCIL = 0x2L } D3D11_DSV_FLAG;

typedef struct D3D11_DEPTH_STENCIL_VIEW_DESC {
    DXGI_FORMAT         Format;
    D3D11_DSV_DIMENSION ViewDimension;
    UINT                Flags;
    union {
        D3D11_TEX1D_DSV         Texture1D;
        D3D11_TEX1D_ARRAY_DSV   Texture1DArray;
        D3D11_TEX2D_DSV         Texture2D;
        D3D11_TEX2D_ARRAY_DSV   Texture2DArray;
        D3D11_TEX2DMS_DSV       Texture2DMS;
        D3D11_TEX2DMS_ARRAY_DSV Texture2DMSArray;
    };
} D3D11_DEPTH_STENCIL_VIEW_DESC;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_DEPTH_STENCIL_VIEW_DESC : public D3D11_DEPTH_STENCIL_VIEW_DESC {
    CD3D11_DEPTH_STENCIL_VIEW_DESC() {}
    explicit CD3D11_DEPTH_STENCIL_VIEW_DESC(const D3D11_DEPTH_STENCIL_VIEW_DESC & o): D3D11_DEPTH_STENCIL_VIEW_DESC(o) {}
    explicit CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION viewDimension, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, UINT mipSlice = 0,
                                            UINT firstArraySlice = 0, UINT arraySize = -1, UINT flags = 0) {
        Format        = format;
        ViewDimension = viewDimension;
        Flags         = flags;
        switch (viewDimension) {
        case D3D11_DSV_DIMENSION_TEXTURE1D:
            Texture1D.MipSlice = mipSlice;
            break;
        case D3D11_DSV_DIMENSION_TEXTURE1DARRAY:
            Texture1DArray.MipSlice        = mipSlice;
            Texture1DArray.FirstArraySlice = firstArraySlice;
            Texture1DArray.ArraySize       = arraySize;
            break;
        case D3D11_DSV_DIMENSION_TEXTURE2D:
            Texture2D.MipSlice = mipSlice;
            break;
        case D3D11_DSV_DIMENSION_TEXTURE2DARRAY:
            Texture2DArray.MipSlice        = mipSlice;
            Texture2DArray.FirstArraySlice = firstArraySlice;
            Texture2DArray.ArraySize       = arraySize;
            break;
        case D3D11_DSV_DIMENSION_TEXTURE2DMS:
            break;
        case D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY:
            Texture2DMSArray.FirstArraySlice = firstArraySlice;
            Texture2DMSArray.ArraySize       = arraySize;
            break;
        default:
            break;
        }
    }
    explicit CD3D11_DEPTH_STENCIL_VIEW_DESC(_In_ ID3D11Texture1D * pTex1D, D3D11_DSV_DIMENSION viewDimension, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN,
                                            UINT mipSlice = 0, UINT firstArraySlice = 0, UINT arraySize = -1, UINT flags = 0) {
        ViewDimension = viewDimension;
        Flags         = flags;
        if (DXGI_FORMAT_UNKNOWN == format || (-1 == arraySize && D3D11_DSV_DIMENSION_TEXTURE1DARRAY == viewDimension)) {
            D3D11_TEXTURE1D_DESC TexDesc;
            pTex1D->GetDesc(&TexDesc);
            if (DXGI_FORMAT_UNKNOWN == format) format = TexDesc.Format;
            if (-1 == arraySize) arraySize = TexDesc.ArraySize - firstArraySlice;
        }
        Format = format;
        switch (viewDimension) {
        case D3D11_DSV_DIMENSION_TEXTURE1D:
            Texture1D.MipSlice = mipSlice;
            break;
        case D3D11_DSV_DIMENSION_TEXTURE1DARRAY:
            Texture1DArray.MipSlice        = mipSlice;
            Texture1DArray.FirstArraySlice = firstArraySlice;
            Texture1DArray.ArraySize       = arraySize;
            break;
        default:
            break;
        }
    }
    explicit CD3D11_DEPTH_STENCIL_VIEW_DESC(_In_ ID3D11Texture2D * pTex2D, D3D11_DSV_DIMENSION viewDimension, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN,
                                            UINT mipSlice = 0, UINT firstArraySlice = 0, UINT arraySize = -1, UINT flags = 0) {
        ViewDimension = viewDimension;
        Flags         = flags;
        if (DXGI_FORMAT_UNKNOWN == format ||
            (-1 == arraySize && (D3D11_DSV_DIMENSION_TEXTURE2DARRAY == viewDimension || D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY == viewDimension))) {
            D3D11_TEXTURE2D_DESC TexDesc;
            pTex2D->GetDesc(&TexDesc);
            if (DXGI_FORMAT_UNKNOWN == format) format = TexDesc.Format;
            if (-1 == arraySize) arraySize = TexDesc.ArraySize - firstArraySlice;
        }
        Format = format;
        switch (viewDimension) {
        case D3D11_DSV_DIMENSION_TEXTURE2D:
            Texture2D.MipSlice = mipSlice;
            break;
        case D3D11_DSV_DIMENSION_TEXTURE2DARRAY:
            Texture2DArray.MipSlice        = mipSlice;
            Texture2DArray.FirstArraySlice = firstArraySlice;
            Texture2DArray.ArraySize       = arraySize;
            break;
        case D3D11_DSV_DIMENSION_TEXTURE2DMS:
            break;
        case D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY:
            Texture2DMSArray.FirstArraySlice = firstArraySlice;
            Texture2DMSArray.ArraySize       = arraySize;
            break;
        default:
            break;
        }
    }
    ~CD3D11_DEPTH_STENCIL_VIEW_DESC() {}
    operator const D3D11_DEPTH_STENCIL_VIEW_DESC &() const { return *this; }
};
extern "C" {
    #endif

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0012_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0012_v0_0_s_ifspec;

    #ifndef __ID3D11DepthStencilView_INTERFACE_DEFINED__
        #define __ID3D11DepthStencilView_INTERFACE_DEFINED__

/* interface ID3D11DepthStencilView */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11DepthStencilView;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("9fdac92a-1876-48c3-afad-25b94f84a9b6")
ID3D11DepthStencilView : public ID3D11View {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11DepthStencilViewVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11DepthStencilView * This,
     /* [in] */ REFIID        riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11DepthStencilView * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11DepthStencilView * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11DepthStencilView * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11DepthStencilView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11DepthStencilView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11DepthStencilView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetResource)(ID3D11DepthStencilView * This,
                                          /* [annotation] */
                                          _Out_ ID3D11Resource ** ppResource);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11DepthStencilView * This,
                                      /* [annotation] */
                                      _Out_ D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc);

    END_INTERFACE
} ID3D11DepthStencilViewVtbl;

interface ID3D11DepthStencilView {
    CONST_VTBL struct ID3D11DepthStencilViewVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11DepthStencilView_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11DepthStencilView_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11DepthStencilView_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11DepthStencilView_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11DepthStencilView_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11DepthStencilView_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11DepthStencilView_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11DepthStencilView_GetResource(This, ppResource) ((This)->lpVtbl->GetResource(This, ppResource))

                #define ID3D11DepthStencilView_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11DepthStencilView_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0013 */
/* [local] */

typedef enum D3D11_BUFFER_UAV_FLAG {
    D3D11_BUFFER_UAV_FLAG_RAW     = 0x1,
    D3D11_BUFFER_UAV_FLAG_APPEND  = 0x2,
    D3D11_BUFFER_UAV_FLAG_COUNTER = 0x4
} D3D11_BUFFER_UAV_FLAG;

typedef struct D3D11_BUFFER_UAV {
    UINT FirstElement;
    UINT NumElements;
    UINT Flags;
} D3D11_BUFFER_UAV;

typedef struct D3D11_TEX1D_UAV {
    UINT MipSlice;
} D3D11_TEX1D_UAV;

typedef struct D3D11_TEX1D_ARRAY_UAV {
    UINT MipSlice;
    UINT FirstArraySlice;
    UINT ArraySize;
} D3D11_TEX1D_ARRAY_UAV;

typedef struct D3D11_TEX2D_UAV {
    UINT MipSlice;
} D3D11_TEX2D_UAV;

typedef struct D3D11_TEX2D_ARRAY_UAV {
    UINT MipSlice;
    UINT FirstArraySlice;
    UINT ArraySize;
} D3D11_TEX2D_ARRAY_UAV;

typedef struct D3D11_TEX3D_UAV {
    UINT MipSlice;
    UINT FirstWSlice;
    UINT WSize;
} D3D11_TEX3D_UAV;

typedef struct D3D11_UNORDERED_ACCESS_VIEW_DESC {
    DXGI_FORMAT         Format;
    D3D11_UAV_DIMENSION ViewDimension;
    union {
        D3D11_BUFFER_UAV      Buffer;
        D3D11_TEX1D_UAV       Texture1D;
        D3D11_TEX1D_ARRAY_UAV Texture1DArray;
        D3D11_TEX2D_UAV       Texture2D;
        D3D11_TEX2D_ARRAY_UAV Texture2DArray;
        D3D11_TEX3D_UAV       Texture3D;
    };
} D3D11_UNORDERED_ACCESS_VIEW_DESC;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_UNORDERED_ACCESS_VIEW_DESC : public D3D11_UNORDERED_ACCESS_VIEW_DESC {
    CD3D11_UNORDERED_ACCESS_VIEW_DESC() {}
    explicit CD3D11_UNORDERED_ACCESS_VIEW_DESC(const D3D11_UNORDERED_ACCESS_VIEW_DESC & o): D3D11_UNORDERED_ACCESS_VIEW_DESC(o) {}
    explicit CD3D11_UNORDERED_ACCESS_VIEW_DESC(D3D11_UAV_DIMENSION viewDimension, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN,
                                               UINT mipSlice        = 0,  // FirstElement for BUFFER
                                               UINT firstArraySlice = 0,  // NumElements for BUFFER, FirstWSlice for TEXTURE3D
                                               UINT arraySize       = -1, // WSize for TEXTURE3D
                                               UINT flags           = 0)            // BUFFER only
    {
        Format        = format;
        ViewDimension = viewDimension;
        switch (viewDimension) {
        case D3D11_UAV_DIMENSION_BUFFER:
            Buffer.FirstElement = mipSlice;
            Buffer.NumElements  = firstArraySlice;
            Buffer.Flags        = flags;
            break;
        case D3D11_UAV_DIMENSION_TEXTURE1D:
            Texture1D.MipSlice = mipSlice;
            break;
        case D3D11_UAV_DIMENSION_TEXTURE1DARRAY:
            Texture1DArray.MipSlice        = mipSlice;
            Texture1DArray.FirstArraySlice = firstArraySlice;
            Texture1DArray.ArraySize       = arraySize;
            break;
        case D3D11_UAV_DIMENSION_TEXTURE2D:
            Texture2D.MipSlice = mipSlice;
            break;
        case D3D11_UAV_DIMENSION_TEXTURE2DARRAY:
            Texture2DArray.MipSlice        = mipSlice;
            Texture2DArray.FirstArraySlice = firstArraySlice;
            Texture2DArray.ArraySize       = arraySize;
            break;
        case D3D11_UAV_DIMENSION_TEXTURE3D:
            Texture3D.MipSlice    = mipSlice;
            Texture3D.FirstWSlice = firstArraySlice;
            Texture3D.WSize       = arraySize;
            break;
        default:
            break;
        }
    }
    explicit CD3D11_UNORDERED_ACCESS_VIEW_DESC(_In_ ID3D11Buffer *, DXGI_FORMAT format, UINT firstElement, UINT numElements, UINT flags = 0) {
        Format              = format;
        ViewDimension       = D3D11_UAV_DIMENSION_BUFFER;
        Buffer.FirstElement = firstElement;
        Buffer.NumElements  = numElements;
        Buffer.Flags        = flags;
    }
    explicit CD3D11_UNORDERED_ACCESS_VIEW_DESC(_In_ ID3D11Texture1D * pTex1D, D3D11_UAV_DIMENSION viewDimension, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN,
                                               UINT mipSlice = 0, UINT firstArraySlice = 0, UINT arraySize = -1) {
        ViewDimension = viewDimension;
        if (DXGI_FORMAT_UNKNOWN == format || (-1 == arraySize && D3D11_UAV_DIMENSION_TEXTURE1DARRAY == viewDimension)) {
            D3D11_TEXTURE1D_DESC TexDesc;
            pTex1D->GetDesc(&TexDesc);
            if (DXGI_FORMAT_UNKNOWN == format) format = TexDesc.Format;
            if (-1 == arraySize) arraySize = TexDesc.ArraySize - firstArraySlice;
        }
        Format = format;
        switch (viewDimension) {
        case D3D11_UAV_DIMENSION_TEXTURE1D:
            Texture1D.MipSlice = mipSlice;
            break;
        case D3D11_UAV_DIMENSION_TEXTURE1DARRAY:
            Texture1DArray.MipSlice        = mipSlice;
            Texture1DArray.FirstArraySlice = firstArraySlice;
            Texture1DArray.ArraySize       = arraySize;
            break;
        default:
            break;
        }
    }
    explicit CD3D11_UNORDERED_ACCESS_VIEW_DESC(_In_ ID3D11Texture2D * pTex2D, D3D11_UAV_DIMENSION viewDimension, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN,
                                               UINT mipSlice = 0, UINT firstArraySlice = 0, UINT arraySize = -1) {
        ViewDimension = viewDimension;
        if (DXGI_FORMAT_UNKNOWN == format || (-1 == arraySize && D3D11_UAV_DIMENSION_TEXTURE2DARRAY == viewDimension)) {
            D3D11_TEXTURE2D_DESC TexDesc;
            pTex2D->GetDesc(&TexDesc);
            if (DXGI_FORMAT_UNKNOWN == format) format = TexDesc.Format;
            if (-1 == arraySize) arraySize = TexDesc.ArraySize - firstArraySlice;
        }
        Format = format;
        switch (viewDimension) {
        case D3D11_UAV_DIMENSION_TEXTURE2D:
            Texture2D.MipSlice = mipSlice;
            break;
        case D3D11_UAV_DIMENSION_TEXTURE2DARRAY:
            Texture2DArray.MipSlice        = mipSlice;
            Texture2DArray.FirstArraySlice = firstArraySlice;
            Texture2DArray.ArraySize       = arraySize;
            break;
        default:
            break;
        }
    }
    explicit CD3D11_UNORDERED_ACCESS_VIEW_DESC(_In_ ID3D11Texture3D * pTex3D, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, UINT mipSlice = 0, UINT firstWSlice = 0,
                                               UINT wSize = -1) {
        ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
        if (DXGI_FORMAT_UNKNOWN == format || -1 == wSize) {
            D3D11_TEXTURE3D_DESC TexDesc;
            pTex3D->GetDesc(&TexDesc);
            if (DXGI_FORMAT_UNKNOWN == format) format = TexDesc.Format;
            if (-1 == wSize) wSize = TexDesc.Depth - firstWSlice;
        }
        Format                = format;
        Texture3D.MipSlice    = mipSlice;
        Texture3D.FirstWSlice = firstWSlice;
        Texture3D.WSize       = wSize;
    }
    ~CD3D11_UNORDERED_ACCESS_VIEW_DESC() {}
    operator const D3D11_UNORDERED_ACCESS_VIEW_DESC &() const { return *this; }
};
extern "C" {
    #endif

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0013_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0013_v0_0_s_ifspec;

    #ifndef __ID3D11UnorderedAccessView_INTERFACE_DEFINED__
        #define __ID3D11UnorderedAccessView_INTERFACE_DEFINED__

/* interface ID3D11UnorderedAccessView */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11UnorderedAccessView;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("28acf509-7f5c-48f6-8611-f316010a6380")
ID3D11UnorderedAccessView : public ID3D11View {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11UnorderedAccessViewVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11UnorderedAccessView * This,
     /* [in] */ REFIID           riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11UnorderedAccessView * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11UnorderedAccessView * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11UnorderedAccessView * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11UnorderedAccessView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11UnorderedAccessView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11UnorderedAccessView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetResource)(ID3D11UnorderedAccessView * This,
                                          /* [annotation] */
                                          _Out_ ID3D11Resource ** ppResource);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11UnorderedAccessView * This,
                                      /* [annotation] */
                                      _Out_ D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc);

    END_INTERFACE
} ID3D11UnorderedAccessViewVtbl;

interface ID3D11UnorderedAccessView {
    CONST_VTBL struct ID3D11UnorderedAccessViewVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11UnorderedAccessView_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11UnorderedAccessView_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11UnorderedAccessView_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11UnorderedAccessView_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11UnorderedAccessView_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11UnorderedAccessView_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11UnorderedAccessView_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11UnorderedAccessView_GetResource(This, ppResource) ((This)->lpVtbl->GetResource(This, ppResource))

                #define ID3D11UnorderedAccessView_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11UnorderedAccessView_INTERFACE_DEFINED__ */

    #ifndef __ID3D11VertexShader_INTERFACE_DEFINED__
        #define __ID3D11VertexShader_INTERFACE_DEFINED__

/* interface ID3D11VertexShader */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11VertexShader;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("3b301d64-d678-4289-8897-22f8928b72f3")
ID3D11VertexShader : public ID3D11DeviceChild {public : };

        #else /* C style interface */

typedef struct ID3D11VertexShaderVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11VertexShader * This,
     /* [in] */ REFIID    riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11VertexShader * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11VertexShader * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11VertexShader * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11VertexShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11VertexShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11VertexShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    END_INTERFACE
} ID3D11VertexShaderVtbl;

interface ID3D11VertexShader {
    CONST_VTBL struct ID3D11VertexShaderVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11VertexShader_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11VertexShader_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11VertexShader_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11VertexShader_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11VertexShader_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11VertexShader_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11VertexShader_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11VertexShader_INTERFACE_DEFINED__ */

    #ifndef __ID3D11HullShader_INTERFACE_DEFINED__
        #define __ID3D11HullShader_INTERFACE_DEFINED__

/* interface ID3D11HullShader */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11HullShader;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("8e5c6061-628a-4c8e-8264-bbe45cb3d5dd")
ID3D11HullShader : public ID3D11DeviceChild {public : };

        #else /* C style interface */

typedef struct ID3D11HullShaderVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11HullShader * This,
     /* [in] */ REFIID  riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11HullShader * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11HullShader * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11HullShader * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11HullShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11HullShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11HullShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    END_INTERFACE
} ID3D11HullShaderVtbl;

interface ID3D11HullShader {
    CONST_VTBL struct ID3D11HullShaderVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11HullShader_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11HullShader_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11HullShader_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11HullShader_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11HullShader_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11HullShader_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11HullShader_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11HullShader_INTERFACE_DEFINED__ */

    #ifndef __ID3D11DomainShader_INTERFACE_DEFINED__
        #define __ID3D11DomainShader_INTERFACE_DEFINED__

/* interface ID3D11DomainShader */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11DomainShader;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("f582c508-0f36-490c-9977-31eece268cfa")
ID3D11DomainShader : public ID3D11DeviceChild {public : };

        #else /* C style interface */

typedef struct ID3D11DomainShaderVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11DomainShader * This,
     /* [in] */ REFIID    riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11DomainShader * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11DomainShader * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11DomainShader * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11DomainShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11DomainShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11DomainShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    END_INTERFACE
} ID3D11DomainShaderVtbl;

interface ID3D11DomainShader {
    CONST_VTBL struct ID3D11DomainShaderVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11DomainShader_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11DomainShader_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11DomainShader_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11DomainShader_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11DomainShader_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11DomainShader_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11DomainShader_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11DomainShader_INTERFACE_DEFINED__ */

    #ifndef __ID3D11GeometryShader_INTERFACE_DEFINED__
        #define __ID3D11GeometryShader_INTERFACE_DEFINED__

/* interface ID3D11GeometryShader */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11GeometryShader;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("38325b96-effb-4022-ba02-2e795b70275c")
ID3D11GeometryShader : public ID3D11DeviceChild {public : };

        #else /* C style interface */

typedef struct ID3D11GeometryShaderVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11GeometryShader * This,
     /* [in] */ REFIID      riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11GeometryShader * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11GeometryShader * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11GeometryShader * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11GeometryShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11GeometryShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11GeometryShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    END_INTERFACE
} ID3D11GeometryShaderVtbl;

interface ID3D11GeometryShader {
    CONST_VTBL struct ID3D11GeometryShaderVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11GeometryShader_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11GeometryShader_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11GeometryShader_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11GeometryShader_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11GeometryShader_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11GeometryShader_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11GeometryShader_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11GeometryShader_INTERFACE_DEFINED__ */

    #ifndef __ID3D11PixelShader_INTERFACE_DEFINED__
        #define __ID3D11PixelShader_INTERFACE_DEFINED__

/* interface ID3D11PixelShader */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11PixelShader;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("ea82e40d-51dc-4f33-93d4-db7c9125ae8c")
ID3D11PixelShader : public ID3D11DeviceChild {public : };

        #else /* C style interface */

typedef struct ID3D11PixelShaderVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11PixelShader * This,
     /* [in] */ REFIID   riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11PixelShader * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11PixelShader * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11PixelShader * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11PixelShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11PixelShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11PixelShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    END_INTERFACE
} ID3D11PixelShaderVtbl;

interface ID3D11PixelShader {
    CONST_VTBL struct ID3D11PixelShaderVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11PixelShader_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11PixelShader_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11PixelShader_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11PixelShader_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11PixelShader_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11PixelShader_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11PixelShader_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11PixelShader_INTERFACE_DEFINED__ */

    #ifndef __ID3D11ComputeShader_INTERFACE_DEFINED__
        #define __ID3D11ComputeShader_INTERFACE_DEFINED__

/* interface ID3D11ComputeShader */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11ComputeShader;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("4f5b196e-c2bd-495e-bd01-1fded38e4969")
ID3D11ComputeShader : public ID3D11DeviceChild {public : };

        #else /* C style interface */

typedef struct ID3D11ComputeShaderVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11ComputeShader * This,
     /* [in] */ REFIID     riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11ComputeShader * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11ComputeShader * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11ComputeShader * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11ComputeShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11ComputeShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11ComputeShader * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    END_INTERFACE
} ID3D11ComputeShaderVtbl;

interface ID3D11ComputeShader {
    CONST_VTBL struct ID3D11ComputeShaderVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11ComputeShader_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11ComputeShader_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11ComputeShader_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11ComputeShader_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11ComputeShader_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11ComputeShader_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11ComputeShader_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11ComputeShader_INTERFACE_DEFINED__ */

    #ifndef __ID3D11InputLayout_INTERFACE_DEFINED__
        #define __ID3D11InputLayout_INTERFACE_DEFINED__

/* interface ID3D11InputLayout */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11InputLayout;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("e4819ddc-4cf0-4025-bd26-5de82a3e07b7")
ID3D11InputLayout : public ID3D11DeviceChild {public : };

        #else /* C style interface */

typedef struct ID3D11InputLayoutVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11InputLayout * This,
     /* [in] */ REFIID   riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11InputLayout * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11InputLayout * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11InputLayout * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11InputLayout * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11InputLayout * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11InputLayout * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    END_INTERFACE
} ID3D11InputLayoutVtbl;

interface ID3D11InputLayout {
    CONST_VTBL struct ID3D11InputLayoutVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11InputLayout_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11InputLayout_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11InputLayout_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11InputLayout_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11InputLayout_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11InputLayout_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11InputLayout_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11InputLayout_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0021 */
/* [local] */

typedef enum D3D11_FILTER {
    D3D11_FILTER_MIN_MAG_MIP_POINT                          = 0,
    D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR                   = 0x1,
    D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT             = 0x4,
    D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR                   = 0x5,
    D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT                   = 0x10,
    D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR            = 0x11,
    D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT                   = 0x14,
    D3D11_FILTER_MIN_MAG_MIP_LINEAR                         = 0x15,
    D3D11_FILTER_ANISOTROPIC                                = 0x55,
    D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT               = 0x80,
    D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR        = 0x81,
    D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT  = 0x84,
    D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR        = 0x85,
    D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT        = 0x90,
    D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
    D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT        = 0x94,
    D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR              = 0x95,
    D3D11_FILTER_COMPARISON_ANISOTROPIC                     = 0xd5
} D3D11_FILTER;

typedef enum D3D11_FILTER_TYPE { D3D11_FILTER_TYPE_POINT = 0, D3D11_FILTER_TYPE_LINEAR = 1 } D3D11_FILTER_TYPE;

    #define D3D11_FILTER_TYPE_MASK (0x3)

    #define D3D11_MIN_FILTER_SHIFT (4)

    #define D3D11_MAG_FILTER_SHIFT (2)

    #define D3D11_MIP_FILTER_SHIFT (0)

    #define D3D11_COMPARISON_FILTERING_BIT (0x80)

    #define D3D11_ANISOTROPIC_FILTERING_BIT (0x40)

    #define D3D11_ENCODE_BASIC_FILTER(min, mag, mip, bComparison)                                                                             \
        ((D3D11_FILTER) (((bComparison) ? D3D11_COMPARISON_FILTERING_BIT : 0) | (((min) &D3D11_FILTER_TYPE_MASK) << D3D11_MIN_FILTER_SHIFT) | \
                         (((mag) &D3D11_FILTER_TYPE_MASK) << D3D11_MAG_FILTER_SHIFT) | (((mip) &D3D11_FILTER_TYPE_MASK) << D3D11_MIP_FILTER_SHIFT)))
    #define D3D11_ENCODE_ANISOTROPIC_FILTER(bComparison)   \
        ((D3D11_FILTER) (D3D11_ANISOTROPIC_FILTERING_BIT | \
                         D3D11_ENCODE_BASIC_FILTER(D3D11_FILTER_TYPE_LINEAR, D3D11_FILTER_TYPE_LINEAR, D3D11_FILTER_TYPE_LINEAR, bComparison)))
    #define D3D11_DECODE_MIN_FILTER(d3d11Filter)           ((D3D11_FILTER_TYPE) (((d3d11Filter) >> D3D11_MIN_FILTER_SHIFT) & D3D11_FILTER_TYPE_MASK))
    #define D3D11_DECODE_MAG_FILTER(d3d11Filter)           ((D3D11_FILTER_TYPE) (((d3d11Filter) >> D3D11_MAG_FILTER_SHIFT) & D3D11_FILTER_TYPE_MASK))
    #define D3D11_DECODE_MIP_FILTER(d3d11Filter)           ((D3D11_FILTER_TYPE) (((d3d11Filter) >> D3D11_MIP_FILTER_SHIFT) & D3D11_FILTER_TYPE_MASK))
    #define D3D11_DECODE_IS_COMPARISON_FILTER(d3d11Filter) ((d3d11Filter) &D3D11_COMPARISON_FILTERING_BIT)
    #define D3D11_DECODE_IS_ANISOTROPIC_FILTER(d3d11Filter)                                                                        \
        (((d3d11Filter) &D3D11_ANISOTROPIC_FILTERING_BIT) && (D3D11_FILTER_TYPE_LINEAR == D3D11_DECODE_MIN_FILTER(d3d11Filter)) && \
         (D3D11_FILTER_TYPE_LINEAR == D3D11_DECODE_MAG_FILTER(d3d11Filter)) && (D3D11_FILTER_TYPE_LINEAR == D3D11_DECODE_MIP_FILTER(d3d11Filter)))
typedef enum D3D11_TEXTURE_ADDRESS_MODE {
    D3D11_TEXTURE_ADDRESS_WRAP        = 1,
    D3D11_TEXTURE_ADDRESS_MIRROR      = 2,
    D3D11_TEXTURE_ADDRESS_CLAMP       = 3,
    D3D11_TEXTURE_ADDRESS_BORDER      = 4,
    D3D11_TEXTURE_ADDRESS_MIRROR_ONCE = 5
} D3D11_TEXTURE_ADDRESS_MODE;

typedef struct D3D11_SAMPLER_DESC {
    D3D11_FILTER               Filter;
    D3D11_TEXTURE_ADDRESS_MODE AddressU;
    D3D11_TEXTURE_ADDRESS_MODE AddressV;
    D3D11_TEXTURE_ADDRESS_MODE AddressW;
    FLOAT                      MipLODBias;
    UINT                       MaxAnisotropy;
    D3D11_COMPARISON_FUNC      ComparisonFunc;
    FLOAT                      BorderColor[4];
    FLOAT                      MinLOD;
    FLOAT                      MaxLOD;
} D3D11_SAMPLER_DESC;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_SAMPLER_DESC : public D3D11_SAMPLER_DESC {
    CD3D11_SAMPLER_DESC() {}
    explicit CD3D11_SAMPLER_DESC(const D3D11_SAMPLER_DESC & o): D3D11_SAMPLER_DESC(o) {}
    explicit CD3D11_SAMPLER_DESC(CD3D11_DEFAULT) {
        Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        AddressU       = D3D11_TEXTURE_ADDRESS_CLAMP;
        AddressV       = D3D11_TEXTURE_ADDRESS_CLAMP;
        AddressW       = D3D11_TEXTURE_ADDRESS_CLAMP;
        MipLODBias     = 0;
        MaxAnisotropy  = 1;
        ComparisonFunc = D3D11_COMPARISON_NEVER;
        BorderColor[0] = 1.0f;
        BorderColor[1] = 1.0f;
        BorderColor[2] = 1.0f;
        BorderColor[3] = 1.0f;
        MinLOD         = -3.402823466e+38F; // -FLT_MAX
        MaxLOD         = 3.402823466e+38F;  // FLT_MAX
    }
    explicit CD3D11_SAMPLER_DESC(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressU, D3D11_TEXTURE_ADDRESS_MODE addressV,
                                 D3D11_TEXTURE_ADDRESS_MODE addressW, FLOAT mipLODBias, UINT maxAnisotropy, D3D11_COMPARISON_FUNC comparisonFunc,
                                 _In_reads_opt_(4) const FLOAT * borderColor, // RGBA
                                 FLOAT minLOD, FLOAT maxLOD) {
        Filter                      = filter;
        AddressU                    = addressU;
        AddressV                    = addressV;
        AddressW                    = addressW;
        MipLODBias                  = mipLODBias;
        MaxAnisotropy               = maxAnisotropy;
        ComparisonFunc              = comparisonFunc;
        const float defaultColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        if (!borderColor) borderColor = defaultColor;
        BorderColor[0] = borderColor[0];
        BorderColor[1] = borderColor[1];
        BorderColor[2] = borderColor[2];
        BorderColor[3] = borderColor[3];
        MinLOD         = minLOD;
        MaxLOD         = maxLOD;
    }
    ~CD3D11_SAMPLER_DESC() {}
    operator const D3D11_SAMPLER_DESC &() const { return *this; }
};
extern "C" {
    #endif

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0021_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0021_v0_0_s_ifspec;

    #ifndef __ID3D11SamplerState_INTERFACE_DEFINED__
        #define __ID3D11SamplerState_INTERFACE_DEFINED__

/* interface ID3D11SamplerState */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11SamplerState;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("da6fea51-564c-4487-9810-f0d0f9b4e3a5")
ID3D11SamplerState : public ID3D11DeviceChild {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_SAMPLER_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11SamplerStateVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11SamplerState * This,
     /* [in] */ REFIID    riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11SamplerState * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11SamplerState * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11SamplerState * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11SamplerState * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11SamplerState * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11SamplerState * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11SamplerState * This,
                                      /* [annotation] */
                                      _Out_ D3D11_SAMPLER_DESC * pDesc);

    END_INTERFACE
} ID3D11SamplerStateVtbl;

interface ID3D11SamplerState {
    CONST_VTBL struct ID3D11SamplerStateVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11SamplerState_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11SamplerState_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11SamplerState_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11SamplerState_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11SamplerState_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11SamplerState_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11SamplerState_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11SamplerState_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11SamplerState_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0022 */
/* [local] */

typedef enum D3D11_FORMAT_SUPPORT {
    D3D11_FORMAT_SUPPORT_BUFFER                      = 0x1,
    D3D11_FORMAT_SUPPORT_IA_VERTEX_BUFFER            = 0x2,
    D3D11_FORMAT_SUPPORT_IA_INDEX_BUFFER             = 0x4,
    D3D11_FORMAT_SUPPORT_SO_BUFFER                   = 0x8,
    D3D11_FORMAT_SUPPORT_TEXTURE1D                   = 0x10,
    D3D11_FORMAT_SUPPORT_TEXTURE2D                   = 0x20,
    D3D11_FORMAT_SUPPORT_TEXTURE3D                   = 0x40,
    D3D11_FORMAT_SUPPORT_TEXTURECUBE                 = 0x80,
    D3D11_FORMAT_SUPPORT_SHADER_LOAD                 = 0x100,
    D3D11_FORMAT_SUPPORT_SHADER_SAMPLE               = 0x200,
    D3D11_FORMAT_SUPPORT_SHADER_SAMPLE_COMPARISON    = 0x400,
    D3D11_FORMAT_SUPPORT_SHADER_SAMPLE_MONO_TEXT     = 0x800,
    D3D11_FORMAT_SUPPORT_MIP                         = 0x1000,
    D3D11_FORMAT_SUPPORT_MIP_AUTOGEN                 = 0x2000,
    D3D11_FORMAT_SUPPORT_RENDER_TARGET               = 0x4000,
    D3D11_FORMAT_SUPPORT_BLENDABLE                   = 0x8000,
    D3D11_FORMAT_SUPPORT_DEPTH_STENCIL               = 0x10000,
    D3D11_FORMAT_SUPPORT_CPU_LOCKABLE                = 0x20000,
    D3D11_FORMAT_SUPPORT_MULTISAMPLE_RESOLVE         = 0x40000,
    D3D11_FORMAT_SUPPORT_DISPLAY                     = 0x80000,
    D3D11_FORMAT_SUPPORT_CAST_WITHIN_BIT_LAYOUT      = 0x100000,
    D3D11_FORMAT_SUPPORT_MULTISAMPLE_RENDERTARGET    = 0x200000,
    D3D11_FORMAT_SUPPORT_MULTISAMPLE_LOAD            = 0x400000,
    D3D11_FORMAT_SUPPORT_SHADER_GATHER               = 0x800000,
    D3D11_FORMAT_SUPPORT_BACK_BUFFER_CAST            = 0x1000000,
    D3D11_FORMAT_SUPPORT_TYPED_UNORDERED_ACCESS_VIEW = 0x2000000,
    D3D11_FORMAT_SUPPORT_SHADER_GATHER_COMPARISON    = 0x4000000,
    D3D11_FORMAT_SUPPORT_DECODER_OUTPUT              = 0x8000000,
    D3D11_FORMAT_SUPPORT_VIDEO_PROCESSOR_OUTPUT      = 0x10000000,
    D3D11_FORMAT_SUPPORT_VIDEO_PROCESSOR_INPUT       = 0x20000000,
    D3D11_FORMAT_SUPPORT_VIDEO_ENCODER               = 0x40000000
} D3D11_FORMAT_SUPPORT;

typedef enum D3D11_FORMAT_SUPPORT2 {
    D3D11_FORMAT_SUPPORT2_UAV_ATOMIC_ADD                               = 0x1,
    D3D11_FORMAT_SUPPORT2_UAV_ATOMIC_BITWISE_OPS                       = 0x2,
    D3D11_FORMAT_SUPPORT2_UAV_ATOMIC_COMPARE_STORE_OR_COMPARE_EXCHANGE = 0x4,
    D3D11_FORMAT_SUPPORT2_UAV_ATOMIC_EXCHANGE                          = 0x8,
    D3D11_FORMAT_SUPPORT2_UAV_ATOMIC_SIGNED_MIN_OR_MAX                 = 0x10,
    D3D11_FORMAT_SUPPORT2_UAV_ATOMIC_UNSIGNED_MIN_OR_MAX               = 0x20,
    D3D11_FORMAT_SUPPORT2_UAV_TYPED_LOAD                               = 0x40,
    D3D11_FORMAT_SUPPORT2_UAV_TYPED_STORE                              = 0x80,
    D3D11_FORMAT_SUPPORT2_OUTPUT_MERGER_LOGIC_OP                       = 0x100
} D3D11_FORMAT_SUPPORT2;

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0022_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0022_v0_0_s_ifspec;

    #ifndef __ID3D11Asynchronous_INTERFACE_DEFINED__
        #define __ID3D11Asynchronous_INTERFACE_DEFINED__

/* interface ID3D11Asynchronous */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11Asynchronous;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("4b35d0cd-1e15-4258-9c98-1b1333f6dd3b")
ID3D11Asynchronous : public ID3D11DeviceChild {
public:
    virtual UINT STDMETHODCALLTYPE GetDataSize(void) = 0;
};

        #else /* C style interface */

typedef struct ID3D11AsynchronousVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11Asynchronous * This,
     /* [in] */ REFIID    riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11Asynchronous * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11Asynchronous * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11Asynchronous * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11Asynchronous * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11Asynchronous * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11Asynchronous * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    UINT(STDMETHODCALLTYPE * GetDataSize)(ID3D11Asynchronous * This);

    END_INTERFACE
} ID3D11AsynchronousVtbl;

interface ID3D11Asynchronous {
    CONST_VTBL struct ID3D11AsynchronousVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11Asynchronous_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11Asynchronous_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11Asynchronous_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11Asynchronous_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11Asynchronous_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11Asynchronous_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11Asynchronous_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11Asynchronous_GetDataSize(This) ((This)->lpVtbl->GetDataSize(This))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11Asynchronous_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0023 */
/* [local] */

typedef enum D3D11_ASYNC_GETDATA_FLAG { D3D11_ASYNC_GETDATA_DONOTFLUSH = 0x1 } D3D11_ASYNC_GETDATA_FLAG;

typedef enum D3D11_QUERY {
    D3D11_QUERY_EVENT                         = 0,
    D3D11_QUERY_OCCLUSION                     = (D3D11_QUERY_EVENT + 1),
    D3D11_QUERY_TIMESTAMP                     = (D3D11_QUERY_OCCLUSION + 1),
    D3D11_QUERY_TIMESTAMP_DISJOINT            = (D3D11_QUERY_TIMESTAMP + 1),
    D3D11_QUERY_PIPELINE_STATISTICS           = (D3D11_QUERY_TIMESTAMP_DISJOINT + 1),
    D3D11_QUERY_OCCLUSION_PREDICATE           = (D3D11_QUERY_PIPELINE_STATISTICS + 1),
    D3D11_QUERY_SO_STATISTICS                 = (D3D11_QUERY_OCCLUSION_PREDICATE + 1),
    D3D11_QUERY_SO_OVERFLOW_PREDICATE         = (D3D11_QUERY_SO_STATISTICS + 1),
    D3D11_QUERY_SO_STATISTICS_STREAM0         = (D3D11_QUERY_SO_OVERFLOW_PREDICATE + 1),
    D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM0 = (D3D11_QUERY_SO_STATISTICS_STREAM0 + 1),
    D3D11_QUERY_SO_STATISTICS_STREAM1         = (D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM0 + 1),
    D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM1 = (D3D11_QUERY_SO_STATISTICS_STREAM1 + 1),
    D3D11_QUERY_SO_STATISTICS_STREAM2         = (D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM1 + 1),
    D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM2 = (D3D11_QUERY_SO_STATISTICS_STREAM2 + 1),
    D3D11_QUERY_SO_STATISTICS_STREAM3         = (D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM2 + 1),
    D3D11_QUERY_SO_OVERFLOW_PREDICATE_STREAM3 = (D3D11_QUERY_SO_STATISTICS_STREAM3 + 1)
} D3D11_QUERY;

typedef enum D3D11_QUERY_MISC_FLAG { D3D11_QUERY_MISC_PREDICATEHINT = 0x1 } D3D11_QUERY_MISC_FLAG;

typedef struct D3D11_QUERY_DESC {
    D3D11_QUERY Query;
    UINT        MiscFlags;
} D3D11_QUERY_DESC;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_QUERY_DESC : public D3D11_QUERY_DESC {
    CD3D11_QUERY_DESC() {}
    explicit CD3D11_QUERY_DESC(const D3D11_QUERY_DESC & o): D3D11_QUERY_DESC(o) {}
    explicit CD3D11_QUERY_DESC(D3D11_QUERY query, UINT miscFlags = 0) {
        Query     = query;
        MiscFlags = miscFlags;
    }
    ~CD3D11_QUERY_DESC() {}
    operator const D3D11_QUERY_DESC &() const { return *this; }
};
extern "C" {
    #endif

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0023_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0023_v0_0_s_ifspec;

    #ifndef __ID3D11Query_INTERFACE_DEFINED__
        #define __ID3D11Query_INTERFACE_DEFINED__

/* interface ID3D11Query */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11Query;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("d6c00747-87b7-425e-b84d-44d108560afd")
ID3D11Query : public ID3D11Asynchronous {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_QUERY_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11QueryVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11Query *     This,
     /* [in] */ REFIID riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11Query * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11Query * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11Query * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11Query * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11Query * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11Query * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    UINT(STDMETHODCALLTYPE * GetDataSize)(ID3D11Query * This);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11Query * This,
                                      /* [annotation] */
                                      _Out_ D3D11_QUERY_DESC * pDesc);

    END_INTERFACE
} ID3D11QueryVtbl;

interface ID3D11Query {
    CONST_VTBL struct ID3D11QueryVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11Query_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11Query_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11Query_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11Query_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11Query_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11Query_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11Query_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11Query_GetDataSize(This) ((This)->lpVtbl->GetDataSize(This))

                #define ID3D11Query_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11Query_INTERFACE_DEFINED__ */

    #ifndef __ID3D11Predicate_INTERFACE_DEFINED__
        #define __ID3D11Predicate_INTERFACE_DEFINED__

/* interface ID3D11Predicate */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11Predicate;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("9eb576dd-9f77-4d86-81aa-8bab5fe490e2")
ID3D11Predicate : public ID3D11Query {public : };

        #else /* C style interface */

typedef struct ID3D11PredicateVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11Predicate * This,
     /* [in] */ REFIID riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11Predicate * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11Predicate * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11Predicate * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11Predicate * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11Predicate * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11Predicate * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    UINT(STDMETHODCALLTYPE * GetDataSize)(ID3D11Predicate * This);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11Predicate * This,
                                      /* [annotation] */
                                      _Out_ D3D11_QUERY_DESC * pDesc);

    END_INTERFACE
} ID3D11PredicateVtbl;

interface ID3D11Predicate {
    CONST_VTBL struct ID3D11PredicateVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11Predicate_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11Predicate_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11Predicate_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11Predicate_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11Predicate_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11Predicate_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11Predicate_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11Predicate_GetDataSize(This) ((This)->lpVtbl->GetDataSize(This))

                #define ID3D11Predicate_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11Predicate_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0025 */
/* [local] */

typedef struct D3D11_QUERY_DATA_TIMESTAMP_DISJOINT {
    UINT64 Frequency;
    BOOL   Disjoint;
} D3D11_QUERY_DATA_TIMESTAMP_DISJOINT;

typedef struct D3D11_QUERY_DATA_PIPELINE_STATISTICS {
    UINT64 IAVertices;
    UINT64 IAPrimitives;
    UINT64 VSInvocations;
    UINT64 GSInvocations;
    UINT64 GSPrimitives;
    UINT64 CInvocations;
    UINT64 CPrimitives;
    UINT64 PSInvocations;
    UINT64 HSInvocations;
    UINT64 DSInvocations;
    UINT64 CSInvocations;
} D3D11_QUERY_DATA_PIPELINE_STATISTICS;

typedef struct D3D11_QUERY_DATA_SO_STATISTICS {
    UINT64 NumPrimitivesWritten;
    UINT64 PrimitivesStorageNeeded;
} D3D11_QUERY_DATA_SO_STATISTICS;

typedef enum D3D11_COUNTER { D3D11_COUNTER_DEVICE_DEPENDENT_0 = 0x40000000 } D3D11_COUNTER;

typedef enum D3D11_COUNTER_TYPE {
    D3D11_COUNTER_TYPE_FLOAT32 = 0,
    D3D11_COUNTER_TYPE_UINT16  = (D3D11_COUNTER_TYPE_FLOAT32 + 1),
    D3D11_COUNTER_TYPE_UINT32  = (D3D11_COUNTER_TYPE_UINT16 + 1),
    D3D11_COUNTER_TYPE_UINT64  = (D3D11_COUNTER_TYPE_UINT32 + 1)
} D3D11_COUNTER_TYPE;

typedef struct D3D11_COUNTER_DESC {
    D3D11_COUNTER Counter;
    UINT          MiscFlags;
} D3D11_COUNTER_DESC;

    #if !defined(D3D11_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_COUNTER_DESC : public D3D11_COUNTER_DESC {
    CD3D11_COUNTER_DESC() {}
    explicit CD3D11_COUNTER_DESC(const D3D11_COUNTER_DESC & o): D3D11_COUNTER_DESC(o) {}
    explicit CD3D11_COUNTER_DESC(D3D11_COUNTER counter, UINT miscFlags = 0) {
        Counter   = counter;
        MiscFlags = miscFlags;
    }
    ~CD3D11_COUNTER_DESC() {}
    operator const D3D11_COUNTER_DESC &() const { return *this; }
};
extern "C" {
    #endif
typedef struct D3D11_COUNTER_INFO {
    D3D11_COUNTER LastDeviceDependentCounter;
    UINT          NumSimultaneousCounters;
    UINT8         NumDetectableParallelUnits;
} D3D11_COUNTER_INFO;

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0025_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0025_v0_0_s_ifspec;

    #ifndef __ID3D11Counter_INTERFACE_DEFINED__
        #define __ID3D11Counter_INTERFACE_DEFINED__

/* interface ID3D11Counter */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11Counter;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("6e8c49fb-a371-4770-b440-29086022b741")
ID3D11Counter : public ID3D11Asynchronous {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_COUNTER_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11CounterVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11Counter *   This,
     /* [in] */ REFIID riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11Counter * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11Counter * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11Counter * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11Counter * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11Counter * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11Counter * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    UINT(STDMETHODCALLTYPE * GetDataSize)(ID3D11Counter * This);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11Counter * This,
                                      /* [annotation] */
                                      _Out_ D3D11_COUNTER_DESC * pDesc);

    END_INTERFACE
} ID3D11CounterVtbl;

interface ID3D11Counter {
    CONST_VTBL struct ID3D11CounterVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11Counter_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11Counter_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11Counter_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11Counter_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11Counter_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11Counter_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11Counter_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11Counter_GetDataSize(This) ((This)->lpVtbl->GetDataSize(This))

                #define ID3D11Counter_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11Counter_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0026 */
/* [local] */

typedef enum D3D11_STANDARD_MULTISAMPLE_QUALITY_LEVELS {
    D3D11_STANDARD_MULTISAMPLE_PATTERN = 0xffffffff,
    D3D11_CENTER_MULTISAMPLE_PATTERN   = 0xfffffffe
} D3D11_STANDARD_MULTISAMPLE_QUALITY_LEVELS;

typedef enum D3D11_DEVICE_CONTEXT_TYPE {
    D3D11_DEVICE_CONTEXT_IMMEDIATE = 0,
    D3D11_DEVICE_CONTEXT_DEFERRED  = (D3D11_DEVICE_CONTEXT_IMMEDIATE + 1)
} D3D11_DEVICE_CONTEXT_TYPE;

typedef struct D3D11_CLASS_INSTANCE_DESC {
    UINT InstanceId;
    UINT InstanceIndex;
    UINT TypeId;
    UINT ConstantBuffer;
    UINT BaseConstantBufferOffset;
    UINT BaseTexture;
    UINT BaseSampler;
    BOOL Created;
} D3D11_CLASS_INSTANCE_DESC;

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0026_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0026_v0_0_s_ifspec;

    #ifndef __ID3D11ClassInstance_INTERFACE_DEFINED__
        #define __ID3D11ClassInstance_INTERFACE_DEFINED__

/* interface ID3D11ClassInstance */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11ClassInstance;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("a6cd7faa-b0b7-4a2f-9436-8662a65797cb")
ID3D11ClassInstance : public ID3D11DeviceChild {
public:
    virtual void STDMETHODCALLTYPE GetClassLinkage(
        /* [annotation] */
        _Out_ ID3D11ClassLinkage * *ppLinkage) = 0;

    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_CLASS_INSTANCE_DESC * pDesc) = 0;

    virtual void STDMETHODCALLTYPE GetInstanceName(
        /* [annotation] */
        _Out_writes_opt_(*pBufferLength) LPSTR pInstanceName,
        /* [annotation] */
        _Inout_ SIZE_T * pBufferLength) = 0;

    virtual void STDMETHODCALLTYPE GetTypeName(
        /* [annotation] */
        _Out_writes_opt_(*pBufferLength) LPSTR pTypeName,
        /* [annotation] */
        _Inout_ SIZE_T * pBufferLength) = 0;
};

        #else /* C style interface */

typedef struct ID3D11ClassInstanceVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11ClassInstance * This,
     /* [in] */ REFIID     riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11ClassInstance * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11ClassInstance * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11ClassInstance * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11ClassInstance * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11ClassInstance * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11ClassInstance * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetClassLinkage)(ID3D11ClassInstance * This,
                                              /* [annotation] */
                                              _Out_ ID3D11ClassLinkage ** ppLinkage);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11ClassInstance * This,
                                      /* [annotation] */
                                      _Out_ D3D11_CLASS_INSTANCE_DESC * pDesc);

    void(STDMETHODCALLTYPE * GetInstanceName)(ID3D11ClassInstance * This,
                                              /* [annotation] */
                                              _Out_writes_opt_(*pBufferLength) LPSTR pInstanceName,
                                              /* [annotation] */
                                              _Inout_ SIZE_T * pBufferLength);

    void(STDMETHODCALLTYPE * GetTypeName)(ID3D11ClassInstance * This,
                                          /* [annotation] */
                                          _Out_writes_opt_(*pBufferLength) LPSTR pTypeName,
                                          /* [annotation] */
                                          _Inout_ SIZE_T * pBufferLength);

    END_INTERFACE
} ID3D11ClassInstanceVtbl;

interface ID3D11ClassInstance {
    CONST_VTBL struct ID3D11ClassInstanceVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11ClassInstance_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11ClassInstance_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11ClassInstance_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11ClassInstance_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11ClassInstance_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11ClassInstance_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11ClassInstance_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11ClassInstance_GetClassLinkage(This, ppLinkage) ((This)->lpVtbl->GetClassLinkage(This, ppLinkage))

                #define ID3D11ClassInstance_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

                #define ID3D11ClassInstance_GetInstanceName(This, pInstanceName, pBufferLength) \
                    ((This)->lpVtbl->GetInstanceName(This, pInstanceName, pBufferLength))

                #define ID3D11ClassInstance_GetTypeName(This, pTypeName, pBufferLength) ((This)->lpVtbl->GetTypeName(This, pTypeName, pBufferLength))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11ClassInstance_INTERFACE_DEFINED__ */

    #ifndef __ID3D11ClassLinkage_INTERFACE_DEFINED__
        #define __ID3D11ClassLinkage_INTERFACE_DEFINED__

/* interface ID3D11ClassLinkage */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11ClassLinkage;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("ddf57cba-9543-46e4-a12b-f207a0fe7fed")
ID3D11ClassLinkage : public ID3D11DeviceChild {
public:
    virtual HRESULT STDMETHODCALLTYPE GetClassInstance(
        /* [annotation] */
        _In_ LPCSTR pClassInstanceName,
        /* [annotation] */
        _In_ UINT InstanceIndex,
        /* [annotation] */
        _Out_ ID3D11ClassInstance * *ppInstance) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateClassInstance(
        /* [annotation] */
        _In_ LPCSTR pClassTypeName,
        /* [annotation] */
        _In_ UINT ConstantBufferOffset,
        /* [annotation] */
        _In_ UINT ConstantVectorOffset,
        /* [annotation] */
        _In_ UINT TextureOffset,
        /* [annotation] */
        _In_ UINT SamplerOffset,
        /* [annotation] */
        _Out_ ID3D11ClassInstance * *ppInstance) = 0;
};

        #else /* C style interface */

typedef struct ID3D11ClassLinkageVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11ClassLinkage * This,
     /* [in] */ REFIID    riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11ClassLinkage * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11ClassLinkage * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11ClassLinkage * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11ClassLinkage * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11ClassLinkage * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11ClassLinkage * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    HRESULT(STDMETHODCALLTYPE * GetClassInstance)
    (ID3D11ClassLinkage * This,
     /* [annotation] */
     _In_ LPCSTR pClassInstanceName,
     /* [annotation] */
     _In_ UINT InstanceIndex,
     /* [annotation] */
     _Out_ ID3D11ClassInstance ** ppInstance);

    HRESULT(STDMETHODCALLTYPE * CreateClassInstance)
    (ID3D11ClassLinkage * This,
     /* [annotation] */
     _In_ LPCSTR pClassTypeName,
     /* [annotation] */
     _In_ UINT ConstantBufferOffset,
     /* [annotation] */
     _In_ UINT ConstantVectorOffset,
     /* [annotation] */
     _In_ UINT TextureOffset,
     /* [annotation] */
     _In_ UINT SamplerOffset,
     /* [annotation] */
     _Out_ ID3D11ClassInstance ** ppInstance);

    END_INTERFACE
} ID3D11ClassLinkageVtbl;

interface ID3D11ClassLinkage {
    CONST_VTBL struct ID3D11ClassLinkageVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11ClassLinkage_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11ClassLinkage_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11ClassLinkage_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11ClassLinkage_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11ClassLinkage_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11ClassLinkage_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11ClassLinkage_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11ClassLinkage_GetClassInstance(This, pClassInstanceName, InstanceIndex, ppInstance) \
                    ((This)->lpVtbl->GetClassInstance(This, pClassInstanceName, InstanceIndex, ppInstance))

                #define ID3D11ClassLinkage_CreateClassInstance(This, pClassTypeName, ConstantBufferOffset, ConstantVectorOffset, TextureOffset, SamplerOffset, \
                                                               ppInstance)                                                                                     \
                    ((This)->lpVtbl->CreateClassInstance(This, pClassTypeName, ConstantBufferOffset, ConstantVectorOffset, TextureOffset, SamplerOffset,       \
                                                         ppInstance))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11ClassLinkage_INTERFACE_DEFINED__ */

    #ifndef __ID3D11CommandList_INTERFACE_DEFINED__
        #define __ID3D11CommandList_INTERFACE_DEFINED__

/* interface ID3D11CommandList */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11CommandList;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("a24bc4d1-769e-43f7-8013-98ff566c18e2")
ID3D11CommandList : public ID3D11DeviceChild {
public:
    virtual UINT STDMETHODCALLTYPE GetContextFlags(void) = 0;
};

        #else /* C style interface */

typedef struct ID3D11CommandListVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11CommandList * This,
     /* [in] */ REFIID   riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11CommandList * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11CommandList * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11CommandList * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11CommandList * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11CommandList * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11CommandList * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    UINT(STDMETHODCALLTYPE * GetContextFlags)(ID3D11CommandList * This);

    END_INTERFACE
} ID3D11CommandListVtbl;

interface ID3D11CommandList {
    CONST_VTBL struct ID3D11CommandListVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11CommandList_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11CommandList_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11CommandList_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11CommandList_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11CommandList_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11CommandList_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11CommandList_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11CommandList_GetContextFlags(This) ((This)->lpVtbl->GetContextFlags(This))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11CommandList_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0029 */
/* [local] */

typedef enum D3D11_FEATURE {
    D3D11_FEATURE_THREADING                    = 0,
    D3D11_FEATURE_DOUBLES                      = (D3D11_FEATURE_THREADING + 1),
    D3D11_FEATURE_FORMAT_SUPPORT               = (D3D11_FEATURE_DOUBLES + 1),
    D3D11_FEATURE_FORMAT_SUPPORT2              = (D3D11_FEATURE_FORMAT_SUPPORT + 1),
    D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS     = (D3D11_FEATURE_FORMAT_SUPPORT2 + 1),
    D3D11_FEATURE_D3D11_OPTIONS                = (D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS + 1),
    D3D11_FEATURE_ARCHITECTURE_INFO            = (D3D11_FEATURE_D3D11_OPTIONS + 1),
    D3D11_FEATURE_D3D9_OPTIONS                 = (D3D11_FEATURE_ARCHITECTURE_INFO + 1),
    D3D11_FEATURE_SHADER_MIN_PRECISION_SUPPORT = (D3D11_FEATURE_D3D9_OPTIONS + 1),
    D3D11_FEATURE_D3D9_SHADOW_SUPPORT          = (D3D11_FEATURE_SHADER_MIN_PRECISION_SUPPORT + 1)
} D3D11_FEATURE;

typedef struct D3D11_FEATURE_DATA_THREADING {
    BOOL DriverConcurrentCreates;
    BOOL DriverCommandLists;
} D3D11_FEATURE_DATA_THREADING;

typedef struct D3D11_FEATURE_DATA_DOUBLES {
    BOOL DoublePrecisionFloatShaderOps;
} D3D11_FEATURE_DATA_DOUBLES;

typedef struct D3D11_FEATURE_DATA_FORMAT_SUPPORT {
    DXGI_FORMAT InFormat;
    UINT        OutFormatSupport;
} D3D11_FEATURE_DATA_FORMAT_SUPPORT;

typedef struct D3D11_FEATURE_DATA_FORMAT_SUPPORT2 {
    DXGI_FORMAT InFormat;
    UINT        OutFormatSupport2;
} D3D11_FEATURE_DATA_FORMAT_SUPPORT2;

typedef struct D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS {
    BOOL ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x;
} D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS;

// =============================================================================
// In the D3D11_FEATURE_DATA_D3D11_OPTIONS struct below,
// the following groupings of capabilities will always be set identically.
// That is, all the BOOLs in a grouping will be TRUE or FALSE together.
//
// Group: DiscardAPIsSeenByDriver, FlagsForUpdateAndCopySeenByDriver
//
// Group: ClearView, CopyWithOverlap, ConstantBufferPartialUpdate
//        ConstantBufferOffsetting, MapNoOverwriteOnDynamicConstantBuffer
//
// Group: MapNoOverwriteOnDynamicBufferSRV,
//        MultisampleRTVWithForcedSampleCountOne
//
typedef struct D3D11_FEATURE_DATA_D3D11_OPTIONS {
    BOOL OutputMergerLogicOp;
    BOOL UAVOnlyRenderingForcedSampleCount;
    BOOL DiscardAPIsSeenByDriver;
    BOOL FlagsForUpdateAndCopySeenByDriver;
    BOOL ClearView;
    BOOL CopyWithOverlap;
    BOOL ConstantBufferPartialUpdate;
    BOOL ConstantBufferOffsetting;
    BOOL MapNoOverwriteOnDynamicConstantBuffer;
    BOOL MapNoOverwriteOnDynamicBufferSRV;
    BOOL MultisampleRTVWithForcedSampleCountOne;
    BOOL SAD4ShaderInstructions;
    BOOL ExtendedDoublesShaderInstructions;
    BOOL ExtendedResourceSharing;
} D3D11_FEATURE_DATA_D3D11_OPTIONS;

typedef struct D3D11_FEATURE_DATA_ARCHITECTURE_INFO {
    BOOL TileBasedDeferredRenderer;
} D3D11_FEATURE_DATA_ARCHITECTURE_INFO;

typedef struct D3D11_FEATURE_DATA_D3D9_OPTIONS {
    BOOL FullNonPow2TextureSupport;
} D3D11_FEATURE_DATA_D3D9_OPTIONS;

typedef struct D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT {
    BOOL SupportsDepthAsTextureWithLessEqualComparisonFilter;
} D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT;

typedef enum D3D11_SHADER_MIN_PRECISION_SUPPORT {
    D3D11_SHADER_MIN_PRECISION_10_BIT = 0x1,
    D3D11_SHADER_MIN_PRECISION_16_BIT = 0x2
} D3D11_SHADER_MIN_PRECISION_SUPPORT;

typedef struct D3D11_FEATURE_DATA_SHADER_MIN_PRECISION_SUPPORT {
    UINT PixelShaderMinPrecision;
    UINT AllOtherShaderStagesMinPrecision;
} D3D11_FEATURE_DATA_SHADER_MIN_PRECISION_SUPPORT;

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0029_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0029_v0_0_s_ifspec;

    #ifndef __ID3D11DeviceContext_INTERFACE_DEFINED__
        #define __ID3D11DeviceContext_INTERFACE_DEFINED__

/* interface ID3D11DeviceContext */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11DeviceContext;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("c0bfa96c-e089-44fb-8eaf-26f8796190da")
ID3D11DeviceContext : public ID3D11DeviceChild {
public:
    virtual void STDMETHODCALLTYPE VSSetConstantBuffers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) = 0;

    virtual void STDMETHODCALLTYPE PSSetShaderResources(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews) ID3D11ShaderResourceView * const * ppShaderResourceViews) = 0;

    virtual void STDMETHODCALLTYPE PSSetShader(
        /* [annotation] */
        _In_opt_ ID3D11PixelShader * pPixelShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances, UINT NumClassInstances) = 0;

    virtual void STDMETHODCALLTYPE PSSetSamplers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers) ID3D11SamplerState * const * ppSamplers) = 0;

    virtual void STDMETHODCALLTYPE VSSetShader(
        /* [annotation] */
        _In_opt_ ID3D11VertexShader * pVertexShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances, UINT NumClassInstances) = 0;

    virtual void STDMETHODCALLTYPE DrawIndexed(
        /* [annotation] */
        _In_ UINT IndexCount,
        /* [annotation] */
        _In_ UINT StartIndexLocation,
        /* [annotation] */
        _In_ INT BaseVertexLocation) = 0;

    virtual void STDMETHODCALLTYPE Draw(
        /* [annotation] */
        _In_ UINT VertexCount,
        /* [annotation] */
        _In_ UINT StartVertexLocation) = 0;

    virtual HRESULT STDMETHODCALLTYPE Map(
        /* [annotation] */
        _In_ ID3D11Resource * pResource,
        /* [annotation] */
        _In_ UINT Subresource,
        /* [annotation] */
        _In_ D3D11_MAP MapType,
        /* [annotation] */
        _In_ UINT MapFlags,
        /* [annotation] */
        _Out_ D3D11_MAPPED_SUBRESOURCE * pMappedResource) = 0;

    virtual void STDMETHODCALLTYPE Unmap(
        /* [annotation] */
        _In_ ID3D11Resource * pResource,
        /* [annotation] */
        _In_ UINT Subresource) = 0;

    virtual void STDMETHODCALLTYPE PSSetConstantBuffers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) = 0;

    virtual void STDMETHODCALLTYPE IASetInputLayout(
        /* [annotation] */
        _In_opt_ ID3D11InputLayout * pInputLayout) = 0;

    virtual void STDMETHODCALLTYPE IASetVertexBuffers(
        /* [annotation] */
        _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppVertexBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers) const UINT * pStrides,
        /* [annotation] */
        _In_reads_opt_(NumBuffers) const UINT * pOffsets) = 0;

    virtual void STDMETHODCALLTYPE IASetIndexBuffer(
        /* [annotation] */
        _In_opt_ ID3D11Buffer * pIndexBuffer,
        /* [annotation] */
        _In_ DXGI_FORMAT Format,
        /* [annotation] */
        _In_ UINT Offset) = 0;

    virtual void STDMETHODCALLTYPE DrawIndexedInstanced(
        /* [annotation] */
        _In_ UINT IndexCountPerInstance,
        /* [annotation] */
        _In_ UINT InstanceCount,
        /* [annotation] */
        _In_ UINT StartIndexLocation,
        /* [annotation] */
        _In_ INT BaseVertexLocation,
        /* [annotation] */
        _In_ UINT StartInstanceLocation) = 0;

    virtual void STDMETHODCALLTYPE DrawInstanced(
        /* [annotation] */
        _In_ UINT VertexCountPerInstance,
        /* [annotation] */
        _In_ UINT InstanceCount,
        /* [annotation] */
        _In_ UINT StartVertexLocation,
        /* [annotation] */
        _In_ UINT StartInstanceLocation) = 0;

    virtual void STDMETHODCALLTYPE GSSetConstantBuffers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) = 0;

    virtual void STDMETHODCALLTYPE GSSetShader(
        /* [annotation] */
        _In_opt_ ID3D11GeometryShader * pShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances, UINT NumClassInstances) = 0;

    virtual void STDMETHODCALLTYPE IASetPrimitiveTopology(
        /* [annotation] */
        _In_ D3D11_PRIMITIVE_TOPOLOGY Topology) = 0;

    virtual void STDMETHODCALLTYPE VSSetShaderResources(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews) ID3D11ShaderResourceView * const * ppShaderResourceViews) = 0;

    virtual void STDMETHODCALLTYPE VSSetSamplers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers) ID3D11SamplerState * const * ppSamplers) = 0;

    virtual void STDMETHODCALLTYPE Begin(
        /* [annotation] */
        _In_ ID3D11Asynchronous * pAsync) = 0;

    virtual void STDMETHODCALLTYPE End(
        /* [annotation] */
        _In_ ID3D11Asynchronous * pAsync) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetData(
        /* [annotation] */
        _In_ ID3D11Asynchronous * pAsync,
        /* [annotation] */
        _Out_writes_bytes_opt_(DataSize) void * pData,
        /* [annotation] */
        _In_ UINT DataSize,
        /* [annotation] */
        _In_ UINT GetDataFlags) = 0;

    virtual void STDMETHODCALLTYPE SetPredication(
        /* [annotation] */
        _In_opt_ ID3D11Predicate * pPredicate,
        /* [annotation] */
        _In_ BOOL PredicateValue) = 0;

    virtual void STDMETHODCALLTYPE GSSetShaderResources(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews) ID3D11ShaderResourceView * const * ppShaderResourceViews) = 0;

    virtual void STDMETHODCALLTYPE GSSetSamplers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers) ID3D11SamplerState * const * ppSamplers) = 0;

    virtual void STDMETHODCALLTYPE OMSetRenderTargets(
        /* [annotation] */
        _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews) ID3D11RenderTargetView * const * ppRenderTargetViews,
        /* [annotation] */
        _In_opt_ ID3D11DepthStencilView * pDepthStencilView) = 0;

    virtual void STDMETHODCALLTYPE OMSetRenderTargetsAndUnorderedAccessViews(
        /* [annotation] */
        _In_ UINT NumRTVs,
        /* [annotation] */
        _In_reads_opt_(NumRTVs) ID3D11RenderTargetView * const * ppRenderTargetViews,
        /* [annotation] */
        _In_opt_ ID3D11DepthStencilView * pDepthStencilView,
        /* [annotation] */
        _In_range_(0, D3D11_1_UAV_SLOT_COUNT - 1) UINT UAVStartSlot,
        /* [annotation] */
        _In_ UINT NumUAVs,
        /* [annotation] */
        _In_reads_opt_(NumUAVs) ID3D11UnorderedAccessView * const * ppUnorderedAccessViews,
        /* [annotation] */
        _In_reads_opt_(NumUAVs) const UINT * pUAVInitialCounts) = 0;

    virtual void STDMETHODCALLTYPE OMSetBlendState(
        /* [annotation] */
        _In_opt_ ID3D11BlendState * pBlendState,
        /* [annotation] */
        _In_opt_ const FLOAT BlendFactor[4],
        /* [annotation] */
        _In_ UINT SampleMask) = 0;

    virtual void STDMETHODCALLTYPE OMSetDepthStencilState(
        /* [annotation] */
        _In_opt_ ID3D11DepthStencilState * pDepthStencilState,
        /* [annotation] */
        _In_ UINT StencilRef) = 0;

    virtual void STDMETHODCALLTYPE SOSetTargets(
        /* [annotation] */
        _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppSOTargets,
        /* [annotation] */
        _In_reads_opt_(NumBuffers) const UINT * pOffsets) = 0;

    virtual void STDMETHODCALLTYPE DrawAuto(void) = 0;

    virtual void STDMETHODCALLTYPE DrawIndexedInstancedIndirect(
        /* [annotation] */
        _In_ ID3D11Buffer * pBufferForArgs,
        /* [annotation] */
        _In_ UINT AlignedByteOffsetForArgs) = 0;

    virtual void STDMETHODCALLTYPE DrawInstancedIndirect(
        /* [annotation] */
        _In_ ID3D11Buffer * pBufferForArgs,
        /* [annotation] */
        _In_ UINT AlignedByteOffsetForArgs) = 0;

    virtual void STDMETHODCALLTYPE Dispatch(
        /* [annotation] */
        _In_ UINT ThreadGroupCountX,
        /* [annotation] */
        _In_ UINT ThreadGroupCountY,
        /* [annotation] */
        _In_ UINT ThreadGroupCountZ) = 0;

    virtual void STDMETHODCALLTYPE DispatchIndirect(
        /* [annotation] */
        _In_ ID3D11Buffer * pBufferForArgs,
        /* [annotation] */
        _In_ UINT AlignedByteOffsetForArgs) = 0;

    virtual void STDMETHODCALLTYPE RSSetState(
        /* [annotation] */
        _In_opt_ ID3D11RasterizerState * pRasterizerState) = 0;

    virtual void STDMETHODCALLTYPE RSSetViewports(
        /* [annotation] */
        _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE) UINT NumViewports,
        /* [annotation] */
        _In_reads_opt_(NumViewports) const D3D11_VIEWPORT * pViewports) = 0;

    virtual void STDMETHODCALLTYPE RSSetScissorRects(
        /* [annotation] */
        _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE) UINT NumRects,
        /* [annotation] */
        _In_reads_opt_(NumRects) const D3D11_RECT * pRects) = 0;

    virtual void STDMETHODCALLTYPE CopySubresourceRegion(
        /* [annotation] */
        _In_ ID3D11Resource * pDstResource,
        /* [annotation] */
        _In_ UINT DstSubresource,
        /* [annotation] */
        _In_ UINT DstX,
        /* [annotation] */
        _In_ UINT DstY,
        /* [annotation] */
        _In_ UINT DstZ,
        /* [annotation] */
        _In_ ID3D11Resource * pSrcResource,
        /* [annotation] */
        _In_ UINT SrcSubresource,
        /* [annotation] */
        _In_opt_ const D3D11_BOX * pSrcBox) = 0;

    virtual void STDMETHODCALLTYPE CopyResource(
        /* [annotation] */
        _In_ ID3D11Resource * pDstResource,
        /* [annotation] */
        _In_ ID3D11Resource * pSrcResource) = 0;

    virtual void STDMETHODCALLTYPE UpdateSubresource(
        /* [annotation] */
        _In_ ID3D11Resource * pDstResource,
        /* [annotation] */
        _In_ UINT DstSubresource,
        /* [annotation] */
        _In_opt_ const D3D11_BOX * pDstBox,
        /* [annotation] */
        _In_ const void * pSrcData,
        /* [annotation] */
        _In_ UINT SrcRowPitch,
        /* [annotation] */
        _In_ UINT SrcDepthPitch) = 0;

    virtual void STDMETHODCALLTYPE CopyStructureCount(
        /* [annotation] */
        _In_ ID3D11Buffer * pDstBuffer,
        /* [annotation] */
        _In_ UINT DstAlignedByteOffset,
        /* [annotation] */
        _In_ ID3D11UnorderedAccessView * pSrcView) = 0;

    virtual void STDMETHODCALLTYPE ClearRenderTargetView(
        /* [annotation] */
        _In_ ID3D11RenderTargetView * pRenderTargetView,
        /* [annotation] */
        _In_ const FLOAT ColorRGBA[4]) = 0;

    virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewUint(
        /* [annotation] */
        _In_ ID3D11UnorderedAccessView * pUnorderedAccessView,
        /* [annotation] */
        _In_ const UINT Values[4]) = 0;

    virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewFloat(
        /* [annotation] */
        _In_ ID3D11UnorderedAccessView * pUnorderedAccessView,
        /* [annotation] */
        _In_ const FLOAT Values[4]) = 0;

    virtual void STDMETHODCALLTYPE ClearDepthStencilView(
        /* [annotation] */
        _In_ ID3D11DepthStencilView * pDepthStencilView,
        /* [annotation] */
        _In_ UINT ClearFlags,
        /* [annotation] */
        _In_ FLOAT Depth,
        /* [annotation] */
        _In_ UINT8 Stencil) = 0;

    virtual void STDMETHODCALLTYPE GenerateMips(
        /* [annotation] */
        _In_ ID3D11ShaderResourceView * pShaderResourceView) = 0;

    virtual void STDMETHODCALLTYPE SetResourceMinLOD(
        /* [annotation] */
        _In_ ID3D11Resource * pResource, FLOAT MinLOD) = 0;

    virtual FLOAT STDMETHODCALLTYPE GetResourceMinLOD(
        /* [annotation] */
        _In_ ID3D11Resource * pResource) = 0;

    virtual void STDMETHODCALLTYPE ResolveSubresource(
        /* [annotation] */
        _In_ ID3D11Resource * pDstResource,
        /* [annotation] */
        _In_ UINT DstSubresource,
        /* [annotation] */
        _In_ ID3D11Resource * pSrcResource,
        /* [annotation] */
        _In_ UINT SrcSubresource,
        /* [annotation] */
        _In_ DXGI_FORMAT Format) = 0;

    virtual void STDMETHODCALLTYPE ExecuteCommandList(
        /* [annotation] */
        _In_ ID3D11CommandList * pCommandList, BOOL RestoreContextState) = 0;

    virtual void STDMETHODCALLTYPE HSSetShaderResources(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews) ID3D11ShaderResourceView * const * ppShaderResourceViews) = 0;

    virtual void STDMETHODCALLTYPE HSSetShader(
        /* [annotation] */
        _In_opt_ ID3D11HullShader * pHullShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances, UINT NumClassInstances) = 0;

    virtual void STDMETHODCALLTYPE HSSetSamplers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers) ID3D11SamplerState * const * ppSamplers) = 0;

    virtual void STDMETHODCALLTYPE HSSetConstantBuffers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) = 0;

    virtual void STDMETHODCALLTYPE DSSetShaderResources(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews) ID3D11ShaderResourceView * const * ppShaderResourceViews) = 0;

    virtual void STDMETHODCALLTYPE DSSetShader(
        /* [annotation] */
        _In_opt_ ID3D11DomainShader * pDomainShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances, UINT NumClassInstances) = 0;

    virtual void STDMETHODCALLTYPE DSSetSamplers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers) ID3D11SamplerState * const * ppSamplers) = 0;

    virtual void STDMETHODCALLTYPE DSSetConstantBuffers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) = 0;

    virtual void STDMETHODCALLTYPE CSSetShaderResources(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews) ID3D11ShaderResourceView * const * ppShaderResourceViews) = 0;

    virtual void STDMETHODCALLTYPE CSSetUnorderedAccessViews(
        /* [annotation] */
        _In_range_(0, D3D11_1_UAV_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_1_UAV_SLOT_COUNT - StartSlot) UINT NumUAVs,
        /* [annotation] */
        _In_reads_opt_(NumUAVs) ID3D11UnorderedAccessView * const * ppUnorderedAccessViews,
        /* [annotation] */
        _In_reads_opt_(NumUAVs) const UINT * pUAVInitialCounts) = 0;

    virtual void STDMETHODCALLTYPE CSSetShader(
        /* [annotation] */
        _In_opt_ ID3D11ComputeShader * pComputeShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances, UINT NumClassInstances) = 0;

    virtual void STDMETHODCALLTYPE CSSetSamplers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers) ID3D11SamplerState * const * ppSamplers) = 0;

    virtual void STDMETHODCALLTYPE CSSetConstantBuffers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) = 0;

    virtual void STDMETHODCALLTYPE VSGetConstantBuffers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers) ID3D11Buffer * *ppConstantBuffers) = 0;

    virtual void STDMETHODCALLTYPE PSGetShaderResources(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews) ID3D11ShaderResourceView * *ppShaderResourceViews) = 0;

    virtual void STDMETHODCALLTYPE PSGetShader(
        /* [annotation] */
        _Out_ ID3D11PixelShader * *ppPixelShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance * *ppClassInstances,
        /* [annotation] */
        _Inout_opt_ UINT * pNumClassInstances) = 0;

    virtual void STDMETHODCALLTYPE PSGetSamplers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers) ID3D11SamplerState * *ppSamplers) = 0;

    virtual void STDMETHODCALLTYPE VSGetShader(
        /* [annotation] */
        _Out_ ID3D11VertexShader * *ppVertexShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance * *ppClassInstances,
        /* [annotation] */
        _Inout_opt_ UINT * pNumClassInstances) = 0;

    virtual void STDMETHODCALLTYPE PSGetConstantBuffers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers) ID3D11Buffer * *ppConstantBuffers) = 0;

    virtual void STDMETHODCALLTYPE IAGetInputLayout(
        /* [annotation] */
        _Out_ ID3D11InputLayout * *ppInputLayout) = 0;

    virtual void STDMETHODCALLTYPE IAGetVertexBuffers(
        /* [annotation] */
        _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers) ID3D11Buffer * *ppVertexBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers) UINT * pStrides,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers) UINT * pOffsets) = 0;

    virtual void STDMETHODCALLTYPE IAGetIndexBuffer(
        /* [annotation] */
        _Out_opt_ ID3D11Buffer * *pIndexBuffer,
        /* [annotation] */
        _Out_opt_ DXGI_FORMAT * Format,
        /* [annotation] */
        _Out_opt_ UINT * Offset) = 0;

    virtual void STDMETHODCALLTYPE GSGetConstantBuffers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers) ID3D11Buffer * *ppConstantBuffers) = 0;

    virtual void STDMETHODCALLTYPE GSGetShader(
        /* [annotation] */
        _Out_ ID3D11GeometryShader * *ppGeometryShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance * *ppClassInstances,
        /* [annotation] */
        _Inout_opt_ UINT * pNumClassInstances) = 0;

    virtual void STDMETHODCALLTYPE IAGetPrimitiveTopology(
        /* [annotation] */
        _Out_ D3D11_PRIMITIVE_TOPOLOGY * pTopology) = 0;

    virtual void STDMETHODCALLTYPE VSGetShaderResources(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews) ID3D11ShaderResourceView * *ppShaderResourceViews) = 0;

    virtual void STDMETHODCALLTYPE VSGetSamplers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers) ID3D11SamplerState * *ppSamplers) = 0;

    virtual void STDMETHODCALLTYPE GetPredication(
        /* [annotation] */
        _Out_opt_ ID3D11Predicate * *ppPredicate,
        /* [annotation] */
        _Out_opt_ BOOL * pPredicateValue) = 0;

    virtual void STDMETHODCALLTYPE GSGetShaderResources(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews) ID3D11ShaderResourceView * *ppShaderResourceViews) = 0;

    virtual void STDMETHODCALLTYPE GSGetSamplers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers) ID3D11SamplerState * *ppSamplers) = 0;

    virtual void STDMETHODCALLTYPE OMGetRenderTargets(
        /* [annotation] */
        _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews) ID3D11RenderTargetView * *ppRenderTargetViews,
        /* [annotation] */
        _Out_opt_ ID3D11DepthStencilView * *ppDepthStencilView) = 0;

    virtual void STDMETHODCALLTYPE OMGetRenderTargetsAndUnorderedAccessViews(
        /* [annotation] */
        _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumRTVs,
        /* [annotation] */
        _Out_writes_opt_(NumRTVs) ID3D11RenderTargetView * *ppRenderTargetViews,
        /* [annotation] */
        _Out_opt_ ID3D11DepthStencilView * *ppDepthStencilView,
        /* [annotation] */
        _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1) UINT UAVStartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot) UINT NumUAVs,
        /* [annotation] */
        _Out_writes_opt_(NumUAVs) ID3D11UnorderedAccessView * *ppUnorderedAccessViews) = 0;

    virtual void STDMETHODCALLTYPE OMGetBlendState(
        /* [annotation] */
        _Out_opt_ ID3D11BlendState * *ppBlendState,
        /* [annotation] */
        _Out_opt_ FLOAT BlendFactor[4],
        /* [annotation] */
        _Out_opt_ UINT * pSampleMask) = 0;

    virtual void STDMETHODCALLTYPE OMGetDepthStencilState(
        /* [annotation] */
        _Out_opt_ ID3D11DepthStencilState * *ppDepthStencilState,
        /* [annotation] */
        _Out_opt_ UINT * pStencilRef) = 0;

    virtual void STDMETHODCALLTYPE SOGetTargets(
        /* [annotation] */
        _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers) ID3D11Buffer * *ppSOTargets) = 0;

    virtual void STDMETHODCALLTYPE RSGetState(
        /* [annotation] */
        _Out_ ID3D11RasterizerState * *ppRasterizerState) = 0;

    virtual void STDMETHODCALLTYPE RSGetViewports(
        /* [annotation] */
        _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/ UINT * pNumViewports,
        /* [annotation] */
        _Out_writes_opt_(*pNumViewports) D3D11_VIEWPORT * pViewports) = 0;

    virtual void STDMETHODCALLTYPE RSGetScissorRects(
        /* [annotation] */
        _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/ UINT * pNumRects,
        /* [annotation] */
        _Out_writes_opt_(*pNumRects) D3D11_RECT * pRects) = 0;

    virtual void STDMETHODCALLTYPE HSGetShaderResources(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews) ID3D11ShaderResourceView * *ppShaderResourceViews) = 0;

    virtual void STDMETHODCALLTYPE HSGetShader(
        /* [annotation] */
        _Out_ ID3D11HullShader * *ppHullShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance * *ppClassInstances,
        /* [annotation] */
        _Inout_opt_ UINT * pNumClassInstances) = 0;

    virtual void STDMETHODCALLTYPE HSGetSamplers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers) ID3D11SamplerState * *ppSamplers) = 0;

    virtual void STDMETHODCALLTYPE HSGetConstantBuffers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers) ID3D11Buffer * *ppConstantBuffers) = 0;

    virtual void STDMETHODCALLTYPE DSGetShaderResources(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews) ID3D11ShaderResourceView * *ppShaderResourceViews) = 0;

    virtual void STDMETHODCALLTYPE DSGetShader(
        /* [annotation] */
        _Out_ ID3D11DomainShader * *ppDomainShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance * *ppClassInstances,
        /* [annotation] */
        _Inout_opt_ UINT * pNumClassInstances) = 0;

    virtual void STDMETHODCALLTYPE DSGetSamplers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers) ID3D11SamplerState * *ppSamplers) = 0;

    virtual void STDMETHODCALLTYPE DSGetConstantBuffers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers) ID3D11Buffer * *ppConstantBuffers) = 0;

    virtual void STDMETHODCALLTYPE CSGetShaderResources(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews) ID3D11ShaderResourceView * *ppShaderResourceViews) = 0;

    virtual void STDMETHODCALLTYPE CSGetUnorderedAccessViews(
        /* [annotation] */
        _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot) UINT NumUAVs,
        /* [annotation] */
        _Out_writes_opt_(NumUAVs) ID3D11UnorderedAccessView * *ppUnorderedAccessViews) = 0;

    virtual void STDMETHODCALLTYPE CSGetShader(
        /* [annotation] */
        _Out_ ID3D11ComputeShader * *ppComputeShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance * *ppClassInstances,
        /* [annotation] */
        _Inout_opt_ UINT * pNumClassInstances) = 0;

    virtual void STDMETHODCALLTYPE CSGetSamplers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers) ID3D11SamplerState * *ppSamplers) = 0;

    virtual void STDMETHODCALLTYPE CSGetConstantBuffers(
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
        /* [annotation] */
        _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers) ID3D11Buffer * *ppConstantBuffers) = 0;

    virtual void STDMETHODCALLTYPE ClearState(void) = 0;

    virtual void STDMETHODCALLTYPE Flush(void) = 0;

    virtual D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE GetType(void) = 0;

    virtual UINT STDMETHODCALLTYPE GetContextFlags(void) = 0;

    virtual HRESULT STDMETHODCALLTYPE FinishCommandList(BOOL RestoreDeferredContextState,
                                                        /* [annotation] */
                                                        _Out_opt_ ID3D11CommandList * *ppCommandList) = 0;
};

        #else /* C style interface */

typedef struct ID3D11DeviceContextVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11DeviceContext * This,
     /* [in] */ REFIID     riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11DeviceContext * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11DeviceContext * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11DeviceContext * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11DeviceContext * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11DeviceContext * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11DeviceContext * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * VSSetConstantBuffers)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
                                                   /* [annotation] */
                                                   _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers);

    void(STDMETHODCALLTYPE * PSSetShaderResources)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
                                                   /* [annotation] */
                                                   _In_reads_opt_(NumViews) ID3D11ShaderResourceView * const * ppShaderResourceViews);

    void(STDMETHODCALLTYPE * PSSetShader)(ID3D11DeviceContext * This,
                                          /* [annotation] */
                                          _In_opt_ ID3D11PixelShader * pPixelShader,
                                          /* [annotation] */
                                          _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances, UINT NumClassInstances);

    void(STDMETHODCALLTYPE * PSSetSamplers)(ID3D11DeviceContext * This,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                            /* [annotation] */
                                            _In_reads_opt_(NumSamplers) ID3D11SamplerState * const * ppSamplers);

    void(STDMETHODCALLTYPE * VSSetShader)(ID3D11DeviceContext * This,
                                          /* [annotation] */
                                          _In_opt_ ID3D11VertexShader * pVertexShader,
                                          /* [annotation] */
                                          _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances, UINT NumClassInstances);

    void(STDMETHODCALLTYPE * DrawIndexed)(ID3D11DeviceContext * This,
                                          /* [annotation] */
                                          _In_ UINT IndexCount,
                                          /* [annotation] */
                                          _In_ UINT StartIndexLocation,
                                          /* [annotation] */
                                          _In_ INT BaseVertexLocation);

    void(STDMETHODCALLTYPE * Draw)(ID3D11DeviceContext * This,
                                   /* [annotation] */
                                   _In_ UINT VertexCount,
                                   /* [annotation] */
                                   _In_ UINT StartVertexLocation);

    HRESULT(STDMETHODCALLTYPE * Map)
    (ID3D11DeviceContext * This,
     /* [annotation] */
     _In_ ID3D11Resource * pResource,
     /* [annotation] */
     _In_ UINT Subresource,
     /* [annotation] */
     _In_ D3D11_MAP MapType,
     /* [annotation] */
     _In_ UINT MapFlags,
     /* [annotation] */
     _Out_ D3D11_MAPPED_SUBRESOURCE * pMappedResource);

    void(STDMETHODCALLTYPE * Unmap)(ID3D11DeviceContext * This,
                                    /* [annotation] */
                                    _In_ ID3D11Resource * pResource,
                                    /* [annotation] */
                                    _In_ UINT Subresource);

    void(STDMETHODCALLTYPE * PSSetConstantBuffers)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
                                                   /* [annotation] */
                                                   _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers);

    void(STDMETHODCALLTYPE * IASetInputLayout)(ID3D11DeviceContext * This,
                                               /* [annotation] */
                                               _In_opt_ ID3D11InputLayout * pInputLayout);

    void(STDMETHODCALLTYPE * IASetVertexBuffers)(ID3D11DeviceContext * This,
                                                 /* [annotation] */
                                                 _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                 /* [annotation] */
                                                 _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumBuffers,
                                                 /* [annotation] */
                                                 _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppVertexBuffers,
                                                 /* [annotation] */
                                                 _In_reads_opt_(NumBuffers) const UINT * pStrides,
                                                 /* [annotation] */
                                                 _In_reads_opt_(NumBuffers) const UINT * pOffsets);

    void(STDMETHODCALLTYPE * IASetIndexBuffer)(ID3D11DeviceContext * This,
                                               /* [annotation] */
                                               _In_opt_ ID3D11Buffer * pIndexBuffer,
                                               /* [annotation] */
                                               _In_ DXGI_FORMAT Format,
                                               /* [annotation] */
                                               _In_ UINT Offset);

    void(STDMETHODCALLTYPE * DrawIndexedInstanced)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_ UINT IndexCountPerInstance,
                                                   /* [annotation] */
                                                   _In_ UINT InstanceCount,
                                                   /* [annotation] */
                                                   _In_ UINT StartIndexLocation,
                                                   /* [annotation] */
                                                   _In_ INT BaseVertexLocation,
                                                   /* [annotation] */
                                                   _In_ UINT StartInstanceLocation);

    void(STDMETHODCALLTYPE * DrawInstanced)(ID3D11DeviceContext * This,
                                            /* [annotation] */
                                            _In_ UINT VertexCountPerInstance,
                                            /* [annotation] */
                                            _In_ UINT InstanceCount,
                                            /* [annotation] */
                                            _In_ UINT StartVertexLocation,
                                            /* [annotation] */
                                            _In_ UINT StartInstanceLocation);

    void(STDMETHODCALLTYPE * GSSetConstantBuffers)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
                                                   /* [annotation] */
                                                   _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers);

    void(STDMETHODCALLTYPE * GSSetShader)(ID3D11DeviceContext * This,
                                          /* [annotation] */
                                          _In_opt_ ID3D11GeometryShader * pShader,
                                          /* [annotation] */
                                          _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances, UINT NumClassInstances);

    void(STDMETHODCALLTYPE * IASetPrimitiveTopology)(ID3D11DeviceContext * This,
                                                     /* [annotation] */
                                                     _In_ D3D11_PRIMITIVE_TOPOLOGY Topology);

    void(STDMETHODCALLTYPE * VSSetShaderResources)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
                                                   /* [annotation] */
                                                   _In_reads_opt_(NumViews) ID3D11ShaderResourceView * const * ppShaderResourceViews);

    void(STDMETHODCALLTYPE * VSSetSamplers)(ID3D11DeviceContext * This,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                            /* [annotation] */
                                            _In_reads_opt_(NumSamplers) ID3D11SamplerState * const * ppSamplers);

    void(STDMETHODCALLTYPE * Begin)(ID3D11DeviceContext * This,
                                    /* [annotation] */
                                    _In_ ID3D11Asynchronous * pAsync);

    void(STDMETHODCALLTYPE * End)(ID3D11DeviceContext * This,
                                  /* [annotation] */
                                  _In_ ID3D11Asynchronous * pAsync);

    HRESULT(STDMETHODCALLTYPE * GetData)
    (ID3D11DeviceContext * This,
     /* [annotation] */
     _In_ ID3D11Asynchronous * pAsync,
     /* [annotation] */
     _Out_writes_bytes_opt_(DataSize) void * pData,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_ UINT GetDataFlags);

    void(STDMETHODCALLTYPE * SetPredication)(ID3D11DeviceContext * This,
                                             /* [annotation] */
                                             _In_opt_ ID3D11Predicate * pPredicate,
                                             /* [annotation] */
                                             _In_ BOOL PredicateValue);

    void(STDMETHODCALLTYPE * GSSetShaderResources)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
                                                   /* [annotation] */
                                                   _In_reads_opt_(NumViews) ID3D11ShaderResourceView * const * ppShaderResourceViews);

    void(STDMETHODCALLTYPE * GSSetSamplers)(ID3D11DeviceContext * This,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                            /* [annotation] */
                                            _In_reads_opt_(NumSamplers) ID3D11SamplerState * const * ppSamplers);

    void(STDMETHODCALLTYPE * OMSetRenderTargets)(ID3D11DeviceContext * This,
                                                 /* [annotation] */
                                                 _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumViews,
                                                 /* [annotation] */
                                                 _In_reads_opt_(NumViews) ID3D11RenderTargetView * const * ppRenderTargetViews,
                                                 /* [annotation] */
                                                 _In_opt_ ID3D11DepthStencilView * pDepthStencilView);

    void(STDMETHODCALLTYPE * OMSetRenderTargetsAndUnorderedAccessViews)(ID3D11DeviceContext * This,
                                                                        /* [annotation] */
                                                                        _In_ UINT NumRTVs,
                                                                        /* [annotation] */
                                                                        _In_reads_opt_(NumRTVs) ID3D11RenderTargetView * const * ppRenderTargetViews,
                                                                        /* [annotation] */
                                                                        _In_opt_ ID3D11DepthStencilView * pDepthStencilView,
                                                                        /* [annotation] */
                                                                        _In_range_(0, D3D11_1_UAV_SLOT_COUNT - 1) UINT UAVStartSlot,
                                                                        /* [annotation] */
                                                                        _In_ UINT NumUAVs,
                                                                        /* [annotation] */
                                                                        _In_reads_opt_(NumUAVs) ID3D11UnorderedAccessView * const * ppUnorderedAccessViews,
                                                                        /* [annotation] */
                                                                        _In_reads_opt_(NumUAVs) const UINT * pUAVInitialCounts);

    void(STDMETHODCALLTYPE * OMSetBlendState)(ID3D11DeviceContext * This,
                                              /* [annotation] */
                                              _In_opt_ ID3D11BlendState * pBlendState,
                                              /* [annotation] */
                                              _In_opt_ const FLOAT BlendFactor[4],
                                              /* [annotation] */
                                              _In_ UINT SampleMask);

    void(STDMETHODCALLTYPE * OMSetDepthStencilState)(ID3D11DeviceContext * This,
                                                     /* [annotation] */
                                                     _In_opt_ ID3D11DepthStencilState * pDepthStencilState,
                                                     /* [annotation] */
                                                     _In_ UINT StencilRef);

    void(STDMETHODCALLTYPE * SOSetTargets)(ID3D11DeviceContext * This,
                                           /* [annotation] */
                                           _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumBuffers,
                                           /* [annotation] */
                                           _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppSOTargets,
                                           /* [annotation] */
                                           _In_reads_opt_(NumBuffers) const UINT * pOffsets);

    void(STDMETHODCALLTYPE * DrawAuto)(ID3D11DeviceContext * This);

    void(STDMETHODCALLTYPE * DrawIndexedInstancedIndirect)(ID3D11DeviceContext * This,
                                                           /* [annotation] */
                                                           _In_ ID3D11Buffer * pBufferForArgs,
                                                           /* [annotation] */
                                                           _In_ UINT AlignedByteOffsetForArgs);

    void(STDMETHODCALLTYPE * DrawInstancedIndirect)(ID3D11DeviceContext * This,
                                                    /* [annotation] */
                                                    _In_ ID3D11Buffer * pBufferForArgs,
                                                    /* [annotation] */
                                                    _In_ UINT AlignedByteOffsetForArgs);

    void(STDMETHODCALLTYPE * Dispatch)(ID3D11DeviceContext * This,
                                       /* [annotation] */
                                       _In_ UINT ThreadGroupCountX,
                                       /* [annotation] */
                                       _In_ UINT ThreadGroupCountY,
                                       /* [annotation] */
                                       _In_ UINT ThreadGroupCountZ);

    void(STDMETHODCALLTYPE * DispatchIndirect)(ID3D11DeviceContext * This,
                                               /* [annotation] */
                                               _In_ ID3D11Buffer * pBufferForArgs,
                                               /* [annotation] */
                                               _In_ UINT AlignedByteOffsetForArgs);

    void(STDMETHODCALLTYPE * RSSetState)(ID3D11DeviceContext * This,
                                         /* [annotation] */
                                         _In_opt_ ID3D11RasterizerState * pRasterizerState);

    void(STDMETHODCALLTYPE * RSSetViewports)(ID3D11DeviceContext * This,
                                             /* [annotation] */
                                             _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE) UINT NumViewports,
                                             /* [annotation] */
                                             _In_reads_opt_(NumViewports) const D3D11_VIEWPORT * pViewports);

    void(STDMETHODCALLTYPE * RSSetScissorRects)(ID3D11DeviceContext * This,
                                                /* [annotation] */
                                                _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE) UINT NumRects,
                                                /* [annotation] */
                                                _In_reads_opt_(NumRects) const D3D11_RECT * pRects);

    void(STDMETHODCALLTYPE * CopySubresourceRegion)(ID3D11DeviceContext * This,
                                                    /* [annotation] */
                                                    _In_ ID3D11Resource * pDstResource,
                                                    /* [annotation] */
                                                    _In_ UINT DstSubresource,
                                                    /* [annotation] */
                                                    _In_ UINT DstX,
                                                    /* [annotation] */
                                                    _In_ UINT DstY,
                                                    /* [annotation] */
                                                    _In_ UINT DstZ,
                                                    /* [annotation] */
                                                    _In_ ID3D11Resource * pSrcResource,
                                                    /* [annotation] */
                                                    _In_ UINT SrcSubresource,
                                                    /* [annotation] */
                                                    _In_opt_ const D3D11_BOX * pSrcBox);

    void(STDMETHODCALLTYPE * CopyResource)(ID3D11DeviceContext * This,
                                           /* [annotation] */
                                           _In_ ID3D11Resource * pDstResource,
                                           /* [annotation] */
                                           _In_ ID3D11Resource * pSrcResource);

    void(STDMETHODCALLTYPE * UpdateSubresource)(ID3D11DeviceContext * This,
                                                /* [annotation] */
                                                _In_ ID3D11Resource * pDstResource,
                                                /* [annotation] */
                                                _In_ UINT DstSubresource,
                                                /* [annotation] */
                                                _In_opt_ const D3D11_BOX * pDstBox,
                                                /* [annotation] */
                                                _In_ const void * pSrcData,
                                                /* [annotation] */
                                                _In_ UINT SrcRowPitch,
                                                /* [annotation] */
                                                _In_ UINT SrcDepthPitch);

    void(STDMETHODCALLTYPE * CopyStructureCount)(ID3D11DeviceContext * This,
                                                 /* [annotation] */
                                                 _In_ ID3D11Buffer * pDstBuffer,
                                                 /* [annotation] */
                                                 _In_ UINT DstAlignedByteOffset,
                                                 /* [annotation] */
                                                 _In_ ID3D11UnorderedAccessView * pSrcView);

    void(STDMETHODCALLTYPE * ClearRenderTargetView)(ID3D11DeviceContext * This,
                                                    /* [annotation] */
                                                    _In_ ID3D11RenderTargetView * pRenderTargetView,
                                                    /* [annotation] */
                                                    _In_ const FLOAT ColorRGBA[4]);

    void(STDMETHODCALLTYPE * ClearUnorderedAccessViewUint)(ID3D11DeviceContext * This,
                                                           /* [annotation] */
                                                           _In_ ID3D11UnorderedAccessView * pUnorderedAccessView,
                                                           /* [annotation] */
                                                           _In_ const UINT Values[4]);

    void(STDMETHODCALLTYPE * ClearUnorderedAccessViewFloat)(ID3D11DeviceContext * This,
                                                            /* [annotation] */
                                                            _In_ ID3D11UnorderedAccessView * pUnorderedAccessView,
                                                            /* [annotation] */
                                                            _In_ const FLOAT Values[4]);

    void(STDMETHODCALLTYPE * ClearDepthStencilView)(ID3D11DeviceContext * This,
                                                    /* [annotation] */
                                                    _In_ ID3D11DepthStencilView * pDepthStencilView,
                                                    /* [annotation] */
                                                    _In_ UINT ClearFlags,
                                                    /* [annotation] */
                                                    _In_ FLOAT Depth,
                                                    /* [annotation] */
                                                    _In_ UINT8 Stencil);

    void(STDMETHODCALLTYPE * GenerateMips)(ID3D11DeviceContext * This,
                                           /* [annotation] */
                                           _In_ ID3D11ShaderResourceView * pShaderResourceView);

    void(STDMETHODCALLTYPE * SetResourceMinLOD)(ID3D11DeviceContext * This,
                                                /* [annotation] */
                                                _In_ ID3D11Resource * pResource, FLOAT MinLOD);

    FLOAT(STDMETHODCALLTYPE * GetResourceMinLOD)
    (ID3D11DeviceContext * This,
     /* [annotation] */
     _In_ ID3D11Resource * pResource);

    void(STDMETHODCALLTYPE * ResolveSubresource)(ID3D11DeviceContext * This,
                                                 /* [annotation] */
                                                 _In_ ID3D11Resource * pDstResource,
                                                 /* [annotation] */
                                                 _In_ UINT DstSubresource,
                                                 /* [annotation] */
                                                 _In_ ID3D11Resource * pSrcResource,
                                                 /* [annotation] */
                                                 _In_ UINT SrcSubresource,
                                                 /* [annotation] */
                                                 _In_ DXGI_FORMAT Format);

    void(STDMETHODCALLTYPE * ExecuteCommandList)(ID3D11DeviceContext * This,
                                                 /* [annotation] */
                                                 _In_ ID3D11CommandList * pCommandList, BOOL RestoreContextState);

    void(STDMETHODCALLTYPE * HSSetShaderResources)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
                                                   /* [annotation] */
                                                   _In_reads_opt_(NumViews) ID3D11ShaderResourceView * const * ppShaderResourceViews);

    void(STDMETHODCALLTYPE * HSSetShader)(ID3D11DeviceContext * This,
                                          /* [annotation] */
                                          _In_opt_ ID3D11HullShader * pHullShader,
                                          /* [annotation] */
                                          _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances, UINT NumClassInstances);

    void(STDMETHODCALLTYPE * HSSetSamplers)(ID3D11DeviceContext * This,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                            /* [annotation] */
                                            _In_reads_opt_(NumSamplers) ID3D11SamplerState * const * ppSamplers);

    void(STDMETHODCALLTYPE * HSSetConstantBuffers)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
                                                   /* [annotation] */
                                                   _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers);

    void(STDMETHODCALLTYPE * DSSetShaderResources)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
                                                   /* [annotation] */
                                                   _In_reads_opt_(NumViews) ID3D11ShaderResourceView * const * ppShaderResourceViews);

    void(STDMETHODCALLTYPE * DSSetShader)(ID3D11DeviceContext * This,
                                          /* [annotation] */
                                          _In_opt_ ID3D11DomainShader * pDomainShader,
                                          /* [annotation] */
                                          _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances, UINT NumClassInstances);

    void(STDMETHODCALLTYPE * DSSetSamplers)(ID3D11DeviceContext * This,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                            /* [annotation] */
                                            _In_reads_opt_(NumSamplers) ID3D11SamplerState * const * ppSamplers);

    void(STDMETHODCALLTYPE * DSSetConstantBuffers)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
                                                   /* [annotation] */
                                                   _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers);

    void(STDMETHODCALLTYPE * CSSetShaderResources)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
                                                   /* [annotation] */
                                                   _In_reads_opt_(NumViews) ID3D11ShaderResourceView * const * ppShaderResourceViews);

    void(STDMETHODCALLTYPE * CSSetUnorderedAccessViews)(ID3D11DeviceContext * This,
                                                        /* [annotation] */
                                                        _In_range_(0, D3D11_1_UAV_SLOT_COUNT - 1) UINT StartSlot,
                                                        /* [annotation] */
                                                        _In_range_(0, D3D11_1_UAV_SLOT_COUNT - StartSlot) UINT NumUAVs,
                                                        /* [annotation] */
                                                        _In_reads_opt_(NumUAVs) ID3D11UnorderedAccessView * const * ppUnorderedAccessViews,
                                                        /* [annotation] */
                                                        _In_reads_opt_(NumUAVs) const UINT * pUAVInitialCounts);

    void(STDMETHODCALLTYPE * CSSetShader)(ID3D11DeviceContext * This,
                                          /* [annotation] */
                                          _In_opt_ ID3D11ComputeShader * pComputeShader,
                                          /* [annotation] */
                                          _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances, UINT NumClassInstances);

    void(STDMETHODCALLTYPE * CSSetSamplers)(ID3D11DeviceContext * This,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                            /* [annotation] */
                                            _In_reads_opt_(NumSamplers) ID3D11SamplerState * const * ppSamplers);

    void(STDMETHODCALLTYPE * CSSetConstantBuffers)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
                                                   /* [annotation] */
                                                   _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers);

    void(STDMETHODCALLTYPE * VSGetConstantBuffers)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
                                                   /* [annotation] */
                                                   _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers);

    void(STDMETHODCALLTYPE * PSGetShaderResources)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
                                                   /* [annotation] */
                                                   _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews);

    void(STDMETHODCALLTYPE * PSGetShader)(ID3D11DeviceContext * This,
                                          /* [annotation] */
                                          _Out_ ID3D11PixelShader ** ppPixelShader,
                                          /* [annotation] */
                                          _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                          /* [annotation] */
                                          _Inout_opt_ UINT * pNumClassInstances);

    void(STDMETHODCALLTYPE * PSGetSamplers)(ID3D11DeviceContext * This,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                            /* [annotation] */
                                            _Out_writes_opt_(NumSamplers) ID3D11SamplerState ** ppSamplers);

    void(STDMETHODCALLTYPE * VSGetShader)(ID3D11DeviceContext * This,
                                          /* [annotation] */
                                          _Out_ ID3D11VertexShader ** ppVertexShader,
                                          /* [annotation] */
                                          _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                          /* [annotation] */
                                          _Inout_opt_ UINT * pNumClassInstances);

    void(STDMETHODCALLTYPE * PSGetConstantBuffers)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
                                                   /* [annotation] */
                                                   _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers);

    void(STDMETHODCALLTYPE * IAGetInputLayout)(ID3D11DeviceContext * This,
                                               /* [annotation] */
                                               _Out_ ID3D11InputLayout ** ppInputLayout);

    void(STDMETHODCALLTYPE * IAGetVertexBuffers)(ID3D11DeviceContext * This,
                                                 /* [annotation] */
                                                 _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                 /* [annotation] */
                                                 _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumBuffers,
                                                 /* [annotation] */
                                                 _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppVertexBuffers,
                                                 /* [annotation] */
                                                 _Out_writes_opt_(NumBuffers) UINT * pStrides,
                                                 /* [annotation] */
                                                 _Out_writes_opt_(NumBuffers) UINT * pOffsets);

    void(STDMETHODCALLTYPE * IAGetIndexBuffer)(ID3D11DeviceContext * This,
                                               /* [annotation] */
                                               _Out_opt_ ID3D11Buffer ** pIndexBuffer,
                                               /* [annotation] */
                                               _Out_opt_ DXGI_FORMAT * Format,
                                               /* [annotation] */
                                               _Out_opt_ UINT * Offset);

    void(STDMETHODCALLTYPE * GSGetConstantBuffers)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
                                                   /* [annotation] */
                                                   _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers);

    void(STDMETHODCALLTYPE * GSGetShader)(ID3D11DeviceContext * This,
                                          /* [annotation] */
                                          _Out_ ID3D11GeometryShader ** ppGeometryShader,
                                          /* [annotation] */
                                          _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                          /* [annotation] */
                                          _Inout_opt_ UINT * pNumClassInstances);

    void(STDMETHODCALLTYPE * IAGetPrimitiveTopology)(ID3D11DeviceContext * This,
                                                     /* [annotation] */
                                                     _Out_ D3D11_PRIMITIVE_TOPOLOGY * pTopology);

    void(STDMETHODCALLTYPE * VSGetShaderResources)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
                                                   /* [annotation] */
                                                   _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews);

    void(STDMETHODCALLTYPE * VSGetSamplers)(ID3D11DeviceContext * This,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                            /* [annotation] */
                                            _Out_writes_opt_(NumSamplers) ID3D11SamplerState ** ppSamplers);

    void(STDMETHODCALLTYPE * GetPredication)(ID3D11DeviceContext * This,
                                             /* [annotation] */
                                             _Out_opt_ ID3D11Predicate ** ppPredicate,
                                             /* [annotation] */
                                             _Out_opt_ BOOL * pPredicateValue);

    void(STDMETHODCALLTYPE * GSGetShaderResources)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
                                                   /* [annotation] */
                                                   _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews);

    void(STDMETHODCALLTYPE * GSGetSamplers)(ID3D11DeviceContext * This,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                            /* [annotation] */
                                            _Out_writes_opt_(NumSamplers) ID3D11SamplerState ** ppSamplers);

    void(STDMETHODCALLTYPE * OMGetRenderTargets)(ID3D11DeviceContext * This,
                                                 /* [annotation] */
                                                 _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumViews,
                                                 /* [annotation] */
                                                 _Out_writes_opt_(NumViews) ID3D11RenderTargetView ** ppRenderTargetViews,
                                                 /* [annotation] */
                                                 _Out_opt_ ID3D11DepthStencilView ** ppDepthStencilView);

    void(STDMETHODCALLTYPE * OMGetRenderTargetsAndUnorderedAccessViews)(ID3D11DeviceContext * This,
                                                                        /* [annotation] */
                                                                        _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumRTVs,
                                                                        /* [annotation] */
                                                                        _Out_writes_opt_(NumRTVs) ID3D11RenderTargetView ** ppRenderTargetViews,
                                                                        /* [annotation] */
                                                                        _Out_opt_ ID3D11DepthStencilView ** ppDepthStencilView,
                                                                        /* [annotation] */
                                                                        _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1) UINT UAVStartSlot,
                                                                        /* [annotation] */
                                                                        _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot) UINT NumUAVs,
                                                                        /* [annotation] */
                                                                        _Out_writes_opt_(NumUAVs) ID3D11UnorderedAccessView ** ppUnorderedAccessViews);

    void(STDMETHODCALLTYPE * OMGetBlendState)(ID3D11DeviceContext * This,
                                              /* [annotation] */
                                              _Out_opt_ ID3D11BlendState ** ppBlendState,
                                              /* [annotation] */
                                              _Out_opt_ FLOAT BlendFactor[4],
                                              /* [annotation] */
                                              _Out_opt_ UINT * pSampleMask);

    void(STDMETHODCALLTYPE * OMGetDepthStencilState)(ID3D11DeviceContext * This,
                                                     /* [annotation] */
                                                     _Out_opt_ ID3D11DepthStencilState ** ppDepthStencilState,
                                                     /* [annotation] */
                                                     _Out_opt_ UINT * pStencilRef);

    void(STDMETHODCALLTYPE * SOGetTargets)(ID3D11DeviceContext * This,
                                           /* [annotation] */
                                           _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumBuffers,
                                           /* [annotation] */
                                           _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppSOTargets);

    void(STDMETHODCALLTYPE * RSGetState)(ID3D11DeviceContext * This,
                                         /* [annotation] */
                                         _Out_ ID3D11RasterizerState ** ppRasterizerState);

    void(STDMETHODCALLTYPE * RSGetViewports)(ID3D11DeviceContext * This,
                                             /* [annotation] */
                                             _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/ UINT * pNumViewports,
                                             /* [annotation] */
                                             _Out_writes_opt_(*pNumViewports) D3D11_VIEWPORT * pViewports);

    void(STDMETHODCALLTYPE * RSGetScissorRects)(ID3D11DeviceContext * This,
                                                /* [annotation] */
                                                _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/ UINT * pNumRects,
                                                /* [annotation] */
                                                _Out_writes_opt_(*pNumRects) D3D11_RECT * pRects);

    void(STDMETHODCALLTYPE * HSGetShaderResources)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
                                                   /* [annotation] */
                                                   _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews);

    void(STDMETHODCALLTYPE * HSGetShader)(ID3D11DeviceContext * This,
                                          /* [annotation] */
                                          _Out_ ID3D11HullShader ** ppHullShader,
                                          /* [annotation] */
                                          _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                          /* [annotation] */
                                          _Inout_opt_ UINT * pNumClassInstances);

    void(STDMETHODCALLTYPE * HSGetSamplers)(ID3D11DeviceContext * This,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                            /* [annotation] */
                                            _Out_writes_opt_(NumSamplers) ID3D11SamplerState ** ppSamplers);

    void(STDMETHODCALLTYPE * HSGetConstantBuffers)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
                                                   /* [annotation] */
                                                   _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers);

    void(STDMETHODCALLTYPE * DSGetShaderResources)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
                                                   /* [annotation] */
                                                   _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews);

    void(STDMETHODCALLTYPE * DSGetShader)(ID3D11DeviceContext * This,
                                          /* [annotation] */
                                          _Out_ ID3D11DomainShader ** ppDomainShader,
                                          /* [annotation] */
                                          _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                          /* [annotation] */
                                          _Inout_opt_ UINT * pNumClassInstances);

    void(STDMETHODCALLTYPE * DSGetSamplers)(ID3D11DeviceContext * This,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                            /* [annotation] */
                                            _Out_writes_opt_(NumSamplers) ID3D11SamplerState ** ppSamplers);

    void(STDMETHODCALLTYPE * DSGetConstantBuffers)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
                                                   /* [annotation] */
                                                   _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers);

    void(STDMETHODCALLTYPE * CSGetShaderResources)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumViews,
                                                   /* [annotation] */
                                                   _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews);

    void(STDMETHODCALLTYPE * CSGetUnorderedAccessViews)(ID3D11DeviceContext * This,
                                                        /* [annotation] */
                                                        _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1) UINT StartSlot,
                                                        /* [annotation] */
                                                        _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot) UINT NumUAVs,
                                                        /* [annotation] */
                                                        _Out_writes_opt_(NumUAVs) ID3D11UnorderedAccessView ** ppUnorderedAccessViews);

    void(STDMETHODCALLTYPE * CSGetShader)(ID3D11DeviceContext * This,
                                          /* [annotation] */
                                          _Out_ ID3D11ComputeShader ** ppComputeShader,
                                          /* [annotation] */
                                          _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                          /* [annotation] */
                                          _Inout_opt_ UINT * pNumClassInstances);

    void(STDMETHODCALLTYPE * CSGetSamplers)(ID3D11DeviceContext * This,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT StartSlot,
                                            /* [annotation] */
                                            _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                            /* [annotation] */
                                            _Out_writes_opt_(NumSamplers) ID3D11SamplerState ** ppSamplers);

    void(STDMETHODCALLTYPE * CSGetConstantBuffers)(ID3D11DeviceContext * This,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
                                                   /* [annotation] */
                                                   _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
                                                   /* [annotation] */
                                                   _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers);

    void(STDMETHODCALLTYPE * ClearState)(ID3D11DeviceContext * This);

    void(STDMETHODCALLTYPE * Flush)(ID3D11DeviceContext * This);

    D3D11_DEVICE_CONTEXT_TYPE(STDMETHODCALLTYPE * GetType)(ID3D11DeviceContext * This);

    UINT(STDMETHODCALLTYPE * GetContextFlags)(ID3D11DeviceContext * This);

    HRESULT(STDMETHODCALLTYPE * FinishCommandList)
    (ID3D11DeviceContext * This, BOOL RestoreDeferredContextState,
     /* [annotation] */
     _Out_opt_ ID3D11CommandList ** ppCommandList);

    END_INTERFACE
} ID3D11DeviceContextVtbl;

interface ID3D11DeviceContext {
    CONST_VTBL struct ID3D11DeviceContextVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11DeviceContext_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11DeviceContext_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11DeviceContext_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11DeviceContext_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11DeviceContext_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11DeviceContext_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11DeviceContext_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11DeviceContext_VSSetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers) \
                    ((This)->lpVtbl->VSSetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers))

                #define ID3D11DeviceContext_PSSetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews) \
                    ((This)->lpVtbl->PSSetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews))

                #define ID3D11DeviceContext_PSSetShader(This, pPixelShader, ppClassInstances, NumClassInstances) \
                    ((This)->lpVtbl->PSSetShader(This, pPixelShader, ppClassInstances, NumClassInstances))

                #define ID3D11DeviceContext_PSSetSamplers(This, StartSlot, NumSamplers, ppSamplers) \
                    ((This)->lpVtbl->PSSetSamplers(This, StartSlot, NumSamplers, ppSamplers))

                #define ID3D11DeviceContext_VSSetShader(This, pVertexShader, ppClassInstances, NumClassInstances) \
                    ((This)->lpVtbl->VSSetShader(This, pVertexShader, ppClassInstances, NumClassInstances))

                #define ID3D11DeviceContext_DrawIndexed(This, IndexCount, StartIndexLocation, BaseVertexLocation) \
                    ((This)->lpVtbl->DrawIndexed(This, IndexCount, StartIndexLocation, BaseVertexLocation))

                #define ID3D11DeviceContext_Draw(This, VertexCount, StartVertexLocation) ((This)->lpVtbl->Draw(This, VertexCount, StartVertexLocation))

                #define ID3D11DeviceContext_Map(This, pResource, Subresource, MapType, MapFlags, pMappedResource) \
                    ((This)->lpVtbl->Map(This, pResource, Subresource, MapType, MapFlags, pMappedResource))

                #define ID3D11DeviceContext_Unmap(This, pResource, Subresource) ((This)->lpVtbl->Unmap(This, pResource, Subresource))

                #define ID3D11DeviceContext_PSSetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers) \
                    ((This)->lpVtbl->PSSetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers))

                #define ID3D11DeviceContext_IASetInputLayout(This, pInputLayout) ((This)->lpVtbl->IASetInputLayout(This, pInputLayout))

                #define ID3D11DeviceContext_IASetVertexBuffers(This, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets) \
                    ((This)->lpVtbl->IASetVertexBuffers(This, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets))

                #define ID3D11DeviceContext_IASetIndexBuffer(This, pIndexBuffer, Format, Offset) \
                    ((This)->lpVtbl->IASetIndexBuffer(This, pIndexBuffer, Format, Offset))

                #define ID3D11DeviceContext_DrawIndexedInstanced(This, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, \
                                                                 StartInstanceLocation)                                                              \
                    ((This)->lpVtbl->DrawIndexedInstanced(This, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation,        \
                                                          StartInstanceLocation))

                #define ID3D11DeviceContext_DrawInstanced(This, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation) \
                    ((This)->lpVtbl->DrawInstanced(This, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation))

                #define ID3D11DeviceContext_GSSetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers) \
                    ((This)->lpVtbl->GSSetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers))

                #define ID3D11DeviceContext_GSSetShader(This, pShader, ppClassInstances, NumClassInstances) \
                    ((This)->lpVtbl->GSSetShader(This, pShader, ppClassInstances, NumClassInstances))

                #define ID3D11DeviceContext_IASetPrimitiveTopology(This, Topology) ((This)->lpVtbl->IASetPrimitiveTopology(This, Topology))

                #define ID3D11DeviceContext_VSSetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews) \
                    ((This)->lpVtbl->VSSetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews))

                #define ID3D11DeviceContext_VSSetSamplers(This, StartSlot, NumSamplers, ppSamplers) \
                    ((This)->lpVtbl->VSSetSamplers(This, StartSlot, NumSamplers, ppSamplers))

                #define ID3D11DeviceContext_Begin(This, pAsync) ((This)->lpVtbl->Begin(This, pAsync))

                #define ID3D11DeviceContext_End(This, pAsync) ((This)->lpVtbl->End(This, pAsync))

                #define ID3D11DeviceContext_GetData(This, pAsync, pData, DataSize, GetDataFlags) \
                    ((This)->lpVtbl->GetData(This, pAsync, pData, DataSize, GetDataFlags))

                #define ID3D11DeviceContext_SetPredication(This, pPredicate, PredicateValue) ((This)->lpVtbl->SetPredication(This, pPredicate, PredicateValue))

                #define ID3D11DeviceContext_GSSetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews) \
                    ((This)->lpVtbl->GSSetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews))

                #define ID3D11DeviceContext_GSSetSamplers(This, StartSlot, NumSamplers, ppSamplers) \
                    ((This)->lpVtbl->GSSetSamplers(This, StartSlot, NumSamplers, ppSamplers))

                #define ID3D11DeviceContext_OMSetRenderTargets(This, NumViews, ppRenderTargetViews, pDepthStencilView) \
                    ((This)->lpVtbl->OMSetRenderTargets(This, NumViews, ppRenderTargetViews, pDepthStencilView))

                #define ID3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews(This, NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot,   \
                                                                                      NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts)                    \
                    ((This)->lpVtbl->OMSetRenderTargetsAndUnorderedAccessViews(This, NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, \
                                                                               ppUnorderedAccessViews, pUAVInitialCounts))

                #define ID3D11DeviceContext_OMSetBlendState(This, pBlendState, BlendFactor, SampleMask) \
                    ((This)->lpVtbl->OMSetBlendState(This, pBlendState, BlendFactor, SampleMask))

                #define ID3D11DeviceContext_OMSetDepthStencilState(This, pDepthStencilState, StencilRef) \
                    ((This)->lpVtbl->OMSetDepthStencilState(This, pDepthStencilState, StencilRef))

                #define ID3D11DeviceContext_SOSetTargets(This, NumBuffers, ppSOTargets, pOffsets) \
                    ((This)->lpVtbl->SOSetTargets(This, NumBuffers, ppSOTargets, pOffsets))

                #define ID3D11DeviceContext_DrawAuto(This) ((This)->lpVtbl->DrawAuto(This))

                #define ID3D11DeviceContext_DrawIndexedInstancedIndirect(This, pBufferForArgs, AlignedByteOffsetForArgs) \
                    ((This)->lpVtbl->DrawIndexedInstancedIndirect(This, pBufferForArgs, AlignedByteOffsetForArgs))

                #define ID3D11DeviceContext_DrawInstancedIndirect(This, pBufferForArgs, AlignedByteOffsetForArgs) \
                    ((This)->lpVtbl->DrawInstancedIndirect(This, pBufferForArgs, AlignedByteOffsetForArgs))

                #define ID3D11DeviceContext_Dispatch(This, ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ) \
                    ((This)->lpVtbl->Dispatch(This, ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ))

                #define ID3D11DeviceContext_DispatchIndirect(This, pBufferForArgs, AlignedByteOffsetForArgs) \
                    ((This)->lpVtbl->DispatchIndirect(This, pBufferForArgs, AlignedByteOffsetForArgs))

                #define ID3D11DeviceContext_RSSetState(This, pRasterizerState) ((This)->lpVtbl->RSSetState(This, pRasterizerState))

                #define ID3D11DeviceContext_RSSetViewports(This, NumViewports, pViewports) ((This)->lpVtbl->RSSetViewports(This, NumViewports, pViewports))

                #define ID3D11DeviceContext_RSSetScissorRects(This, NumRects, pRects) ((This)->lpVtbl->RSSetScissorRects(This, NumRects, pRects))

                #define ID3D11DeviceContext_CopySubresourceRegion(This, pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox) \
                    ((This)->lpVtbl->CopySubresourceRegion(This, pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox))

                #define ID3D11DeviceContext_CopyResource(This, pDstResource, pSrcResource) ((This)->lpVtbl->CopyResource(This, pDstResource, pSrcResource))

                #define ID3D11DeviceContext_UpdateSubresource(This, pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch) \
                    ((This)->lpVtbl->UpdateSubresource(This, pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch))

                #define ID3D11DeviceContext_CopyStructureCount(This, pDstBuffer, DstAlignedByteOffset, pSrcView) \
                    ((This)->lpVtbl->CopyStructureCount(This, pDstBuffer, DstAlignedByteOffset, pSrcView))

                #define ID3D11DeviceContext_ClearRenderTargetView(This, pRenderTargetView, ColorRGBA) \
                    ((This)->lpVtbl->ClearRenderTargetView(This, pRenderTargetView, ColorRGBA))

                #define ID3D11DeviceContext_ClearUnorderedAccessViewUint(This, pUnorderedAccessView, Values) \
                    ((This)->lpVtbl->ClearUnorderedAccessViewUint(This, pUnorderedAccessView, Values))

                #define ID3D11DeviceContext_ClearUnorderedAccessViewFloat(This, pUnorderedAccessView, Values) \
                    ((This)->lpVtbl->ClearUnorderedAccessViewFloat(This, pUnorderedAccessView, Values))

                #define ID3D11DeviceContext_ClearDepthStencilView(This, pDepthStencilView, ClearFlags, Depth, Stencil) \
                    ((This)->lpVtbl->ClearDepthStencilView(This, pDepthStencilView, ClearFlags, Depth, Stencil))

                #define ID3D11DeviceContext_GenerateMips(This, pShaderResourceView) ((This)->lpVtbl->GenerateMips(This, pShaderResourceView))

                #define ID3D11DeviceContext_SetResourceMinLOD(This, pResource, MinLOD) ((This)->lpVtbl->SetResourceMinLOD(This, pResource, MinLOD))

                #define ID3D11DeviceContext_GetResourceMinLOD(This, pResource) ((This)->lpVtbl->GetResourceMinLOD(This, pResource))

                #define ID3D11DeviceContext_ResolveSubresource(This, pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format) \
                    ((This)->lpVtbl->ResolveSubresource(This, pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format))

                #define ID3D11DeviceContext_ExecuteCommandList(This, pCommandList, RestoreContextState) \
                    ((This)->lpVtbl->ExecuteCommandList(This, pCommandList, RestoreContextState))

                #define ID3D11DeviceContext_HSSetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews) \
                    ((This)->lpVtbl->HSSetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews))

                #define ID3D11DeviceContext_HSSetShader(This, pHullShader, ppClassInstances, NumClassInstances) \
                    ((This)->lpVtbl->HSSetShader(This, pHullShader, ppClassInstances, NumClassInstances))

                #define ID3D11DeviceContext_HSSetSamplers(This, StartSlot, NumSamplers, ppSamplers) \
                    ((This)->lpVtbl->HSSetSamplers(This, StartSlot, NumSamplers, ppSamplers))

                #define ID3D11DeviceContext_HSSetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers) \
                    ((This)->lpVtbl->HSSetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers))

                #define ID3D11DeviceContext_DSSetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews) \
                    ((This)->lpVtbl->DSSetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews))

                #define ID3D11DeviceContext_DSSetShader(This, pDomainShader, ppClassInstances, NumClassInstances) \
                    ((This)->lpVtbl->DSSetShader(This, pDomainShader, ppClassInstances, NumClassInstances))

                #define ID3D11DeviceContext_DSSetSamplers(This, StartSlot, NumSamplers, ppSamplers) \
                    ((This)->lpVtbl->DSSetSamplers(This, StartSlot, NumSamplers, ppSamplers))

                #define ID3D11DeviceContext_DSSetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers) \
                    ((This)->lpVtbl->DSSetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers))

                #define ID3D11DeviceContext_CSSetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews) \
                    ((This)->lpVtbl->CSSetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews))

                #define ID3D11DeviceContext_CSSetUnorderedAccessViews(This, StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts) \
                    ((This)->lpVtbl->CSSetUnorderedAccessViews(This, StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts))

                #define ID3D11DeviceContext_CSSetShader(This, pComputeShader, ppClassInstances, NumClassInstances) \
                    ((This)->lpVtbl->CSSetShader(This, pComputeShader, ppClassInstances, NumClassInstances))

                #define ID3D11DeviceContext_CSSetSamplers(This, StartSlot, NumSamplers, ppSamplers) \
                    ((This)->lpVtbl->CSSetSamplers(This, StartSlot, NumSamplers, ppSamplers))

                #define ID3D11DeviceContext_CSSetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers) \
                    ((This)->lpVtbl->CSSetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers))

                #define ID3D11DeviceContext_VSGetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers) \
                    ((This)->lpVtbl->VSGetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers))

                #define ID3D11DeviceContext_PSGetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews) \
                    ((This)->lpVtbl->PSGetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews))

                #define ID3D11DeviceContext_PSGetShader(This, ppPixelShader, ppClassInstances, pNumClassInstances) \
                    ((This)->lpVtbl->PSGetShader(This, ppPixelShader, ppClassInstances, pNumClassInstances))

                #define ID3D11DeviceContext_PSGetSamplers(This, StartSlot, NumSamplers, ppSamplers) \
                    ((This)->lpVtbl->PSGetSamplers(This, StartSlot, NumSamplers, ppSamplers))

                #define ID3D11DeviceContext_VSGetShader(This, ppVertexShader, ppClassInstances, pNumClassInstances) \
                    ((This)->lpVtbl->VSGetShader(This, ppVertexShader, ppClassInstances, pNumClassInstances))

                #define ID3D11DeviceContext_PSGetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers) \
                    ((This)->lpVtbl->PSGetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers))

                #define ID3D11DeviceContext_IAGetInputLayout(This, ppInputLayout) ((This)->lpVtbl->IAGetInputLayout(This, ppInputLayout))

                #define ID3D11DeviceContext_IAGetVertexBuffers(This, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets) \
                    ((This)->lpVtbl->IAGetVertexBuffers(This, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets))

                #define ID3D11DeviceContext_IAGetIndexBuffer(This, pIndexBuffer, Format, Offset) \
                    ((This)->lpVtbl->IAGetIndexBuffer(This, pIndexBuffer, Format, Offset))

                #define ID3D11DeviceContext_GSGetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers) \
                    ((This)->lpVtbl->GSGetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers))

                #define ID3D11DeviceContext_GSGetShader(This, ppGeometryShader, ppClassInstances, pNumClassInstances) \
                    ((This)->lpVtbl->GSGetShader(This, ppGeometryShader, ppClassInstances, pNumClassInstances))

                #define ID3D11DeviceContext_IAGetPrimitiveTopology(This, pTopology) ((This)->lpVtbl->IAGetPrimitiveTopology(This, pTopology))

                #define ID3D11DeviceContext_VSGetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews) \
                    ((This)->lpVtbl->VSGetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews))

                #define ID3D11DeviceContext_VSGetSamplers(This, StartSlot, NumSamplers, ppSamplers) \
                    ((This)->lpVtbl->VSGetSamplers(This, StartSlot, NumSamplers, ppSamplers))

                #define ID3D11DeviceContext_GetPredication(This, ppPredicate, pPredicateValue) \
                    ((This)->lpVtbl->GetPredication(This, ppPredicate, pPredicateValue))

                #define ID3D11DeviceContext_GSGetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews) \
                    ((This)->lpVtbl->GSGetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews))

                #define ID3D11DeviceContext_GSGetSamplers(This, StartSlot, NumSamplers, ppSamplers) \
                    ((This)->lpVtbl->GSGetSamplers(This, StartSlot, NumSamplers, ppSamplers))

                #define ID3D11DeviceContext_OMGetRenderTargets(This, NumViews, ppRenderTargetViews, ppDepthStencilView) \
                    ((This)->lpVtbl->OMGetRenderTargets(This, NumViews, ppRenderTargetViews, ppDepthStencilView))

                #define ID3D11DeviceContext_OMGetRenderTargetsAndUnorderedAccessViews(This, NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot,   \
                                                                                      NumUAVs, ppUnorderedAccessViews)                                        \
                    ((This)->lpVtbl->OMGetRenderTargetsAndUnorderedAccessViews(This, NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, \
                                                                               ppUnorderedAccessViews))

                #define ID3D11DeviceContext_OMGetBlendState(This, ppBlendState, BlendFactor, pSampleMask) \
                    ((This)->lpVtbl->OMGetBlendState(This, ppBlendState, BlendFactor, pSampleMask))

                #define ID3D11DeviceContext_OMGetDepthStencilState(This, ppDepthStencilState, pStencilRef) \
                    ((This)->lpVtbl->OMGetDepthStencilState(This, ppDepthStencilState, pStencilRef))

                #define ID3D11DeviceContext_SOGetTargets(This, NumBuffers, ppSOTargets) ((This)->lpVtbl->SOGetTargets(This, NumBuffers, ppSOTargets))

                #define ID3D11DeviceContext_RSGetState(This, ppRasterizerState) ((This)->lpVtbl->RSGetState(This, ppRasterizerState))

                #define ID3D11DeviceContext_RSGetViewports(This, pNumViewports, pViewports) ((This)->lpVtbl->RSGetViewports(This, pNumViewports, pViewports))

                #define ID3D11DeviceContext_RSGetScissorRects(This, pNumRects, pRects) ((This)->lpVtbl->RSGetScissorRects(This, pNumRects, pRects))

                #define ID3D11DeviceContext_HSGetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews) \
                    ((This)->lpVtbl->HSGetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews))

                #define ID3D11DeviceContext_HSGetShader(This, ppHullShader, ppClassInstances, pNumClassInstances) \
                    ((This)->lpVtbl->HSGetShader(This, ppHullShader, ppClassInstances, pNumClassInstances))

                #define ID3D11DeviceContext_HSGetSamplers(This, StartSlot, NumSamplers, ppSamplers) \
                    ((This)->lpVtbl->HSGetSamplers(This, StartSlot, NumSamplers, ppSamplers))

                #define ID3D11DeviceContext_HSGetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers) \
                    ((This)->lpVtbl->HSGetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers))

                #define ID3D11DeviceContext_DSGetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews) \
                    ((This)->lpVtbl->DSGetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews))

                #define ID3D11DeviceContext_DSGetShader(This, ppDomainShader, ppClassInstances, pNumClassInstances) \
                    ((This)->lpVtbl->DSGetShader(This, ppDomainShader, ppClassInstances, pNumClassInstances))

                #define ID3D11DeviceContext_DSGetSamplers(This, StartSlot, NumSamplers, ppSamplers) \
                    ((This)->lpVtbl->DSGetSamplers(This, StartSlot, NumSamplers, ppSamplers))

                #define ID3D11DeviceContext_DSGetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers) \
                    ((This)->lpVtbl->DSGetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers))

                #define ID3D11DeviceContext_CSGetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews) \
                    ((This)->lpVtbl->CSGetShaderResources(This, StartSlot, NumViews, ppShaderResourceViews))

                #define ID3D11DeviceContext_CSGetUnorderedAccessViews(This, StartSlot, NumUAVs, ppUnorderedAccessViews) \
                    ((This)->lpVtbl->CSGetUnorderedAccessViews(This, StartSlot, NumUAVs, ppUnorderedAccessViews))

                #define ID3D11DeviceContext_CSGetShader(This, ppComputeShader, ppClassInstances, pNumClassInstances) \
                    ((This)->lpVtbl->CSGetShader(This, ppComputeShader, ppClassInstances, pNumClassInstances))

                #define ID3D11DeviceContext_CSGetSamplers(This, StartSlot, NumSamplers, ppSamplers) \
                    ((This)->lpVtbl->CSGetSamplers(This, StartSlot, NumSamplers, ppSamplers))

                #define ID3D11DeviceContext_CSGetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers) \
                    ((This)->lpVtbl->CSGetConstantBuffers(This, StartSlot, NumBuffers, ppConstantBuffers))

                #define ID3D11DeviceContext_ClearState(This) ((This)->lpVtbl->ClearState(This))

                #define ID3D11DeviceContext_Flush(This) ((This)->lpVtbl->Flush(This))

                #define ID3D11DeviceContext_GetType(This) ((This)->lpVtbl->GetType(This))

                #define ID3D11DeviceContext_GetContextFlags(This) ((This)->lpVtbl->GetContextFlags(This))

                #define ID3D11DeviceContext_FinishCommandList(This, RestoreDeferredContextState, ppCommandList) \
                    ((This)->lpVtbl->FinishCommandList(This, RestoreDeferredContextState, ppCommandList))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11DeviceContext_INTERFACE_DEFINED__ */

    /* interface __MIDL_itf_d3d11_0000_0030 */
    /* [local] */

    #if !defined(D3D11_VIDEO_NO_HELPERS) && defined(__cplusplus)
}
struct CD3D11_VIDEO_DEFAULT {};
extern const DECLSPEC_SELECTANY CD3D11_VIDEO_DEFAULT D3D11_VIDEO_DEFAULT;
extern "C" {
    #endif

DEFINE_GUID(D3D11_DECODER_PROFILE_MPEG2_MOCOMP, 0xe6a9f44b, 0x61b0, 0x4563, 0x9e, 0xa4, 0x63, 0xd2, 0xa3, 0xc6, 0xfe, 0x66);
DEFINE_GUID(D3D11_DECODER_PROFILE_MPEG2_IDCT, 0xbf22ad00, 0x03ea, 0x4690, 0x80, 0x77, 0x47, 0x33, 0x46, 0x20, 0x9b, 0x7e);
DEFINE_GUID(D3D11_DECODER_PROFILE_MPEG2_VLD, 0xee27417f, 0x5e28, 0x4e65, 0xbe, 0xea, 0x1d, 0x26, 0xb5, 0x08, 0xad, 0xc9);
DEFINE_GUID(D3D11_DECODER_PROFILE_MPEG1_VLD, 0x6f3ec719, 0x3735, 0x42cc, 0x80, 0x63, 0x65, 0xcc, 0x3c, 0xb3, 0x66, 0x16);
DEFINE_GUID(D3D11_DECODER_PROFILE_MPEG2and1_VLD, 0x86695f12, 0x340e, 0x4f04, 0x9f, 0xd3, 0x92, 0x53, 0xdd, 0x32, 0x74, 0x60);
DEFINE_GUID(D3D11_DECODER_PROFILE_H264_MOCOMP_NOFGT, 0x1b81be64, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_H264_MOCOMP_FGT, 0x1b81be65, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_H264_IDCT_NOFGT, 0x1b81be66, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_H264_IDCT_FGT, 0x1b81be67, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_H264_VLD_NOFGT, 0x1b81be68, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_H264_VLD_FGT, 0x1b81be69, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_H264_VLD_WITHFMOASO_NOFGT, 0xd5f04ff9, 0x3418, 0x45d8, 0x95, 0x61, 0x32, 0xa7, 0x6a, 0xae, 0x2d, 0xdd);
DEFINE_GUID(D3D11_DECODER_PROFILE_H264_VLD_STEREO_PROGRESSIVE_NOFGT, 0xd79be8da, 0x0cf1, 0x4c81, 0xb8, 0x2a, 0x69, 0xa4, 0xe2, 0x36, 0xf4, 0x3d);
DEFINE_GUID(D3D11_DECODER_PROFILE_H264_VLD_STEREO_NOFGT, 0xf9aaccbb, 0xc2b6, 0x4cfc, 0x87, 0x79, 0x57, 0x07, 0xb1, 0x76, 0x05, 0x52);
DEFINE_GUID(D3D11_DECODER_PROFILE_H264_VLD_MULTIVIEW_NOFGT, 0x705b9d82, 0x76cf, 0x49d6, 0xb7, 0xe6, 0xac, 0x88, 0x72, 0xdb, 0x01, 0x3c);
DEFINE_GUID(D3D11_DECODER_PROFILE_WMV8_POSTPROC, 0x1b81be80, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_WMV8_MOCOMP, 0x1b81be81, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_WMV9_POSTPROC, 0x1b81be90, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_WMV9_MOCOMP, 0x1b81be91, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_WMV9_IDCT, 0x1b81be94, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_VC1_POSTPROC, 0x1b81beA0, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_VC1_MOCOMP, 0x1b81beA1, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_VC1_IDCT, 0x1b81beA2, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_VC1_VLD, 0x1b81beA3, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_VC1_D2010, 0x1b81beA4, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
DEFINE_GUID(D3D11_DECODER_PROFILE_MPEG4PT2_VLD_SIMPLE, 0xefd64d74, 0xc9e8, 0x41d7, 0xa5, 0xe9, 0xe9, 0xb0, 0xe3, 0x9f, 0xa3, 0x19);
DEFINE_GUID(D3D11_DECODER_PROFILE_MPEG4PT2_VLD_ADVSIMPLE_NOGMC, 0xed418a9f, 0x010d, 0x4eda, 0x9a, 0xe3, 0x9a, 0x65, 0x35, 0x8d, 0x8d, 0x2e);
DEFINE_GUID(D3D11_DECODER_PROFILE_MPEG4PT2_VLD_ADVSIMPLE_GMC, 0xab998b5b, 0x4258, 0x44a9, 0x9f, 0xeb, 0x94, 0xe5, 0x97, 0xa6, 0xba, 0xae);
typedef struct D3D11_VIDEO_DECODER_DESC {
    GUID        Guid;
    UINT        SampleWidth;
    UINT        SampleHeight;
    DXGI_FORMAT OutputFormat;
} D3D11_VIDEO_DECODER_DESC;

typedef struct D3D11_VIDEO_DECODER_CONFIG {
    GUID   guidConfigBitstreamEncryption;
    GUID   guidConfigMBcontrolEncryption;
    GUID   guidConfigResidDiffEncryption;
    UINT   ConfigBitstreamRaw;
    UINT   ConfigMBcontrolRasterOrder;
    UINT   ConfigResidDiffHost;
    UINT   ConfigSpatialResid8;
    UINT   ConfigResid8Subtraction;
    UINT   ConfigSpatialHost8or9Clipping;
    UINT   ConfigSpatialResidInterleaved;
    UINT   ConfigIntraResidUnsigned;
    UINT   ConfigResidDiffAccelerator;
    UINT   ConfigHostInverseScan;
    UINT   ConfigSpecificIDCT;
    UINT   Config4GroupedCoefs;
    USHORT ConfigMinRenderTargetBuffCount;
    USHORT ConfigDecoderSpecific;
} D3D11_VIDEO_DECODER_CONFIG;

typedef enum D3D11_VIDEO_DECODER_BUFFER_TYPE {
    D3D11_VIDEO_DECODER_BUFFER_PICTURE_PARAMETERS          = 0,
    D3D11_VIDEO_DECODER_BUFFER_MACROBLOCK_CONTROL          = 1,
    D3D11_VIDEO_DECODER_BUFFER_RESIDUAL_DIFFERENCE         = 2,
    D3D11_VIDEO_DECODER_BUFFER_DEBLOCKING_CONTROL          = 3,
    D3D11_VIDEO_DECODER_BUFFER_INVERSE_QUANTIZATION_MATRIX = 4,
    D3D11_VIDEO_DECODER_BUFFER_SLICE_CONTROL               = 5,
    D3D11_VIDEO_DECODER_BUFFER_BITSTREAM                   = 6,
    D3D11_VIDEO_DECODER_BUFFER_MOTION_VECTOR               = 7,
    D3D11_VIDEO_DECODER_BUFFER_FILM_GRAIN                  = 8
} D3D11_VIDEO_DECODER_BUFFER_TYPE;

typedef struct _D3D11_AES_CTR_IV {
    UINT64 IV;
    UINT64 Count;
} D3D11_AES_CTR_IV;

typedef struct D3D11_ENCRYPTED_BLOCK_INFO {
    UINT NumEncryptedBytesAtBeginning;
    UINT NumBytesInSkipPattern;
    UINT NumBytesInEncryptPattern;
} D3D11_ENCRYPTED_BLOCK_INFO;

typedef struct D3D11_VIDEO_DECODER_BUFFER_DESC {
    D3D11_VIDEO_DECODER_BUFFER_TYPE BufferType;
    UINT                            BufferIndex;
    UINT                            DataOffset;
    UINT                            DataSize;
    UINT                            FirstMBaddress;
    UINT                            NumMBsInBuffer;
    UINT                            Width;
    UINT                            Height;
    UINT                            Stride;
    UINT                            ReservedBits;
    void *                          pIV;
    UINT                            IVSize;
    BOOL                            PartialEncryption;
    D3D11_ENCRYPTED_BLOCK_INFO      EncryptedBlockInfo;
} D3D11_VIDEO_DECODER_BUFFER_DESC;

typedef struct D3D11_VIDEO_DECODER_EXTENSION {
    UINT              Function;
    void *            pPrivateInputData;
    UINT              PrivateInputDataSize;
    void *            pPrivateOutputData;
    UINT              PrivateOutputDataSize;
    UINT              ResourceCount;
    ID3D11Resource ** ppResourceList;
} D3D11_VIDEO_DECODER_EXTENSION;

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0030_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0030_v0_0_s_ifspec;

    #ifndef __ID3D11VideoDecoder_INTERFACE_DEFINED__
        #define __ID3D11VideoDecoder_INTERFACE_DEFINED__

/* interface ID3D11VideoDecoder */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11VideoDecoder;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("3C9C5B51-995D-48d1-9B8D-FA5CAEDED65C")
ID3D11VideoDecoder : public ID3D11DeviceChild {
public:
    virtual HRESULT STDMETHODCALLTYPE GetCreationParameters(
        /* [annotation] */
        _Out_ D3D11_VIDEO_DECODER_DESC * pVideoDesc,
        /* [annotation] */
        _Out_ D3D11_VIDEO_DECODER_CONFIG * pConfig) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetDriverHandle(
        /* [annotation] */
        _Out_ HANDLE * pDriverHandle) = 0;
};

        #else /* C style interface */

typedef struct ID3D11VideoDecoderVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11VideoDecoder * This,
     /* [in] */ REFIID    riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11VideoDecoder * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11VideoDecoder * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11VideoDecoder * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11VideoDecoder * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11VideoDecoder * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11VideoDecoder * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    HRESULT(STDMETHODCALLTYPE * GetCreationParameters)
    (ID3D11VideoDecoder * This,
     /* [annotation] */
     _Out_ D3D11_VIDEO_DECODER_DESC * pVideoDesc,
     /* [annotation] */
     _Out_ D3D11_VIDEO_DECODER_CONFIG * pConfig);

    HRESULT(STDMETHODCALLTYPE * GetDriverHandle)
    (ID3D11VideoDecoder * This,
     /* [annotation] */
     _Out_ HANDLE * pDriverHandle);

    END_INTERFACE
} ID3D11VideoDecoderVtbl;

interface ID3D11VideoDecoder {
    CONST_VTBL struct ID3D11VideoDecoderVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11VideoDecoder_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11VideoDecoder_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11VideoDecoder_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11VideoDecoder_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11VideoDecoder_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11VideoDecoder_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11VideoDecoder_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11VideoDecoder_GetCreationParameters(This, pVideoDesc, pConfig) ((This)->lpVtbl->GetCreationParameters(This, pVideoDesc, pConfig))

                #define ID3D11VideoDecoder_GetDriverHandle(This, pDriverHandle) ((This)->lpVtbl->GetDriverHandle(This, pDriverHandle))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11VideoDecoder_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0031 */
/* [local] */

typedef enum D3D11_VIDEO_PROCESSOR_FORMAT_SUPPORT {
    D3D11_VIDEO_PROCESSOR_FORMAT_SUPPORT_INPUT  = 0x1,
    D3D11_VIDEO_PROCESSOR_FORMAT_SUPPORT_OUTPUT = 0x2
} D3D11_VIDEO_PROCESSOR_FORMAT_SUPPORT;

typedef enum D3D11_VIDEO_PROCESSOR_DEVICE_CAPS {
    D3D11_VIDEO_PROCESSOR_DEVICE_CAPS_LINEAR_SPACE            = 0x1,
    D3D11_VIDEO_PROCESSOR_DEVICE_CAPS_xvYCC                   = 0x2,
    D3D11_VIDEO_PROCESSOR_DEVICE_CAPS_RGB_RANGE_CONVERSION    = 0x4,
    D3D11_VIDEO_PROCESSOR_DEVICE_CAPS_YCbCr_MATRIX_CONVERSION = 0x8
} D3D11_VIDEO_PROCESSOR_DEVICE_CAPS;

typedef enum D3D11_VIDEO_PROCESSOR_FEATURE_CAPS {
    D3D11_VIDEO_PROCESSOR_FEATURE_CAPS_ALPHA_FILL         = 0x1,
    D3D11_VIDEO_PROCESSOR_FEATURE_CAPS_CONSTRICTION       = 0x2,
    D3D11_VIDEO_PROCESSOR_FEATURE_CAPS_LUMA_KEY           = 0x4,
    D3D11_VIDEO_PROCESSOR_FEATURE_CAPS_ALPHA_PALETTE      = 0x8,
    D3D11_VIDEO_PROCESSOR_FEATURE_CAPS_LEGACY             = 0x10,
    D3D11_VIDEO_PROCESSOR_FEATURE_CAPS_STEREO             = 0x20,
    D3D11_VIDEO_PROCESSOR_FEATURE_CAPS_ROTATION           = 0x40,
    D3D11_VIDEO_PROCESSOR_FEATURE_CAPS_ALPHA_STREAM       = 0x80,
    D3D11_VIDEO_PROCESSOR_FEATURE_CAPS_PIXEL_ASPECT_RATIO = 0x100
} D3D11_VIDEO_PROCESSOR_FEATURE_CAPS;

typedef enum D3D11_VIDEO_PROCESSOR_FILTER_CAPS {
    D3D11_VIDEO_PROCESSOR_FILTER_CAPS_BRIGHTNESS         = 0x1,
    D3D11_VIDEO_PROCESSOR_FILTER_CAPS_CONTRAST           = 0x2,
    D3D11_VIDEO_PROCESSOR_FILTER_CAPS_HUE                = 0x4,
    D3D11_VIDEO_PROCESSOR_FILTER_CAPS_SATURATION         = 0x8,
    D3D11_VIDEO_PROCESSOR_FILTER_CAPS_NOISE_REDUCTION    = 0x10,
    D3D11_VIDEO_PROCESSOR_FILTER_CAPS_EDGE_ENHANCEMENT   = 0x20,
    D3D11_VIDEO_PROCESSOR_FILTER_CAPS_ANAMORPHIC_SCALING = 0x40,
    D3D11_VIDEO_PROCESSOR_FILTER_CAPS_STEREO_ADJUSTMENT  = 0x80
} D3D11_VIDEO_PROCESSOR_FILTER_CAPS;

typedef enum D3D11_VIDEO_PROCESSOR_FORMAT_CAPS {
    D3D11_VIDEO_PROCESSOR_FORMAT_CAPS_RGB_INTERLACED     = 0x1,
    D3D11_VIDEO_PROCESSOR_FORMAT_CAPS_RGB_PROCAMP        = 0x2,
    D3D11_VIDEO_PROCESSOR_FORMAT_CAPS_RGB_LUMA_KEY       = 0x4,
    D3D11_VIDEO_PROCESSOR_FORMAT_CAPS_PALETTE_INTERLACED = 0x8
} D3D11_VIDEO_PROCESSOR_FORMAT_CAPS;

typedef enum D3D11_VIDEO_PROCESSOR_AUTO_STREAM_CAPS {
    D3D11_VIDEO_PROCESSOR_AUTO_STREAM_CAPS_DENOISE             = 0x1,
    D3D11_VIDEO_PROCESSOR_AUTO_STREAM_CAPS_DERINGING           = 0x2,
    D3D11_VIDEO_PROCESSOR_AUTO_STREAM_CAPS_EDGE_ENHANCEMENT    = 0x4,
    D3D11_VIDEO_PROCESSOR_AUTO_STREAM_CAPS_COLOR_CORRECTION    = 0x8,
    D3D11_VIDEO_PROCESSOR_AUTO_STREAM_CAPS_FLESH_TONE_MAPPING  = 0x10,
    D3D11_VIDEO_PROCESSOR_AUTO_STREAM_CAPS_IMAGE_STABILIZATION = 0x20,
    D3D11_VIDEO_PROCESSOR_AUTO_STREAM_CAPS_SUPER_RESOLUTION    = 0x40,
    D3D11_VIDEO_PROCESSOR_AUTO_STREAM_CAPS_ANAMORPHIC_SCALING  = 0x80
} D3D11_VIDEO_PROCESSOR_AUTO_STREAM_CAPS;

typedef enum D3D11_VIDEO_PROCESSOR_STEREO_CAPS {
    D3D11_VIDEO_PROCESSOR_STEREO_CAPS_MONO_OFFSET        = 0x1,
    D3D11_VIDEO_PROCESSOR_STEREO_CAPS_ROW_INTERLEAVED    = 0x2,
    D3D11_VIDEO_PROCESSOR_STEREO_CAPS_COLUMN_INTERLEAVED = 0x4,
    D3D11_VIDEO_PROCESSOR_STEREO_CAPS_CHECKERBOARD       = 0x8,
    D3D11_VIDEO_PROCESSOR_STEREO_CAPS_FLIP_MODE          = 0x10
} D3D11_VIDEO_PROCESSOR_STEREO_CAPS;

typedef struct D3D11_VIDEO_PROCESSOR_CAPS {
    UINT DeviceCaps;
    UINT FeatureCaps;
    UINT FilterCaps;
    UINT InputFormatCaps;
    UINT AutoStreamCaps;
    UINT StereoCaps;
    UINT RateConversionCapsCount;
    UINT MaxInputStreams;
    UINT MaxStreamStates;
} D3D11_VIDEO_PROCESSOR_CAPS;

typedef enum D3D11_VIDEO_PROCESSOR_PROCESSOR_CAPS {
    D3D11_VIDEO_PROCESSOR_PROCESSOR_CAPS_DEINTERLACE_BLEND               = 0x1,
    D3D11_VIDEO_PROCESSOR_PROCESSOR_CAPS_DEINTERLACE_BOB                 = 0x2,
    D3D11_VIDEO_PROCESSOR_PROCESSOR_CAPS_DEINTERLACE_ADAPTIVE            = 0x4,
    D3D11_VIDEO_PROCESSOR_PROCESSOR_CAPS_DEINTERLACE_MOTION_COMPENSATION = 0x8,
    D3D11_VIDEO_PROCESSOR_PROCESSOR_CAPS_INVERSE_TELECINE                = 0x10,
    D3D11_VIDEO_PROCESSOR_PROCESSOR_CAPS_FRAME_RATE_CONVERSION           = 0x20
} D3D11_VIDEO_PROCESSOR_PROCESSOR_CAPS;

typedef enum D3D11_VIDEO_PROCESSOR_ITELECINE_CAPS {
    D3D11_VIDEO_PROCESSOR_ITELECINE_CAPS_32           = 0x1,
    D3D11_VIDEO_PROCESSOR_ITELECINE_CAPS_22           = 0x2,
    D3D11_VIDEO_PROCESSOR_ITELECINE_CAPS_2224         = 0x4,
    D3D11_VIDEO_PROCESSOR_ITELECINE_CAPS_2332         = 0x8,
    D3D11_VIDEO_PROCESSOR_ITELECINE_CAPS_32322        = 0x10,
    D3D11_VIDEO_PROCESSOR_ITELECINE_CAPS_55           = 0x20,
    D3D11_VIDEO_PROCESSOR_ITELECINE_CAPS_64           = 0x40,
    D3D11_VIDEO_PROCESSOR_ITELECINE_CAPS_87           = 0x80,
    D3D11_VIDEO_PROCESSOR_ITELECINE_CAPS_222222222223 = 0x100,
    D3D11_VIDEO_PROCESSOR_ITELECINE_CAPS_OTHER        = 0x80000000
} D3D11_VIDEO_PROCESSOR_ITELECINE_CAPS;

typedef struct D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS {
    UINT PastFrames;
    UINT FutureFrames;
    UINT ProcessorCaps;
    UINT ITelecineCaps;
    UINT CustomRateCount;
} D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS;

typedef enum D3D11_CONTENT_PROTECTION_CAPS {
    D3D11_CONTENT_PROTECTION_CAPS_SOFTWARE                = 0x1,
    D3D11_CONTENT_PROTECTION_CAPS_HARDWARE                = 0x2,
    D3D11_CONTENT_PROTECTION_CAPS_PROTECTION_ALWAYS_ON    = 0x4,
    D3D11_CONTENT_PROTECTION_CAPS_PARTIAL_DECRYPTION      = 0x8,
    D3D11_CONTENT_PROTECTION_CAPS_CONTENT_KEY             = 0x10,
    D3D11_CONTENT_PROTECTION_CAPS_FRESHEN_SESSION_KEY     = 0x20,
    D3D11_CONTENT_PROTECTION_CAPS_ENCRYPTED_READ_BACK     = 0x40,
    D3D11_CONTENT_PROTECTION_CAPS_ENCRYPTED_READ_BACK_KEY = 0x80,
    D3D11_CONTENT_PROTECTION_CAPS_SEQUENTIAL_CTR_IV       = 0x100,
    D3D11_CONTENT_PROTECTION_CAPS_ENCRYPT_SLICEDATA_ONLY  = 0x200,
    D3D11_CONTENT_PROTECTION_CAPS_DECRYPTION_BLT          = 0x400
} D3D11_CONTENT_PROTECTION_CAPS;

DEFINE_GUID(D3D11_CRYPTO_TYPE_AES128_CTR, 0x9b6bd711, 0x4f74, 0x41c9, 0x9e, 0x7b, 0xb, 0xe2, 0xd7, 0xd9, 0x3b, 0x4f);
typedef struct D3D11_VIDEO_CONTENT_PROTECTION_CAPS {
    UINT      Caps;
    UINT      KeyExchangeTypeCount;
    UINT      BlockAlignmentSize;
    ULONGLONG ProtectedMemorySize;
} D3D11_VIDEO_CONTENT_PROTECTION_CAPS;

typedef struct D3D11_VIDEO_PROCESSOR_CUSTOM_RATE {
    DXGI_RATIONAL CustomRate;
    UINT          OutputFrames;
    BOOL          InputInterlaced;
    UINT          InputFramesOrFields;
} D3D11_VIDEO_PROCESSOR_CUSTOM_RATE;

typedef enum D3D11_VIDEO_PROCESSOR_FILTER {
    D3D11_VIDEO_PROCESSOR_FILTER_BRIGHTNESS         = 0,
    D3D11_VIDEO_PROCESSOR_FILTER_CONTRAST           = 1,
    D3D11_VIDEO_PROCESSOR_FILTER_HUE                = 2,
    D3D11_VIDEO_PROCESSOR_FILTER_SATURATION         = 3,
    D3D11_VIDEO_PROCESSOR_FILTER_NOISE_REDUCTION    = 4,
    D3D11_VIDEO_PROCESSOR_FILTER_EDGE_ENHANCEMENT   = 5,
    D3D11_VIDEO_PROCESSOR_FILTER_ANAMORPHIC_SCALING = 6,
    D3D11_VIDEO_PROCESSOR_FILTER_STEREO_ADJUSTMENT  = 7
} D3D11_VIDEO_PROCESSOR_FILTER;

typedef struct D3D11_VIDEO_PROCESSOR_FILTER_RANGE {
    int   Minimum;
    int   Maximum;
    int   Default;
    float Multiplier;
} D3D11_VIDEO_PROCESSOR_FILTER_RANGE;

typedef enum D3D11_VIDEO_FRAME_FORMAT {
    D3D11_VIDEO_FRAME_FORMAT_PROGRESSIVE                   = 0,
    D3D11_VIDEO_FRAME_FORMAT_INTERLACED_TOP_FIELD_FIRST    = 1,
    D3D11_VIDEO_FRAME_FORMAT_INTERLACED_BOTTOM_FIELD_FIRST = 2
} D3D11_VIDEO_FRAME_FORMAT;

typedef enum D3D11_VIDEO_USAGE {
    D3D11_VIDEO_USAGE_PLAYBACK_NORMAL = 0,
    D3D11_VIDEO_USAGE_OPTIMAL_SPEED   = 1,
    D3D11_VIDEO_USAGE_OPTIMAL_QUALITY = 2
} D3D11_VIDEO_USAGE;

typedef struct D3D11_VIDEO_PROCESSOR_CONTENT_DESC {
    D3D11_VIDEO_FRAME_FORMAT InputFrameFormat;
    DXGI_RATIONAL            InputFrameRate;
    UINT                     InputWidth;
    UINT                     InputHeight;
    DXGI_RATIONAL            OutputFrameRate;
    UINT                     OutputWidth;
    UINT                     OutputHeight;
    D3D11_VIDEO_USAGE        Usage;
} D3D11_VIDEO_PROCESSOR_CONTENT_DESC;

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0031_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0031_v0_0_s_ifspec;

    #ifndef __ID3D11VideoProcessorEnumerator_INTERFACE_DEFINED__
        #define __ID3D11VideoProcessorEnumerator_INTERFACE_DEFINED__

/* interface ID3D11VideoProcessorEnumerator */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11VideoProcessorEnumerator;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("31627037-53AB-4200-9061-05FAA9AB45F9")
ID3D11VideoProcessorEnumerator : public ID3D11DeviceChild {
public:
    virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorContentDesc(
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pContentDesc) = 0;

    virtual HRESULT STDMETHODCALLTYPE CheckVideoProcessorFormat(
        /* [annotation] */
        _In_ DXGI_FORMAT Format,
        /* [annotation] */
        _Out_ UINT * pFlags) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorCaps(
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_CAPS * pCaps) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorRateConversionCaps(
        /* [annotation] */
        _In_ UINT TypeIndex,
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorCustomRate(
        /* [annotation] */
        _In_ UINT TypeIndex,
        /* [annotation] */
        _In_ UINT CustomRateIndex,
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_CUSTOM_RATE * pRate) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetVideoProcessorFilterRange(
        /* [annotation] */
        _In_ D3D11_VIDEO_PROCESSOR_FILTER Filter,
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_FILTER_RANGE * pRange) = 0;
};

        #else /* C style interface */

typedef struct ID3D11VideoProcessorEnumeratorVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11VideoProcessorEnumerator * This,
     /* [in] */ REFIID                riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11VideoProcessorEnumerator * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11VideoProcessorEnumerator * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11VideoProcessorEnumerator * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11VideoProcessorEnumerator * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11VideoProcessorEnumerator * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11VideoProcessorEnumerator * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    HRESULT(STDMETHODCALLTYPE * GetVideoProcessorContentDesc)
    (ID3D11VideoProcessorEnumerator * This,
     /* [annotation] */
     _Out_ D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pContentDesc);

    HRESULT(STDMETHODCALLTYPE * CheckVideoProcessorFormat)
    (ID3D11VideoProcessorEnumerator * This,
     /* [annotation] */
     _In_ DXGI_FORMAT Format,
     /* [annotation] */
     _Out_ UINT * pFlags);

    HRESULT(STDMETHODCALLTYPE * GetVideoProcessorCaps)
    (ID3D11VideoProcessorEnumerator * This,
     /* [annotation] */
     _Out_ D3D11_VIDEO_PROCESSOR_CAPS * pCaps);

    HRESULT(STDMETHODCALLTYPE * GetVideoProcessorRateConversionCaps)
    (ID3D11VideoProcessorEnumerator * This,
     /* [annotation] */
     _In_ UINT TypeIndex,
     /* [annotation] */
     _Out_ D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps);

    HRESULT(STDMETHODCALLTYPE * GetVideoProcessorCustomRate)
    (ID3D11VideoProcessorEnumerator * This,
     /* [annotation] */
     _In_ UINT TypeIndex,
     /* [annotation] */
     _In_ UINT CustomRateIndex,
     /* [annotation] */
     _Out_ D3D11_VIDEO_PROCESSOR_CUSTOM_RATE * pRate);

    HRESULT(STDMETHODCALLTYPE * GetVideoProcessorFilterRange)
    (ID3D11VideoProcessorEnumerator * This,
     /* [annotation] */
     _In_ D3D11_VIDEO_PROCESSOR_FILTER Filter,
     /* [annotation] */
     _Out_ D3D11_VIDEO_PROCESSOR_FILTER_RANGE * pRange);

    END_INTERFACE
} ID3D11VideoProcessorEnumeratorVtbl;

interface ID3D11VideoProcessorEnumerator {
    CONST_VTBL struct ID3D11VideoProcessorEnumeratorVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11VideoProcessorEnumerator_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11VideoProcessorEnumerator_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11VideoProcessorEnumerator_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11VideoProcessorEnumerator_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11VideoProcessorEnumerator_GetPrivateData(This, guid, pDataSize, pData) \
                    ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11VideoProcessorEnumerator_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11VideoProcessorEnumerator_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11VideoProcessorEnumerator_GetVideoProcessorContentDesc(This, pContentDesc) \
                    ((This)->lpVtbl->GetVideoProcessorContentDesc(This, pContentDesc))

                #define ID3D11VideoProcessorEnumerator_CheckVideoProcessorFormat(This, Format, pFlags) \
                    ((This)->lpVtbl->CheckVideoProcessorFormat(This, Format, pFlags))

                #define ID3D11VideoProcessorEnumerator_GetVideoProcessorCaps(This, pCaps) ((This)->lpVtbl->GetVideoProcessorCaps(This, pCaps))

                #define ID3D11VideoProcessorEnumerator_GetVideoProcessorRateConversionCaps(This, TypeIndex, pCaps) \
                    ((This)->lpVtbl->GetVideoProcessorRateConversionCaps(This, TypeIndex, pCaps))

                #define ID3D11VideoProcessorEnumerator_GetVideoProcessorCustomRate(This, TypeIndex, CustomRateIndex, pRate) \
                    ((This)->lpVtbl->GetVideoProcessorCustomRate(This, TypeIndex, CustomRateIndex, pRate))

                #define ID3D11VideoProcessorEnumerator_GetVideoProcessorFilterRange(This, Filter, pRange) \
                    ((This)->lpVtbl->GetVideoProcessorFilterRange(This, Filter, pRange))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11VideoProcessorEnumerator_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0032 */
/* [local] */

typedef struct D3D11_VIDEO_COLOR_RGBA {
    float R;
    float G;
    float B;
    float A;
} D3D11_VIDEO_COLOR_RGBA;

typedef struct D3D11_VIDEO_COLOR_YCbCrA {
    float Y;
    float Cb;
    float Cr;
    float A;
} D3D11_VIDEO_COLOR_YCbCrA;

typedef struct D3D11_VIDEO_COLOR {
    union {
        D3D11_VIDEO_COLOR_YCbCrA YCbCr;
        D3D11_VIDEO_COLOR_RGBA   RGBA;
    };
} D3D11_VIDEO_COLOR;

typedef struct D3D11_VIDEO_PROCESSOR_COLOR_SPACE {
    UINT Usage        : 1;
    UINT RGB_Range    : 1;
    UINT YCbCr_Matrix : 1;
    UINT YCbCr_xvYCC  : 1;
    UINT Reserved     : 28;
} D3D11_VIDEO_PROCESSOR_COLOR_SPACE;

typedef enum D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE {
    D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE_OPAQUE        = 0,
    D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE_BACKGROUND    = 1,
    D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE_DESTINATION   = 2,
    D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE_SOURCE_STREAM = 3
} D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE;

typedef enum D3D11_VIDEO_PROCESSOR_OUTPUT_RATE {
    D3D11_VIDEO_PROCESSOR_OUTPUT_RATE_NORMAL = 0,
    D3D11_VIDEO_PROCESSOR_OUTPUT_RATE_HALF   = 1,
    D3D11_VIDEO_PROCESSOR_OUTPUT_RATE_CUSTOM = 2
} D3D11_VIDEO_PROCESSOR_OUTPUT_RATE;

typedef enum D3D11_VIDEO_PROCESSOR_STEREO_FORMAT {
    D3D11_VIDEO_PROCESSOR_STEREO_FORMAT_MONO               = 0,
    D3D11_VIDEO_PROCESSOR_STEREO_FORMAT_HORIZONTAL         = 1,
    D3D11_VIDEO_PROCESSOR_STEREO_FORMAT_VERTICAL           = 2,
    D3D11_VIDEO_PROCESSOR_STEREO_FORMAT_SEPARATE           = 3,
    D3D11_VIDEO_PROCESSOR_STEREO_FORMAT_MONO_OFFSET        = 4,
    D3D11_VIDEO_PROCESSOR_STEREO_FORMAT_ROW_INTERLEAVED    = 5,
    D3D11_VIDEO_PROCESSOR_STEREO_FORMAT_COLUMN_INTERLEAVED = 6,
    D3D11_VIDEO_PROCESSOR_STEREO_FORMAT_CHECKERBOARD       = 7
} D3D11_VIDEO_PROCESSOR_STEREO_FORMAT;

typedef enum D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE {
    D3D11_VIDEO_PROCESSOR_STEREO_FLIP_NONE   = 0,
    D3D11_VIDEO_PROCESSOR_STEREO_FLIP_FRAME0 = 1,
    D3D11_VIDEO_PROCESSOR_STEREO_FLIP_FRAME1 = 2
} D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE;

typedef enum D3D11_VIDEO_PROCESSOR_ROTATION {
    D3D11_VIDEO_PROCESSOR_ROTATION_IDENTITY = 0,
    D3D11_VIDEO_PROCESSOR_ROTATION_90       = 1,
    D3D11_VIDEO_PROCESSOR_ROTATION_180      = 2,
    D3D11_VIDEO_PROCESSOR_ROTATION_270      = 3
} D3D11_VIDEO_PROCESSOR_ROTATION;

typedef struct D3D11_VIDEO_PROCESSOR_STREAM {
    BOOL                             Enable;
    UINT                             OutputIndex;
    UINT                             InputFrameOrField;
    UINT                             PastFrames;
    UINT                             FutureFrames;
    ID3D11VideoProcessorInputView ** ppPastSurfaces;
    ID3D11VideoProcessorInputView *  pInputSurface;
    ID3D11VideoProcessorInputView ** ppFutureSurfaces;
    ID3D11VideoProcessorInputView ** ppPastSurfacesRight;
    ID3D11VideoProcessorInputView *  pInputSurfaceRight;
    ID3D11VideoProcessorInputView ** ppFutureSurfacesRight;
} D3D11_VIDEO_PROCESSOR_STREAM;

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0032_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0032_v0_0_s_ifspec;

    #ifndef __ID3D11VideoProcessor_INTERFACE_DEFINED__
        #define __ID3D11VideoProcessor_INTERFACE_DEFINED__

/* interface ID3D11VideoProcessor */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11VideoProcessor;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("1D7B0652-185F-41c6-85CE-0C5BE3D4AE6C")
ID3D11VideoProcessor : public ID3D11DeviceChild {
public:
    virtual void STDMETHODCALLTYPE GetContentDesc(
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc) = 0;

    virtual void STDMETHODCALLTYPE GetRateConversionCaps(
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps) = 0;
};

        #else /* C style interface */

typedef struct ID3D11VideoProcessorVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11VideoProcessor * This,
     /* [in] */ REFIID      riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11VideoProcessor * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11VideoProcessor * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11VideoProcessor * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11VideoProcessor * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11VideoProcessor * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11VideoProcessor * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetContentDesc)(ID3D11VideoProcessor * This,
                                             /* [annotation] */
                                             _Out_ D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc);

    void(STDMETHODCALLTYPE * GetRateConversionCaps)(ID3D11VideoProcessor * This,
                                                    /* [annotation] */
                                                    _Out_ D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps);

    END_INTERFACE
} ID3D11VideoProcessorVtbl;

interface ID3D11VideoProcessor {
    CONST_VTBL struct ID3D11VideoProcessorVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11VideoProcessor_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11VideoProcessor_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11VideoProcessor_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11VideoProcessor_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11VideoProcessor_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11VideoProcessor_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11VideoProcessor_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11VideoProcessor_GetContentDesc(This, pDesc) ((This)->lpVtbl->GetContentDesc(This, pDesc))

                #define ID3D11VideoProcessor_GetRateConversionCaps(This, pCaps) ((This)->lpVtbl->GetRateConversionCaps(This, pCaps))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11VideoProcessor_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0033 */
/* [local] */

typedef struct D3D11_OMAC {
    BYTE Omac[16];
} D3D11_OMAC;

typedef enum D3D11_AUTHENTICATED_CHANNEL_TYPE {
    D3D11_AUTHENTICATED_CHANNEL_D3D11           = 1,
    D3D11_AUTHENTICATED_CHANNEL_DRIVER_SOFTWARE = 2,
    D3D11_AUTHENTICATED_CHANNEL_DRIVER_HARDWARE = 3
} D3D11_AUTHENTICATED_CHANNEL_TYPE;

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0033_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0033_v0_0_s_ifspec;

    #ifndef __ID3D11AuthenticatedChannel_INTERFACE_DEFINED__
        #define __ID3D11AuthenticatedChannel_INTERFACE_DEFINED__

/* interface ID3D11AuthenticatedChannel */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11AuthenticatedChannel;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("3015A308-DCBD-47aa-A747-192486D14D4A")
ID3D11AuthenticatedChannel : public ID3D11DeviceChild {
public:
    virtual HRESULT STDMETHODCALLTYPE GetCertificateSize(
        /* [annotation] */
        _Out_ UINT * pCertificateSize) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetCertificate(
        /* [annotation] */
        _In_ UINT CertificateSize,
        /* [annotation] */
        _Out_writes_bytes_(CertificateSize) BYTE * pCertificate) = 0;

    virtual void STDMETHODCALLTYPE GetChannelHandle(
        /* [annotation] */
        _Out_ HANDLE * pChannelHandle) = 0;
};

        #else /* C style interface */

typedef struct ID3D11AuthenticatedChannelVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11AuthenticatedChannel * This,
     /* [in] */ REFIID            riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11AuthenticatedChannel * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11AuthenticatedChannel * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11AuthenticatedChannel * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11AuthenticatedChannel * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11AuthenticatedChannel * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11AuthenticatedChannel * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    HRESULT(STDMETHODCALLTYPE * GetCertificateSize)
    (ID3D11AuthenticatedChannel * This,
     /* [annotation] */
     _Out_ UINT * pCertificateSize);

    HRESULT(STDMETHODCALLTYPE * GetCertificate)
    (ID3D11AuthenticatedChannel * This,
     /* [annotation] */
     _In_ UINT CertificateSize,
     /* [annotation] */
     _Out_writes_bytes_(CertificateSize) BYTE * pCertificate);

    void(STDMETHODCALLTYPE * GetChannelHandle)(ID3D11AuthenticatedChannel * This,
                                               /* [annotation] */
                                               _Out_ HANDLE * pChannelHandle);

    END_INTERFACE
} ID3D11AuthenticatedChannelVtbl;

interface ID3D11AuthenticatedChannel {
    CONST_VTBL struct ID3D11AuthenticatedChannelVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11AuthenticatedChannel_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11AuthenticatedChannel_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11AuthenticatedChannel_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11AuthenticatedChannel_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11AuthenticatedChannel_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11AuthenticatedChannel_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11AuthenticatedChannel_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11AuthenticatedChannel_GetCertificateSize(This, pCertificateSize) ((This)->lpVtbl->GetCertificateSize(This, pCertificateSize))

                #define ID3D11AuthenticatedChannel_GetCertificate(This, CertificateSize, pCertificate) \
                    ((This)->lpVtbl->GetCertificate(This, CertificateSize, pCertificate))

                #define ID3D11AuthenticatedChannel_GetChannelHandle(This, pChannelHandle) ((This)->lpVtbl->GetChannelHandle(This, pChannelHandle))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11AuthenticatedChannel_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0034 */
/* [local] */

DEFINE_GUID(D3D11_AUTHENTICATED_QUERY_PROTECTION, 0xa84eb584, 0xc495, 0x48aa, 0xb9, 0x4d, 0x8b, 0xd2, 0xd6, 0xfb, 0xce, 0x5);
DEFINE_GUID(D3D11_AUTHENTICATED_QUERY_CHANNEL_TYPE, 0xbc1b18a5, 0xb1fb, 0x42ab, 0xbd, 0x94, 0xb5, 0x82, 0x8b, 0x4b, 0xf7, 0xbe);
DEFINE_GUID(D3D11_AUTHENTICATED_QUERY_DEVICE_HANDLE, 0xec1c539d, 0x8cff, 0x4e2a, 0xbc, 0xc4, 0xf5, 0x69, 0x2f, 0x99, 0xf4, 0x80);
DEFINE_GUID(D3D11_AUTHENTICATED_QUERY_CRYPTO_SESSION, 0x2634499e, 0xd018, 0x4d74, 0xac, 0x17, 0x7f, 0x72, 0x40, 0x59, 0x52, 0x8d);
DEFINE_GUID(D3D11_AUTHENTICATED_QUERY_RESTRICTED_SHARED_RESOURCE_PROCESS_COUNT, 0xdb207b3, 0x9450, 0x46a6, 0x82, 0xde, 0x1b, 0x96, 0xd4, 0x4f, 0x9c, 0xf2);
DEFINE_GUID(D3D11_AUTHENTICATED_QUERY_RESTRICTED_SHARED_RESOURCE_PROCESS, 0x649bbadb, 0xf0f4, 0x4639, 0xa1, 0x5b, 0x24, 0x39, 0x3f, 0xc3, 0xab, 0xac);
DEFINE_GUID(D3D11_AUTHENTICATED_QUERY_UNRESTRICTED_PROTECTED_SHARED_RESOURCE_COUNT, 0x12f0bd6, 0xe662, 0x4474, 0xbe, 0xfd, 0xaa, 0x53, 0xe5, 0x14, 0x3c, 0x6d);
DEFINE_GUID(D3D11_AUTHENTICATED_QUERY_OUTPUT_ID_COUNT, 0x2c042b5e, 0x8c07, 0x46d5, 0xaa, 0xbe, 0x8f, 0x75, 0xcb, 0xad, 0x4c, 0x31);
DEFINE_GUID(D3D11_AUTHENTICATED_QUERY_OUTPUT_ID, 0x839ddca3, 0x9b4e, 0x41e4, 0xb0, 0x53, 0x89, 0x2b, 0xd2, 0xa1, 0x1e, 0xe7);
DEFINE_GUID(D3D11_AUTHENTICATED_QUERY_ACCESSIBILITY_ATTRIBUTES, 0x6214d9d2, 0x432c, 0x4abb, 0x9f, 0xce, 0x21, 0x6e, 0xea, 0x26, 0x9e, 0x3b);
DEFINE_GUID(D3D11_AUTHENTICATED_QUERY_ENCRYPTION_WHEN_ACCESSIBLE_GUID_COUNT, 0xb30f7066, 0x203c, 0x4b07, 0x93, 0xfc, 0xce, 0xaa, 0xfd, 0x61, 0x24, 0x1e);
DEFINE_GUID(D3D11_AUTHENTICATED_QUERY_ENCRYPTION_WHEN_ACCESSIBLE_GUID, 0xf83a5958, 0xe986, 0x4bda, 0xbe, 0xb0, 0x41, 0x1f, 0x6a, 0x7a, 0x1, 0xb7);
DEFINE_GUID(D3D11_AUTHENTICATED_QUERY_CURRENT_ENCRYPTION_WHEN_ACCESSIBLE, 0xec1791c7, 0xdad3, 0x4f15, 0x9e, 0xc3, 0xfa, 0xa9, 0x3d, 0x60, 0xd4, 0xf0);
DEFINE_GUID(D3D11_AUTHENTICATED_CONFIGURE_INITIALIZE, 0x6114bdb, 0x3523, 0x470a, 0x8d, 0xca, 0xfb, 0xc2, 0x84, 0x51, 0x54, 0xf0);
DEFINE_GUID(D3D11_AUTHENTICATED_CONFIGURE_PROTECTION, 0x50455658, 0x3f47, 0x4362, 0xbf, 0x99, 0xbf, 0xdf, 0xcd, 0xe9, 0xed, 0x29);
DEFINE_GUID(D3D11_AUTHENTICATED_CONFIGURE_CRYPTO_SESSION, 0x6346cc54, 0x2cfc, 0x4ad4, 0x82, 0x24, 0xd1, 0x58, 0x37, 0xde, 0x77, 0x0);
DEFINE_GUID(D3D11_AUTHENTICATED_CONFIGURE_SHARED_RESOURCE, 0x772d047, 0x1b40, 0x48e8, 0x9c, 0xa6, 0xb5, 0xf5, 0x10, 0xde, 0x9f, 0x1);
DEFINE_GUID(D3D11_AUTHENTICATED_CONFIGURE_ENCRYPTION_WHEN_ACCESSIBLE, 0x41fff286, 0x6ae0, 0x4d43, 0x9d, 0x55, 0xa4, 0x6e, 0x9e, 0xfd, 0x15, 0x8a);
typedef struct D3D11_AUTHENTICATED_QUERY_INPUT {
    GUID   QueryType;
    HANDLE hChannel;
    UINT   SequenceNumber;
} D3D11_AUTHENTICATED_QUERY_INPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_OUTPUT {
    D3D11_OMAC omac;
    GUID       QueryType;
    HANDLE     hChannel;
    UINT       SequenceNumber;
    HRESULT    ReturnCode;
} D3D11_AUTHENTICATED_QUERY_OUTPUT;

typedef union D3D11_AUTHENTICATED_PROTECTION_FLAGS {
    struct __MIDL___MIDL_itf_d3d11_0000_0034_0001 {
        UINT ProtectionEnabled           : 1;
        UINT OverlayOrFullscreenRequired : 1;
        UINT Reserved                    : 30;
    } Flags;
    UINT Value;
} D3D11_AUTHENTICATED_PROTECTION_FLAGS;

typedef struct D3D11_AUTHENTICATED_QUERY_PROTECTION_OUTPUT {
    D3D11_AUTHENTICATED_QUERY_OUTPUT     Output;
    D3D11_AUTHENTICATED_PROTECTION_FLAGS ProtectionFlags;
} D3D11_AUTHENTICATED_QUERY_PROTECTION_OUTPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_CHANNEL_TYPE_OUTPUT {
    D3D11_AUTHENTICATED_QUERY_OUTPUT Output;
    D3D11_AUTHENTICATED_CHANNEL_TYPE ChannelType;
} D3D11_AUTHENTICATED_QUERY_CHANNEL_TYPE_OUTPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_DEVICE_HANDLE_OUTPUT {
    D3D11_AUTHENTICATED_QUERY_OUTPUT Output;
    HANDLE                           DeviceHandle;
} D3D11_AUTHENTICATED_QUERY_DEVICE_HANDLE_OUTPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_CRYPTO_SESSION_INPUT {
    D3D11_AUTHENTICATED_QUERY_INPUT Input;
    HANDLE                          DecoderHandle;
} D3D11_AUTHENTICATED_QUERY_CRYPTO_SESSION_INPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_CRYPTO_SESSION_OUTPUT {
    D3D11_AUTHENTICATED_QUERY_OUTPUT Output;
    HANDLE                           DecoderHandle;
    HANDLE                           CryptoSessionHandle;
    HANDLE                           DeviceHandle;
} D3D11_AUTHENTICATED_QUERY_CRYPTO_SESSION_OUTPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_RESTRICTED_SHARED_RESOURCE_PROCESS_COUNT_OUTPUT {
    D3D11_AUTHENTICATED_QUERY_OUTPUT Output;
    UINT                             RestrictedSharedResourceProcessCount;
} D3D11_AUTHENTICATED_QUERY_RESTRICTED_SHARED_RESOURCE_PROCESS_COUNT_OUTPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_RESTRICTED_SHARED_RESOURCE_PROCESS_INPUT {
    D3D11_AUTHENTICATED_QUERY_INPUT Input;
    UINT                            ProcessIndex;
} D3D11_AUTHENTICATED_QUERY_RESTRICTED_SHARED_RESOURCE_PROCESS_INPUT;

typedef enum D3D11_AUTHENTICATED_PROCESS_IDENTIFIER_TYPE {
    D3D11_PROCESSIDTYPE_UNKNOWN = 0,
    D3D11_PROCESSIDTYPE_DWM     = 1,
    D3D11_PROCESSIDTYPE_HANDLE  = 2
} D3D11_AUTHENTICATED_PROCESS_IDENTIFIER_TYPE;

typedef struct D3D11_AUTHENTICATED_QUERY_RESTRICTED_SHARED_RESOURCE_PROCESS_OUTPUT {
    D3D11_AUTHENTICATED_QUERY_OUTPUT            Output;
    UINT                                        ProcessIndex;
    D3D11_AUTHENTICATED_PROCESS_IDENTIFIER_TYPE ProcessIdentifier;
    HANDLE                                      ProcessHandle;
} D3D11_AUTHENTICATED_QUERY_RESTRICTED_SHARED_RESOURCE_PROCESS_OUTPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_UNRESTRICTED_PROTECTED_SHARED_RESOURCE_COUNT_OUTPUT {
    D3D11_AUTHENTICATED_QUERY_OUTPUT Output;
    UINT                             UnrestrictedProtectedSharedResourceCount;
} D3D11_AUTHENTICATED_QUERY_UNRESTRICTED_PROTECTED_SHARED_RESOURCE_COUNT_OUTPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_OUTPUT_ID_COUNT_INPUT {
    D3D11_AUTHENTICATED_QUERY_INPUT Input;
    HANDLE                          DeviceHandle;
    HANDLE                          CryptoSessionHandle;
} D3D11_AUTHENTICATED_QUERY_OUTPUT_ID_COUNT_INPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_OUTPUT_ID_COUNT_OUTPUT {
    D3D11_AUTHENTICATED_QUERY_OUTPUT Output;
    HANDLE                           DeviceHandle;
    HANDLE                           CryptoSessionHandle;
    UINT                             OutputIDCount;
} D3D11_AUTHENTICATED_QUERY_OUTPUT_ID_COUNT_OUTPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_OUTPUT_ID_INPUT {
    D3D11_AUTHENTICATED_QUERY_INPUT Input;
    HANDLE                          DeviceHandle;
    HANDLE                          CryptoSessionHandle;
    UINT                            OutputIDIndex;
} D3D11_AUTHENTICATED_QUERY_OUTPUT_ID_INPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_OUTPUT_ID_OUTPUT {
    D3D11_AUTHENTICATED_QUERY_OUTPUT Output;
    HANDLE                           DeviceHandle;
    HANDLE                           CryptoSessionHandle;
    UINT                             OutputIDIndex;
    UINT64                           OutputID;
} D3D11_AUTHENTICATED_QUERY_OUTPUT_ID_OUTPUT;

typedef enum D3D11_BUS_TYPE {
    D3D11_BUS_TYPE_OTHER                                            = 0,
    D3D11_BUS_TYPE_PCI                                              = 0x1,
    D3D11_BUS_TYPE_PCIX                                             = 0x2,
    D3D11_BUS_TYPE_PCIEXPRESS                                       = 0x3,
    D3D11_BUS_TYPE_AGP                                              = 0x4,
    D3D11_BUS_IMPL_MODIFIER_INSIDE_OF_CHIPSET                       = 0x10000,
    D3D11_BUS_IMPL_MODIFIER_TRACKS_ON_MOTHER_BOARD_TO_CHIP          = 0x20000,
    D3D11_BUS_IMPL_MODIFIER_TRACKS_ON_MOTHER_BOARD_TO_SOCKET        = 0x30000,
    D3D11_BUS_IMPL_MODIFIER_DAUGHTER_BOARD_CONNECTOR                = 0x40000,
    D3D11_BUS_IMPL_MODIFIER_DAUGHTER_BOARD_CONNECTOR_INSIDE_OF_NUAE = 0x50000,
    D3D11_BUS_IMPL_MODIFIER_NON_STANDARD                            = 0x80000000
} D3D11_BUS_TYPE;

typedef struct D3D11_AUTHENTICATED_QUERY_ACESSIBILITY_OUTPUT {
    D3D11_AUTHENTICATED_QUERY_OUTPUT Output;
    D3D11_BUS_TYPE                   BusType;
    BOOL                             AccessibleInContiguousBlocks;
    BOOL                             AccessibleInNonContiguousBlocks;
} D3D11_AUTHENTICATED_QUERY_ACCESSIBILITY_OUTPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_ACCESSIBILITY_ENCRYPTION_GUID_COUNT_OUTPUT {
    D3D11_AUTHENTICATED_QUERY_OUTPUT Output;
    UINT                             EncryptionGuidCount;
} D3D11_AUTHENTICATED_QUERY_ACCESSIBILITY_ENCRYPTION_GUID_COUNT_OUTPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_ACCESSIBILITY_ENCRYPTION_GUID_INPUT {
    D3D11_AUTHENTICATED_QUERY_INPUT Input;
    UINT                            EncryptionGuidIndex;
} D3D11_AUTHENTICATED_QUERY_ACCESSIBILITY_ENCRYPTION_GUID_INPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_ACCESSIBILITY_ENCRYPTION_GUID_OUTPUT {
    D3D11_AUTHENTICATED_QUERY_OUTPUT Output;
    UINT                             EncryptionGuidIndex;
    GUID                             EncryptionGuid;
} D3D11_AUTHENTICATED_QUERY_ACCESSIBILITY_ENCRYPTION_GUID_OUTPUT;

typedef struct D3D11_AUTHENTICATED_QUERY_CURRENT_ACCESSIBILITY_ENCRYPTION_OUTPUT {
    D3D11_AUTHENTICATED_QUERY_OUTPUT Output;
    GUID                             EncryptionGuid;
} D3D11_AUTHENTICATED_QUERY_CURRENT_ACCESSIBILITY_ENCRYPTION_OUTPUT;

typedef struct D3D11_AUTHENTICATED_CONFIGURE_INPUT {
    D3D11_OMAC omac;
    GUID       ConfigureType;
    HANDLE     hChannel;
    UINT       SequenceNumber;
} D3D11_AUTHENTICATED_CONFIGURE_INPUT;

typedef struct D3D11_AUTHENTICATED_CONFIGURE_OUTPUT {
    D3D11_OMAC omac;
    GUID       ConfigureType;
    HANDLE     hChannel;
    UINT       SequenceNumber;
    HRESULT    ReturnCode;
} D3D11_AUTHENTICATED_CONFIGURE_OUTPUT;

typedef struct D3D11_AUTHENTICATED_CONFIGURE_INITIALIZE_INPUT {
    D3D11_AUTHENTICATED_CONFIGURE_INPUT Parameters;
    UINT                                StartSequenceQuery;
    UINT                                StartSequenceConfigure;
} D3D11_AUTHENTICATED_CONFIGURE_INITIALIZE_INPUT;

typedef struct D3D11_AUTHENTICATED_CONFIGURE_PROTECTION_INPUT {
    D3D11_AUTHENTICATED_CONFIGURE_INPUT  Parameters;
    D3D11_AUTHENTICATED_PROTECTION_FLAGS Protections;
} D3D11_AUTHENTICATED_CONFIGURE_PROTECTION_INPUT;

typedef struct D3D11_AUTHENTICATED_CONFIGURE_CRYPTO_SESSION_INPUT {
    D3D11_AUTHENTICATED_CONFIGURE_INPUT Parameters;
    HANDLE                              DecoderHandle;
    HANDLE                              CryptoSessionHandle;
    HANDLE                              DeviceHandle;
} D3D11_AUTHENTICATED_CONFIGURE_CRYPTO_SESSION_INPUT;

typedef struct D3D11_AUTHENTICATED_CONFIGURE_SHARED_RESOURCE_INPUT {
    D3D11_AUTHENTICATED_CONFIGURE_INPUT         Parameters;
    D3D11_AUTHENTICATED_PROCESS_IDENTIFIER_TYPE ProcessType;
    HANDLE                                      ProcessHandle;
    BOOL                                        AllowAccess;
} D3D11_AUTHENTICATED_CONFIGURE_SHARED_RESOURCE_INPUT;

typedef struct D3D11_AUTHENTICATED_CONFIGURE_ACCESSIBLE_ENCRYPTION_INPUT {
    D3D11_AUTHENTICATED_CONFIGURE_INPUT Parameters;
    GUID                                EncryptionGuid;
} D3D11_AUTHENTICATED_CONFIGURE_ACCESSIBLE_ENCRYPTION_INPUT;

DEFINE_GUID(D3D11_KEY_EXCHANGE_RSAES_OAEP, 0xc1949895, 0xd72a, 0x4a1d, 0x8e, 0x5d, 0xed, 0x85, 0x7d, 0x17, 0x15, 0x20);

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0034_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0034_v0_0_s_ifspec;

    #ifndef __ID3D11CryptoSession_INTERFACE_DEFINED__
        #define __ID3D11CryptoSession_INTERFACE_DEFINED__

/* interface ID3D11CryptoSession */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11CryptoSession;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("9B32F9AD-BDCC-40a6-A39D-D5C865845720")
ID3D11CryptoSession : public ID3D11DeviceChild {
public:
    virtual void STDMETHODCALLTYPE GetCryptoType(
        /* [annotation] */
        _Out_ GUID * pCryptoType) = 0;

    virtual void STDMETHODCALLTYPE GetDecoderProfile(
        /* [annotation] */
        _Out_ GUID * pDecoderProfile) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetCertificateSize(
        /* [annotation] */
        _Out_ UINT * pCertificateSize) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetCertificate(
        /* [annotation] */
        _In_ UINT CertificateSize,
        /* [annotation] */
        _Out_writes_bytes_(CertificateSize) BYTE * pCertificate) = 0;

    virtual void STDMETHODCALLTYPE GetCryptoSessionHandle(
        /* [annotation] */
        _Out_ HANDLE * pCryptoSessionHandle) = 0;
};

        #else /* C style interface */

typedef struct ID3D11CryptoSessionVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11CryptoSession * This,
     /* [in] */ REFIID     riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11CryptoSession * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11CryptoSession * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11CryptoSession * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11CryptoSession * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11CryptoSession * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11CryptoSession * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetCryptoType)(ID3D11CryptoSession * This,
                                            /* [annotation] */
                                            _Out_ GUID * pCryptoType);

    void(STDMETHODCALLTYPE * GetDecoderProfile)(ID3D11CryptoSession * This,
                                                /* [annotation] */
                                                _Out_ GUID * pDecoderProfile);

    HRESULT(STDMETHODCALLTYPE * GetCertificateSize)
    (ID3D11CryptoSession * This,
     /* [annotation] */
     _Out_ UINT * pCertificateSize);

    HRESULT(STDMETHODCALLTYPE * GetCertificate)
    (ID3D11CryptoSession * This,
     /* [annotation] */
     _In_ UINT CertificateSize,
     /* [annotation] */
     _Out_writes_bytes_(CertificateSize) BYTE * pCertificate);

    void(STDMETHODCALLTYPE * GetCryptoSessionHandle)(ID3D11CryptoSession * This,
                                                     /* [annotation] */
                                                     _Out_ HANDLE * pCryptoSessionHandle);

    END_INTERFACE
} ID3D11CryptoSessionVtbl;

interface ID3D11CryptoSession {
    CONST_VTBL struct ID3D11CryptoSessionVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11CryptoSession_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11CryptoSession_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11CryptoSession_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11CryptoSession_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11CryptoSession_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11CryptoSession_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11CryptoSession_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11CryptoSession_GetCryptoType(This, pCryptoType) ((This)->lpVtbl->GetCryptoType(This, pCryptoType))

                #define ID3D11CryptoSession_GetDecoderProfile(This, pDecoderProfile) ((This)->lpVtbl->GetDecoderProfile(This, pDecoderProfile))

                #define ID3D11CryptoSession_GetCertificateSize(This, pCertificateSize) ((This)->lpVtbl->GetCertificateSize(This, pCertificateSize))

                #define ID3D11CryptoSession_GetCertificate(This, CertificateSize, pCertificate) \
                    ((This)->lpVtbl->GetCertificate(This, CertificateSize, pCertificate))

                #define ID3D11CryptoSession_GetCryptoSessionHandle(This, pCryptoSessionHandle) \
                    ((This)->lpVtbl->GetCryptoSessionHandle(This, pCryptoSessionHandle))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11CryptoSession_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0035 */
/* [local] */

typedef enum D3D11_VDOV_DIMENSION { D3D11_VDOV_DIMENSION_UNKNOWN = 0, D3D11_VDOV_DIMENSION_TEXTURE2D = 1 } D3D11_VDOV_DIMENSION;

typedef struct D3D11_TEX2D_VDOV {
    UINT ArraySlice;
} D3D11_TEX2D_VDOV;

typedef struct D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC {
    GUID                 DecodeProfile;
    D3D11_VDOV_DIMENSION ViewDimension;
    union {
        D3D11_TEX2D_VDOV Texture2D;
    };
} D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC;

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0035_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0035_v0_0_s_ifspec;

    #ifndef __ID3D11VideoDecoderOutputView_INTERFACE_DEFINED__
        #define __ID3D11VideoDecoderOutputView_INTERFACE_DEFINED__

/* interface ID3D11VideoDecoderOutputView */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11VideoDecoderOutputView;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("C2931AEA-2A85-4f20-860F-FBA1FD256E18")
ID3D11VideoDecoderOutputView : public ID3D11View {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11VideoDecoderOutputViewVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11VideoDecoderOutputView * This,
     /* [in] */ REFIID              riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11VideoDecoderOutputView * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11VideoDecoderOutputView * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11VideoDecoderOutputView * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11VideoDecoderOutputView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11VideoDecoderOutputView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11VideoDecoderOutputView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetResource)(ID3D11VideoDecoderOutputView * This,
                                          /* [annotation] */
                                          _Out_ ID3D11Resource ** ppResource);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11VideoDecoderOutputView * This,
                                      /* [annotation] */
                                      _Out_ D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc);

    END_INTERFACE
} ID3D11VideoDecoderOutputViewVtbl;

interface ID3D11VideoDecoderOutputView {
    CONST_VTBL struct ID3D11VideoDecoderOutputViewVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11VideoDecoderOutputView_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11VideoDecoderOutputView_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11VideoDecoderOutputView_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11VideoDecoderOutputView_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11VideoDecoderOutputView_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11VideoDecoderOutputView_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11VideoDecoderOutputView_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11VideoDecoderOutputView_GetResource(This, ppResource) ((This)->lpVtbl->GetResource(This, ppResource))

                #define ID3D11VideoDecoderOutputView_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11VideoDecoderOutputView_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0036 */
/* [local] */

typedef enum D3D11_VPIV_DIMENSION { D3D11_VPIV_DIMENSION_UNKNOWN = 0, D3D11_VPIV_DIMENSION_TEXTURE2D = 1 } D3D11_VPIV_DIMENSION;

typedef struct D3D11_TEX2D_VPIV {
    UINT MipSlice;
    UINT ArraySlice;
} D3D11_TEX2D_VPIV;

typedef struct D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC {
    UINT                 FourCC;
    D3D11_VPIV_DIMENSION ViewDimension;
    union {
        D3D11_TEX2D_VPIV Texture2D;
    };
} D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC;

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0036_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0036_v0_0_s_ifspec;

    #ifndef __ID3D11VideoProcessorInputView_INTERFACE_DEFINED__
        #define __ID3D11VideoProcessorInputView_INTERFACE_DEFINED__

/* interface ID3D11VideoProcessorInputView */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11VideoProcessorInputView;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("11EC5A5F-51DC-4945-AB34-6E8C21300EA5")
ID3D11VideoProcessorInputView : public ID3D11View {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11VideoProcessorInputViewVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11VideoProcessorInputView * This,
     /* [in] */ REFIID               riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11VideoProcessorInputView * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11VideoProcessorInputView * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11VideoProcessorInputView * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11VideoProcessorInputView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11VideoProcessorInputView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11VideoProcessorInputView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetResource)(ID3D11VideoProcessorInputView * This,
                                          /* [annotation] */
                                          _Out_ ID3D11Resource ** ppResource);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11VideoProcessorInputView * This,
                                      /* [annotation] */
                                      _Out_ D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc);

    END_INTERFACE
} ID3D11VideoProcessorInputViewVtbl;

interface ID3D11VideoProcessorInputView {
    CONST_VTBL struct ID3D11VideoProcessorInputViewVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11VideoProcessorInputView_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11VideoProcessorInputView_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11VideoProcessorInputView_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11VideoProcessorInputView_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11VideoProcessorInputView_GetPrivateData(This, guid, pDataSize, pData) \
                    ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11VideoProcessorInputView_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11VideoProcessorInputView_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11VideoProcessorInputView_GetResource(This, ppResource) ((This)->lpVtbl->GetResource(This, ppResource))

                #define ID3D11VideoProcessorInputView_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11VideoProcessorInputView_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0037 */
/* [local] */

typedef enum D3D11_VPOV_DIMENSION {
    D3D11_VPOV_DIMENSION_UNKNOWN        = 0,
    D3D11_VPOV_DIMENSION_TEXTURE2D      = 1,
    D3D11_VPOV_DIMENSION_TEXTURE2DARRAY = 2
} D3D11_VPOV_DIMENSION;

typedef struct D3D11_TEX2D_VPOV {
    UINT MipSlice;
} D3D11_TEX2D_VPOV;

typedef struct D3D11_TEX2D_ARRAY_VPOV {
    UINT MipSlice;
    UINT FirstArraySlice;
    UINT ArraySize;
} D3D11_TEX2D_ARRAY_VPOV;

typedef struct D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC {
    D3D11_VPOV_DIMENSION ViewDimension;
    union {
        D3D11_TEX2D_VPOV       Texture2D;
        D3D11_TEX2D_ARRAY_VPOV Texture2DArray;
    };
} D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC;

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0037_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0037_v0_0_s_ifspec;

    #ifndef __ID3D11VideoProcessorOutputView_INTERFACE_DEFINED__
        #define __ID3D11VideoProcessorOutputView_INTERFACE_DEFINED__

/* interface ID3D11VideoProcessorOutputView */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11VideoProcessorOutputView;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("A048285E-25A9-4527-BD93-D68B68C44254")
ID3D11VideoProcessorOutputView : public ID3D11View {
public:
    virtual void STDMETHODCALLTYPE GetDesc(
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc) = 0;
};

        #else /* C style interface */

typedef struct ID3D11VideoProcessorOutputViewVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11VideoProcessorOutputView * This,
     /* [in] */ REFIID                riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11VideoProcessorOutputView * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11VideoProcessorOutputView * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11VideoProcessorOutputView * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11VideoProcessorOutputView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11VideoProcessorOutputView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11VideoProcessorOutputView * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    void(STDMETHODCALLTYPE * GetResource)(ID3D11VideoProcessorOutputView * This,
                                          /* [annotation] */
                                          _Out_ ID3D11Resource ** ppResource);

    void(STDMETHODCALLTYPE * GetDesc)(ID3D11VideoProcessorOutputView * This,
                                      /* [annotation] */
                                      _Out_ D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc);

    END_INTERFACE
} ID3D11VideoProcessorOutputViewVtbl;

interface ID3D11VideoProcessorOutputView {
    CONST_VTBL struct ID3D11VideoProcessorOutputViewVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11VideoProcessorOutputView_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11VideoProcessorOutputView_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11VideoProcessorOutputView_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11VideoProcessorOutputView_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11VideoProcessorOutputView_GetPrivateData(This, guid, pDataSize, pData) \
                    ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11VideoProcessorOutputView_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11VideoProcessorOutputView_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11VideoProcessorOutputView_GetResource(This, ppResource) ((This)->lpVtbl->GetResource(This, ppResource))

                #define ID3D11VideoProcessorOutputView_GetDesc(This, pDesc) ((This)->lpVtbl->GetDesc(This, pDesc))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11VideoProcessorOutputView_INTERFACE_DEFINED__ */

    #ifndef __ID3D11VideoContext_INTERFACE_DEFINED__
        #define __ID3D11VideoContext_INTERFACE_DEFINED__

/* interface ID3D11VideoContext */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11VideoContext;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("61F21C45-3C0E-4a74-9CEA-67100D9AD5E4")
ID3D11VideoContext : public ID3D11DeviceChild {
public:
    virtual HRESULT STDMETHODCALLTYPE GetDecoderBuffer(
        /* [annotation] */
        _In_ ID3D11VideoDecoder * pDecoder,
        /* [annotation] */
        _In_ D3D11_VIDEO_DECODER_BUFFER_TYPE Type,
        /* [annotation] */
        _Out_ UINT * pBufferSize,
        /* [annotation] */
        _Out_writes_bytes_opt_(*pBufferSize) void ** ppBuffer) = 0;

    virtual HRESULT STDMETHODCALLTYPE ReleaseDecoderBuffer(
        /* [annotation] */
        _In_ ID3D11VideoDecoder * pDecoder,
        /* [annotation] */
        _In_ D3D11_VIDEO_DECODER_BUFFER_TYPE Type) = 0;

    virtual HRESULT STDMETHODCALLTYPE DecoderBeginFrame(
        /* [annotation] */
        _In_ ID3D11VideoDecoder * pDecoder,
        /* [annotation] */
        _In_ ID3D11VideoDecoderOutputView * pView,
        /* [annotation] */
        _In_ UINT ContentKeySize,
        /* [annotation] */
        _In_reads_bytes_opt_(ContentKeySize) const void * pContentKey) = 0;

    virtual HRESULT STDMETHODCALLTYPE DecoderEndFrame(
        /* [annotation] */
        _In_ ID3D11VideoDecoder * pDecoder) = 0;

    virtual HRESULT STDMETHODCALLTYPE SubmitDecoderBuffers(
        /* [annotation] */
        _In_ ID3D11VideoDecoder * pDecoder,
        /* [annotation] */
        _In_ UINT NumBuffers,
        /* [annotation] */
        _In_reads_(NumBuffers) const D3D11_VIDEO_DECODER_BUFFER_DESC * pBufferDesc) = 0;

    virtual HRESULT STDMETHODCALLTYPE DecoderExtension(
        /* [annotation] */
        _In_ ID3D11VideoDecoder * pDecoder,
        /* [annotation] */
        _In_ const D3D11_VIDEO_DECODER_EXTENSION * pExtensionData) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetOutputTargetRect(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ BOOL Enable,
        /* [annotation] */
        _In_opt_ const RECT * pRect) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetOutputBackgroundColor(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ BOOL YCbCr,
        /* [annotation] */
        _In_ const D3D11_VIDEO_COLOR * pColor) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetOutputColorSpace(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetOutputAlphaFillMode(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE AlphaFillMode,
        /* [annotation] */
        _In_ UINT StreamIndex) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetOutputConstriction(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ BOOL Enable,
        /* [annotation] */
        _In_ SIZE Size) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetOutputStereoMode(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ BOOL Enable) = 0;

    virtual HRESULT STDMETHODCALLTYPE VideoProcessorSetOutputExtension(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ const GUID * pExtensionGuid,
        /* [annotation] */
        _In_ UINT DataSize,
        /* [annotation] */
        _In_ void * pData) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetOutputTargetRect(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _Out_ BOOL * Enabled,
        /* [annotation] */
        _Out_ RECT * pRect) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetOutputBackgroundColor(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _Out_ BOOL * pYCbCr,
        /* [annotation] */
        _Out_ D3D11_VIDEO_COLOR * pColor) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetOutputColorSpace(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetOutputAlphaFillMode(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE * pAlphaFillMode,
        /* [annotation] */
        _Out_ UINT * pStreamIndex) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetOutputConstriction(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _Out_ BOOL * pEnabled,
        /* [annotation] */
        _Out_ SIZE * pSize) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetOutputStereoMode(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _Out_ BOOL * pEnabled) = 0;

    virtual HRESULT STDMETHODCALLTYPE VideoProcessorGetOutputExtension(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ const GUID * pExtensionGuid,
        /* [annotation] */
        _In_ UINT DataSize,
        /* [annotation] */
        _Out_ void * pData) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamFrameFormat(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ D3D11_VIDEO_FRAME_FORMAT FrameFormat) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamColorSpace(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamOutputRate(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ D3D11_VIDEO_PROCESSOR_OUTPUT_RATE OutputRate,
        /* [annotation] */
        _In_ BOOL RepeatFrame,
        /* [annotation] */
        _In_opt_ const DXGI_RATIONAL * pCustomRate) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamSourceRect(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ BOOL Enable,
        /* [annotation] */
        _In_opt_ const RECT * pRect) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamDestRect(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ BOOL Enable,
        /* [annotation] */
        _In_opt_ const RECT * pRect) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamAlpha(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ BOOL Enable,
        /* [annotation] */
        _In_ FLOAT Alpha) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamPalette(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ UINT Count,
        /* [annotation] */
        _In_reads_opt_(Count) const UINT * pEntries) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamPixelAspectRatio(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ BOOL Enable,
        /* [annotation] */
        _In_opt_ const DXGI_RATIONAL * pSourceAspectRatio,
        /* [annotation] */
        _In_opt_ const DXGI_RATIONAL * pDestinationAspectRatio) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamLumaKey(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ BOOL Enable,
        /* [annotation] */
        _In_ FLOAT Lower,
        /* [annotation] */
        _In_ FLOAT Upper) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamStereoFormat(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ BOOL Enable,
        /* [annotation] */
        _In_ D3D11_VIDEO_PROCESSOR_STEREO_FORMAT Format,
        /* [annotation] */
        _In_ BOOL LeftViewFrame0,
        /* [annotation] */
        _In_ BOOL BaseViewFrame0,
        /* [annotation] */
        _In_ D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE FlipMode,
        /* [annotation] */
        _In_ int MonoOffset) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamAutoProcessingMode(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ BOOL Enable) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamFilter(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ D3D11_VIDEO_PROCESSOR_FILTER Filter,
        /* [annotation] */
        _In_ BOOL Enable,
        /* [annotation] */
        _In_ int Level) = 0;

    virtual HRESULT STDMETHODCALLTYPE VideoProcessorSetStreamExtension(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ const GUID * pExtensionGuid,
        /* [annotation] */
        _In_ UINT DataSize,
        /* [annotation] */
        _In_ void * pData) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamFrameFormat(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _Out_ D3D11_VIDEO_FRAME_FORMAT * pFrameFormat) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamColorSpace(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamOutputRate(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_OUTPUT_RATE * pOutputRate,
        /* [annotation] */
        _Out_ BOOL * pRepeatFrame,
        /* [annotation] */
        _Out_ DXGI_RATIONAL * pCustomRate) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamSourceRect(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _Out_ BOOL * pEnabled,
        /* [annotation] */
        _Out_ RECT * pRect) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamDestRect(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _Out_ BOOL * pEnabled,
        /* [annotation] */
        _Out_ RECT * pRect) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamAlpha(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _Out_ BOOL * pEnabled,
        /* [annotation] */
        _Out_ FLOAT * pAlpha) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamPalette(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ UINT Count,
        /* [annotation] */
        _Out_writes_(Count) UINT * pEntries) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamPixelAspectRatio(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _Out_ BOOL * pEnabled,
        /* [annotation] */
        _Out_ DXGI_RATIONAL * pSourceAspectRatio,
        /* [annotation] */
        _Out_ DXGI_RATIONAL * pDestinationAspectRatio) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamLumaKey(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _Out_ BOOL * pEnabled,
        /* [annotation] */
        _Out_ FLOAT * pLower,
        /* [annotation] */
        _Out_ FLOAT * pUpper) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamStereoFormat(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _Out_ BOOL * pEnable,
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_STEREO_FORMAT * pFormat,
        /* [annotation] */
        _Out_ BOOL * pLeftViewFrame0,
        /* [annotation] */
        _Out_ BOOL * pBaseViewFrame0,
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE * pFlipMode,
        /* [annotation] */
        _Out_ int * MonoOffset) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamAutoProcessingMode(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _Out_ BOOL * pEnabled) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamFilter(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ D3D11_VIDEO_PROCESSOR_FILTER Filter,
        /* [annotation] */
        _Out_ BOOL * pEnabled,
        /* [annotation] */
        _Out_ int * pLevel) = 0;

    virtual HRESULT STDMETHODCALLTYPE VideoProcessorGetStreamExtension(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ const GUID * pExtensionGuid,
        /* [annotation] */
        _In_ UINT DataSize,
        /* [annotation] */
        _Out_ void * pData) = 0;

    virtual HRESULT STDMETHODCALLTYPE VideoProcessorBlt(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ ID3D11VideoProcessorOutputView * pView,
        /* [annotation] */
        _In_ UINT OutputFrame,
        /* [annotation] */
        _In_ UINT StreamCount,
        /* [annotation] */
        _In_reads_(StreamCount) const D3D11_VIDEO_PROCESSOR_STREAM * pStreams) = 0;

    virtual HRESULT STDMETHODCALLTYPE NegotiateCryptoSessionKeyExchange(
        /* [annotation] */
        _In_ ID3D11CryptoSession * pCryptoSession,
        /* [annotation] */
        _In_ UINT DataSize,
        /* [annotation] */
        _Inout_updates_bytes_(DataSize) void * pData) = 0;

    virtual void STDMETHODCALLTYPE EncryptionBlt(
        /* [annotation] */
        _In_ ID3D11CryptoSession * pCryptoSession,
        /* [annotation] */
        _In_ ID3D11Texture2D * pSrcSurface,
        /* [annotation] */
        _In_ ID3D11Texture2D * pDstSurface,
        /* [annotation] */
        _In_ UINT IVSize,
        /* [annotation] */
        _In_reads_bytes_opt_(IVSize) void * pIV) = 0;

    virtual void STDMETHODCALLTYPE DecryptionBlt(
        /* [annotation] */
        _In_ ID3D11CryptoSession * pCryptoSession,
        /* [annotation] */
        _In_ ID3D11Texture2D * pSrcSurface,
        /* [annotation] */
        _In_ ID3D11Texture2D * pDstSurface,
        /* [annotation] */
        _In_opt_ D3D11_ENCRYPTED_BLOCK_INFO * pEncryptedBlockInfo,
        /* [annotation] */
        _In_ UINT ContentKeySize,
        /* [annotation] */
        _In_reads_bytes_opt_(ContentKeySize) const void * pContentKey,
        /* [annotation] */
        _In_ UINT IVSize,
        /* [annotation] */
        _In_reads_bytes_opt_(IVSize) void * pIV) = 0;

    virtual void STDMETHODCALLTYPE StartSessionKeyRefresh(
        /* [annotation] */
        _In_ ID3D11CryptoSession * pCryptoSession,
        /* [annotation] */
        _In_ UINT RandomNumberSize,
        /* [annotation] */
        _Out_writes_bytes_(RandomNumberSize) void * pRandomNumber) = 0;

    virtual void STDMETHODCALLTYPE FinishSessionKeyRefresh(
        /* [annotation] */
        _In_ ID3D11CryptoSession * pCryptoSession) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetEncryptionBltKey(
        /* [annotation] */
        _In_ ID3D11CryptoSession * pCryptoSession,
        /* [annotation] */
        _In_ UINT KeySize,
        /* [annotation] */
        _Out_writes_bytes_(KeySize) void * pReadbackKey) = 0;

    virtual HRESULT STDMETHODCALLTYPE NegotiateAuthenticatedChannelKeyExchange(
        /* [annotation] */
        _In_ ID3D11AuthenticatedChannel * pChannel,
        /* [annotation] */
        _In_ UINT DataSize,
        /* [annotation] */
        _Inout_updates_bytes_(DataSize) void * pData) = 0;

    virtual HRESULT STDMETHODCALLTYPE QueryAuthenticatedChannel(
        /* [annotation] */
        _In_ ID3D11AuthenticatedChannel * pChannel,
        /* [annotation] */
        _In_ UINT InputSize,
        /* [annotation] */
        _In_reads_bytes_(InputSize) const void * pInput,
        /* [annotation] */
        _In_ UINT OutputSize,
        /* [annotation] */
        _Out_writes_bytes_(OutputSize) void * pOutput) = 0;

    virtual HRESULT STDMETHODCALLTYPE ConfigureAuthenticatedChannel(
        /* [annotation] */
        _In_ ID3D11AuthenticatedChannel * pChannel,
        /* [annotation] */
        _In_ UINT InputSize,
        /* [annotation] */
        _In_reads_bytes_(InputSize) const void * pInput,
        /* [annotation] */
        _Out_ D3D11_AUTHENTICATED_CONFIGURE_OUTPUT * pOutput) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorSetStreamRotation(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _In_ BOOL Enable,
        /* [annotation] */
        _In_ D3D11_VIDEO_PROCESSOR_ROTATION Rotation) = 0;

    virtual void STDMETHODCALLTYPE VideoProcessorGetStreamRotation(
        /* [annotation] */
        _In_ ID3D11VideoProcessor * pVideoProcessor,
        /* [annotation] */
        _In_ UINT StreamIndex,
        /* [annotation] */
        _Out_ BOOL * pEnable,
        /* [annotation] */
        _Out_ D3D11_VIDEO_PROCESSOR_ROTATION * pRotation) = 0;
};

        #else /* C style interface */

typedef struct ID3D11VideoContextVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11VideoContext * This,
     /* [in] */ REFIID    riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11VideoContext * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11VideoContext * This);

    void(STDMETHODCALLTYPE * GetDevice)(ID3D11VideoContext * This,
                                        /* [annotation] */
                                        _Out_ ID3D11Device ** ppDevice);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    HRESULT(STDMETHODCALLTYPE * GetDecoderBuffer)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11VideoDecoder * pDecoder,
     /* [annotation] */
     _In_ D3D11_VIDEO_DECODER_BUFFER_TYPE Type,
     /* [annotation] */
     _Out_ UINT * pBufferSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pBufferSize) void ** ppBuffer);

    HRESULT(STDMETHODCALLTYPE * ReleaseDecoderBuffer)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11VideoDecoder * pDecoder,
     /* [annotation] */
     _In_ D3D11_VIDEO_DECODER_BUFFER_TYPE Type);

    HRESULT(STDMETHODCALLTYPE * DecoderBeginFrame)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11VideoDecoder * pDecoder,
     /* [annotation] */
     _In_ ID3D11VideoDecoderOutputView * pView,
     /* [annotation] */
     _In_ UINT ContentKeySize,
     /* [annotation] */
     _In_reads_bytes_opt_(ContentKeySize) const void * pContentKey);

    HRESULT(STDMETHODCALLTYPE * DecoderEndFrame)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11VideoDecoder * pDecoder);

    HRESULT(STDMETHODCALLTYPE * SubmitDecoderBuffers)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11VideoDecoder * pDecoder,
     /* [annotation] */
     _In_ UINT NumBuffers,
     /* [annotation] */
     _In_reads_(NumBuffers) const D3D11_VIDEO_DECODER_BUFFER_DESC * pBufferDesc);

    HRESULT(STDMETHODCALLTYPE * DecoderExtension)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11VideoDecoder * pDecoder,
     /* [annotation] */
     _In_ const D3D11_VIDEO_DECODER_EXTENSION * pExtensionData);

    void(STDMETHODCALLTYPE * VideoProcessorSetOutputTargetRect)(ID3D11VideoContext * This,
                                                                /* [annotation] */
                                                                _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                /* [annotation] */
                                                                _In_ BOOL Enable,
                                                                /* [annotation] */
                                                                _In_opt_ const RECT * pRect);

    void(STDMETHODCALLTYPE * VideoProcessorSetOutputBackgroundColor)(ID3D11VideoContext * This,
                                                                     /* [annotation] */
                                                                     _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                     /* [annotation] */
                                                                     _In_ BOOL YCbCr,
                                                                     /* [annotation] */
                                                                     _In_ const D3D11_VIDEO_COLOR * pColor);

    void(STDMETHODCALLTYPE * VideoProcessorSetOutputColorSpace)(ID3D11VideoContext * This,
                                                                /* [annotation] */
                                                                _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                /* [annotation] */
                                                                _In_ const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace);

    void(STDMETHODCALLTYPE * VideoProcessorSetOutputAlphaFillMode)(ID3D11VideoContext * This,
                                                                   /* [annotation] */
                                                                   _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                   /* [annotation] */
                                                                   _In_ D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE AlphaFillMode,
                                                                   /* [annotation] */
                                                                   _In_ UINT StreamIndex);

    void(STDMETHODCALLTYPE * VideoProcessorSetOutputConstriction)(ID3D11VideoContext * This,
                                                                  /* [annotation] */
                                                                  _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                  /* [annotation] */
                                                                  _In_ BOOL Enable,
                                                                  /* [annotation] */
                                                                  _In_ SIZE Size);

    void(STDMETHODCALLTYPE * VideoProcessorSetOutputStereoMode)(ID3D11VideoContext * This,
                                                                /* [annotation] */
                                                                _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                /* [annotation] */
                                                                _In_ BOOL Enable);

    HRESULT(STDMETHODCALLTYPE * VideoProcessorSetOutputExtension)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11VideoProcessor * pVideoProcessor,
     /* [annotation] */
     _In_ const GUID * pExtensionGuid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_ void * pData);

    void(STDMETHODCALLTYPE * VideoProcessorGetOutputTargetRect)(ID3D11VideoContext * This,
                                                                /* [annotation] */
                                                                _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                /* [annotation] */
                                                                _Out_ BOOL * Enabled,
                                                                /* [annotation] */
                                                                _Out_ RECT * pRect);

    void(STDMETHODCALLTYPE * VideoProcessorGetOutputBackgroundColor)(ID3D11VideoContext * This,
                                                                     /* [annotation] */
                                                                     _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                     /* [annotation] */
                                                                     _Out_ BOOL * pYCbCr,
                                                                     /* [annotation] */
                                                                     _Out_ D3D11_VIDEO_COLOR * pColor);

    void(STDMETHODCALLTYPE * VideoProcessorGetOutputColorSpace)(ID3D11VideoContext * This,
                                                                /* [annotation] */
                                                                _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                /* [annotation] */
                                                                _Out_ D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace);

    void(STDMETHODCALLTYPE * VideoProcessorGetOutputAlphaFillMode)(ID3D11VideoContext * This,
                                                                   /* [annotation] */
                                                                   _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                   /* [annotation] */
                                                                   _Out_ D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE * pAlphaFillMode,
                                                                   /* [annotation] */
                                                                   _Out_ UINT * pStreamIndex);

    void(STDMETHODCALLTYPE * VideoProcessorGetOutputConstriction)(ID3D11VideoContext * This,
                                                                  /* [annotation] */
                                                                  _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                  /* [annotation] */
                                                                  _Out_ BOOL * pEnabled,
                                                                  /* [annotation] */
                                                                  _Out_ SIZE * pSize);

    void(STDMETHODCALLTYPE * VideoProcessorGetOutputStereoMode)(ID3D11VideoContext * This,
                                                                /* [annotation] */
                                                                _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                /* [annotation] */
                                                                _Out_ BOOL * pEnabled);

    HRESULT(STDMETHODCALLTYPE * VideoProcessorGetOutputExtension)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11VideoProcessor * pVideoProcessor,
     /* [annotation] */
     _In_ const GUID * pExtensionGuid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _Out_ void * pData);

    void(STDMETHODCALLTYPE * VideoProcessorSetStreamFrameFormat)(ID3D11VideoContext * This,
                                                                 /* [annotation] */
                                                                 _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                 /* [annotation] */
                                                                 _In_ UINT StreamIndex,
                                                                 /* [annotation] */
                                                                 _In_ D3D11_VIDEO_FRAME_FORMAT FrameFormat);

    void(STDMETHODCALLTYPE * VideoProcessorSetStreamColorSpace)(ID3D11VideoContext * This,
                                                                /* [annotation] */
                                                                _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                /* [annotation] */
                                                                _In_ UINT StreamIndex,
                                                                /* [annotation] */
                                                                _In_ const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace);

    void(STDMETHODCALLTYPE * VideoProcessorSetStreamOutputRate)(ID3D11VideoContext * This,
                                                                /* [annotation] */
                                                                _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                /* [annotation] */
                                                                _In_ UINT StreamIndex,
                                                                /* [annotation] */
                                                                _In_ D3D11_VIDEO_PROCESSOR_OUTPUT_RATE OutputRate,
                                                                /* [annotation] */
                                                                _In_ BOOL RepeatFrame,
                                                                /* [annotation] */
                                                                _In_opt_ const DXGI_RATIONAL * pCustomRate);

    void(STDMETHODCALLTYPE * VideoProcessorSetStreamSourceRect)(ID3D11VideoContext * This,
                                                                /* [annotation] */
                                                                _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                /* [annotation] */
                                                                _In_ UINT StreamIndex,
                                                                /* [annotation] */
                                                                _In_ BOOL Enable,
                                                                /* [annotation] */
                                                                _In_opt_ const RECT * pRect);

    void(STDMETHODCALLTYPE * VideoProcessorSetStreamDestRect)(ID3D11VideoContext * This,
                                                              /* [annotation] */
                                                              _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                              /* [annotation] */
                                                              _In_ UINT StreamIndex,
                                                              /* [annotation] */
                                                              _In_ BOOL Enable,
                                                              /* [annotation] */
                                                              _In_opt_ const RECT * pRect);

    void(STDMETHODCALLTYPE * VideoProcessorSetStreamAlpha)(ID3D11VideoContext * This,
                                                           /* [annotation] */
                                                           _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                           /* [annotation] */
                                                           _In_ UINT StreamIndex,
                                                           /* [annotation] */
                                                           _In_ BOOL Enable,
                                                           /* [annotation] */
                                                           _In_ FLOAT Alpha);

    void(STDMETHODCALLTYPE * VideoProcessorSetStreamPalette)(ID3D11VideoContext * This,
                                                             /* [annotation] */
                                                             _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                             /* [annotation] */
                                                             _In_ UINT StreamIndex,
                                                             /* [annotation] */
                                                             _In_ UINT Count,
                                                             /* [annotation] */
                                                             _In_reads_opt_(Count) const UINT * pEntries);

    void(STDMETHODCALLTYPE * VideoProcessorSetStreamPixelAspectRatio)(ID3D11VideoContext * This,
                                                                      /* [annotation] */
                                                                      _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                      /* [annotation] */
                                                                      _In_ UINT StreamIndex,
                                                                      /* [annotation] */
                                                                      _In_ BOOL Enable,
                                                                      /* [annotation] */
                                                                      _In_opt_ const DXGI_RATIONAL * pSourceAspectRatio,
                                                                      /* [annotation] */
                                                                      _In_opt_ const DXGI_RATIONAL * pDestinationAspectRatio);

    void(STDMETHODCALLTYPE * VideoProcessorSetStreamLumaKey)(ID3D11VideoContext * This,
                                                             /* [annotation] */
                                                             _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                             /* [annotation] */
                                                             _In_ UINT StreamIndex,
                                                             /* [annotation] */
                                                             _In_ BOOL Enable,
                                                             /* [annotation] */
                                                             _In_ FLOAT Lower,
                                                             /* [annotation] */
                                                             _In_ FLOAT Upper);

    void(STDMETHODCALLTYPE * VideoProcessorSetStreamStereoFormat)(ID3D11VideoContext * This,
                                                                  /* [annotation] */
                                                                  _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                  /* [annotation] */
                                                                  _In_ UINT StreamIndex,
                                                                  /* [annotation] */
                                                                  _In_ BOOL Enable,
                                                                  /* [annotation] */
                                                                  _In_ D3D11_VIDEO_PROCESSOR_STEREO_FORMAT Format,
                                                                  /* [annotation] */
                                                                  _In_ BOOL LeftViewFrame0,
                                                                  /* [annotation] */
                                                                  _In_ BOOL BaseViewFrame0,
                                                                  /* [annotation] */
                                                                  _In_ D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE FlipMode,
                                                                  /* [annotation] */
                                                                  _In_ int MonoOffset);

    void(STDMETHODCALLTYPE * VideoProcessorSetStreamAutoProcessingMode)(ID3D11VideoContext * This,
                                                                        /* [annotation] */
                                                                        _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                        /* [annotation] */
                                                                        _In_ UINT StreamIndex,
                                                                        /* [annotation] */
                                                                        _In_ BOOL Enable);

    void(STDMETHODCALLTYPE * VideoProcessorSetStreamFilter)(ID3D11VideoContext * This,
                                                            /* [annotation] */
                                                            _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                            /* [annotation] */
                                                            _In_ UINT StreamIndex,
                                                            /* [annotation] */
                                                            _In_ D3D11_VIDEO_PROCESSOR_FILTER Filter,
                                                            /* [annotation] */
                                                            _In_ BOOL Enable,
                                                            /* [annotation] */
                                                            _In_ int Level);

    HRESULT(STDMETHODCALLTYPE * VideoProcessorSetStreamExtension)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11VideoProcessor * pVideoProcessor,
     /* [annotation] */
     _In_ UINT StreamIndex,
     /* [annotation] */
     _In_ const GUID * pExtensionGuid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_ void * pData);

    void(STDMETHODCALLTYPE * VideoProcessorGetStreamFrameFormat)(ID3D11VideoContext * This,
                                                                 /* [annotation] */
                                                                 _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                 /* [annotation] */
                                                                 _In_ UINT StreamIndex,
                                                                 /* [annotation] */
                                                                 _Out_ D3D11_VIDEO_FRAME_FORMAT * pFrameFormat);

    void(STDMETHODCALLTYPE * VideoProcessorGetStreamColorSpace)(ID3D11VideoContext * This,
                                                                /* [annotation] */
                                                                _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                /* [annotation] */
                                                                _In_ UINT StreamIndex,
                                                                /* [annotation] */
                                                                _Out_ D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace);

    void(STDMETHODCALLTYPE * VideoProcessorGetStreamOutputRate)(ID3D11VideoContext * This,
                                                                /* [annotation] */
                                                                _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                /* [annotation] */
                                                                _In_ UINT StreamIndex,
                                                                /* [annotation] */
                                                                _Out_ D3D11_VIDEO_PROCESSOR_OUTPUT_RATE * pOutputRate,
                                                                /* [annotation] */
                                                                _Out_ BOOL * pRepeatFrame,
                                                                /* [annotation] */
                                                                _Out_ DXGI_RATIONAL * pCustomRate);

    void(STDMETHODCALLTYPE * VideoProcessorGetStreamSourceRect)(ID3D11VideoContext * This,
                                                                /* [annotation] */
                                                                _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                /* [annotation] */
                                                                _In_ UINT StreamIndex,
                                                                /* [annotation] */
                                                                _Out_ BOOL * pEnabled,
                                                                /* [annotation] */
                                                                _Out_ RECT * pRect);

    void(STDMETHODCALLTYPE * VideoProcessorGetStreamDestRect)(ID3D11VideoContext * This,
                                                              /* [annotation] */
                                                              _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                              /* [annotation] */
                                                              _In_ UINT StreamIndex,
                                                              /* [annotation] */
                                                              _Out_ BOOL * pEnabled,
                                                              /* [annotation] */
                                                              _Out_ RECT * pRect);

    void(STDMETHODCALLTYPE * VideoProcessorGetStreamAlpha)(ID3D11VideoContext * This,
                                                           /* [annotation] */
                                                           _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                           /* [annotation] */
                                                           _In_ UINT StreamIndex,
                                                           /* [annotation] */
                                                           _Out_ BOOL * pEnabled,
                                                           /* [annotation] */
                                                           _Out_ FLOAT * pAlpha);

    void(STDMETHODCALLTYPE * VideoProcessorGetStreamPalette)(ID3D11VideoContext * This,
                                                             /* [annotation] */
                                                             _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                             /* [annotation] */
                                                             _In_ UINT StreamIndex,
                                                             /* [annotation] */
                                                             _In_ UINT Count,
                                                             /* [annotation] */
                                                             _Out_writes_(Count) UINT * pEntries);

    void(STDMETHODCALLTYPE * VideoProcessorGetStreamPixelAspectRatio)(ID3D11VideoContext * This,
                                                                      /* [annotation] */
                                                                      _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                      /* [annotation] */
                                                                      _In_ UINT StreamIndex,
                                                                      /* [annotation] */
                                                                      _Out_ BOOL * pEnabled,
                                                                      /* [annotation] */
                                                                      _Out_ DXGI_RATIONAL * pSourceAspectRatio,
                                                                      /* [annotation] */
                                                                      _Out_ DXGI_RATIONAL * pDestinationAspectRatio);

    void(STDMETHODCALLTYPE * VideoProcessorGetStreamLumaKey)(ID3D11VideoContext * This,
                                                             /* [annotation] */
                                                             _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                             /* [annotation] */
                                                             _In_ UINT StreamIndex,
                                                             /* [annotation] */
                                                             _Out_ BOOL * pEnabled,
                                                             /* [annotation] */
                                                             _Out_ FLOAT * pLower,
                                                             /* [annotation] */
                                                             _Out_ FLOAT * pUpper);

    void(STDMETHODCALLTYPE * VideoProcessorGetStreamStereoFormat)(ID3D11VideoContext * This,
                                                                  /* [annotation] */
                                                                  _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                  /* [annotation] */
                                                                  _In_ UINT StreamIndex,
                                                                  /* [annotation] */
                                                                  _Out_ BOOL * pEnable,
                                                                  /* [annotation] */
                                                                  _Out_ D3D11_VIDEO_PROCESSOR_STEREO_FORMAT * pFormat,
                                                                  /* [annotation] */
                                                                  _Out_ BOOL * pLeftViewFrame0,
                                                                  /* [annotation] */
                                                                  _Out_ BOOL * pBaseViewFrame0,
                                                                  /* [annotation] */
                                                                  _Out_ D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE * pFlipMode,
                                                                  /* [annotation] */
                                                                  _Out_ int * MonoOffset);

    void(STDMETHODCALLTYPE * VideoProcessorGetStreamAutoProcessingMode)(ID3D11VideoContext * This,
                                                                        /* [annotation] */
                                                                        _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                        /* [annotation] */
                                                                        _In_ UINT StreamIndex,
                                                                        /* [annotation] */
                                                                        _Out_ BOOL * pEnabled);

    void(STDMETHODCALLTYPE * VideoProcessorGetStreamFilter)(ID3D11VideoContext * This,
                                                            /* [annotation] */
                                                            _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                            /* [annotation] */
                                                            _In_ UINT StreamIndex,
                                                            /* [annotation] */
                                                            _In_ D3D11_VIDEO_PROCESSOR_FILTER Filter,
                                                            /* [annotation] */
                                                            _Out_ BOOL * pEnabled,
                                                            /* [annotation] */
                                                            _Out_ int * pLevel);

    HRESULT(STDMETHODCALLTYPE * VideoProcessorGetStreamExtension)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11VideoProcessor * pVideoProcessor,
     /* [annotation] */
     _In_ UINT StreamIndex,
     /* [annotation] */
     _In_ const GUID * pExtensionGuid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _Out_ void * pData);

    HRESULT(STDMETHODCALLTYPE * VideoProcessorBlt)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11VideoProcessor * pVideoProcessor,
     /* [annotation] */
     _In_ ID3D11VideoProcessorOutputView * pView,
     /* [annotation] */
     _In_ UINT OutputFrame,
     /* [annotation] */
     _In_ UINT StreamCount,
     /* [annotation] */
     _In_reads_(StreamCount) const D3D11_VIDEO_PROCESSOR_STREAM * pStreams);

    HRESULT(STDMETHODCALLTYPE * NegotiateCryptoSessionKeyExchange)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11CryptoSession * pCryptoSession,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _Inout_updates_bytes_(DataSize) void * pData);

    void(STDMETHODCALLTYPE * EncryptionBlt)(ID3D11VideoContext * This,
                                            /* [annotation] */
                                            _In_ ID3D11CryptoSession * pCryptoSession,
                                            /* [annotation] */
                                            _In_ ID3D11Texture2D * pSrcSurface,
                                            /* [annotation] */
                                            _In_ ID3D11Texture2D * pDstSurface,
                                            /* [annotation] */
                                            _In_ UINT IVSize,
                                            /* [annotation] */
                                            _In_reads_bytes_opt_(IVSize) void * pIV);

    void(STDMETHODCALLTYPE * DecryptionBlt)(ID3D11VideoContext * This,
                                            /* [annotation] */
                                            _In_ ID3D11CryptoSession * pCryptoSession,
                                            /* [annotation] */
                                            _In_ ID3D11Texture2D * pSrcSurface,
                                            /* [annotation] */
                                            _In_ ID3D11Texture2D * pDstSurface,
                                            /* [annotation] */
                                            _In_opt_ D3D11_ENCRYPTED_BLOCK_INFO * pEncryptedBlockInfo,
                                            /* [annotation] */
                                            _In_ UINT ContentKeySize,
                                            /* [annotation] */
                                            _In_reads_bytes_opt_(ContentKeySize) const void * pContentKey,
                                            /* [annotation] */
                                            _In_ UINT IVSize,
                                            /* [annotation] */
                                            _In_reads_bytes_opt_(IVSize) void * pIV);

    void(STDMETHODCALLTYPE * StartSessionKeyRefresh)(ID3D11VideoContext * This,
                                                     /* [annotation] */
                                                     _In_ ID3D11CryptoSession * pCryptoSession,
                                                     /* [annotation] */
                                                     _In_ UINT RandomNumberSize,
                                                     /* [annotation] */
                                                     _Out_writes_bytes_(RandomNumberSize) void * pRandomNumber);

    void(STDMETHODCALLTYPE * FinishSessionKeyRefresh)(ID3D11VideoContext * This,
                                                      /* [annotation] */
                                                      _In_ ID3D11CryptoSession * pCryptoSession);

    HRESULT(STDMETHODCALLTYPE * GetEncryptionBltKey)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11CryptoSession * pCryptoSession,
     /* [annotation] */
     _In_ UINT KeySize,
     /* [annotation] */
     _Out_writes_bytes_(KeySize) void * pReadbackKey);

    HRESULT(STDMETHODCALLTYPE * NegotiateAuthenticatedChannelKeyExchange)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11AuthenticatedChannel * pChannel,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _Inout_updates_bytes_(DataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * QueryAuthenticatedChannel)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11AuthenticatedChannel * pChannel,
     /* [annotation] */
     _In_ UINT InputSize,
     /* [annotation] */
     _In_reads_bytes_(InputSize) const void * pInput,
     /* [annotation] */
     _In_ UINT OutputSize,
     /* [annotation] */
     _Out_writes_bytes_(OutputSize) void * pOutput);

    HRESULT(STDMETHODCALLTYPE * ConfigureAuthenticatedChannel)
    (ID3D11VideoContext * This,
     /* [annotation] */
     _In_ ID3D11AuthenticatedChannel * pChannel,
     /* [annotation] */
     _In_ UINT InputSize,
     /* [annotation] */
     _In_reads_bytes_(InputSize) const void * pInput,
     /* [annotation] */
     _Out_ D3D11_AUTHENTICATED_CONFIGURE_OUTPUT * pOutput);

    void(STDMETHODCALLTYPE * VideoProcessorSetStreamRotation)(ID3D11VideoContext * This,
                                                              /* [annotation] */
                                                              _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                              /* [annotation] */
                                                              _In_ UINT StreamIndex,
                                                              /* [annotation] */
                                                              _In_ BOOL Enable,
                                                              /* [annotation] */
                                                              _In_ D3D11_VIDEO_PROCESSOR_ROTATION Rotation);

    void(STDMETHODCALLTYPE * VideoProcessorGetStreamRotation)(ID3D11VideoContext * This,
                                                              /* [annotation] */
                                                              _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                              /* [annotation] */
                                                              _In_ UINT StreamIndex,
                                                              /* [annotation] */
                                                              _Out_ BOOL * pEnable,
                                                              /* [annotation] */
                                                              _Out_ D3D11_VIDEO_PROCESSOR_ROTATION * pRotation);

    END_INTERFACE
} ID3D11VideoContextVtbl;

interface ID3D11VideoContext {
    CONST_VTBL struct ID3D11VideoContextVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11VideoContext_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11VideoContext_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11VideoContext_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11VideoContext_GetDevice(This, ppDevice) ((This)->lpVtbl->GetDevice(This, ppDevice))

                #define ID3D11VideoContext_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11VideoContext_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11VideoContext_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11VideoContext_GetDecoderBuffer(This, pDecoder, Type, pBufferSize, ppBuffer) \
                    ((This)->lpVtbl->GetDecoderBuffer(This, pDecoder, Type, pBufferSize, ppBuffer))

                #define ID3D11VideoContext_ReleaseDecoderBuffer(This, pDecoder, Type) ((This)->lpVtbl->ReleaseDecoderBuffer(This, pDecoder, Type))

                #define ID3D11VideoContext_DecoderBeginFrame(This, pDecoder, pView, ContentKeySize, pContentKey) \
                    ((This)->lpVtbl->DecoderBeginFrame(This, pDecoder, pView, ContentKeySize, pContentKey))

                #define ID3D11VideoContext_DecoderEndFrame(This, pDecoder) ((This)->lpVtbl->DecoderEndFrame(This, pDecoder))

                #define ID3D11VideoContext_SubmitDecoderBuffers(This, pDecoder, NumBuffers, pBufferDesc) \
                    ((This)->lpVtbl->SubmitDecoderBuffers(This, pDecoder, NumBuffers, pBufferDesc))

                #define ID3D11VideoContext_DecoderExtension(This, pDecoder, pExtensionData) ((This)->lpVtbl->DecoderExtension(This, pDecoder, pExtensionData))

                #define ID3D11VideoContext_VideoProcessorSetOutputTargetRect(This, pVideoProcessor, Enable, pRect) \
                    ((This)->lpVtbl->VideoProcessorSetOutputTargetRect(This, pVideoProcessor, Enable, pRect))

                #define ID3D11VideoContext_VideoProcessorSetOutputBackgroundColor(This, pVideoProcessor, YCbCr, pColor) \
                    ((This)->lpVtbl->VideoProcessorSetOutputBackgroundColor(This, pVideoProcessor, YCbCr, pColor))

                #define ID3D11VideoContext_VideoProcessorSetOutputColorSpace(This, pVideoProcessor, pColorSpace) \
                    ((This)->lpVtbl->VideoProcessorSetOutputColorSpace(This, pVideoProcessor, pColorSpace))

                #define ID3D11VideoContext_VideoProcessorSetOutputAlphaFillMode(This, pVideoProcessor, AlphaFillMode, StreamIndex) \
                    ((This)->lpVtbl->VideoProcessorSetOutputAlphaFillMode(This, pVideoProcessor, AlphaFillMode, StreamIndex))

                #define ID3D11VideoContext_VideoProcessorSetOutputConstriction(This, pVideoProcessor, Enable, Size) \
                    ((This)->lpVtbl->VideoProcessorSetOutputConstriction(This, pVideoProcessor, Enable, Size))

                #define ID3D11VideoContext_VideoProcessorSetOutputStereoMode(This, pVideoProcessor, Enable) \
                    ((This)->lpVtbl->VideoProcessorSetOutputStereoMode(This, pVideoProcessor, Enable))

                #define ID3D11VideoContext_VideoProcessorSetOutputExtension(This, pVideoProcessor, pExtensionGuid, DataSize, pData) \
                    ((This)->lpVtbl->VideoProcessorSetOutputExtension(This, pVideoProcessor, pExtensionGuid, DataSize, pData))

                #define ID3D11VideoContext_VideoProcessorGetOutputTargetRect(This, pVideoProcessor, Enabled, pRect) \
                    ((This)->lpVtbl->VideoProcessorGetOutputTargetRect(This, pVideoProcessor, Enabled, pRect))

                #define ID3D11VideoContext_VideoProcessorGetOutputBackgroundColor(This, pVideoProcessor, pYCbCr, pColor) \
                    ((This)->lpVtbl->VideoProcessorGetOutputBackgroundColor(This, pVideoProcessor, pYCbCr, pColor))

                #define ID3D11VideoContext_VideoProcessorGetOutputColorSpace(This, pVideoProcessor, pColorSpace) \
                    ((This)->lpVtbl->VideoProcessorGetOutputColorSpace(This, pVideoProcessor, pColorSpace))

                #define ID3D11VideoContext_VideoProcessorGetOutputAlphaFillMode(This, pVideoProcessor, pAlphaFillMode, pStreamIndex) \
                    ((This)->lpVtbl->VideoProcessorGetOutputAlphaFillMode(This, pVideoProcessor, pAlphaFillMode, pStreamIndex))

                #define ID3D11VideoContext_VideoProcessorGetOutputConstriction(This, pVideoProcessor, pEnabled, pSize) \
                    ((This)->lpVtbl->VideoProcessorGetOutputConstriction(This, pVideoProcessor, pEnabled, pSize))

                #define ID3D11VideoContext_VideoProcessorGetOutputStereoMode(This, pVideoProcessor, pEnabled) \
                    ((This)->lpVtbl->VideoProcessorGetOutputStereoMode(This, pVideoProcessor, pEnabled))

                #define ID3D11VideoContext_VideoProcessorGetOutputExtension(This, pVideoProcessor, pExtensionGuid, DataSize, pData) \
                    ((This)->lpVtbl->VideoProcessorGetOutputExtension(This, pVideoProcessor, pExtensionGuid, DataSize, pData))

                #define ID3D11VideoContext_VideoProcessorSetStreamFrameFormat(This, pVideoProcessor, StreamIndex, FrameFormat) \
                    ((This)->lpVtbl->VideoProcessorSetStreamFrameFormat(This, pVideoProcessor, StreamIndex, FrameFormat))

                #define ID3D11VideoContext_VideoProcessorSetStreamColorSpace(This, pVideoProcessor, StreamIndex, pColorSpace) \
                    ((This)->lpVtbl->VideoProcessorSetStreamColorSpace(This, pVideoProcessor, StreamIndex, pColorSpace))

                #define ID3D11VideoContext_VideoProcessorSetStreamOutputRate(This, pVideoProcessor, StreamIndex, OutputRate, RepeatFrame, pCustomRate) \
                    ((This)->lpVtbl->VideoProcessorSetStreamOutputRate(This, pVideoProcessor, StreamIndex, OutputRate, RepeatFrame, pCustomRate))

                #define ID3D11VideoContext_VideoProcessorSetStreamSourceRect(This, pVideoProcessor, StreamIndex, Enable, pRect) \
                    ((This)->lpVtbl->VideoProcessorSetStreamSourceRect(This, pVideoProcessor, StreamIndex, Enable, pRect))

                #define ID3D11VideoContext_VideoProcessorSetStreamDestRect(This, pVideoProcessor, StreamIndex, Enable, pRect) \
                    ((This)->lpVtbl->VideoProcessorSetStreamDestRect(This, pVideoProcessor, StreamIndex, Enable, pRect))

                #define ID3D11VideoContext_VideoProcessorSetStreamAlpha(This, pVideoProcessor, StreamIndex, Enable, Alpha) \
                    ((This)->lpVtbl->VideoProcessorSetStreamAlpha(This, pVideoProcessor, StreamIndex, Enable, Alpha))

                #define ID3D11VideoContext_VideoProcessorSetStreamPalette(This, pVideoProcessor, StreamIndex, Count, pEntries) \
                    ((This)->lpVtbl->VideoProcessorSetStreamPalette(This, pVideoProcessor, StreamIndex, Count, pEntries))

                #define ID3D11VideoContext_VideoProcessorSetStreamPixelAspectRatio(This, pVideoProcessor, StreamIndex, Enable, pSourceAspectRatio, \
                                                                                   pDestinationAspectRatio)                                        \
                    ((This)->lpVtbl->VideoProcessorSetStreamPixelAspectRatio(This, pVideoProcessor, StreamIndex, Enable, pSourceAspectRatio,       \
                                                                             pDestinationAspectRatio))

                #define ID3D11VideoContext_VideoProcessorSetStreamLumaKey(This, pVideoProcessor, StreamIndex, Enable, Lower, Upper) \
                    ((This)->lpVtbl->VideoProcessorSetStreamLumaKey(This, pVideoProcessor, StreamIndex, Enable, Lower, Upper))

                #define ID3D11VideoContext_VideoProcessorSetStreamStereoFormat(This, pVideoProcessor, StreamIndex, Enable, Format, LeftViewFrame0,           \
                                                                               BaseViewFrame0, FlipMode, MonoOffset)                                         \
                    ((This)->lpVtbl->VideoProcessorSetStreamStereoFormat(This, pVideoProcessor, StreamIndex, Enable, Format, LeftViewFrame0, BaseViewFrame0, \
                                                                         FlipMode, MonoOffset))

                #define ID3D11VideoContext_VideoProcessorSetStreamAutoProcessingMode(This, pVideoProcessor, StreamIndex, Enable) \
                    ((This)->lpVtbl->VideoProcessorSetStreamAutoProcessingMode(This, pVideoProcessor, StreamIndex, Enable))

                #define ID3D11VideoContext_VideoProcessorSetStreamFilter(This, pVideoProcessor, StreamIndex, Filter, Enable, Level) \
                    ((This)->lpVtbl->VideoProcessorSetStreamFilter(This, pVideoProcessor, StreamIndex, Filter, Enable, Level))

                #define ID3D11VideoContext_VideoProcessorSetStreamExtension(This, pVideoProcessor, StreamIndex, pExtensionGuid, DataSize, pData) \
                    ((This)->lpVtbl->VideoProcessorSetStreamExtension(This, pVideoProcessor, StreamIndex, pExtensionGuid, DataSize, pData))

                #define ID3D11VideoContext_VideoProcessorGetStreamFrameFormat(This, pVideoProcessor, StreamIndex, pFrameFormat) \
                    ((This)->lpVtbl->VideoProcessorGetStreamFrameFormat(This, pVideoProcessor, StreamIndex, pFrameFormat))

                #define ID3D11VideoContext_VideoProcessorGetStreamColorSpace(This, pVideoProcessor, StreamIndex, pColorSpace) \
                    ((This)->lpVtbl->VideoProcessorGetStreamColorSpace(This, pVideoProcessor, StreamIndex, pColorSpace))

                #define ID3D11VideoContext_VideoProcessorGetStreamOutputRate(This, pVideoProcessor, StreamIndex, pOutputRate, pRepeatFrame, pCustomRate) \
                    ((This)->lpVtbl->VideoProcessorGetStreamOutputRate(This, pVideoProcessor, StreamIndex, pOutputRate, pRepeatFrame, pCustomRate))

                #define ID3D11VideoContext_VideoProcessorGetStreamSourceRect(This, pVideoProcessor, StreamIndex, pEnabled, pRect) \
                    ((This)->lpVtbl->VideoProcessorGetStreamSourceRect(This, pVideoProcessor, StreamIndex, pEnabled, pRect))

                #define ID3D11VideoContext_VideoProcessorGetStreamDestRect(This, pVideoProcessor, StreamIndex, pEnabled, pRect) \
                    ((This)->lpVtbl->VideoProcessorGetStreamDestRect(This, pVideoProcessor, StreamIndex, pEnabled, pRect))

                #define ID3D11VideoContext_VideoProcessorGetStreamAlpha(This, pVideoProcessor, StreamIndex, pEnabled, pAlpha) \
                    ((This)->lpVtbl->VideoProcessorGetStreamAlpha(This, pVideoProcessor, StreamIndex, pEnabled, pAlpha))

                #define ID3D11VideoContext_VideoProcessorGetStreamPalette(This, pVideoProcessor, StreamIndex, Count, pEntries) \
                    ((This)->lpVtbl->VideoProcessorGetStreamPalette(This, pVideoProcessor, StreamIndex, Count, pEntries))

                #define ID3D11VideoContext_VideoProcessorGetStreamPixelAspectRatio(This, pVideoProcessor, StreamIndex, pEnabled, pSourceAspectRatio, \
                                                                                   pDestinationAspectRatio)                                          \
                    ((This)->lpVtbl->VideoProcessorGetStreamPixelAspectRatio(This, pVideoProcessor, StreamIndex, pEnabled, pSourceAspectRatio,       \
                                                                             pDestinationAspectRatio))

                #define ID3D11VideoContext_VideoProcessorGetStreamLumaKey(This, pVideoProcessor, StreamIndex, pEnabled, pLower, pUpper) \
                    ((This)->lpVtbl->VideoProcessorGetStreamLumaKey(This, pVideoProcessor, StreamIndex, pEnabled, pLower, pUpper))

                #define ID3D11VideoContext_VideoProcessorGetStreamStereoFormat(This, pVideoProcessor, StreamIndex, pEnable, pFormat, pLeftViewFrame0, \
                                                                               pBaseViewFrame0, pFlipMode, MonoOffset)                                \
                    ((This)->lpVtbl->VideoProcessorGetStreamStereoFormat(This, pVideoProcessor, StreamIndex, pEnable, pFormat, pLeftViewFrame0,       \
                                                                         pBaseViewFrame0, pFlipMode, MonoOffset))

                #define ID3D11VideoContext_VideoProcessorGetStreamAutoProcessingMode(This, pVideoProcessor, StreamIndex, pEnabled) \
                    ((This)->lpVtbl->VideoProcessorGetStreamAutoProcessingMode(This, pVideoProcessor, StreamIndex, pEnabled))

                #define ID3D11VideoContext_VideoProcessorGetStreamFilter(This, pVideoProcessor, StreamIndex, Filter, pEnabled, pLevel) \
                    ((This)->lpVtbl->VideoProcessorGetStreamFilter(This, pVideoProcessor, StreamIndex, Filter, pEnabled, pLevel))

                #define ID3D11VideoContext_VideoProcessorGetStreamExtension(This, pVideoProcessor, StreamIndex, pExtensionGuid, DataSize, pData) \
                    ((This)->lpVtbl->VideoProcessorGetStreamExtension(This, pVideoProcessor, StreamIndex, pExtensionGuid, DataSize, pData))

                #define ID3D11VideoContext_VideoProcessorBlt(This, pVideoProcessor, pView, OutputFrame, StreamCount, pStreams) \
                    ((This)->lpVtbl->VideoProcessorBlt(This, pVideoProcessor, pView, OutputFrame, StreamCount, pStreams))

                #define ID3D11VideoContext_NegotiateCryptoSessionKeyExchange(This, pCryptoSession, DataSize, pData) \
                    ((This)->lpVtbl->NegotiateCryptoSessionKeyExchange(This, pCryptoSession, DataSize, pData))

                #define ID3D11VideoContext_EncryptionBlt(This, pCryptoSession, pSrcSurface, pDstSurface, IVSize, pIV) \
                    ((This)->lpVtbl->EncryptionBlt(This, pCryptoSession, pSrcSurface, pDstSurface, IVSize, pIV))

                #define ID3D11VideoContext_DecryptionBlt(This, pCryptoSession, pSrcSurface, pDstSurface, pEncryptedBlockInfo, ContentKeySize, pContentKey,   \
                                                         IVSize, pIV)                                                                                        \
                    ((This)->lpVtbl->DecryptionBlt(This, pCryptoSession, pSrcSurface, pDstSurface, pEncryptedBlockInfo, ContentKeySize, pContentKey, IVSize, \
                                                   pIV))

                #define ID3D11VideoContext_StartSessionKeyRefresh(This, pCryptoSession, RandomNumberSize, pRandomNumber) \
                    ((This)->lpVtbl->StartSessionKeyRefresh(This, pCryptoSession, RandomNumberSize, pRandomNumber))

                #define ID3D11VideoContext_FinishSessionKeyRefresh(This, pCryptoSession) ((This)->lpVtbl->FinishSessionKeyRefresh(This, pCryptoSession))

                #define ID3D11VideoContext_GetEncryptionBltKey(This, pCryptoSession, KeySize, pReadbackKey) \
                    ((This)->lpVtbl->GetEncryptionBltKey(This, pCryptoSession, KeySize, pReadbackKey))

                #define ID3D11VideoContext_NegotiateAuthenticatedChannelKeyExchange(This, pChannel, DataSize, pData) \
                    ((This)->lpVtbl->NegotiateAuthenticatedChannelKeyExchange(This, pChannel, DataSize, pData))

                #define ID3D11VideoContext_QueryAuthenticatedChannel(This, pChannel, InputSize, pInput, OutputSize, pOutput) \
                    ((This)->lpVtbl->QueryAuthenticatedChannel(This, pChannel, InputSize, pInput, OutputSize, pOutput))

                #define ID3D11VideoContext_ConfigureAuthenticatedChannel(This, pChannel, InputSize, pInput, pOutput) \
                    ((This)->lpVtbl->ConfigureAuthenticatedChannel(This, pChannel, InputSize, pInput, pOutput))

                #define ID3D11VideoContext_VideoProcessorSetStreamRotation(This, pVideoProcessor, StreamIndex, Enable, Rotation) \
                    ((This)->lpVtbl->VideoProcessorSetStreamRotation(This, pVideoProcessor, StreamIndex, Enable, Rotation))

                #define ID3D11VideoContext_VideoProcessorGetStreamRotation(This, pVideoProcessor, StreamIndex, pEnable, pRotation) \
                    ((This)->lpVtbl->VideoProcessorGetStreamRotation(This, pVideoProcessor, StreamIndex, pEnable, pRotation))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11VideoContext_INTERFACE_DEFINED__ */

    #ifndef __ID3D11VideoDevice_INTERFACE_DEFINED__
        #define __ID3D11VideoDevice_INTERFACE_DEFINED__

/* interface ID3D11VideoDevice */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11VideoDevice;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("10EC4D5B-975A-4689-B9E4-D0AAC30FE333")
ID3D11VideoDevice : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE CreateVideoDecoder(
        /* [annotation] */
        _In_ const D3D11_VIDEO_DECODER_DESC * pVideoDesc,
        /* [annotation] */
        _In_ const D3D11_VIDEO_DECODER_CONFIG * pConfig,
        /* [annotation] */
        _Out_ ID3D11VideoDecoder ** ppDecoder) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateVideoProcessor(
        /* [annotation] */
        _In_ ID3D11VideoProcessorEnumerator * pEnum,
        /* [annotation] */
        _In_ UINT RateConversionIndex,
        /* [annotation] */
        _Out_ ID3D11VideoProcessor * *ppVideoProcessor) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateAuthenticatedChannel(
        /* [annotation] */
        _In_ D3D11_AUTHENTICATED_CHANNEL_TYPE ChannelType,
        /* [annotation] */
        _Out_ ID3D11AuthenticatedChannel * *ppAuthenticatedChannel) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateCryptoSession(
        /* [annotation] */
        _In_ const GUID * pCryptoType,
        /* [annotation] */
        _In_opt_ const GUID * pDecoderProfile,
        /* [annotation] */
        _In_ const GUID * pKeyExchangeType,
        /* [annotation] */
        _Outptr_ ID3D11CryptoSession ** ppCryptoSession) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateVideoDecoderOutputView(
        /* [annotation] */
        _In_ ID3D11Resource * pResource,
        /* [annotation] */
        _In_ const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc,
        /* [annotation] */
        _Out_opt_ ID3D11VideoDecoderOutputView ** ppVDOVView) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateVideoProcessorInputView(
        /* [annotation] */
        _In_ ID3D11Resource * pResource,
        /* [annotation] */
        _In_ ID3D11VideoProcessorEnumerator * pEnum,
        /* [annotation] */
        _In_ const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc,
        /* [annotation] */
        _Out_opt_ ID3D11VideoProcessorInputView ** ppVPIView) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateVideoProcessorOutputView(
        /* [annotation] */
        _In_ ID3D11Resource * pResource,
        /* [annotation] */
        _In_ ID3D11VideoProcessorEnumerator * pEnum,
        /* [annotation] */
        _In_ const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc,
        /* [annotation] */
        _Out_opt_ ID3D11VideoProcessorOutputView ** ppVPOView) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateVideoProcessorEnumerator(
        /* [annotation] */
        _In_ const D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc,
        /* [annotation] */
        _Out_ ID3D11VideoProcessorEnumerator ** ppEnum) = 0;

    virtual UINT STDMETHODCALLTYPE GetVideoDecoderProfileCount(void) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetVideoDecoderProfile(
        /* [annotation] */
        _In_ UINT Index,
        /* [annotation] */
        _Out_ GUID * pDecoderProfile) = 0;

    virtual HRESULT STDMETHODCALLTYPE CheckVideoDecoderFormat(
        /* [annotation] */
        _In_ const GUID * pDecoderProfile,
        /* [annotation] */
        _In_ DXGI_FORMAT Format,
        /* [annotation] */
        _Out_ BOOL * pSupported) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetVideoDecoderConfigCount(
        /* [annotation] */
        _In_ const D3D11_VIDEO_DECODER_DESC * pDesc,
        /* [annotation] */
        _Out_ UINT * pCount) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetVideoDecoderConfig(
        /* [annotation] */
        _In_ const D3D11_VIDEO_DECODER_DESC * pDesc,
        /* [annotation] */
        _In_ UINT Index,
        /* [annotation] */
        _Out_ D3D11_VIDEO_DECODER_CONFIG * pConfig) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetContentProtectionCaps(
        /* [annotation] */
        _In_opt_ const GUID * pCryptoType,
        /* [annotation] */
        _In_opt_ const GUID * pDecoderProfile,
        /* [annotation] */
        _Out_ D3D11_VIDEO_CONTENT_PROTECTION_CAPS * pCaps) = 0;

    virtual HRESULT STDMETHODCALLTYPE CheckCryptoKeyExchange(
        /* [annotation] */
        _In_ const GUID * pCryptoType,
        /* [annotation] */
        _In_opt_ const GUID * pDecoderProfile,
        /* [annotation] */
        _In_ UINT Index,
        /* [annotation] */
        _Out_ GUID * pKeyExchangeType) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
        /* [annotation] */
        _In_ REFGUID guid,
        /* [annotation] */
        _In_ UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_(DataSize) const void * pData) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
        /* [annotation] */
        _In_ REFGUID guid,
        /* [annotation] */
        _In_opt_ const IUnknown * pData) = 0;
};

        #else /* C style interface */

typedef struct ID3D11VideoDeviceVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11VideoDevice * This,
     /* [in] */ REFIID   riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11VideoDevice * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11VideoDevice * This);

    HRESULT(STDMETHODCALLTYPE * CreateVideoDecoder)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ const D3D11_VIDEO_DECODER_DESC * pVideoDesc,
     /* [annotation] */
     _In_ const D3D11_VIDEO_DECODER_CONFIG * pConfig,
     /* [annotation] */
     _Out_ ID3D11VideoDecoder ** ppDecoder);

    HRESULT(STDMETHODCALLTYPE * CreateVideoProcessor)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ ID3D11VideoProcessorEnumerator * pEnum,
     /* [annotation] */
     _In_ UINT RateConversionIndex,
     /* [annotation] */
     _Out_ ID3D11VideoProcessor ** ppVideoProcessor);

    HRESULT(STDMETHODCALLTYPE * CreateAuthenticatedChannel)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ D3D11_AUTHENTICATED_CHANNEL_TYPE ChannelType,
     /* [annotation] */
     _Out_ ID3D11AuthenticatedChannel ** ppAuthenticatedChannel);

    HRESULT(STDMETHODCALLTYPE * CreateCryptoSession)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ const GUID * pCryptoType,
     /* [annotation] */
     _In_opt_ const GUID * pDecoderProfile,
     /* [annotation] */
     _In_ const GUID * pKeyExchangeType,
     /* [annotation] */
     _Outptr_ ID3D11CryptoSession ** ppCryptoSession);

    HRESULT(STDMETHODCALLTYPE * CreateVideoDecoderOutputView)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ ID3D11Resource * pResource,
     /* [annotation] */
     _In_ const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc,
     /* [annotation] */
     _Out_opt_ ID3D11VideoDecoderOutputView ** ppVDOVView);

    HRESULT(STDMETHODCALLTYPE * CreateVideoProcessorInputView)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ ID3D11Resource * pResource,
     /* [annotation] */
     _In_ ID3D11VideoProcessorEnumerator * pEnum,
     /* [annotation] */
     _In_ const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc,
     /* [annotation] */
     _Out_opt_ ID3D11VideoProcessorInputView ** ppVPIView);

    HRESULT(STDMETHODCALLTYPE * CreateVideoProcessorOutputView)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ ID3D11Resource * pResource,
     /* [annotation] */
     _In_ ID3D11VideoProcessorEnumerator * pEnum,
     /* [annotation] */
     _In_ const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc,
     /* [annotation] */
     _Out_opt_ ID3D11VideoProcessorOutputView ** ppVPOView);

    HRESULT(STDMETHODCALLTYPE * CreateVideoProcessorEnumerator)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ const D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc,
     /* [annotation] */
     _Out_ ID3D11VideoProcessorEnumerator ** ppEnum);

    UINT(STDMETHODCALLTYPE * GetVideoDecoderProfileCount)(ID3D11VideoDevice * This);

    HRESULT(STDMETHODCALLTYPE * GetVideoDecoderProfile)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ UINT Index,
     /* [annotation] */
     _Out_ GUID * pDecoderProfile);

    HRESULT(STDMETHODCALLTYPE * CheckVideoDecoderFormat)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ const GUID * pDecoderProfile,
     /* [annotation] */
     _In_ DXGI_FORMAT Format,
     /* [annotation] */
     _Out_ BOOL * pSupported);

    HRESULT(STDMETHODCALLTYPE * GetVideoDecoderConfigCount)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ const D3D11_VIDEO_DECODER_DESC * pDesc,
     /* [annotation] */
     _Out_ UINT * pCount);

    HRESULT(STDMETHODCALLTYPE * GetVideoDecoderConfig)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ const D3D11_VIDEO_DECODER_DESC * pDesc,
     /* [annotation] */
     _In_ UINT Index,
     /* [annotation] */
     _Out_ D3D11_VIDEO_DECODER_CONFIG * pConfig);

    HRESULT(STDMETHODCALLTYPE * GetContentProtectionCaps)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_opt_ const GUID * pCryptoType,
     /* [annotation] */
     _In_opt_ const GUID * pDecoderProfile,
     /* [annotation] */
     _Out_ D3D11_VIDEO_CONTENT_PROTECTION_CAPS * pCaps);

    HRESULT(STDMETHODCALLTYPE * CheckCryptoKeyExchange)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ const GUID * pCryptoType,
     /* [annotation] */
     _In_opt_ const GUID * pDecoderProfile,
     /* [annotation] */
     _In_ UINT Index,
     /* [annotation] */
     _Out_ GUID * pKeyExchangeType);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11VideoDevice * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    END_INTERFACE
} ID3D11VideoDeviceVtbl;

interface ID3D11VideoDevice {
    CONST_VTBL struct ID3D11VideoDeviceVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11VideoDevice_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11VideoDevice_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11VideoDevice_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11VideoDevice_CreateVideoDecoder(This, pVideoDesc, pConfig, ppDecoder) \
                    ((This)->lpVtbl->CreateVideoDecoder(This, pVideoDesc, pConfig, ppDecoder))

                #define ID3D11VideoDevice_CreateVideoProcessor(This, pEnum, RateConversionIndex, ppVideoProcessor) \
                    ((This)->lpVtbl->CreateVideoProcessor(This, pEnum, RateConversionIndex, ppVideoProcessor))

                #define ID3D11VideoDevice_CreateAuthenticatedChannel(This, ChannelType, ppAuthenticatedChannel) \
                    ((This)->lpVtbl->CreateAuthenticatedChannel(This, ChannelType, ppAuthenticatedChannel))

                #define ID3D11VideoDevice_CreateCryptoSession(This, pCryptoType, pDecoderProfile, pKeyExchangeType, ppCryptoSession) \
                    ((This)->lpVtbl->CreateCryptoSession(This, pCryptoType, pDecoderProfile, pKeyExchangeType, ppCryptoSession))

                #define ID3D11VideoDevice_CreateVideoDecoderOutputView(This, pResource, pDesc, ppVDOVView) \
                    ((This)->lpVtbl->CreateVideoDecoderOutputView(This, pResource, pDesc, ppVDOVView))

                #define ID3D11VideoDevice_CreateVideoProcessorInputView(This, pResource, pEnum, pDesc, ppVPIView) \
                    ((This)->lpVtbl->CreateVideoProcessorInputView(This, pResource, pEnum, pDesc, ppVPIView))

                #define ID3D11VideoDevice_CreateVideoProcessorOutputView(This, pResource, pEnum, pDesc, ppVPOView) \
                    ((This)->lpVtbl->CreateVideoProcessorOutputView(This, pResource, pEnum, pDesc, ppVPOView))

                #define ID3D11VideoDevice_CreateVideoProcessorEnumerator(This, pDesc, ppEnum) \
                    ((This)->lpVtbl->CreateVideoProcessorEnumerator(This, pDesc, ppEnum))

                #define ID3D11VideoDevice_GetVideoDecoderProfileCount(This) ((This)->lpVtbl->GetVideoDecoderProfileCount(This))

                #define ID3D11VideoDevice_GetVideoDecoderProfile(This, Index, pDecoderProfile) \
                    ((This)->lpVtbl->GetVideoDecoderProfile(This, Index, pDecoderProfile))

                #define ID3D11VideoDevice_CheckVideoDecoderFormat(This, pDecoderProfile, Format, pSupported) \
                    ((This)->lpVtbl->CheckVideoDecoderFormat(This, pDecoderProfile, Format, pSupported))

                #define ID3D11VideoDevice_GetVideoDecoderConfigCount(This, pDesc, pCount) ((This)->lpVtbl->GetVideoDecoderConfigCount(This, pDesc, pCount))

                #define ID3D11VideoDevice_GetVideoDecoderConfig(This, pDesc, Index, pConfig) \
                    ((This)->lpVtbl->GetVideoDecoderConfig(This, pDesc, Index, pConfig))

                #define ID3D11VideoDevice_GetContentProtectionCaps(This, pCryptoType, pDecoderProfile, pCaps) \
                    ((This)->lpVtbl->GetContentProtectionCaps(This, pCryptoType, pDecoderProfile, pCaps))

                #define ID3D11VideoDevice_CheckCryptoKeyExchange(This, pCryptoType, pDecoderProfile, Index, pKeyExchangeType) \
                    ((This)->lpVtbl->CheckCryptoKeyExchange(This, pCryptoType, pDecoderProfile, Index, pKeyExchangeType))

                #define ID3D11VideoDevice_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11VideoDevice_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11VideoDevice_INTERFACE_DEFINED__ */

    /* interface __MIDL_itf_d3d11_0000_0040 */
    /* [local] */

    #include "winapifamily.h"

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0040_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0040_v0_0_s_ifspec;

    #ifndef __ID3D11Device_INTERFACE_DEFINED__
        #define __ID3D11Device_INTERFACE_DEFINED__

/* interface ID3D11Device */
/* [unique][local][object][uuid] */

EXTERN_C const IID IID_ID3D11Device;

        #if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("db6f6ddb-ac77-4e88-8253-819df9bbf140")
ID3D11Device : public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE CreateBuffer(
        /* [annotation] */
        _In_ const D3D11_BUFFER_DESC * pDesc,
        /* [annotation] */
        _In_opt_ const D3D11_SUBRESOURCE_DATA * pInitialData,
        /* [annotation] */
        _Out_opt_ ID3D11Buffer ** ppBuffer) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateTexture1D(
        /* [annotation] */
        _In_ const D3D11_TEXTURE1D_DESC * pDesc,
        /* [annotation] */
        _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize)) const D3D11_SUBRESOURCE_DATA * pInitialData,
        /* [annotation] */
        _Out_opt_ ID3D11Texture1D ** ppTexture1D) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateTexture2D(
        /* [annotation] */
        _In_ const D3D11_TEXTURE2D_DESC * pDesc,
        /* [annotation] */
        _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize)) const D3D11_SUBRESOURCE_DATA * pInitialData,
        /* [annotation] */
        _Out_opt_ ID3D11Texture2D ** ppTexture2D) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateTexture3D(
        /* [annotation] */
        _In_ const D3D11_TEXTURE3D_DESC * pDesc,
        /* [annotation] */
        _In_reads_opt_(_Inexpressible_(pDesc->MipLevels)) const D3D11_SUBRESOURCE_DATA * pInitialData,
        /* [annotation] */
        _Out_opt_ ID3D11Texture3D ** ppTexture3D) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateShaderResourceView(
        /* [annotation] */
        _In_ ID3D11Resource * pResource,
        /* [annotation] */
        _In_opt_ const D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc,
        /* [annotation] */
        _Out_opt_ ID3D11ShaderResourceView ** ppSRView) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateUnorderedAccessView(
        /* [annotation] */
        _In_ ID3D11Resource * pResource,
        /* [annotation] */
        _In_opt_ const D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc,
        /* [annotation] */
        _Out_opt_ ID3D11UnorderedAccessView ** ppUAView) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateRenderTargetView(
        /* [annotation] */
        _In_ ID3D11Resource * pResource,
        /* [annotation] */
        _In_opt_ const D3D11_RENDER_TARGET_VIEW_DESC * pDesc,
        /* [annotation] */
        _Out_opt_ ID3D11RenderTargetView ** ppRTView) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilView(
        /* [annotation] */
        _In_ ID3D11Resource * pResource,
        /* [annotation] */
        _In_opt_ const D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc,
        /* [annotation] */
        _Out_opt_ ID3D11DepthStencilView ** ppDepthStencilView) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateInputLayout(
        /* [annotation] */
        _In_reads_(NumElements) const D3D11_INPUT_ELEMENT_DESC * pInputElementDescs,
        /* [annotation] */
        _In_range_(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT) UINT NumElements,
        /* [annotation] */
        _In_ const void * pShaderBytecodeWithInputSignature,
        /* [annotation] */
        _In_ SIZE_T BytecodeLength,
        /* [annotation] */
        _Out_opt_ ID3D11InputLayout ** ppInputLayout) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateVertexShader(
        /* [annotation] */
        _In_ const void * pShaderBytecode,
        /* [annotation] */
        _In_ SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_ ID3D11ClassLinkage * pClassLinkage,
        /* [annotation] */
        _Out_opt_ ID3D11VertexShader ** ppVertexShader) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateGeometryShader(
        /* [annotation] */
        _In_ const void * pShaderBytecode,
        /* [annotation] */
        _In_ SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_ ID3D11ClassLinkage * pClassLinkage,
        /* [annotation] */
        _Out_opt_ ID3D11GeometryShader ** ppGeometryShader) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateGeometryShaderWithStreamOutput(
        /* [annotation] */
        _In_ const void * pShaderBytecode,
        /* [annotation] */
        _In_ SIZE_T BytecodeLength,
        /* [annotation] */
        _In_reads_opt_(NumEntries) const D3D11_SO_DECLARATION_ENTRY * pSODeclaration,
        /* [annotation] */
        _In_range_(0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT) UINT NumEntries,
        /* [annotation] */
        _In_reads_opt_(NumStrides) const UINT * pBufferStrides,
        /* [annotation] */
        _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumStrides,
        /* [annotation] */
        _In_ UINT RasterizedStream,
        /* [annotation] */
        _In_opt_ ID3D11ClassLinkage * pClassLinkage,
        /* [annotation] */
        _Out_opt_ ID3D11GeometryShader ** ppGeometryShader) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreatePixelShader(
        /* [annotation] */
        _In_ const void * pShaderBytecode,
        /* [annotation] */
        _In_ SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_ ID3D11ClassLinkage * pClassLinkage,
        /* [annotation] */
        _Out_opt_ ID3D11PixelShader ** ppPixelShader) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateHullShader(
        /* [annotation] */
        _In_ const void * pShaderBytecode,
        /* [annotation] */
        _In_ SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_ ID3D11ClassLinkage * pClassLinkage,
        /* [annotation] */
        _Out_opt_ ID3D11HullShader ** ppHullShader) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateDomainShader(
        /* [annotation] */
        _In_ const void * pShaderBytecode,
        /* [annotation] */
        _In_ SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_ ID3D11ClassLinkage * pClassLinkage,
        /* [annotation] */
        _Out_opt_ ID3D11DomainShader ** ppDomainShader) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateComputeShader(
        /* [annotation] */
        _In_ const void * pShaderBytecode,
        /* [annotation] */
        _In_ SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_ ID3D11ClassLinkage * pClassLinkage,
        /* [annotation] */
        _Out_opt_ ID3D11ComputeShader ** ppComputeShader) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateClassLinkage(
        /* [annotation] */
        _Out_ ID3D11ClassLinkage * *ppLinkage) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateBlendState(
        /* [annotation] */
        _In_ const D3D11_BLEND_DESC * pBlendStateDesc,
        /* [annotation] */
        _Out_opt_ ID3D11BlendState ** ppBlendState) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilState(
        /* [annotation] */
        _In_ const D3D11_DEPTH_STENCIL_DESC * pDepthStencilDesc,
        /* [annotation] */
        _Out_opt_ ID3D11DepthStencilState ** ppDepthStencilState) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateRasterizerState(
        /* [annotation] */
        _In_ const D3D11_RASTERIZER_DESC * pRasterizerDesc,
        /* [annotation] */
        _Out_opt_ ID3D11RasterizerState ** ppRasterizerState) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateSamplerState(
        /* [annotation] */
        _In_ const D3D11_SAMPLER_DESC * pSamplerDesc,
        /* [annotation] */
        _Out_opt_ ID3D11SamplerState ** ppSamplerState) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateQuery(
        /* [annotation] */
        _In_ const D3D11_QUERY_DESC * pQueryDesc,
        /* [annotation] */
        _Out_opt_ ID3D11Query ** ppQuery) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreatePredicate(
        /* [annotation] */
        _In_ const D3D11_QUERY_DESC * pPredicateDesc,
        /* [annotation] */
        _Out_opt_ ID3D11Predicate ** ppPredicate) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateCounter(
        /* [annotation] */
        _In_ const D3D11_COUNTER_DESC * pCounterDesc,
        /* [annotation] */
        _Out_opt_ ID3D11Counter ** ppCounter) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateDeferredContext(UINT ContextFlags,
                                                            /* [annotation] */
                                                            _Out_opt_ ID3D11DeviceContext * *ppDeferredContext) = 0;

    virtual HRESULT STDMETHODCALLTYPE OpenSharedResource(
        /* [annotation] */
        _In_ HANDLE hResource,
        /* [annotation] */
        _In_ REFIID ReturnedInterface,
        /* [annotation] */
        _Out_opt_ void ** ppResource) = 0;

    virtual HRESULT STDMETHODCALLTYPE CheckFormatSupport(
        /* [annotation] */
        _In_ DXGI_FORMAT Format,
        /* [annotation] */
        _Out_ UINT * pFormatSupport) = 0;

    virtual HRESULT STDMETHODCALLTYPE CheckMultisampleQualityLevels(
        /* [annotation] */
        _In_ DXGI_FORMAT Format,
        /* [annotation] */
        _In_ UINT SampleCount,
        /* [annotation] */
        _Out_ UINT * pNumQualityLevels) = 0;

    virtual void STDMETHODCALLTYPE CheckCounterInfo(
        /* [annotation] */
        _Out_ D3D11_COUNTER_INFO * pCounterInfo) = 0;

    virtual HRESULT STDMETHODCALLTYPE CheckCounter(
        /* [annotation] */
        _In_ const D3D11_COUNTER_DESC * pDesc,
        /* [annotation] */
        _Out_ D3D11_COUNTER_TYPE * pType,
        /* [annotation] */
        _Out_ UINT * pActiveCounters,
        /* [annotation] */
        _Out_writes_opt_(*pNameLength) LPSTR szName,
        /* [annotation] */
        _Inout_opt_ UINT * pNameLength,
        /* [annotation] */
        _Out_writes_opt_(*pUnitsLength) LPSTR szUnits,
        /* [annotation] */
        _Inout_opt_ UINT * pUnitsLength,
        /* [annotation] */
        _Out_writes_opt_(*pDescriptionLength) LPSTR szDescription,
        /* [annotation] */
        _Inout_opt_ UINT * pDescriptionLength) = 0;

    virtual HRESULT STDMETHODCALLTYPE CheckFeatureSupport(D3D11_FEATURE Feature,
                                                          /* [annotation] */
                                                          _Out_writes_bytes_(FeatureSupportDataSize) void * pFeatureSupportData,
                                                          UINT                                              FeatureSupportDataSize) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
        /* [annotation] */
        _In_ REFGUID guid,
        /* [annotation] */
        _Inout_ UINT * pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_(*pDataSize) void * pData) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
        /* [annotation] */
        _In_ REFGUID guid,
        /* [annotation] */
        _In_ UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_(DataSize) const void * pData) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
        /* [annotation] */
        _In_ REFGUID guid,
        /* [annotation] */
        _In_opt_ const IUnknown * pData) = 0;

    virtual D3D_FEATURE_LEVEL STDMETHODCALLTYPE GetFeatureLevel(void) = 0;

    virtual UINT STDMETHODCALLTYPE GetCreationFlags(void) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetDeviceRemovedReason(void) = 0;

    virtual void STDMETHODCALLTYPE GetImmediateContext(
        /* [annotation] */
        _Out_ ID3D11DeviceContext * *ppImmediateContext) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetExceptionMode(UINT RaiseFlags) = 0;

    virtual UINT STDMETHODCALLTYPE GetExceptionMode(void) = 0;
};

        #else /* C style interface */

typedef struct ID3D11DeviceVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE * QueryInterface)
    (ID3D11Device *    This,
     /* [in] */ REFIID riid,
     /* [annotation][iid_is][out] */
     _COM_Outptr_ void ** ppvObject);

    ULONG(STDMETHODCALLTYPE * AddRef)(ID3D11Device * This);

    ULONG(STDMETHODCALLTYPE * Release)(ID3D11Device * This);

    HRESULT(STDMETHODCALLTYPE * CreateBuffer)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const D3D11_BUFFER_DESC * pDesc,
     /* [annotation] */
     _In_opt_ const D3D11_SUBRESOURCE_DATA * pInitialData,
     /* [annotation] */
     _Out_opt_ ID3D11Buffer ** ppBuffer);

    HRESULT(STDMETHODCALLTYPE * CreateTexture1D)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const D3D11_TEXTURE1D_DESC * pDesc,
     /* [annotation] */
     _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize)) const D3D11_SUBRESOURCE_DATA * pInitialData,
     /* [annotation] */
     _Out_opt_ ID3D11Texture1D ** ppTexture1D);

    HRESULT(STDMETHODCALLTYPE * CreateTexture2D)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const D3D11_TEXTURE2D_DESC * pDesc,
     /* [annotation] */
     _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize)) const D3D11_SUBRESOURCE_DATA * pInitialData,
     /* [annotation] */
     _Out_opt_ ID3D11Texture2D ** ppTexture2D);

    HRESULT(STDMETHODCALLTYPE * CreateTexture3D)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const D3D11_TEXTURE3D_DESC * pDesc,
     /* [annotation] */
     _In_reads_opt_(_Inexpressible_(pDesc->MipLevels)) const D3D11_SUBRESOURCE_DATA * pInitialData,
     /* [annotation] */
     _Out_opt_ ID3D11Texture3D ** ppTexture3D);

    HRESULT(STDMETHODCALLTYPE * CreateShaderResourceView)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ ID3D11Resource * pResource,
     /* [annotation] */
     _In_opt_ const D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc,
     /* [annotation] */
     _Out_opt_ ID3D11ShaderResourceView ** ppSRView);

    HRESULT(STDMETHODCALLTYPE * CreateUnorderedAccessView)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ ID3D11Resource * pResource,
     /* [annotation] */
     _In_opt_ const D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc,
     /* [annotation] */
     _Out_opt_ ID3D11UnorderedAccessView ** ppUAView);

    HRESULT(STDMETHODCALLTYPE * CreateRenderTargetView)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ ID3D11Resource * pResource,
     /* [annotation] */
     _In_opt_ const D3D11_RENDER_TARGET_VIEW_DESC * pDesc,
     /* [annotation] */
     _Out_opt_ ID3D11RenderTargetView ** ppRTView);

    HRESULT(STDMETHODCALLTYPE * CreateDepthStencilView)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ ID3D11Resource * pResource,
     /* [annotation] */
     _In_opt_ const D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc,
     /* [annotation] */
     _Out_opt_ ID3D11DepthStencilView ** ppDepthStencilView);

    HRESULT(STDMETHODCALLTYPE * CreateInputLayout)
    (ID3D11Device * This,
     /* [annotation] */
     _In_reads_(NumElements) const D3D11_INPUT_ELEMENT_DESC * pInputElementDescs,
     /* [annotation] */
     _In_range_(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT) UINT NumElements,
     /* [annotation] */
     _In_ const void * pShaderBytecodeWithInputSignature,
     /* [annotation] */
     _In_ SIZE_T BytecodeLength,
     /* [annotation] */
     _Out_opt_ ID3D11InputLayout ** ppInputLayout);

    HRESULT(STDMETHODCALLTYPE * CreateVertexShader)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const void * pShaderBytecode,
     /* [annotation] */
     _In_ SIZE_T BytecodeLength,
     /* [annotation] */
     _In_opt_ ID3D11ClassLinkage * pClassLinkage,
     /* [annotation] */
     _Out_opt_ ID3D11VertexShader ** ppVertexShader);

    HRESULT(STDMETHODCALLTYPE * CreateGeometryShader)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const void * pShaderBytecode,
     /* [annotation] */
     _In_ SIZE_T BytecodeLength,
     /* [annotation] */
     _In_opt_ ID3D11ClassLinkage * pClassLinkage,
     /* [annotation] */
     _Out_opt_ ID3D11GeometryShader ** ppGeometryShader);

    HRESULT(STDMETHODCALLTYPE * CreateGeometryShaderWithStreamOutput)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const void * pShaderBytecode,
     /* [annotation] */
     _In_ SIZE_T BytecodeLength,
     /* [annotation] */
     _In_reads_opt_(NumEntries) const D3D11_SO_DECLARATION_ENTRY * pSODeclaration,
     /* [annotation] */
     _In_range_(0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT) UINT NumEntries,
     /* [annotation] */
     _In_reads_opt_(NumStrides) const UINT * pBufferStrides,
     /* [annotation] */
     _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumStrides,
     /* [annotation] */
     _In_ UINT RasterizedStream,
     /* [annotation] */
     _In_opt_ ID3D11ClassLinkage * pClassLinkage,
     /* [annotation] */
     _Out_opt_ ID3D11GeometryShader ** ppGeometryShader);

    HRESULT(STDMETHODCALLTYPE * CreatePixelShader)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const void * pShaderBytecode,
     /* [annotation] */
     _In_ SIZE_T BytecodeLength,
     /* [annotation] */
     _In_opt_ ID3D11ClassLinkage * pClassLinkage,
     /* [annotation] */
     _Out_opt_ ID3D11PixelShader ** ppPixelShader);

    HRESULT(STDMETHODCALLTYPE * CreateHullShader)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const void * pShaderBytecode,
     /* [annotation] */
     _In_ SIZE_T BytecodeLength,
     /* [annotation] */
     _In_opt_ ID3D11ClassLinkage * pClassLinkage,
     /* [annotation] */
     _Out_opt_ ID3D11HullShader ** ppHullShader);

    HRESULT(STDMETHODCALLTYPE * CreateDomainShader)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const void * pShaderBytecode,
     /* [annotation] */
     _In_ SIZE_T BytecodeLength,
     /* [annotation] */
     _In_opt_ ID3D11ClassLinkage * pClassLinkage,
     /* [annotation] */
     _Out_opt_ ID3D11DomainShader ** ppDomainShader);

    HRESULT(STDMETHODCALLTYPE * CreateComputeShader)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const void * pShaderBytecode,
     /* [annotation] */
     _In_ SIZE_T BytecodeLength,
     /* [annotation] */
     _In_opt_ ID3D11ClassLinkage * pClassLinkage,
     /* [annotation] */
     _Out_opt_ ID3D11ComputeShader ** ppComputeShader);

    HRESULT(STDMETHODCALLTYPE * CreateClassLinkage)
    (ID3D11Device * This,
     /* [annotation] */
     _Out_ ID3D11ClassLinkage ** ppLinkage);

    HRESULT(STDMETHODCALLTYPE * CreateBlendState)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const D3D11_BLEND_DESC * pBlendStateDesc,
     /* [annotation] */
     _Out_opt_ ID3D11BlendState ** ppBlendState);

    HRESULT(STDMETHODCALLTYPE * CreateDepthStencilState)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const D3D11_DEPTH_STENCIL_DESC * pDepthStencilDesc,
     /* [annotation] */
     _Out_opt_ ID3D11DepthStencilState ** ppDepthStencilState);

    HRESULT(STDMETHODCALLTYPE * CreateRasterizerState)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const D3D11_RASTERIZER_DESC * pRasterizerDesc,
     /* [annotation] */
     _Out_opt_ ID3D11RasterizerState ** ppRasterizerState);

    HRESULT(STDMETHODCALLTYPE * CreateSamplerState)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const D3D11_SAMPLER_DESC * pSamplerDesc,
     /* [annotation] */
     _Out_opt_ ID3D11SamplerState ** ppSamplerState);

    HRESULT(STDMETHODCALLTYPE * CreateQuery)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const D3D11_QUERY_DESC * pQueryDesc,
     /* [annotation] */
     _Out_opt_ ID3D11Query ** ppQuery);

    HRESULT(STDMETHODCALLTYPE * CreatePredicate)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const D3D11_QUERY_DESC * pPredicateDesc,
     /* [annotation] */
     _Out_opt_ ID3D11Predicate ** ppPredicate);

    HRESULT(STDMETHODCALLTYPE * CreateCounter)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const D3D11_COUNTER_DESC * pCounterDesc,
     /* [annotation] */
     _Out_opt_ ID3D11Counter ** ppCounter);

    HRESULT(STDMETHODCALLTYPE * CreateDeferredContext)
    (ID3D11Device * This, UINT ContextFlags,
     /* [annotation] */
     _Out_opt_ ID3D11DeviceContext ** ppDeferredContext);

    HRESULT(STDMETHODCALLTYPE * OpenSharedResource)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ HANDLE hResource,
     /* [annotation] */
     _In_ REFIID ReturnedInterface,
     /* [annotation] */
     _Out_opt_ void ** ppResource);

    HRESULT(STDMETHODCALLTYPE * CheckFormatSupport)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ DXGI_FORMAT Format,
     /* [annotation] */
     _Out_ UINT * pFormatSupport);

    HRESULT(STDMETHODCALLTYPE * CheckMultisampleQualityLevels)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ DXGI_FORMAT Format,
     /* [annotation] */
     _In_ UINT SampleCount,
     /* [annotation] */
     _Out_ UINT * pNumQualityLevels);

    void(STDMETHODCALLTYPE * CheckCounterInfo)(ID3D11Device * This,
                                               /* [annotation] */
                                               _Out_ D3D11_COUNTER_INFO * pCounterInfo);

    HRESULT(STDMETHODCALLTYPE * CheckCounter)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ const D3D11_COUNTER_DESC * pDesc,
     /* [annotation] */
     _Out_ D3D11_COUNTER_TYPE * pType,
     /* [annotation] */
     _Out_ UINT * pActiveCounters,
     /* [annotation] */
     _Out_writes_opt_(*pNameLength) LPSTR szName,
     /* [annotation] */
     _Inout_opt_ UINT * pNameLength,
     /* [annotation] */
     _Out_writes_opt_(*pUnitsLength) LPSTR szUnits,
     /* [annotation] */
     _Inout_opt_ UINT * pUnitsLength,
     /* [annotation] */
     _Out_writes_opt_(*pDescriptionLength) LPSTR szDescription,
     /* [annotation] */
     _Inout_opt_ UINT * pDescriptionLength);

    HRESULT(STDMETHODCALLTYPE * CheckFeatureSupport)
    (ID3D11Device * This, D3D11_FEATURE Feature,
     /* [annotation] */
     _Out_writes_bytes_(FeatureSupportDataSize) void * pFeatureSupportData, UINT FeatureSupportDataSize);

    HRESULT(STDMETHODCALLTYPE * GetPrivateData)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _Inout_ UINT * pDataSize,
     /* [annotation] */
     _Out_writes_bytes_opt_(*pDataSize) void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateData)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_ UINT DataSize,
     /* [annotation] */
     _In_reads_bytes_opt_(DataSize) const void * pData);

    HRESULT(STDMETHODCALLTYPE * SetPrivateDataInterface)
    (ID3D11Device * This,
     /* [annotation] */
     _In_ REFGUID guid,
     /* [annotation] */
     _In_opt_ const IUnknown * pData);

    D3D_FEATURE_LEVEL(STDMETHODCALLTYPE * GetFeatureLevel)(ID3D11Device * This);

    UINT(STDMETHODCALLTYPE * GetCreationFlags)(ID3D11Device * This);

    HRESULT(STDMETHODCALLTYPE * GetDeviceRemovedReason)(ID3D11Device * This);

    void(STDMETHODCALLTYPE * GetImmediateContext)(ID3D11Device * This,
                                                  /* [annotation] */
                                                  _Out_ ID3D11DeviceContext ** ppImmediateContext);

    HRESULT(STDMETHODCALLTYPE * SetExceptionMode)(ID3D11Device * This, UINT RaiseFlags);

    UINT(STDMETHODCALLTYPE * GetExceptionMode)(ID3D11Device * This);

    END_INTERFACE
} ID3D11DeviceVtbl;

interface ID3D11Device {
    CONST_VTBL struct ID3D11DeviceVtbl * lpVtbl;
};

            #ifdef COBJMACROS

                #define ID3D11Device_QueryInterface(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface(This, riid, ppvObject))

                #define ID3D11Device_AddRef(This) ((This)->lpVtbl->AddRef(This))

                #define ID3D11Device_Release(This) ((This)->lpVtbl->Release(This))

                #define ID3D11Device_CreateBuffer(This, pDesc, pInitialData, ppBuffer) ((This)->lpVtbl->CreateBuffer(This, pDesc, pInitialData, ppBuffer))

                #define ID3D11Device_CreateTexture1D(This, pDesc, pInitialData, ppTexture1D) \
                    ((This)->lpVtbl->CreateTexture1D(This, pDesc, pInitialData, ppTexture1D))

                #define ID3D11Device_CreateTexture2D(This, pDesc, pInitialData, ppTexture2D) \
                    ((This)->lpVtbl->CreateTexture2D(This, pDesc, pInitialData, ppTexture2D))

                #define ID3D11Device_CreateTexture3D(This, pDesc, pInitialData, ppTexture3D) \
                    ((This)->lpVtbl->CreateTexture3D(This, pDesc, pInitialData, ppTexture3D))

                #define ID3D11Device_CreateShaderResourceView(This, pResource, pDesc, ppSRView) \
                    ((This)->lpVtbl->CreateShaderResourceView(This, pResource, pDesc, ppSRView))

                #define ID3D11Device_CreateUnorderedAccessView(This, pResource, pDesc, ppUAView) \
                    ((This)->lpVtbl->CreateUnorderedAccessView(This, pResource, pDesc, ppUAView))

                #define ID3D11Device_CreateRenderTargetView(This, pResource, pDesc, ppRTView) \
                    ((This)->lpVtbl->CreateRenderTargetView(This, pResource, pDesc, ppRTView))

                #define ID3D11Device_CreateDepthStencilView(This, pResource, pDesc, ppDepthStencilView) \
                    ((This)->lpVtbl->CreateDepthStencilView(This, pResource, pDesc, ppDepthStencilView))

                #define ID3D11Device_CreateInputLayout(This, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, \
                                                       ppInputLayout)                                                                            \
                    ((This)->lpVtbl->CreateInputLayout(This, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout))

                #define ID3D11Device_CreateVertexShader(This, pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader) \
                    ((This)->lpVtbl->CreateVertexShader(This, pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader))

                #define ID3D11Device_CreateGeometryShader(This, pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader) \
                    ((This)->lpVtbl->CreateGeometryShader(This, pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader))

                #define ID3D11Device_CreateGeometryShaderWithStreamOutput(This, pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, \
                                                                          NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader)                     \
                    ((This)->lpVtbl->CreateGeometryShaderWithStreamOutput(This, pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, \
                                                                          NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader))

                #define ID3D11Device_CreatePixelShader(This, pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader) \
                    ((This)->lpVtbl->CreatePixelShader(This, pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader))

                #define ID3D11Device_CreateHullShader(This, pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader) \
                    ((This)->lpVtbl->CreateHullShader(This, pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader))

                #define ID3D11Device_CreateDomainShader(This, pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader) \
                    ((This)->lpVtbl->CreateDomainShader(This, pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader))

                #define ID3D11Device_CreateComputeShader(This, pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader) \
                    ((This)->lpVtbl->CreateComputeShader(This, pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader))

                #define ID3D11Device_CreateClassLinkage(This, ppLinkage) ((This)->lpVtbl->CreateClassLinkage(This, ppLinkage))

                #define ID3D11Device_CreateBlendState(This, pBlendStateDesc, ppBlendState) \
                    ((This)->lpVtbl->CreateBlendState(This, pBlendStateDesc, ppBlendState))

                #define ID3D11Device_CreateDepthStencilState(This, pDepthStencilDesc, ppDepthStencilState) \
                    ((This)->lpVtbl->CreateDepthStencilState(This, pDepthStencilDesc, ppDepthStencilState))

                #define ID3D11Device_CreateRasterizerState(This, pRasterizerDesc, ppRasterizerState) \
                    ((This)->lpVtbl->CreateRasterizerState(This, pRasterizerDesc, ppRasterizerState))

                #define ID3D11Device_CreateSamplerState(This, pSamplerDesc, ppSamplerState) \
                    ((This)->lpVtbl->CreateSamplerState(This, pSamplerDesc, ppSamplerState))

                #define ID3D11Device_CreateQuery(This, pQueryDesc, ppQuery) ((This)->lpVtbl->CreateQuery(This, pQueryDesc, ppQuery))

                #define ID3D11Device_CreatePredicate(This, pPredicateDesc, ppPredicate) ((This)->lpVtbl->CreatePredicate(This, pPredicateDesc, ppPredicate))

                #define ID3D11Device_CreateCounter(This, pCounterDesc, ppCounter) ((This)->lpVtbl->CreateCounter(This, pCounterDesc, ppCounter))

                #define ID3D11Device_CreateDeferredContext(This, ContextFlags, ppDeferredContext) \
                    ((This)->lpVtbl->CreateDeferredContext(This, ContextFlags, ppDeferredContext))

                #define ID3D11Device_OpenSharedResource(This, hResource, ReturnedInterface, ppResource) \
                    ((This)->lpVtbl->OpenSharedResource(This, hResource, ReturnedInterface, ppResource))

                #define ID3D11Device_CheckFormatSupport(This, Format, pFormatSupport) ((This)->lpVtbl->CheckFormatSupport(This, Format, pFormatSupport))

                #define ID3D11Device_CheckMultisampleQualityLevels(This, Format, SampleCount, pNumQualityLevels) \
                    ((This)->lpVtbl->CheckMultisampleQualityLevels(This, Format, SampleCount, pNumQualityLevels))

                #define ID3D11Device_CheckCounterInfo(This, pCounterInfo) ((This)->lpVtbl->CheckCounterInfo(This, pCounterInfo))

                #define ID3D11Device_CheckCounter(This, pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, \
                                                  pDescriptionLength)                                                                             \
                    ((This)->lpVtbl->CheckCounter(This, pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, \
                                                  pDescriptionLength))

                #define ID3D11Device_CheckFeatureSupport(This, Feature, pFeatureSupportData, FeatureSupportDataSize) \
                    ((This)->lpVtbl->CheckFeatureSupport(This, Feature, pFeatureSupportData, FeatureSupportDataSize))

                #define ID3D11Device_GetPrivateData(This, guid, pDataSize, pData) ((This)->lpVtbl->GetPrivateData(This, guid, pDataSize, pData))

                #define ID3D11Device_SetPrivateData(This, guid, DataSize, pData) ((This)->lpVtbl->SetPrivateData(This, guid, DataSize, pData))

                #define ID3D11Device_SetPrivateDataInterface(This, guid, pData) ((This)->lpVtbl->SetPrivateDataInterface(This, guid, pData))

                #define ID3D11Device_GetFeatureLevel(This) ((This)->lpVtbl->GetFeatureLevel(This))

                #define ID3D11Device_GetCreationFlags(This) ((This)->lpVtbl->GetCreationFlags(This))

                #define ID3D11Device_GetDeviceRemovedReason(This) ((This)->lpVtbl->GetDeviceRemovedReason(This))

                #define ID3D11Device_GetImmediateContext(This, ppImmediateContext) ((This)->lpVtbl->GetImmediateContext(This, ppImmediateContext))

                #define ID3D11Device_SetExceptionMode(This, RaiseFlags) ((This)->lpVtbl->SetExceptionMode(This, RaiseFlags))

                #define ID3D11Device_GetExceptionMode(This) ((This)->lpVtbl->GetExceptionMode(This))

            #endif /* COBJMACROS */

        #endif /* C style interface */

    #endif /* __ID3D11Device_INTERFACE_DEFINED__ */

/* interface __MIDL_itf_d3d11_0000_0041 */
/* [local] */

typedef enum D3D11_CREATE_DEVICE_FLAG {
    D3D11_CREATE_DEVICE_SINGLETHREADED                                = 0x1,
    D3D11_CREATE_DEVICE_DEBUG                                         = 0x2,
    D3D11_CREATE_DEVICE_SWITCH_TO_REF                                 = 0x4,
    D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS      = 0x8,
    D3D11_CREATE_DEVICE_BGRA_SUPPORT                                  = 0x20,
    D3D11_CREATE_DEVICE_DEBUGGABLE                                    = 0x40,
    D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY = 0x80,
    D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT                           = 0x100,
    D3D11_CREATE_DEVICE_VIDEO_SUPPORT                                 = 0x800
} D3D11_CREATE_DEVICE_FLAG;

    #define D3D11_SDK_VERSION (7)

    #if !defined(D3D11_IGNORE_SDK_LAYERS)
        #include "d3d11sdklayers.h"
    #endif
    #include "d3d10_1.h"
    #pragma region Desktop Family
    #if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
        #include "d3d10misc.h"
        #include "d3d10shader.h"
        #include "d3d10effect.h"
        #include "d3d10_1shader.h"
    #endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) */
    #pragma endregion

///////////////////////////////////////////////////////////////////////////
// D3D11CreateDevice
// ------------------
//
// pAdapter
//      If NULL, D3D11CreateDevice will choose the primary adapter and
//      create a new instance from a temporarily created IDXGIFactory.
//      If non-NULL, D3D11CreateDevice will register the appropriate
//      device, if necessary (via IDXGIAdapter::RegisterDrver), before
//      creating the device.
// DriverType
//      Specifies the driver type to be created: hardware, reference or
//      null.
// Software
//      HMODULE of a DLL implementing a software rasterizer. Must be NULL for
//      non-Software driver types.
// Flags
//      Any of those documented for D3D11CreateDeviceAndSwapChain.
// pFeatureLevels
//      Any of those documented for D3D11CreateDeviceAndSwapChain.
// FeatureLevels
//      Size of feature levels array.
// SDKVersion
//      SDK version. Use the D3D11_SDK_VERSION macro.
// ppDevice
//      Pointer to returned interface. May be NULL.
// pFeatureLevel
//      Pointer to returned feature level. May be NULL.
// ppImmediateContext
//      Pointer to returned interface. May be NULL.
//
// Return Values
//  Any of those documented for
//          CreateDXGIFactory1
//          IDXGIFactory::EnumAdapters
//          IDXGIAdapter::RegisterDriver
//          D3D11CreateDevice
//
///////////////////////////////////////////////////////////////////////////
typedef HRESULT(WINAPI * PFN_D3D11_CREATE_DEVICE)(_In_opt_                            IDXGIAdapter *, D3D_DRIVER_TYPE, HMODULE, UINT,
                                                  _In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL *, UINT FeatureLevels, UINT, _Out_opt_ ID3D11Device **,
                                                  _Out_opt_ D3D_FEATURE_LEVEL *, _Out_opt_ ID3D11DeviceContext **);

HRESULT WINAPI D3D11CreateDevice(_In_opt_ IDXGIAdapter * pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags,
                                 _In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL * pFeatureLevels, UINT FeatureLevels, UINT SDKVersion,
                                 _Out_opt_ ID3D11Device ** ppDevice, _Out_opt_ D3D_FEATURE_LEVEL * pFeatureLevel,
                                 _Out_opt_ ID3D11DeviceContext ** ppImmediateContext);

    #pragma region Desktop Family
    #if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
///////////////////////////////////////////////////////////////////////////
// D3D11CreateDeviceAndSwapChain
// ------------------------------
//
// ppAdapter
//      If NULL, D3D11CreateDevice will choose the primary adapter and
//      create a new instance from a temporarily created IDXGIFactory.
//      If non-NULL, D3D11CreateDevice will register the appropriate
//      device, if necessary (via IDXGIAdapter::RegisterDrver), before
//      creating the device.
// DriverType
//      Specifies the driver type to be created: hardware, reference or
//      null.
// Software
//      HMODULE of a DLL implementing a software rasterizer. Must be NULL for
//      non-Software driver types.
// Flags
//      Any of those documented for D3D11CreateDevice.
// pFeatureLevels
//      Array of any of the following:
//          D3D_FEATURE_LEVEL_11_0
//          D3D_FEATURE_LEVEL_10_1
//          D3D_FEATURE_LEVEL_10_0
//          D3D_FEATURE_LEVEL_9_3
//          D3D_FEATURE_LEVEL_9_2
//          D3D_FEATURE_LEVEL_9_1
//      Order indicates sequence in which instantiation will be attempted. If
//      NULL, then the implied order is the same as previously listed (i.e.
//      prefer most features available).
// FeatureLevels
//      Size of feature levels array.
// SDKVersion
//      SDK version. Use the D3D11_SDK_VERSION macro.
// pSwapChainDesc
//      Swap chain description, may be NULL.
// ppSwapChain
//      Pointer to returned interface. May be NULL.
// ppDevice
//      Pointer to returned interface. May be NULL.
// pFeatureLevel
//      Pointer to returned feature level. May be NULL.
// ppImmediateContext
//      Pointer to returned interface. May be NULL.
//
// Return Values
//  Any of those documented for
//          CreateDXGIFactory1
//          IDXGIFactory::EnumAdapters
//          IDXGIAdapter::RegisterDriver
//          D3D11CreateDevice
//          IDXGIFactory::CreateSwapChain
//
///////////////////////////////////////////////////////////////////////////
typedef HRESULT(WINAPI * PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN)(_In_opt_                            IDXGIAdapter *, D3D_DRIVER_TYPE, HMODULE, UINT,
                                                                 _In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL *, UINT FeatureLevels, UINT,
                                                                 _In_opt_ CONST DXGI_SWAP_CHAIN_DESC *, _Out_opt_ IDXGISwapChain **, _Out_opt_ ID3D11Device **,
                                                                 _Out_opt_ D3D_FEATURE_LEVEL *, _Out_opt_ ID3D11DeviceContext **);

HRESULT WINAPI D3D11CreateDeviceAndSwapChain(_In_opt_ IDXGIAdapter * pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags,
                                             _In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL * pFeatureLevels, UINT FeatureLevels, UINT SDKVersion,
                                             _In_opt_ CONST DXGI_SWAP_CHAIN_DESC * pSwapChainDesc, _Out_opt_ IDXGISwapChain ** ppSwapChain,
                                             _Out_opt_ ID3D11Device ** ppDevice, _Out_opt_ D3D_FEATURE_LEVEL * pFeatureLevel,
                                             _Out_opt_ ID3D11DeviceContext ** ppImmediateContext);

    #endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) */
    #pragma endregion
DEFINE_GUID(IID_ID3D11DeviceChild, 0x1841e5c8, 0x16b0, 0x489b, 0xbc, 0xc8, 0x44, 0xcf, 0xb0, 0xd5, 0xde, 0xae);
DEFINE_GUID(IID_ID3D11DepthStencilState, 0x03823efb, 0x8d8f, 0x4e1c, 0x9a, 0xa2, 0xf6, 0x4b, 0xb2, 0xcb, 0xfd, 0xf1);
DEFINE_GUID(IID_ID3D11BlendState, 0x75b68faa, 0x347d, 0x4159, 0x8f, 0x45, 0xa0, 0x64, 0x0f, 0x01, 0xcd, 0x9a);
DEFINE_GUID(IID_ID3D11RasterizerState, 0x9bb4ab81, 0xab1a, 0x4d8f, 0xb5, 0x06, 0xfc, 0x04, 0x20, 0x0b, 0x6e, 0xe7);
DEFINE_GUID(IID_ID3D11Resource, 0xdc8e63f3, 0xd12b, 0x4952, 0xb4, 0x7b, 0x5e, 0x45, 0x02, 0x6a, 0x86, 0x2d);
DEFINE_GUID(IID_ID3D11Buffer, 0x48570b85, 0xd1ee, 0x4fcd, 0xa2, 0x50, 0xeb, 0x35, 0x07, 0x22, 0xb0, 0x37);
DEFINE_GUID(IID_ID3D11Texture1D, 0xf8fb5c27, 0xc6b3, 0x4f75, 0xa4, 0xc8, 0x43, 0x9a, 0xf2, 0xef, 0x56, 0x4c);
DEFINE_GUID(IID_ID3D11Texture2D, 0x6f15aaf2, 0xd208, 0x4e89, 0x9a, 0xb4, 0x48, 0x95, 0x35, 0xd3, 0x4f, 0x9c);
DEFINE_GUID(IID_ID3D11Texture3D, 0x037e866e, 0xf56d, 0x4357, 0xa8, 0xaf, 0x9d, 0xab, 0xbe, 0x6e, 0x25, 0x0e);
DEFINE_GUID(IID_ID3D11View, 0x839d1216, 0xbb2e, 0x412b, 0xb7, 0xf4, 0xa9, 0xdb, 0xeb, 0xe0, 0x8e, 0xd1);
DEFINE_GUID(IID_ID3D11ShaderResourceView, 0xb0e06fe0, 0x8192, 0x4e1a, 0xb1, 0xca, 0x36, 0xd7, 0x41, 0x47, 0x10, 0xb2);
DEFINE_GUID(IID_ID3D11RenderTargetView, 0xdfdba067, 0x0b8d, 0x4865, 0x87, 0x5b, 0xd7, 0xb4, 0x51, 0x6c, 0xc1, 0x64);
DEFINE_GUID(IID_ID3D11DepthStencilView, 0x9fdac92a, 0x1876, 0x48c3, 0xaf, 0xad, 0x25, 0xb9, 0x4f, 0x84, 0xa9, 0xb6);
DEFINE_GUID(IID_ID3D11UnorderedAccessView, 0x28acf509, 0x7f5c, 0x48f6, 0x86, 0x11, 0xf3, 0x16, 0x01, 0x0a, 0x63, 0x80);
DEFINE_GUID(IID_ID3D11VertexShader, 0x3b301d64, 0xd678, 0x4289, 0x88, 0x97, 0x22, 0xf8, 0x92, 0x8b, 0x72, 0xf3);
DEFINE_GUID(IID_ID3D11HullShader, 0x8e5c6061, 0x628a, 0x4c8e, 0x82, 0x64, 0xbb, 0xe4, 0x5c, 0xb3, 0xd5, 0xdd);
DEFINE_GUID(IID_ID3D11DomainShader, 0xf582c508, 0x0f36, 0x490c, 0x99, 0x77, 0x31, 0xee, 0xce, 0x26, 0x8c, 0xfa);
DEFINE_GUID(IID_ID3D11GeometryShader, 0x38325b96, 0xeffb, 0x4022, 0xba, 0x02, 0x2e, 0x79, 0x5b, 0x70, 0x27, 0x5c);
DEFINE_GUID(IID_ID3D11PixelShader, 0xea82e40d, 0x51dc, 0x4f33, 0x93, 0xd4, 0xdb, 0x7c, 0x91, 0x25, 0xae, 0x8c);
DEFINE_GUID(IID_ID3D11ComputeShader, 0x4f5b196e, 0xc2bd, 0x495e, 0xbd, 0x01, 0x1f, 0xde, 0xd3, 0x8e, 0x49, 0x69);
DEFINE_GUID(IID_ID3D11InputLayout, 0xe4819ddc, 0x4cf0, 0x4025, 0xbd, 0x26, 0x5d, 0xe8, 0x2a, 0x3e, 0x07, 0xb7);
DEFINE_GUID(IID_ID3D11SamplerState, 0xda6fea51, 0x564c, 0x4487, 0x98, 0x10, 0xf0, 0xd0, 0xf9, 0xb4, 0xe3, 0xa5);
DEFINE_GUID(IID_ID3D11Asynchronous, 0x4b35d0cd, 0x1e15, 0x4258, 0x9c, 0x98, 0x1b, 0x13, 0x33, 0xf6, 0xdd, 0x3b);
DEFINE_GUID(IID_ID3D11Query, 0xd6c00747, 0x87b7, 0x425e, 0xb8, 0x4d, 0x44, 0xd1, 0x08, 0x56, 0x0a, 0xfd);
DEFINE_GUID(IID_ID3D11Predicate, 0x9eb576dd, 0x9f77, 0x4d86, 0x81, 0xaa, 0x8b, 0xab, 0x5f, 0xe4, 0x90, 0xe2);
DEFINE_GUID(IID_ID3D11Counter, 0x6e8c49fb, 0xa371, 0x4770, 0xb4, 0x40, 0x29, 0x08, 0x60, 0x22, 0xb7, 0x41);
DEFINE_GUID(IID_ID3D11ClassInstance, 0xa6cd7faa, 0xb0b7, 0x4a2f, 0x94, 0x36, 0x86, 0x62, 0xa6, 0x57, 0x97, 0xcb);
DEFINE_GUID(IID_ID3D11ClassLinkage, 0xddf57cba, 0x9543, 0x46e4, 0xa1, 0x2b, 0xf2, 0x07, 0xa0, 0xfe, 0x7f, 0xed);
DEFINE_GUID(IID_ID3D11CommandList, 0xa24bc4d1, 0x769e, 0x43f7, 0x80, 0x13, 0x98, 0xff, 0x56, 0x6c, 0x18, 0xe2);
DEFINE_GUID(IID_ID3D11DeviceContext, 0xc0bfa96c, 0xe089, 0x44fb, 0x8e, 0xaf, 0x26, 0xf8, 0x79, 0x61, 0x90, 0xda);
DEFINE_GUID(IID_ID3D11VideoDecoder, 0x3C9C5B51, 0x995D, 0x48d1, 0x9B, 0x8D, 0xFA, 0x5C, 0xAE, 0xDE, 0xD6, 0x5C);
DEFINE_GUID(IID_ID3D11VideoProcessorEnumerator, 0x31627037, 0x53AB, 0x4200, 0x90, 0x61, 0x05, 0xFA, 0xA9, 0xAB, 0x45, 0xF9);
DEFINE_GUID(IID_ID3D11VideoProcessor, 0x1D7B0652, 0x185F, 0x41c6, 0x85, 0xCE, 0x0C, 0x5B, 0xE3, 0xD4, 0xAE, 0x6C);
DEFINE_GUID(IID_ID3D11AuthenticatedChannel, 0x3015A308, 0xDCBD, 0x47aa, 0xA7, 0x47, 0x19, 0x24, 0x86, 0xD1, 0x4D, 0x4A);
DEFINE_GUID(IID_ID3D11CryptoSession, 0x9B32F9AD, 0xBDCC, 0x40a6, 0xA3, 0x9D, 0xD5, 0xC8, 0x65, 0x84, 0x57, 0x20);
DEFINE_GUID(IID_ID3D11VideoDecoderOutputView, 0xC2931AEA, 0x2A85, 0x4f20, 0x86, 0x0F, 0xFB, 0xA1, 0xFD, 0x25, 0x6E, 0x18);
DEFINE_GUID(IID_ID3D11VideoProcessorInputView, 0x11EC5A5F, 0x51DC, 0x4945, 0xAB, 0x34, 0x6E, 0x8C, 0x21, 0x30, 0x0E, 0xA5);
DEFINE_GUID(IID_ID3D11VideoProcessorOutputView, 0xA048285E, 0x25A9, 0x4527, 0xBD, 0x93, 0xD6, 0x8B, 0x68, 0xC4, 0x42, 0x54);
DEFINE_GUID(IID_ID3D11VideoContext, 0x61F21C45, 0x3C0E, 0x4a74, 0x9C, 0xEA, 0x67, 0x10, 0x0D, 0x9A, 0xD5, 0xE4);
DEFINE_GUID(IID_ID3D11VideoDevice, 0x10EC4D5B, 0x975A, 0x4689, 0xB9, 0xE4, 0xD0, 0xAA, 0xC3, 0x0F, 0xE3, 0x33);
DEFINE_GUID(IID_ID3D11Device, 0xdb6f6ddb, 0xac77, 0x4e88, 0x82, 0x53, 0x81, 0x9d, 0xf9, 0xbb, 0xf1, 0x40);

extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0041_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_d3d11_0000_0041_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

    #ifdef __cplusplus
}
    #endif

#endif
