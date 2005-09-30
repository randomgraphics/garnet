#ifndef __GN_GFX_GFXRENDERER_H__
#define __GN_GFX_GFXRENDERER_H__
// *****************************************************************************
//! \file    gfxRenderer.h
//! \brief   Main renderer interface of GFX module
//! \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN
{
    namespace gfx
    {
        struct Texture;
        struct Shader;
        struct RenderStateBlock;
        struct VtxBuf;
        struct IdxBuf;

        // ***************************************************************************
        //!
        //! gfx模块的主接口类
        //!
        //! \nosubgrouping
        //!
        // ***************************************************************************
        struct Renderer : public NoCopy
        {
            // ****************************************************************************
            //
            //! \name Device Manager
            //
            // ****************************************************************************

            //@{

            //!
            //! Triggered after rendering device is restored to normal stage.
            //!
            //! \note
            //! - Resources that may lost their contents after device reset are:
            //!   - Textures, and
            //!   - Vertex buffers that are NOT type of BUFUSAGE_READBACK.
            //!   - Note that only contents are lost, not resources themselves.
            //! - Index buffers, shaders and rsblocks survive device reset.
            //!
            Signal0<bool> sigDeviceRestore;

            //!
            //! Triggered right before invalidating of rendering device.
            //!
            //! \note
            //! - After receiving this signal, no rendering function should be called,
            //!   before you receive sigDeviceRestore.
            //! - This signal will also be triggered, before rendering device is
            //!   destroyed.
            //!
            Signal0<void> sigDeviceDispose;

            //!
            //! Change device configuration.
            //!
            //! \param ds
            //!     new device settings
            //! \param forceDeviceRecreation
            //!     force a full device recreation
            //! \note
            //!     This function may trigger sigDeviceRestore.
            //!
            virtual bool changeDevice( const DeviceSettings & ds,
                                        bool forceDeviceRecreation = false ) = 0;

            //!
            //! 获得当前的设备选项
            //!
            virtual const DeviceSettings & getDeviceSettings() const = 0;

            //@}

            // ****************************************************************************
            //
            //! \name Display Manager
            //! \note
            //!     We provide two functions to get API specific rendering context:
            //!     - Sometime, you want to run some API specific codes,
            //!       for debug, test or any other purpose.
            //!     - You may use these functions to detect the current API
            //
            // ****************************************************************************

            //@{

            //!
            //! 获得当前的屏幕描述
            //!
            virtual const ScreenDesc & getScreenDesc() const = 0;

            //!
            //! For D3D, return pointer to current D3D device; for OGL, return NULL.
            //!
            virtual void * getD3DDevice() const = 0;

            //!
            //! For OGL, return current rendering context; for D3D, return NULL.
            //!
            virtual void * getOGLRC() const = 0;

            //@}

            // ****************************************************************************
            //
            //! \name Capabilities Manager
            //
            // ****************************************************************************

            //@{

        private:

            //!
            //! caps structure
            //!
            class CapsDesc
            {
                uint32_t value; //!< caps value
#if GN_DEBUG
                bool     valid; //!< caps is initialized or not.
#endif

            public:
                CapsDesc()
#if GN_DEBUG
                    : valid(false)
#endif
                {}

                //!
                //! get caps value
                //!
                uint32_t get() const
                {
                    GN_ASSERT( valid );
                    return value;
                }

                //!
                //! set caps value
                //!
                void set( uint32_t value );

                //!
                //! reset caps value (to invalid state)
                //!
                void reset();
            };

            CapsDesc mCaps[NUM_CAPS];

        protected:

            //!
            //! update caps. called by child class.
            //!
            void setCaps( Caps, uint32_t );

            //!
            //! reset(clear) all caps. called by child class.
            //!
            void resetAllCaps();

        public:

            //!
            //! Get render device caps
            //!
            uint32_t getCaps( Caps c ) const
            {
                GN_ASSERT( 0 <= c && c < NUM_CAPS );
                return mCaps[c].get();
            }

            //@}

            // ****************************************************************************
            //
            //! \name Shader Manager
            //
            // ****************************************************************************

            //@{

            //!
            //! Does specific shading language supported by hardware?
            //!
            virtual bool supportShader( ShaderUsage, ShadingLanguage ) = 0;

            //!
            //! request a instance of vertex shader
            //!
            virtual AutoRef<Shader>
            createVertexShader( ShadingLanguage slang, const StrA  & code ) = 0;

            //!
            //! request a instance of pixel shader
            //!
            virtual AutoRef<Shader>
            createPixelShader( ShadingLanguage slang, const StrA  & code ) = 0;

            //!
            //! request a instance of shader
            //!
            //! \param usage       shader usage
            //! \param slang       shading language
            //! \param code        shader code string. (ignored for FFP shaders)
            //! \return            instance of shader
            //! \sa                create_vshader()
            //! \sa                create_pshader()
            //! \note              This is inline function implemented in rshader.h
            //!
            AutoRef<Shader>
            createShader( ShaderUsage     usage,
                          ShadingLanguage slang,
                          const StrA &    code );

            //@}

        // ****************************************************************************
        //
        //! \name Render State Block Manager
        //
        // ****************************************************************************

            //@{

            //!
            //! request a rsblock object with specific rsblock structure.
            //! 对于相同的参数，该函数会返回同一个rsblock的实例
            //!
            virtual AutoRef<RenderStateBlock>
            createRenderStateBlock( const RenderStateBlockDesc & ) = 0;

            //!
            //! Set per-stage constant color used by fixed pixel pipeline.
            //! Default value is (1,1,1,1).
            //!
            //! \note
            //!     If current device does not support per-stage constant,
            //!     this function will be ignored.
            //!
            virtual void
            setPerStageConstant( uint32_t stage, const Vector4f & ) = 0;

            //@}

        // ****************************************************************************
        //
        //! \name Texture Manager
        //
        // ****************************************************************************

            //@{

            //!
            //! Create new texture.
            //!
            //! \param textype     texture type
            //! \param sx, sy, sz  texture size
            //! \param miplevels   how many mipmap levels?
            //!                    "0" means generate full mipmap levels down to 1x1
            //! \param format      texture format, FMT_DEFAULT means
            //!                    using default/appropriating format of current
            //!                    rendering hardware.
            //! \param usages      texture usages, one or combination of TexUsage
            //! \note
            //!    - sy/sz will be ignored for 1D/Cube texture,
            //!    - sz will be ignored for 2D texture.
            //!
            virtual AutoRef<Texture>
            createTexture( TexType textype,
                           uint32_t sx, uint32_t sy, uint32_t sz,
                           uint32_t miplevels = 0,
                           ClrFmt format = FMT_DEFAULT,
                           uint32_t usages = USAGE_NORMAL ) = 0;

            //!
            //! Load texture from file
            //!
            virtual AutoRef<Texture>
            createTextureFromFile( File & file ) = 0;

            //!
            //! bind textures ( from stage[start] to stage[start+numtex-1] )
            //!
            //! \param texlist texture list
            //! \param start   start stage
            //! \param numtex  number of textures
            //!
            virtual void
            bindTextures( const Texture * const texlist[],
                          uint32_t start, uint32_t numtex ) = 0;

            //@}

        // ****************************************************************************
        //
        //! \name Renderable Buffer Manager
        //
        // ****************************************************************************

            //@{

            //!
            //! Create new vertex buffer
            //!
            //! \param format
            //!     vertex format
            //! \param numVtx
            //!     max number of vertices this buffer could hold
            //! \param usage
            //!     buffer type
            //! \param sysCopy
            //!     has system copy or not
            //!
            virtual AutoRef<VtxBuf>
            createVtxBuf( const VtxFmtDesc & format,
                          size_t             numVtx,
                          ResourceUsage      usage,
                          bool               sysCopy ) = 0;

            //!
            //! Create new index buffer
            //!
            //! \param numIdx
            //!     number of indices
            //! \param usage
            //!     buffer type ( see ResourceUsage )
            //! \param sysCopy
            //!     has system copy or not
            //!
            //! \note
            //!     每个索引固定占用16bit
            //!
            virtual AutoRef<IdxBuf>
            createIdxBuf( size_t        numIdx,
                          ResourceUsage usage,
                          bool          sysCopy ) = 0;

            //@}

        /* ****************************************************************************
        //
        //! \name Light Manager
        //
        // ****************************************************************************

            //@{

            enum { MAX_LIGHTS = 8 };

            //!
            //! 光源类型
            //!
            enum LightType
            {
                POINT_LIGHT,       //!< 点光源
                SPOT_LIGHT,        //!< 聚光灯
                DIRECTIONAL_LIGHT, //!< 投射光（平行光）
                NUM_LIGHTTYPES,
            };

            //!
            //! 光源属性
            //!
            struct light_s
            {
                LightType  type;          //!< 见lighttype_t
                Vector3  position;      //!< ignored with DIRECTIONAL_LIGHT
                Vector3f  direction;     //!< ignored with POINT_LIGHT
                Vector4f diffuse;       //!< Diffuse [0-1]
                Vector4f specular;      //!< Specular [0-1]
                Vector4f ambient;       //!< Ambient [0-1]
                float cutoff;        //!< same as GL_SPOT_CUTOFF [0-90]
                float attenuation_c; //!< 常衰减系数, default is 1
                float attenuation_l; //!< 线性衰减系数, default is 0
                float attenuation_q; //!< 二次衰减系数, default is 0

                //!
                //! build default light structure
                //!
                static light_s build_default_light()
                {
                    light_s l;
                    l.type             = POINT_LIGHT;
                    l.position         = Vector3f(0,0,0);
                    l.direction        = Vector3f(0,0,1);
                    l.diffuse          = Vector4f(   1,   1,   1, 1 );
                    l.ambient          = Vector4f( .2f, .2f, .2f, 1 );
                    l.specular         = Vector4f(   0,   0,   0, 1 );
                    l.cutoff           = 45.0f;
                    l.attenuation_c    = 1.0f;
                    l.attenuation_l    = 0.0f;
                    l.attenuation_q    = 0.0f;
                    return l;
                }
            };

            //!
            //! 材质属性
            //!
            struct material_s
            {
                Vector4f diffuse;   //!< diffuse color, default is (1,1,1,1)
                Vector4f ambient;   //!< ambient color, default is (0.2,0.2,0.2,1)
                Vector4f specular;  //!< specular color, default is (0,0,0,1)
                Vector4f emission;  //!< emission color, default is (0,0,0,1)
                uint32_t         shininess; //!< shininess factor, default is 0
            };

            //!
            //! Enable/disable light
            //!
            void enable_light( size_t, bool );

            //!
            //! Get light enable status
            //!
            bool is_light_enabled( size_t ) const;

            //!
            //! 获取光源属性, except enable bit
            //!
            //! \param index  light index [0, MAX_LIGHTS-1]
            //! \return       light property
            //!
            const light_s & get_light( size_t index ) const;

            // 设置光源属性, except enable bit.
            // Light index must be in range of [0, MAX_LIGHTS-1]
            void set_light( size_t index, const light_s & value );
            void set_light_type( size_t, LightType );
            void set_light_position( size_t, const Vector3f & );
            void set_light_direction( size_t, const Vector3f & );
            void set_light_diffuse( size_t, const Vector4f & );
            void set_light_ambient( size_t, const Vector4f & );
            void set_light_specular( size_t, const Vector4f & );
            void set_light_cutoff( size_t, float );
            void set_light_attenuation( size_t, float, float, float );

            //!
            //! 获取当前材质属性
            //!
            const material_s & get_material() const { return m_material; }

            // 设置材质属性
            void set_material( const material_s & );
            void set_diffuse( const Vector4f & );
            void set_ambient( const Vector4f & );
            void set_specular( const Vector4f & );
            void set_emission( const Vector4f & );
            void set_shininess( uint32_t );

        private :

            struct mylight_s
            {
                light_s light;
                bool    enable;
            };

            mylight_s  m_lights[MAX_LIGHTS];
            material_s m_material;

        private :

            virtual void dev_enable_light( size_t, bool ) = 0;
            virtual void dev_set_light( size_t, const light_s & ) = 0;
            virtual void dev_set_material( const material_s & ) = 0;

            void light_ctor()
            {
                // setup default lights
                light_s l = light_s::build_default_light();
                for ( size_t i = 0; i < MAX_LIGHTS; ++i)
                {
                    m_lights[i].light  = l;
                    m_lights[i].enable = (0==i);
                }

                m_material.diffuse   = Vector4f(1,1,1,1);
                m_material.ambient   = Vector4f(.2f,.2f,.2f,1);
                m_material.specular  = Vector4f(0,0,0,1);
                m_material.emission  = Vector4f(0,0,0,1);
                m_material.shininess = 0;
            }

            //@}

        // ****************************************************************************
        //
        //! \name Transform Manager
        //! \note viewport以当前render target的大小为单位尺寸，
        //!       左上角为(0,0)，右下角为(1,1)。
        //
        // ****************************************************************************

            //@{
        public :

            // matrix manipulation
            const matrix44f_c & get_proj() const { return m_proj_stack.top(); }
            void set_proj( const matrix44f_c & );
            void mul_proj( const matrix44f_c & );
            void push_proj();
            void pop_proj();

            const matrix44f_c & get_view() const { return m_view_stack.top();  }
            virtual void set_view( const matrix44f_c & );
            virtual void mul_view( const matrix44f_c & );
            virtual void push_view();
            virtual void pop_view();

            const matrix44f_c & get_world() const { return m_world_stack.top(); }
            virtual void set_world( const matrix44f_c & );
            virtual void mul_world( const matrix44f_c & );
            virtual void push_world();
            virtual void pop_world();

            //!
            //! viewport structure
            //!
            struct viewport_s
            {
                //!
                //! size of viewport (left, right, width, height)
                //!
                float l, t, w, h;

                //!
                //! set viewport size
                //!
                void set( float l_, float t_, float w_, float h_ )
                { l = l_; t = t_; w = w_; h = h_; }
            };

            //viewport manipulation
            const viewport_s & get_viewport() const;
            void get_viewport( float & left, float & top,
                               float & width, float & height ) const;
            void set_viewport( const viewport_s & );
            void set_viewport( float left, float top, float width, float height );
            void push_viewport();
            void pop_viewport();

            // 因为不同的API计算投影变换矩阵的方法不同，所以我们特别设置了这几个函数。

            virtual matrix44f_c &
            compute_perspective_matrix(
                matrix44f_c & result,
                float fovy,  // Field of view, in radians, in y-direction.
                float ratio, // width / height
                float znear,
                float zfar) const = 0;
            virtual matrix44f_c &
            compute_ortho_matrix(
                matrix44f_c & result,
                float left,
                float bottom,
                float width,
                float height,
                float znear = -1.0f,
                float zfar  =  1.0f ) const = 0;

            // private variables
        private :

            //!
            //! Fixed-size stack container that do not perform any runtime
            //! memory allocation/deallocation.
            //!
            template< class T, size_t MAX_DEPTH = 256 >
            class fixedstack_tc
            {
                T      m_top;              //!< top element
                T      m_stack[MAX_DEPTH]; //!< element stack
                size_t m_depth;            //!< current depth
            public :

                //!
                //! default constructor
                //!
                fixedstack_tc() : m_depth(0) {}

                //!
                //! push the top element into stack
                //!
                void push()
                {
                    GN_ASSERT( m_depth < MAX_DEPTH );
                    m_stack[m_depth] = m_top;
                    ++m_depth;
                }

                //!
                //! pop out the stack to top element
                //!
                T & pop()
                {
                    GN_ASSERT( m_depth > 0 );
                    m_top = m_stack[--m_depth];
                    return m_top;
                }

                //!
                //! return top element
                //!
                T       & top()       { return m_top; }

                //!
                //! return constant top element
                //!
                const T & top() const { return m_top; }
            };

            fixedstack_tc<matrix44f_c>  m_proj_stack;
            fixedstack_tc<matrix44f_c>  m_view_stack;
            fixedstack_tc<matrix44f_c>  m_world_stack;
            fixedstack_tc<viewport_s>       m_viewport_stack;

            // private functions
        private:

            virtual void dev_set_proj( const matrix44f_c & ) = 0;
            virtual void dev_set_view( const matrix44f_c & ) = 0;
            virtual void dev_set_world( const matrix44f_c & ) = 0;
            virtual void dev_set_viewport( const viewport_s & ) = 0;

            void transform_ctor()
            {
                m_proj_stack.top().identify();
                m_view_stack.top().identify();
                m_world_stack.top().identify();
                m_viewport_stack.top().set(0,0,1,1);
            }

            //@}*/

            // ****************************************************************************
            //
            //! \name Drawing Manager
            //
            // ****************************************************************************

            //@{
        public :

            //!
            //! set render target texture
            //!
            //! \param index
            //!     render target index, starting from 0
            //! \param texture
            //!     target texture, must be created with flag TEXUSAGE_RENDERTARGET. Set
            //!     this parameter to NULL will reset to default target (back buffer
            //!     for RT0 and null for others.
            //! \param face
            //!     Ignored if target_texture is not cubemap.
            //!
            virtual void setRenderTarget( size_t index,
                                          const Texture * texture,
                                          TexFace face = TEXFACE_PX ) = 0;

            //!
            //! set render target texture
            //!
            //! \param texture
            //!     Target texture, must be created with flag TEXUSAGE_DEPTH. Set this
            //!     parameter to NULL will reset to default depth buffer.
            //! \param face
            //!     Ignored if target_texture is not cubemap.
            //!
            virtual void setRenderDepth( const Texture * texture,
                                         TexFace face = TEXFACE_PX ) = 0;

            //!
            //! user defined clip plane
            //!
            //! \param index
            //!     Clip plane index, must be in range of [0,caps_max_clip_planes())
            //! \param plane
            //!     Clip plane parameters. Set to NULL to disable it.
            //!
            virtual void setClipPlane( size_t index, const Plane3f * plane ) = 0;

            //!
            //! 开始绘图操作.
            //!
            //! 所有的绘图操作都必须在 draw_begin() 和 draw_end() 之间调用
            //!
            virtual bool drawBegin() = 0;

            //!
            //! 结束绘图操作. Similar as EndScene() followed by Present()
            //!
            //! call draw_end() <b>if and only if</b> draw_begin() returns true.
            //!
            virtual void drawEnd() = 0;

            //!
            //! 本函数的功能类似于OpenGL中的glFinish()
            //!
            //! \note 必须在 draw_begin() 和 draw_end() 之间调用
            //!
            virtual void drawFinish() = 0;

            //!
            //! 清屏操作
            //!
            //! \param flags 清屏标志, see ClearFlag
            //! \param c     背景色
            //! \param z     深度值
            //! \param s     模板值
            //!
            //! \note Can be called outside of draw_begin()/draw_end() scope.
            //!
            virtual void
            clearScreen( const Vector4f & c = Vector4f(0,0,0,1),
                         float z = 1.0f, uint32_t s = 0,
                         uint32_t flags = C_BUFFER | Z_BUFFER ) = 0;

            //!
            //! Draw indexed primitives.
            //!
            //! \param prim
            //!     primititive type
            //! \param numPrim
            //!     number of primitives
            //! \param baseVtx
            //!     vertex index into vertex buffer that index "0" will be refering to.
            //! \param minVtxIdx, numVtx
            //!     define effective range in vertex buffer, starting from baseVtx.
            //! \param startIdx
            //!     index into index buffer of the first index
            //!
            //! \note 必须在 draw_begin() 和 draw_end() 之间调用
            //!
            virtual void drawIndexed( PrimitiveType prim,
                                      size_t        numPrim,
                                      size_t        baseVtx,
                                      size_t        minVtxIdx,
                                      size_t        numVtx,
                                      size_t        startIdx ) = 0;

            //!
            //! Draw non-indexed primitives.
            //!
            //! \param prim
            //!     primititive type
            //! \param numPrim
            //!     number of primitives
            //! \param startVtx
            //!     index into vertex buffer of the first vertex.
            //!
            //! \note 必须在 draw_begin() 和 draw_end() 之间调用
            //!
            virtual void draw( PrimitiveType prim,
                               size_t        numPrim,
                               size_t        startVtx ) = 0;

            //!
            //! 在屏幕上指定的位置绘制2D字符串.
            //!
            //! - 屏幕左上角为坐标原点，X轴向左、Y轴向下。
            //!   被绘制的字符串的坐标位置以第一个字符的左上角为准。
            //! - 函数可以处理中英文的混合字符串，但运行速度较慢，主要为测试而用。
            //!   文字的高度固定为16个象素，宽度为8(ASCII)/16(unicode)个象素。
            //! - 必须在 draw_begin() 和 draw_end() 之间调用
            //!
            //! \param text  待绘制度字符串
            //! \param x, y  第一个字符左上角的屏幕坐标
            //! \param color 文字颜色
            //!
            virtual void
            drawTextA( const char * text, int x, int y,
                       const Vector4f & color = Vector4f(1,1,1,1) ) = 0;

            //!
            //!  绘制unicode文字
            //!
            //! \sa draw_texta()
            //!
            //! \note 必须在 draw_begin() 和 draw_end() 之间调用
            //!
            virtual void
            drawTextW( const wchar_t * text, int x, int y,
                       const Vector4f & color = Vector4f(1,1,1,1) ) = 0;

            //!
            //! 返回上一次 drawEnd() 到现在所绘制的原语的个数
            //!
            virtual size_t getNumPrimitives() const = 0;

            //!
            //! 返回上一次 drawEnd() 到现在 draw() / drawindexed() 的次数
            //!
            virtual size_t getNumDraws() const = 0;

            //!
            //! 返回上一次 drawEnd() 到现在平均每次 draw()/drawIndexed() 的原语数
            //!
            size_t getNumPrimsPerDraw() const
            {
                size_t np = getNumPrimitives();
                size_t nd = getNumDraws();
                return 0 == nd ? 0 : np / nd;
            }

            //@}
        };
    }
}

#include "gfxRenderer.inl"

// *****************************************************************************
//                           End of gfxRenderer.h
// *****************************************************************************
#endif // __GN_GFX_GFXRENDERER_H__
