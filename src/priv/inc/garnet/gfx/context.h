#ifndef _GN_GFX_CONTEXT_H__
#define _GN_GFX_CONTEXT_H__
// *****************************************************************************
/// \file    context.h
/// \brief   Rendering context
/// \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN { namespace gfx
{
    typedef UIntPtr VtxFmtHandle; ///< Vertex format handle

    enum
    {
        MAX_RENDER_TARGETS = 8 ///< We support 8 render targets at most.
    };

    ///
    /// Msaa type
    ///
    enum MsaaType
    {
        MSAA_NONE,   ///< No MSAA
        MSAA_LOW,    ///< low quality MSAA
        MSAA_MEDIUM, ///< medium quality MSAA
        MSAA_HIGH,   ///< high quality MSAA
        MSAA_ULTRA,  ///< ultra quality MSAA
        NUM_MSAA_TYPES, ///< number of MSAA types
    };

    ///
    /// describe a render target texture
    ///
    struct RenderTargetTexture
    {
        const Texture * texture; ///< render target texture
        unsigned int face  : 14; ///< cubemap face. Must be zero for non-cube/stack texture.
        unsigned int slice : 14; ///< slice index. Must be zero for non 3D texture.
        unsigned int level :  4; ///< mipmap level

        ///
        /// equality check
        ///
        bool operator==( const RenderTargetTexture & rhs ) const
        {
            if( texture != rhs.texture ) return false;
            if( NULL == texture ) return true; // ignore remaining parameters, if texture is NULL.
            return face == rhs.face && level == rhs.level && slice == rhs.slice;
        }

        ///
        /// equality check
        ///
        bool operator!=( const RenderTargetTexture & rhs ) const
        {
            if( texture != rhs.texture ) return true;
            if( NULL == texture ) return false; // ignore remaining parameters, if texture is NULL.
            return face != rhs.face || level != rhs.level || slice != rhs.slice;
        }
    };

    ///
    /// render target description
    ///
    struct RenderTargetDesc
    {
        RenderTargetTexture cbuffers[MAX_RENDER_TARGETS]; ///< color buffer descriptions. Ignored when draw to back buffer.
        RenderTargetTexture zbuffer; ///< z buffer description. Set zbuffer.texture to NULL to use auto-zbuffer.
        unsigned int count :  5; ///< color buffer count. 0 means draw to back buffer.
        unsigned int aa    :  3; ///< anti-alias type. One of MsaaType. Ignored when draw to back buffer.

        ///
        /// set color buffer
        ///
        void setcbuf( UInt32 index, const Texture * tex, UInt32 level = 0, UInt32 face = 0, UInt32 slice = 0 )
        {
            GN_ASSERT( index < MAX_RENDER_TARGETS );
            cbuffers[index].texture = tex;
            cbuffers[index].face = face;
            cbuffers[index].level = level;
            cbuffers[index].slice = slice;
        }

        ///
        /// set z buffer
        ///
        void setzbuf( const Texture * tex, UInt32 level = 0, UInt32 face = 0, UInt32 slice = 0 )
        {
            zbuffer.texture = tex;
            zbuffer.level = level;
            zbuffer.face = face;
            zbuffer.slice = slice;
        }

        ///
        /// set descriptor as "draw-to-backbuffer"
        ///
        void drawToBackbuffer()
        {
            memset( this, 0, sizeof(*this) );
        }

        ///
        /// draw to base map of texture(s)
        ///
        void drawToBaseMap(
            UInt32 count_,
            const Texture * rt0,
            const Texture * rt1 = 0,
            const Texture * rt2 = 0,
            const Texture * rt3 = 0,
            const Texture * z = 0,
            MsaaType aa_ = MSAA_NONE )
        {
            count = count_;
            aa = aa_;
            setcbuf( 0, rt0 );
            setcbuf( 1, rt1 );
            setcbuf( 2, rt2 );
            setcbuf( 3, rt3 );
            setzbuf( z );
        }
        //@}

        ///
        /// validality check
        ///
        bool valid() const
        {
            if( count > MAX_RENDER_TARGETS ) return false;
            if( aa > NUM_MSAA_TYPES ) return false;
            return true;
        }

        ///
        /// equality check
        ///
        bool operator==( const RenderTargetDesc & rhs ) const
        {
            if( this == &rhs ) return true;
            if( count != rhs.count ) return false;
            if( 0 == count ) return true;
            if( aa != rhs.aa ) return false;
            for( UInt i = 0; i < count; ++i ) if( cbuffers[i] != rhs.cbuffers[i] ) return false;
            if( zbuffer != rhs.zbuffer ) return false;
            return true;
        }

        ///
        /// equality check
        ///
        bool operator!=( const RenderTargetDesc & rhs ) const { return !( *this == rhs ); }
    };

    ///
    /// Renderer context state. Completely define how rendering would be done
    ///
    struct RendererContext
    {
        ///
        /// Context flag structure.
        /// Zero means that field is undefined, and should be ignored.
        ///
        union FieldFlags
        {
            unsigned int u32; ///< all flags as uint32

            struct
            {
                unsigned char state;    ///< frequently used states
                unsigned char ffp;      ///< fixed function pipeline states
                unsigned char data;     ///< rendering data flags
                unsigned char reserved; ///< reserved for future use.
            };

            struct
            {
                // byte 0 (general states)
                unsigned int shaders            : 3; ///< one bit for each shader type
                unsigned int rsb                : 1; ///< render state block
                unsigned int renderTargets      : 1; ///< render target textures
                unsigned int viewport           : 1; ///< viewport
                unsigned int                    : 2; ///< reserved
                // byte 1 (fixed functional pipeline states)
                unsigned int world              : 1; ///< world transformation
                unsigned int view               : 1; ///< view transformation
                unsigned int proj               : 1; ///< projection transformation
                unsigned int light0Pos          : 1; ///< light 0 position
                unsigned int light0Diffuse      : 1; ///< light 0 diffuse
                unsigned int materialDiffuse    : 1; ///< material diffues color
                unsigned int materialSpecular   : 1; ///< material specular color
                unsigned int tsb                : 1; ///< texture state block
                // byte 2 (graphics resources)
                unsigned int textures           : 1; ///< textures
                unsigned int vtxFmt             : 1; ///< vertex format
                unsigned int vtxBufs            : 1; ///< vertex buffers
                unsigned int idxBuf             : 1; ///< index buffer
                unsigned int                    : 4; ///< reserved
                // byte 3 (reserved)
                unsigned int                    : 8; ///< reserved
            };

            /// \name helper functions to access shader bits
            //@{
            bool shaderBit( int type ) const { GN_ASSERT( 0 <= type && type < NUM_SHADER_TYPES ); return 0 != ( shaders & (1<<type) ); }
            bool vsBit() const { return 0 != ( shaders & (1<<SHADER_VS) ); }
            bool psBit() const { return 0 != ( shaders & (1<<SHADER_PS) ); }
            bool gsBit() const { return 0 != ( shaders & (1<<SHADER_GS) ); }
            void setShaderBit( int type ) { GN_ASSERT( 0 <= type && type < NUM_SHADER_TYPES ); shaders |= 1 << type; }
            //@}
        };

        ///
        /// Vertex buffer binding descriptor
        ///
        struct VtxBufDesc
        {
            const VtxBuf * buffer; ///< buffer pointer
            UInt32         offset; ///< byte offset of the first vertex in buffer.
            UInt32         stride; ///< vertex stride in bytes

            ///
            /// equality check
            ///
            bool operator==( const VtxBufDesc & rhs ) const
            {
                return buffer == rhs.buffer && offset == rhs.offset && stride == rhs.stride;
            }

            ///
            /// equality check
            ///
            bool operator!=( const VtxBufDesc & rhs ) const
            {
                return buffer != rhs.buffer || offset != rhs.offset || stride != rhs.stride;
            }
        };

        // context flags
        FieldFlags            flags; ///< field flags

        // general states
        const Shader *        shaders[NUM_SHADER_TYPES]; ///< shaders
        RenderStateBlockDesc  rsb; ///< render state block.
        RenderTargetDesc      renderTargets; ///< render target descriptor
        Rectf                 viewport; ///< Viewport. Note that viewport is relative to current render target size.
                                        ///< For example, viewport [0,0,1,1] means the whole render target.

        // fixed functional pipeline states
        Matrix44f             world,            ///< world transformation
                              view,             ///< view transformation
                              proj;             ///< projection transformation
        Vector4f              light0Pos,        ///< light0 position
                              light0Diffuse,    ///< light0 diffuse color
                              materialDiffuse,  ///< diffuse material color
                              materialSpecular; ///< specular material color
        TextureStateBlockDesc tsb;              ///< texture state block

        // graphics resources
        const Texture *       textures[MAX_TEXTURE_STAGES]; ///< texture list
        UInt32                numTextures; ///< texture count
        VtxFmtHandle          vtxFmt; ///< vertex format handle. 0 means no vertex data at all.
        VtxBufDesc            vtxBufs[MAX_VERTEX_ATTRIBUTES]; ///< vertex buffers.
        UInt32                numVtxBufs; ///< vertex buffer count.
        const IdxBuf *        idxBuf; ///< index buffer

        ///
        /// Clear to null context, all fields are unused/undefined.
        ///
        void clearToNull()
        {
#if GN_DEBUG_BUILD
            // fill with invalid data
            ::memset( this, sizeof(*this), 0xcd );
#endif
            GN_CASSERT( 4 == sizeof(FieldFlags) );
            flags.u32 = 0;
            rsb.resetToEmpty();
            numTextures = 0;
            numVtxBufs = 0;
        }

        ///
        /// Reset to default context.
        ///
        void resetToDefault()
        {
#if GN_DEBUG_BUILD
            // fill with invalid data
            ::memset( this, sizeof(*this), 0xcd );
#endif
            flags.u32 = 0xFFFFFFFF; // set all flags to true.
            for( int i = 0; i < NUM_SHADER_TYPES; ++i ) shaders[i] = 0;
            rsb.resetToDefault();
            renderTargets.drawToBackbuffer();
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

        ///
        /// Merge incoming context into current one.
        ///
        void mergeWith( const RendererContext & another )
        {
            if( another.flags.state )
            {
                for( int i = 0; i < NUM_SHADER_TYPES; ++i )
                {
                   if( another.flags.shaderBit(i) ) shaders[i] = another.shaders[i];
                }
                if( another.flags.rsb ) rsb.mergeWith( another.rsb );
                if( another.flags.renderTargets ) renderTargets = another.renderTargets;
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

        ///
        /// \name Helper functions to set single state.
        ///
        /// These functions are recommended over directly accessing of data member,
        /// Because these functions can update fieid flags as well.
        ///
        //@{

        ///
        /// Set a shader. Set NULL to use fixed pipeline.
        ///
        inline void setShader( ShaderType type, const Shader * shader );

        ///
        /// Set shaders. Set to NULL to use fixed pipeline.
        ///
        inline void setShaders( const Shader * vs, const Shader * ps, const Shader * gs );

        ///
        /// Set vertex shader. Set to NULL to use fixed pipeline.
        ///
        inline void setVS( const Shader * s );

        ///
        /// Set pixel shader. Set to NULL to use fixed pipeline.
        ///
        inline void setPS( const Shader * s );

        ///
        /// Set geometry shader. Set to NULL to use fixed pipeline.
        ///
        inline void setGS( const Shader * s );

        ///
        /// Set render state block.
        ///
        inline void setRenderStateBlock( const RenderStateBlockDesc & );

        ///
        /// Set individual render state.
        ///
        inline void setRenderState( RenderState state, SInt32 value );

        ///
        /// Set render target textures.
        ///
        inline void setRenderTargets( const RenderTargetDesc & );

        ///
        /// Set draw to back buffer.
        ///
        void setDrawToBackBuf();

        ///
        /// Set draw to texture(s)
        ///
        void setDrawToTextures(
            UInt32 count_,
            const Texture * rt0,
            const Texture * rt1 = 0,
            const Texture * rt2 = 0,
            const Texture * rt3 = 0,
            const Texture * z = 0,
            MsaaType aa_ = MSAA_NONE );

        ///
        /// Set draw to texture(s)
        ///
        void setDrawToTextureWithoutDepth(
            const Texture * tex,
            UInt32 level = 0,
            UInt32 face = 0,
            UInt32 slice = 0,
            MsaaType aa_ = MSAA_NONE );

        ///
        /// Set viewport.
        ///
        inline void setViewport( const Rectf & );

        ///
        /// Set viewport.
        ///
        inline void setViewport( float left, float top, float width, float height );

        ///
        /// Set world matrix
        ///
        inline void setWorld( const Matrix44f & value ) { flags.world = 1; world = value; }

        ///
        /// Set view matrix
        ///
        inline void setView( const Matrix44f & value ) { flags.view = 1; view = value; }

        ///
        /// Set proj matrix
        ///
        inline void setProj( const Matrix44f & value ) { flags.proj = 1; proj = value; }

        ///
        /// Set texture stage state block.
        ///
        inline void setTextureStateBlock( const TextureStateBlockDesc & );

        ///
        /// Set texture stage state.
        ///
        inline void setTextureState( size_t stage, TextureState state, TextureStateValue value );

        ///
        /// Set a texture.
        ///
        inline void setTexture( size_t stage, const Texture * tex );

        ///
        /// set textures, from stage[start] to stage[start+numtex-1].
        ///
        /// \param texlist texture list
        /// \param start   start stage
        /// \param count   number of textures
        ///
        inline void setTextures( const Texture * const texlist[], size_t start, size_t count );

        ///
        /// Set vertex format.
        ///
        inline void setVtxFmt( VtxFmtHandle );

        ///
        /// Set vertex buffer
        ///
        inline void setVtxBuf( size_t index, const VtxBuf * buffer, size_t offset, size_t stride );

        ///
        /// Set index buffer.
        ///
        inline void setIdxBuf( const IdxBuf * );

        //@}
    };
}}

#include "context.inl"

// *****************************************************************************
//                           End of context.h
// *****************************************************************************
#endif // _GN_GFX_CONTEXT_H__
