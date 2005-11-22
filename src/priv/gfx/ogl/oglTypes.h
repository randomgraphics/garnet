#ifndef __GN_GFX_OGLTYPES_H__
#define __GN_GFX_OGLTYPES_H__
// *****************************************************************************
//! \file    oglTypes.h
//! \brief
//! \author  chenlee (2005.10.2)
// *****************************************************************************

//! \def GNGFX_OGLCAPS
//! Define OGL special caps.

namespace GN { namespace gfx
{
    class OGLRenderer; // forware declaration of OGL renderer.

    //!
    //! define API dependent caps
    //!
    enum OGLCaps
    {
        #define GNGFX_OGLCAPS(X) OGLCAPS_##X,
        #include "oglCapsMeta.h"
        #undef GNGFX_OGLCAPS
        NUM_OGLCAPS,
        OGLCAPS_INVALID,
    };


    //!
    //! General OGL resource class
    //!
    struct OGLResource
    {
        virtual bool deviceCreate() = 0;  //!< Respond to OGL device creation.
        virtual bool deviceRestore() = 0; //!< Respond to OGL device restoration.
        virtual void deviceDispose() = 0; //!< Respond to OGL device dispose(lost).
        virtual void deviceDestroy() = 0; //!< Respond to OGL device deletion.

    protected :
        //@{
        OGLResource( OGLRenderer & );
        virtual ~OGLResource();
        //@}

    private:
        OGLRenderer & mRenderer;
    };

    //!
    //! OGL draw state structure
    //!
    struct OGLDrawState
    {
        //!
        //! Renderer state dirty flags
        //!
        union DirtyFlags
        {
            uint32_t u32; //!< Dirty flags as unsigned integer
            int32_t  i32; //!< Dirty flags as signed integer

            struct
            {
                int  vtxBuf     : 17; //!< Vertex buffer dirty flags. bit 0-15 for vertex buffers, bit 16 for vertex binding.
                bool shader     : 1;  //!< vertex shader dirty flag
                int  reserved   : 14; //!< Reserved for future use.
            };
        };

        //!
        //! Vertex buffer state
        //!
        struct VtxBufDesc
        {
            AutoRef<const VtxBuf> buf;    //!< the buffer pointer
            size_t                stride; //!< vertex stride
        };

        VtxBufDesc            vtxBufs[MAX_VERTEX_STREAMS]; //!< current vertex buffers
        uint32_t              vtxBinding;                  //!< current vertex binding handle
        AutoRef<const Shader> vtxShader;                   //!< current vertex shader
        AutoRef<const Shader> pxlShader;                   //!< current pixel shader
        DirtyFlags            dirtyFlags;                  //!< dirty flags

        //!
        //! bind vertex binding
        //!
        void bindVtxBinding( uint32_t handle )
        {
            dirtyFlags.vtxBuf |= 1 << 16;
            vtxBinding = handle;
        }

        //!
        //! bind vertex buffer
        //!
        void bindVtxBuf( size_t index, const VtxBuf * buf, size_t stride )
        {
            GN_ASSERT( index < MAX_VERTEX_STREAMS );
            dirtyFlags.vtxBuf |= 1 << index;
            vtxBufs[index].buf.reset( buf );
            vtxBufs[index].stride = stride;
        }

        //!
        //! bind vertex shader
        //!
        void bindVtxShader( const Shader * shader )
        {
            dirtyFlags.shader = true;
            vtxShader.reset( shader );
        }

        //!
        //! bind pixel shader
        //!
        void bindPxlShader( const Shader * shader )
        {
            dirtyFlags.shader = true;
            pxlShader.reset( shader );
        }
    };
}}

// *****************************************************************************
//                           End of oglTypes.h
// *****************************************************************************
#endif // __GN_GFX_OGLTYPES_H__
