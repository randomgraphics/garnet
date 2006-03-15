#ifndef __GN_GFX_OGLTYPES_H__
#define __GN_GFX_OGLTYPES_H__
// *****************************************************************************
//! \file    oglTypes.h
//! \brief
//! \author  chenlee (2005.10.2)
// *****************************************************************************

namespace GN { namespace gfx
{
    class OGLRenderer; // forware declaration of OGL renderer.

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

        //!
        //! Get renderer that the resource belongs to
        //!
        OGLRenderer & getRenderer() const { return mRenderer; }

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
                int vtxBuf    : 16; //!< Vertex buffer dirty flags. bit 0-15 for vertex buffers, bit 16 for vertex binding.
                int vtxFmt    : 1;  //!< Vertex buffer dirty flags. bit 0-15 for vertex buffers, bit 16 for vertex binding.
                int vtxShader : 1;  //!< Vertex shader dirty flag
                int pxlShader : 1;  //!< Pixel shader dirty flag
                int reserved  : 13; //!< Reserved for future use.
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
        VtxFmtHandle          vtxFmt;                     //!< current vertex format handle
        AutoRef<const Shader> vtxShader;                   //!< current vertex shader
        AutoRef<const Shader> pxlShader;                   //!< current pixel shader
        DirtyFlags            dirtyFlags;                  //!< dirty flags

        //!
        //! clear the state structure
        //!
        void clear()
        {
            for( size_t i = 0; i < MAX_VERTEX_STREAMS; ++i )
            {
                vtxBufs[i].buf.clear();
                vtxBufs[i].stride = 0;
            }
            vtxFmt = 0;
            vtxShader.clear();
            pxlShader.clear();
            dirtyFlags.u32 = 0;
        }

        //!
        //! bind vertex binding
        //!
        void bindVtxFmt( VtxFmtHandle handle )
        {
            dirtyFlags.vtxFmt = true;
            vtxFmt = handle;
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
        //! bind single shader
        //!
        void bindShader( ShaderType type, const Shader * shader )
        {
            if( VERTEX_SHADER == type )
            {
                if( shader != vtxShader )
                {
                    dirtyFlags.vtxShader = true;
                    vtxShader.reset( shader );
                }
            }
            else if( PIXEL_SHADER == type )
            {
                if( shader != pxlShader )
                {
                    dirtyFlags.pxlShader = true;
                    pxlShader.reset( shader );
                }
            }
            else
            {
                GN_ERROR( "invalid shader type: %d", type );
            }
        }

        //!
        //! bind shader list
        //!
        void bindShaders( const Shader * const shaders[] )
        {
            if( 0 == shaders )
            {
                GN_ERROR( "shader list can't be NULL." );
                return;
            }
            if( shaders[VERTEX_SHADER] != vtxShader )
            {
                dirtyFlags.vtxShader = true;
                vtxShader.reset( shaders[VERTEX_SHADER] );
            }
            if( shaders[PIXEL_SHADER] != pxlShader )
            {
                dirtyFlags.pxlShader = true;
                pxlShader.reset( shaders[PIXEL_SHADER] );
            }
        }
    };
}}

// *****************************************************************************
//                           End of oglTypes.h
// *****************************************************************************
#endif // __GN_GFX_OGLTYPES_H__
