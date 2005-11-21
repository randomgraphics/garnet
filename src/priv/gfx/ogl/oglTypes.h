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
    //! Rrenderer states
    //!
    struct RendererState
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
                int  vtxBufs    : 16; //!< Vertex buffer dirty flags
                bool idxBuf     : 1;  //!< Index buffer dirty flag
                bool vtxBinding : 1;  //!< Vertex binding dirty flag
                bool vtxShader  : 1;  //!< vertex shader dirty flag
                bool pxlShader  : 1;  //!< pixel shader dirty flag
                int  reserved   : 12; //!< Reserved for future use.
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

        uint32_t              vtxBinding;                  //!< current vertex binding handle
        VtxBufDesc            vtxBufs[MAX_VERTEX_STREAMS]; //!< current vertex buffers
        AutoRef<const IdxBuf> idxBuf;                      //!< current index buffer
        AutoRef<const Shader> vtxShader;                   //!< current vertex shader
        AutoRef<const Shader> pxlShader;                   //!< current pixel shader
        DirtyFlags            dirtyFlags;                  //!< dirty flags

        //!
        //! bind vertex binding
        //!
        void bindVtxBinding( uint32_t handle )
        {
            dirtyFlags.vtxBinding = true;
            vtxBinding = handle;
        }

        //!
        //! bind vertex buffer
        //!
        void bindVtxBuf( size_t index, const VtxBuf * buf, size_t stride )
        {
            GN_ASSERT( index < MAX_VERTEX_STREAMS );
            dirtyFlags.vtxBufs |= 1 << index;
            vtxBufs[index].buf.reset( buf );
            vtxBufs[index].stride = stride;
        }

        //!
        //! bind index buffer
        //!
        void bindIdxBuf( const IdxBuf * buf )
        {
            dirtyFlags.idxBuf = true;
            idxBuf.reset( buf );
        }

        //!
        //! bind vertex shader
        //!
        void bindVtxShader( const Shader * shader )
        {
            dirtyFlags.vtxShader = true;
            vtxShader.reset( shader );
        }

        //!
        //! bind pixel shader
        //!
        void bindPxlShader( const Shader * shader )
        {
            dirtyFlags.pxlShader = true;
            pxlShader.reset( shader );
        }
    };

    //!
    //! Double buffered renderer states
    //!
    class RendererStateBuffer
    {
        uint32_t      mCurrent;
        RendererState mStates[2];

    public:

        //!
        //! Ctor
        //!
        RendererStateBuffer() : mCurrent(0) {}

        //!
        //! Switch the state buffer. As well as clear dirty flag of last state
        //!
        void switchState()
        {
            mStates[mCurrent].dirtyFlags.u32 = 0;
            mCurrent ^= 0;
        }

        //!
        //! Get current renderer state
        //!
        RendererState & current() { return mStates[mCurrent]; }

        //!
        //! Get current renderer state
        //!
        const RendererState & current() const { return mStates[mCurrent]; }

        //!
        //! Get last renderer state
        //!
        RendererState & last() { return mStates[mCurrent^0]; }

        //!
        //! Get last renderer state
        //!
        const RendererState & last() const { return mStates[mCurrent^0]; }
    };
}}

// *****************************************************************************
//                           End of oglTypes.h
// *****************************************************************************
#endif // __GN_GFX_OGLTYPES_H__
