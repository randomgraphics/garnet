#include "pch.h"

#if GN_MSVC
#pragma warning(disable:4100) // 未引用的形参
#endif

namespace GN { namespace gfx
{
    static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.FAKE");

    class FakeRenderer : public Renderer
    {
        // ********************************************************************
        //
        /// \name Ctor / dtor
        //
        // ********************************************************************

        //@{

    public:

        ///
        /// ctor
        ///
        FakeRenderer() {}

        ///
        /// dtor
        ///
        ~FakeRenderer()
        {
            if( mInitialized )
            {
                sSigDispose();
                sSigDestroy();
            }
        }

        //@}

        // ********************************************************************
        //
        /// \name Device Manager
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
        /// \name Display Manager
        /// \note
        ///     We provide two functions to get API specific rendering context:
        ///     - Sometime, you want to run some API specific codes,
        ///       for debug, test or any other purpose.
        ///     - You may use these functions to detect the current API
        //
        // ********************************************************************

        //@{

    public:

        virtual void * getD3DDevice() const { return 0; }
        virtual void * getOGLRC() const { return 0; }

        //@}

        // ********************************************************************
        //
        /// \name Capabilities Manager
        //
        // ********************************************************************

        //@{

    public:

        virtual UInt32 getCaps( RendererCaps c ) const
        {
            switch( c )
            {
                case CAPS_MAX_2D_TEXTURE_SIZE : return 4096;
                case CAPS_MAX_CLIP_PLANES     : return 8;
                case CAPS_MAX_RENDER_TARGETS  : return 4;
                case CAPS_MAX_PRIMITIVES      : return 65535;
                case CAPS_MAX_TEXTURE_STAGES  : return 8;
                default :
                    GN_ERROR(sLogger)( "invlid cap: %d", c );
                    return 0;
            };
        }
        virtual bool supportShader( const StrA & ) { return true; }
        virtual bool supportTextureFormat( TexType type, BitFields usage, ClrFmt format ) const { return true; }

        //@}

        // ********************************************************************
        //
        /// \name resource Manager
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
            std::vector<UInt8> mBuffer;

        public:

            FakeTexture() {}

            bool init( const TextureDesc & desc )
            {
                if( !setDesc( desc ) ) return false;
                mBuffer.resize( getDesc().width * getDesc().height * getDesc().depth * getClrFmtDesc(getDesc().format).bits / 8 );
                for( size_t i = 0; i < getDesc().levels; ++i )
                {
                    Vector3<UInt32> sz( getBaseSize() );
                    sz.x >>= i; if( 0 == sz.x ) sz.x = 1;
                    sz.y >>= i; if( 0 == sz.y ) sz.y = 1;
                    sz.z >>= i; if( 0 == sz.z ) sz.z = 1;
                    setMipSize( i, sz );
                }
                return true;
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
            std::vector<UInt8> mBuffer;

        public:

            FakeVtxBuf( size_t bytes ) { mBuffer.resize(bytes); }

            virtual void * lock( size_t offset, size_t bytes, LockFlag flag ) { return &mBuffer[0]; }
            virtual void unlock() {}
        };

        class FakeIdxBuf : public IdxBuf
        {
            std::vector<UInt16> mBuffer;

        public:

            FakeIdxBuf( size_t numIdx ) { mBuffer.resize(numIdx); }

            virtual UInt16 * lock( size_t startIdx, size_t numIdx, LockFlag flag ) { return &mBuffer[0]; }
            virtual void unlock() {}
        };

    public:

        virtual Shader * createShader( ShaderType type, ShadingLanguage lang, const StrA & code, const StrA & hints )
        {
            return new FakeShader( type, lang, code, hints );
        }
        virtual Texture * createTexture( const TextureDesc & desc )
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
        virtual UInt32 createVtxFmt( const VtxFmtDesc & ) { return 1; }
        virtual VtxBuf * createVtxBuf( size_t bytes, bool dynamic, bool sysCopy ) { return new FakeVtxBuf( bytes ); }
        virtual IdxBuf * createIdxBuf( size_t numIdx, bool dynamic, bool sysCopy ) { return new FakeIdxBuf( numIdx ); }

        //@}

        // ********************************************************************
        //
        /// \name Context manager
        //
        // ********************************************************************

        //@{

        virtual void setContext( const RendererContext & ) {}
        virtual void rebindContext( RendererContext::FieldFlags ) {}
        virtual const RenderStateBlockDesc & getCurrentRenderStateBlock() const { return RenderStateBlockDesc::DEFAULT; }

        //@}

        // ********************************************************************
        //
        /// \name Drawing Manager
        //
        // ********************************************************************

        //@{

    public :

        virtual bool drawBegin() { return true; }
        virtual void drawEnd() {}
        virtual void clearScreen( const Vector4f & c, float z, UInt32 s, BitFields flags ) {}
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
                             const UInt16 * indexData ) {}
        virtual void drawUp( PrimitiveType prim,
                             size_t        numPrims,
                             const void *  vertexData,
                             size_t        strideInBytes ) {}
        virtual void drawQuads( BitFields options,
                                const void * positions, size_t posStride,
                                const void * texcoords, size_t texStride,
                                const void * colors, size_t clrStride,
                                size_t count ) {}
        virtual void drawLines( BitFields options,
                                const void * positions,
                                size_t stride,
                                size_t count,
                                UInt32 color,
                                const Matrix44f & model,
                                const Matrix44f & view,
                                const Matrix44f & proj ) {}
        virtual void drawDebugText( const char * text, int x, int y, const Vector4f & color ) {}

        //@}

        // ********************************************************************
        //
        /// \name Misc. utilities
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
