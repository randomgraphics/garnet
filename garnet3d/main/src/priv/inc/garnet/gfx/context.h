#ifndef _GN_GFX_CONTEXT_H__
#define _GN_GFX_CONTEXT_H__
// *****************************************************************************
//! \file    context.h
//! \brief   Rendering context
//! \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN { namespace gfx
{
    typedef uint32_t VtxFmtHandle; //!< Vertex format handle

    enum
    {
        MAX_RENDER_TARGETS = 4 //!< We support 4 render targets at most.
    };

    //!
    //! Msaa type
    //!
    enum MsaaType
    {
        MSAA_NONE,   //!< No MSAA
        MSAA_LOW,    //!< low quality MSAA
        MSAA_MEDIUM, //!< medium quality MSAA
        MSAA_HIGH,   //!< high quality MSAA
        MSAA_ULTRA,  //!< ultra quality MSAA
    };

    //!
    //! Renderer context state. Completely define how rendering would be done
    //!
    struct RendererContext
    {
        //!
        //! Context flag structure. If flag is zero, means that field is undefined,
        //! and should not being used to update device state.
        //!
        union FieldFlags
        {
            unsigned int u32; //!< all flags as uint32

            struct
            {
                unsigned char state;    //!< frequently used states
                unsigned char ffp;      //!< fixed function pipeline states
                unsigned char data;     //!< rendering data flags
                unsigned char reserved; //!< reserved for future use.
            };

            struct
            {
                // byte 0
                unsigned int shaders            : 2; //!< one bit for each shader type
                unsigned int rsb                : 1; //!< render state block
                unsigned int colorBuffers       : 1; //!< render target textures
                unsigned int depthBuffer        : 1; //!< depth texture
                unsigned int msaa               : 1; //!< MSAA for RTT
                unsigned int viewport           : 1; //!< viewport
                unsigned int                    : 1; //!< reserved
                // byte 1 (fixed function pipeline)
                unsigned int world              : 1; //!< world transformation
                unsigned int view               : 1; //!< view transformation
                unsigned int proj               : 1; //!< projection transformation
                unsigned int light0Pos          : 1; //!< light 0 position
                unsigned int light0Diffuse      : 1; //!< light 0 diffuse
                unsigned int materialDiffuse    : 1; //!< material diffues color
                unsigned int materialSpecular   : 1; //!< material specular color
                unsigned int tsb                : 1; //!< texture state block
                // byte 2 (texture and mesh)
                unsigned int textures           : 1; //!< textures
                unsigned int vtxFmt             : 1; //!< vertex format
                unsigned int vtxBufs            : 1; //!< vertex buffers
                unsigned int idxBuf             : 1; //!< index buffer
                unsigned int                    : 4; //!< reserved
                // byte 3 (reserved)
                unsigned int                    : 8; //!< reserved
            };

            //! \name helper functions to access shader bits
            //@{
            bool shaderBit( int type ) const { GN_ASSERT( 0 <= type && type < NUM_SHADER_TYPES ); return 0 != ( shaders & (1<<type) ); }
            bool vtxShaderBit() const { return 0 != ( shaders & (1<<VERTEX_SHADER) ); }
            bool pxlShaderBit() const { return 0 != ( shaders & (1<<PIXEL_SHADER) ); }
            void setShaderBit( int type ) { GN_ASSERT( 0 <= type && type < NUM_SHADER_TYPES ); shaders |= 1 << type; }
            //@}
        };

        //!
        //! Render target surface descriptor
        //!
        struct SurfaceDesc
        {
            const Texture * texture; //!< render target
            size_t          face;    //!< cubemap face. Must be zero for non-cube/stack texture.
            size_t          level;   //!< mipmap level
            size_t          slice;   //!< slice index. Must be zero for 3D texture.

            //!
            //! equality check
            //!
            bool operator!=( const SurfaceDesc & rhs ) const
            {
                if( texture != rhs.texture ) return true;
                if( NULL == texture ) return false; // ignore remaining parameters, if texture is NULL.
                return face != rhs.face || level != level || slice != slice;
            }
        };

        //!
        //! Vertex buffer binding descriptor
        //!
        struct VtxBufDesc
        {
            const VtxBuf * buffer; //!< buffer pointer
            size_t         stride; //!< buffer stride
        };

        FieldFlags            flags; //!< field flags
        const Shader *        shaders[NUM_SHADER_TYPES]; //!< shaders
        RenderStateBlockDesc  rsb; //!< render state block.
        SurfaceDesc           colorBuffers[MAX_RENDER_TARGETS]; //!< color buffers
        size_t                numColorBuffers;  //!< number of color buffers. Set to 0 to render to back buffer.
        SurfaceDesc           depthBuffer; //!< depth surface.
        MsaaType              msaa; //!< MSAA type for RTT. Not effective, if rendering to back buffer
        Rectf                 viewport; //!< viewport

        Matrix44f             world, //!< world transformation
                              view, //!< view transformation
                              proj; //!< projection transformation
        Vector4f              light0Pos, //!< light0 position
                              light0Diffuse, //!< light0 diffuse color
                              materialDiffuse, //!< diffuse material color
                              materialSpecular; //!< specular material color
        TextureStateBlockDesc tsb; //!< texture state block

        const Texture *       textures[MAX_TEXTURE_STAGES]; //!< texture list
        size_t                numTextures; //!< texture count
        VtxFmtHandle          vtxFmt; //!< vertex format handle. 0 means no vertex data at all.
        VtxBufDesc            vtxBufs[MAX_VERTEX_STREAMS]; //!< vertex buffers.
        size_t                numVtxBufs; //!< vertex buffer count.
        const IdxBuf *        idxBuf; //!< index buffer

        //!
        //! Clear to null context, all fields are unused/undefined.
        //!
        void clearToNull()
        {
#if GN_DEBUG
            // fill with invalid data
            ::memset( this, sizeof(*this), 0xcd );
#endif
            GN_CASSERT( 4 == sizeof(FieldFlags) );
            flags.u32 = 0;
            rsb.resetToEmpty();
            numColorBuffers = 0;
            numTextures = 0;
            numVtxBufs = 0;
        }

        //!
        //! Reset to default context.
        //!
        void resetToDefault()
        {
#if GN_DEBUG
            // fill with invalid data
            ::memset( this, sizeof(*this), 0xcd );
#endif
            flags.u32 = 0xFFFFFFFF; // set all flags to true.
            for( int i = 0; i < NUM_SHADER_TYPES; ++i ) shaders[i] = 0;
            rsb.resetToDefault();
            numColorBuffers = 0;
            depthBuffer.texture = 0;
            msaa = MSAA_NONE;
            viewport.set( 0.0f, 0.0f, 1.0f, 1.0f );

            world.identity();
            view.identity();
            proj.identity();
            light0Pos.set( 0.0f, 0.0f, 0.0f, 1.0f );
            light0Diffuse.set( 1.0f, 1.0f, 1.0f, 1.0f );
            materialDiffuse.set( 1.0f, 1.0f, 1.0f, 1.0f );
            materialSpecular.set( 0.2f, 0.2f, 0.2f, 1.0f );
            tsb.resetToDefault();

            numTextures = 0;
            vtxFmt = 0;
            numVtxBufs = 0;
            idxBuf = 0;
        }

        //!
        //! Merge incoming context into current one.
        //!
        void mergeWith( const RendererContext & another )
        {
            if( another.flags.state )
            {
                for( int i = 0; i < NUM_SHADER_TYPES; ++i )
                {
                   if( another.flags.shaderBit(i) ) shaders[i] = another.shaders[i];
                }
                if( another.flags.rsb ) rsb.mergeWith( another.rsb );
                if( another.flags.colorBuffers )
                {
                    for( size_t i = 0; i < another.numColorBuffers; ++i ) colorBuffers[i] = another.colorBuffers[i];
                    numColorBuffers = another.numColorBuffers;
                }
                if( another.flags.depthBuffer ) depthBuffer = another.depthBuffer;
                if( another.flags.msaa ) msaa = another.msaa;
                if( another.flags.viewport ) viewport = another.viewport;
            }

            if( another.flags.ffp )
            {
                if( another.flags.world ) world = another.world;
                if( another.flags.view ) view = another.view;
                if( another.flags.proj ) proj = another.proj;
                if( another.flags.light0Pos ) light0Pos = another.light0Pos;
                if( another.flags.light0Diffuse ) light0Diffuse = another.light0Diffuse;
                if( another.flags.materialDiffuse ) materialDiffuse = another.materialDiffuse;
                if( another.flags.materialSpecular ) materialSpecular = another.materialSpecular;
                if( another.flags.tsb ) tsb.mergeWith( tsb );
            }

            if( another.flags.data )
            {
                if( another.flags.textures )
                {
                    for( size_t i = 0; i < another.numTextures; ++i ) textures[i] = another.textures[i];
                    numTextures = another.numTextures;
                }
                if( another.flags.vtxFmt ) vtxFmt = another.vtxFmt;
                if( another.flags.vtxBufs )
                {
                    for( size_t i = 0; i < another.numVtxBufs; ++i ) vtxBufs[i] = another.vtxBufs[i];
                    numVtxBufs = another.numVtxBufs;
                }
                if( another.flags.idxBuf ) idxBuf = another.idxBuf;
            }

            flags.u32 |= another.flags.u32;
        }

        //!
        //! \name Helper functions to set single state.
        //!
        //! These functions are recommended over directly accessing of data member,
        //! Because these functions can update fieid flags as well.
        //!
        //@{

        //!
        //! Set a shader. Set NULL to use fixed pipeline.
        //!
        inline void setShader( ShaderType type, const Shader * shader );

        //!
        //! Set a list of shaders. The list must have at least NUM_SHADER_TYPES elements.
        //!
        inline void setShaders( const Shader * const shaders[] );

        //!
        //! Set shaders. Set to NULL to use fixed pipeline.
        //!
        inline void setShaders( const Shader * vtxShader, const Shader * pxlShader );

        //!
        //! Set shaders by handle. Set to 0 to use fixed pipeline.
        //!
        inline void setShaderHandles( ShaderDictionary::HandleType vtxShader, ShaderDictionary::HandleType pxlShader );

        //!
        //! Set vertex shader. Set to NULL to use fixed pipeline.
        //!
        inline void setVtxShader( const Shader * s );

        //!
        //! Set vertex shader by handle.
        //!
        inline void setVtxShaderHandle( ShaderDictionary::HandleType h );

        //!
        //! Set pixel shader. Set to NULL to use fixed pipeline.
        //!
        inline void setPxlShader( const Shader * s );

        //!
        //! Set pixel shader by handle. Set 0 to use fixed function pipeline
        //!
        inline void setPxlShaderHandle( ShaderDictionary::HandleType h );

        //!
        //! Set render state block.
        //!
        inline void setRenderStateBlock( const RenderStateBlockDesc & );

        //!
        //! Set individual render state.
        //!
        inline void setRenderState( RenderState state, int32_t value );

        //!
        //! Set render target texture.
        //!
        //! \param index
        //!     Render target index. Must be in range [0,MAX_RENDERT_TARGETS)
        //! \param texture
        //!     Render target texture pointer. Set to NULL to disable this and all render targets
        //!     with larger index. Set index 0 to NULL to render to back buffer.
        //! \param face, level, slice
        //!     Surface in texture. Ignored if texture is NULL.
        //!
        inline void setColorBuffer( size_t index, const Texture * texture, size_t face = 0, size_t level = 0, size_t slice = 0 );

        //!
        //! Set depth buffer
        //!
        inline void setDepthBuffer( const Texture * texture, size_t face = 0, size_t level = 0, size_t slice = 0 );

        //!
        //! Set render target MSAA type
        //!
        inline void setMsaa( MsaaType );

        //!
        //! Set viewport.
        //!
        inline void setViewport( const Rectf & );

        //!
        //! Set viewport.
        //!
        inline void setViewport( float left, float top, float width, float height );

        //!
        //! Set world matrix
        //!
        inline void setWorld( const Matrix44f & value ) { flags.world = 1; world = value; }

        //!
        //! Set view matrix
        //!
        inline void setView( const Matrix44f & value ) { flags.view = 1; view = value; }

        //!
        //! Set proj matrix
        //!
        inline void setProj( const Matrix44f & value ) { flags.proj = 1; proj = value; }

        //!
        //! Set texture stage state block.
        //!
        inline void setTextureStateBlock( const TextureStateBlockDesc & );

        //!
        //! Set texture stage state.
        //!
        inline void setTextureState( size_t stage, TextureState state, TextureStateValue value );

        //!
        //! Set a texture.
        //!
        inline void setTexture( size_t stage, const Texture * tex );

        //!
        //! Set a texture by handle.
        //!
        inline void setTextureHandle( size_t stage, TextureDictionary::HandleType tex );

        //!
        //! set textures, from stage[start] to stage[start+numtex-1].
        //!
        //! \param texlist texture list
        //! \param start   start stage
        //! \param count   number of textures
        //!
        inline void setTextures( const Texture * const texlist[], size_t start, size_t count );

        //!
        //! set textures by handle.
        //!
        inline void setTextureHandles( const TextureDictionary::HandleType texlist[], size_t start, size_t count );

        //!
        //! Set vertex format.
        //!
        inline void setVtxFmt( VtxFmtHandle );

        //!
        //! Set vertex buffer
        //!
        inline void setVtxBuf( size_t index, const VtxBuf * buffer, size_t stride );

        //!
        //! Set index buffer.
        //!
        inline void setIdxBuf( const IdxBuf * );

        //@}
    };
}}

#include "context.inl"

// *****************************************************************************
//                           End of context.h
// *****************************************************************************
#endif // _GN_GFX_CONTEXT_H__
