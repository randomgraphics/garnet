#include "pch.h"

#if GN_MSVC
#pragma warning(disable:4100) // 未引用的形参
#endif

namespace GN { namespace gfx
{
    class FakeRenderer : public Renderer
    {
        // ********************************************************************
        //
        //! \name Ctor / dtor
        //
        // ********************************************************************

        //@{

    public:

        //!
        //! ctor
        //!
        FakeRenderer()
        {
            capsCtor();
        }

        //!
        //! dtor
        //!
        ~FakeRenderer()
        {
            sSigDeviceDispose();
            sSigDeviceDestroy();
        }

        //@}
        
        // ********************************************************************
        //
        //! \name Device Manager
        //
        // ********************************************************************

        //@{

    public:

        bool init( const RendererOptions & ro )
        {
            changeOptions( ro, false );
            return sSigDeviceCreate() &&  sSigDeviceRestore();
        }

        virtual bool changeOptions( RendererOptions ro, bool forceDeviceRecreation ) { setOptions( ro ); return true; }

        //@}

        // ********************************************************************
        //
        //! \name Display Manager
        //! \note
        //!     We provide two functions to get API specific rendering context:
        //!     - Sometime, you want to run some API specific codes,
        //!       for debug, test or any other purpose.
        //!     - You may use these functions to detect the current API
        //
        // ********************************************************************

        //@{

    public:

        virtual void * getD3DDevice() const { return 0; }
        virtual void * getOGLRC() const { return 0; }

        //@}

        // ********************************************************************
        //
        //! \name Capabilities Manager
        //
        // ********************************************************************

        //@{

    public:

        void capsCtor()
        {
            setCaps( CAPS_MAX_2D_TEXTURE_SIZE, 4096 );
            setCaps( CAPS_MAX_CLIP_PLANES    , 8 );
            setCaps( CAPS_MAX_RENDER_TARGETS , 4 );
            setCaps( CAPS_MAX_PRIMITIVES     , 65535 );
            setCaps( CAPS_MAX_TEXTURE_STAGES , 8 );
            setCaps( CAPS_PER_STAGE_CONSTANT , 1 );
            setCaps( CAPS_PSCAPS             , 0xFF );
            setCaps( CAPS_VSCAPS             , 0xFF );
        }

        //@}

        // ********************************************************************
        //
        //! \name Shader Manager
        //
        // ********************************************************************

        //@{

        class FakeShader : public Shader
        {
            StrA mCode, mEntry;

            virtual bool queryDeviceUniform( const char * name, HandleType & userData ) const { return true; }

        public:

            FakeShader( ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & entry )
                : Shader(type,lang), mCode(code), mEntry(entry) {}
        };

        virtual bool supportShader( ShaderType, ShadingLanguage ) { return true; }

        virtual Shader *
        createShader( ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & entry )
        {
            return new FakeShader( type, lang, code, entry );
        }

        virtual void bindShader( ShaderType type, const Shader * shader ) {}
        virtual void bindShaders( const Shader * const shaders[] ) {}

        //@}

        // ********************************************************************
        //
        //! \name Render State Block Manager
        //
        // ********************************************************************

        //@{

        virtual uint32_t createRenderStateBlock( const RenderStateBlockDesc & ) { return 1; }
        virtual void bindRenderStateBlock( uint32_t )  {}
        virtual void getCurrentRenderStateBlock( RenderStateBlockDesc & ) const {}
        virtual uint32_t setRenderState( RenderState state, RenderStateValue value ) { return 1; }

        //@}

        // ********************************************************************
        //
        //! \name Texture Manager
        //
        // ********************************************************************

        //@{

        class FakeTexture : public Texture
        {
            std::vector<uint8_t> mBuffer;

        public:

            FakeTexture() {}

            bool init( TexType type,
                       uint32_t sx, uint32_t sy, uint32_t sz,
                       uint32_t levels,
                       ClrFmt format,
                       uint32_t usage )
            {
                if( !setProperties( type, sx, sy, sz, levels, format, usage ) ) return false;
                mBuffer.resize( sx * sy * sz * getClrFmtDesc(format).bits / 8 );
                return true;
            }

            virtual void getMipMapSize( uint32_t level, uint32_t * sx, uint32_t * sy = 0, uint32_t * sz = 0 ) const
            {
                uint32_t x, y, z;
                getBaseMapSize( &x, &y, &z );
                x >>= level; if( 0 == x ) x = 1; if( sx ) *sx = x;
                y >>= level; if( 0 == y ) y = 1; if( sy ) *sy = y;
                z >>= level; if( 0 == z ) z = 1; if( sz ) *sz = z;
            }
            virtual void setFilter( TexFilter, TexFilter ) const {}
            virtual void setWrap( TexWrap, TexWrap, TexWrap ) const {}
            virtual void * lock1D( uint32_t level, uint32_t offset, uint32_t length, uint32_t flag ) { return &mBuffer[0]; }
            virtual bool lock2D( LockedRect &  result, uint32_t level, const Recti * area, uint32_t flag )
            {
                result.data = &mBuffer[0];
                const ClrFmtDesc & desc = getClrFmtDesc( getFormat() );
                result.rowBytes = getBaseMapSize().x * desc.bits * desc.blockHeight / 8;
                return true;
            }
            virtual bool lock3D( LockedBox &  result, uint32_t level, const Boxi * box, uint32_t flag )
            {
                result.data = &mBuffer[0];
                const ClrFmtDesc & desc = getClrFmtDesc( getFormat() );
                result.rowBytes = getBaseMapSize().x * desc.bits * desc.blockHeight / 8;
                result.sliceBytes = result.rowBytes * getBaseMapSize().y / desc.blockHeight;
                return true;
            }
            virtual bool lockCube( LockedRect &  result,
                                   TexFace       face,
                                   uint32_t      level,
                                   const Recti * area,
                                   uint32_t      flag )
            {
                result.data = &mBuffer[0];
                const ClrFmtDesc & desc = getClrFmtDesc( getFormat() );
                result.rowBytes = getBaseMapSize().x * desc.bits * desc.blockHeight / 8;
                return true;
            }
            virtual void unlock() {}
            virtual void updateMipmap() {}
            virtual void * getAPIDependentData() const { return 0; }
        };

        virtual Texture *
        createTexture( TexType textype,
                       uint32_t sx, uint32_t sy, uint32_t sz,
                       uint32_t levels,
                       ClrFmt format,
                       uint32_t usage,
                       const TextureLoader & loader )
        {
            AutoRef<FakeTexture> tex( new FakeTexture );
            if( !tex->init( textype, sx, sy, sz, levels, format, usage ) ) return 0;
            return tex.detach();
        }

        virtual Texture * createTextureFromFile( File & )
        {
            AutoRef<FakeTexture> tex( new FakeTexture );
            if( !tex->init( TEXTYPE_2D, 256, 256, 1, 1, FMT_D3DCOLOR, 0 ) ) return 0;
            return tex.detach();
        }

        //@}

        // ********************************************************************
        //
        //! \name Renderable Buffer Manager
        //
        // ********************************************************************

        //@{

        class FakeVtxBuf : public VtxBuf
        {
            std::vector<uint8_t> mBuffer;

        public:

            FakeVtxBuf( size_t bytes ) { mBuffer.resize(bytes); }

            virtual void * lock( size_t offset, size_t bytes, uint32_t flag ) { return &mBuffer[0]; }
            virtual void unlock() {}
        };

        class FakeIdxBuf : public IdxBuf
        {
            std::vector<uint16_t> mBuffer;

        public:

            FakeIdxBuf( size_t numIdx ) { mBuffer.resize(numIdx); }

            virtual uint16_t * lock( size_t startIdx, size_t numIdx, uint32_t flag ) { return &mBuffer[0]; }
            virtual void unlock() {}
        };

        virtual uint32_t createVtxBinding( const VtxFmtDesc & ) { return 1; }
        virtual VtxBuf * createVtxBuf( size_t bytes, bool dynamic, bool sysCopy, const VtxBufLoader & loader ) { return new FakeVtxBuf( bytes ); }
        virtual IdxBuf * createIdxBuf( size_t numIdx, bool dynamic, bool sysCopy, const IdxBufLoader & loader ) { return new FakeIdxBuf( numIdx ); }
        virtual void bindVtxBinding( uint32_t )  {}
        virtual void bindVtxBufs( const VtxBuf * const buffers[], size_t start, size_t count ) {}
        virtual void bindVtxBuf( size_t index, const VtxBuf * buffer, size_t stride ) {}
        virtual void bindIdxBuf( const IdxBuf * ) {}

        //@}

        // ********************************************************************
        //
        //! \name Fixed Function Pipeline Manager.
        //
        // ********************************************************************

        //@{

    public:

        virtual Matrix44f &
        composePerspectiveMatrix( Matrix44f & result,
                                  float fovy,
                                  float ratio,
                                  float znear,
                                  float zfar ) const { return Matrix44f::IDENTITY; }
        virtual Matrix44f &
        composeOrthoMatrix( Matrix44f & result,
                            float left,
                            float bottom,
                            float width,
                            float height,
                            float znear,
                            float zfar ) const { return Matrix44f::IDENTITY; }

        //@}

        // ********************************************************************
        //
        //! \name Render Target Manager
        //
        // ********************************************************************

        //@{

    public:

        virtual void setRenderTarget( size_t index,
                                      const Texture * texture,
                                      uint32_t level,
                                      TexFace face ) {}
        virtual void setRenderDepth( const Texture * texture,
                                     uint32_t level,
                                     TexFace face ) {}

        //@}

        // ********************************************************************
        //
        //! \name Drawing Manager
        //
        // ********************************************************************

        //@{

    public :

        virtual bool drawBegin() { return true; }
        virtual void drawEnd() {}
        virtual void drawFinish() {}
        virtual void clearScreen( const Vector4f & c, float z, uint32_t s, uint32_t flags ) {}
        virtual void drawIndexed( PrimitiveType prim,
                                  size_t        numPrim,
                                  size_t        startVtx,
                                  size_t        minVtxIdx,
                                  size_t        numVtx,
                                  size_t        startIdx ) {}
        virtual void draw( PrimitiveType prim,
                           size_t        numPrim,
                           size_t        startVtx ) {}
        virtual void drawIndexedUp(
                             PrimitiveType    prim,
                             size_t           numPrims,
                             size_t           numVertices,
                             const void *     vertexData,
                             size_t           strideInBytes,
                             const uint16_t * indexData ) {}
        virtual void drawUp( PrimitiveType prim,
                             size_t        numPrims,
                             const void *  vertexData,
                             size_t        strideInBytes ) {}
        virtual void drawGeometry( const RenderingParameters &, const RenderingGeometry *, size_t ) {}
        virtual void drawQuads( uint32_t options,
                                const void * positions, size_t posStride,
                                const void * texcoords, size_t texStride,
                                size_t count ) {}
        virtual void drawDebugTextA( const char * text, int x, int y, const Vector4f & color ) {}
        virtual void drawDebugTextW( const wchar_t * text, int x, int y, const Vector4f & color ) {}

        //@}

        // ********************************************************************
        //
        //! \name Misc. utilities
        //
        // ********************************************************************

        //@{

    public:

        void dumpCurrentState( StrA & ) const {}

        //@}

    };
}}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Renderer * createFakeRenderer( const GN::gfx::RendererOptions & ro )
{
    GN_GUARD;

    GN::gfx::FakeRenderer * p = new GN::gfx::FakeRenderer;
    p->init( ro );
    return p;

    GN_UNGUARD;
}
