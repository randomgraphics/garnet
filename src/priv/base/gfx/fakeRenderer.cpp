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
        FakeRenderer() {}

        //!
        //! dtor
        //!
        ~FakeRenderer()
        {
            if( mInitialized )
            {
                sSigCreate();
                sSigDestroy();
            }
        }

        //@}

        // ********************************************************************
        //
        //! \name Device Manager
        //
        // ********************************************************************

        //@{

    public:

        virtual bool changeOptions( const RendererOptions & ro, bool forceDeviceRecreation )
        {
            setOptions( ro );
            if( !mInitialized )
            {
                mInitialized = true;
                if( !sSigCreate() || !sSigRestore() ) return false;
            }
            return true;
        }

    private:

        AutoInit<bool,false> mInitialized;

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

        virtual uint32_t getCaps( RendererCaps c ) const
        {
            switch( c )
            {
                case CAPS_MAX_2D_TEXTURE_SIZE : return 4096;
                case CAPS_MAX_CLIP_PLANES     : return 8;
                case CAPS_MAX_RENDER_TARGETS  : return 4;
                case CAPS_MAX_PRIMITIVES      : return 65535;
                case CAPS_MAX_TEXTURE_STAGES  : return 8;
                case CAPS_PER_STAGE_CONSTANT  : return 1;
                case CAPS_PSCAPS              : return 0xFF;
                case CAPS_VSCAPS              : return 0xFF;
                default :
                    GN_ERROR( "invlid cap: %d", c );
                    return 0;
            };
        }
        virtual bool supportShader( ShaderType, ShadingLanguage ) { return true; }
        virtual bool supportTextureFormat( TexType type, BitField usage, ClrFmt format ) const { return true; }

        //@}

        // ********************************************************************
        //
        //! \name resource Manager
        //
        // ********************************************************************

        //@{

        class FakeShader : public Shader
        {
            StrA mCode, mHints;

            virtual bool queryDeviceUniform( const char * name, HandleType & userData ) const { return true; }

        public:

            FakeShader( ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & hints )
                : Shader(type,lang), mCode(code), mHints(hints) {}
        };

        class FakeTexture : public Texture
        {
            std::vector<uint8_t> mBuffer;

        public:

            FakeTexture() {}

            bool init( const TextureDesc & desc )
            {
                if( !setDesc( desc ) ) return false;
                mBuffer.resize( getDesc().width * getDesc().height * getDesc().depth * getClrFmtDesc(getDesc().format).bits / 8 );
                return true;
            }

            virtual Vector3<uint32_t> getMipSize( size_t level ) const
            {
                Vector3<uint32_t> sz( getBaseSize() );
                sz.x >>= level; if( 0 == sz.x ) sz.x = 1;
                sz.y >>= level; if( 0 == sz.y ) sz.y = 1;
                sz.z >>= level; if( 0 == sz.z ) sz.z = 1;
                return sz;
            }
            virtual void setFilter( TexFilter, TexFilter ) const {}
            virtual void setWrap( TexWrap, TexWrap, TexWrap ) const {}
            virtual bool lock(
                TexLockedResult & result,
                size_t face,
                size_t level,
                const Boxi * area,
                LockFlag flag )
            {
                result.data = &mBuffer[0];
                const ClrFmtDesc & desc = getClrFmtDesc( getDesc().format );
                result.rowBytes = getBaseSize().x * desc.bits * desc.blockHeight / 8;
                result.sliceBytes = result.rowBytes * getBaseSize().y / desc.blockHeight;
                return true;
            }
            virtual void unlock() {}
            virtual void updateMipmap() {}
            virtual void * getAPIDependentData() const { return 0; }
        };

        class FakeVtxBuf : public VtxBuf
        {
            std::vector<uint8_t> mBuffer;

        public:

            FakeVtxBuf( size_t bytes ) { mBuffer.resize(bytes); }

            virtual void * lock( size_t offset, size_t bytes, LockFlag flag ) { return &mBuffer[0]; }
            virtual void unlock() {}
        };

        class FakeIdxBuf : public IdxBuf
        {
            std::vector<uint16_t> mBuffer;

        public:

            FakeIdxBuf( size_t numIdx ) { mBuffer.resize(numIdx); }

            virtual uint16_t * lock( size_t startIdx, size_t numIdx, LockFlag flag ) { return &mBuffer[0]; }
            virtual void unlock() {}
        };

    public:

        virtual Shader * createShader( ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & hints )
        {
            return new FakeShader( type, lang, code, hints );
        }
        virtual Texture * createTexture( const TextureDesc & desc, const TextureLoader & loader )
        {
            AutoRef<FakeTexture> tex( new FakeTexture );
            if( !tex->init( desc ) ) return 0;
            return tex.detach();
        }
        virtual Texture * createTextureFromFile( File & )
        {
            TextureDesc desc = { TEXTYPE_2D, 256, 256, 1, 1, 1, FMT_D3DCOLOR, 0 };
            AutoRef<FakeTexture> tex( new FakeTexture );
            if( !tex->init( desc ) ) return 0;
            return tex.detach();
        }
        virtual uint32_t createVtxFmt( const VtxFmtDesc & ) { return 1; }
        virtual VtxBuf * createVtxBuf( size_t bytes, bool dynamic, bool sysCopy, const VtxBufLoader & loader ) { return new FakeVtxBuf( bytes ); }
        virtual IdxBuf * createIdxBuf( size_t numIdx, bool dynamic, bool sysCopy, const IdxBufLoader & loader ) { return new FakeIdxBuf( numIdx ); }

        //@}

        // ********************************************************************
        //
        //! \name Context manager
        //
        // ********************************************************************

        //@{

        virtual void setContext( const RendererContext & ) {}
        virtual void rebindContext( RendererContext::FieldFlags ) {}
        virtual const RenderStateBlockDesc & getCurrentRenderStateBlock() const { return RenderStateBlockDesc::DEFAULT; }

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
        virtual void clearScreen( const Vector4f & c, float z, uint32_t s, BitField flags ) {}
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
        virtual void drawQuads( BitField options,
                                const void * positions, size_t posStride,
                                const void * texcoords, size_t texStride,
                                const void * colors, size_t clrStride,
                                size_t count ) {}
        virtual void drawLines( BitField options,
                                const void * positions,
                                size_t stride,
                                size_t count,
                                uint32_t color,
                                const Matrix44f & model,
                                const Matrix44f & view,
                                const Matrix44f & proj ) {}
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

        virtual Matrix44f & composePerspectiveMatrix( Matrix44f & result,
                                                      float fovy,
                                                      float ratio,
                                                      float znear,
                                                      float zfar ) const { return Matrix44f::IDENTITY; }
        virtual Matrix44f & composeOrthoMatrix( Matrix44f & result,
                                                float left,
                                                float bottom,
                                                float width,
                                                float height,
                                                float znear,
                                                float zfar ) const { return Matrix44f::IDENTITY; }
        void dumpCurrentState( StrA & ) const {}

        //@}

    };
}}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Renderer * createFakeRenderer( )
{
    GN_GUARD;
    return new GN::gfx::FakeRenderer;
    GN_UNGUARD;
}
