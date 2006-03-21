#ifndef __GN_GFXD3D9_D3D9SHADER_H__
#define __GN_GFXD3D9_D3D9SHADER_H__
// *****************************************************************************
//! \file    d3d9/d3d9Shader.h
//! \brief   D3D shader classes
//! \author  chenlee (2005.11.26)
// *****************************************************************************

#include "d3d9Resource.h"

namespace GN { namespace gfx
{
    //!
    //! Basic D3D9 shader class
    //!
    struct D3D9BasicShader : public Shader
    {
        //!
        //! Apply shader as well as shader constants to D3D device
        //!
        virtual void apply() const = 0;

        //!
        //! Apply only dirty uniforms to D3D device
        //!
        virtual void applyDirtyUniforms() const = 0;

    protected:

        //!
        //! protected ctor
        //!
        D3D9BasicShader( ShaderType type, ShadingLanguage lang ) : Shader(type,lang) {}
    };

    // *************************************************************************
    // ASM shaders
    // *************************************************************************

    //!
    //! Basic D3D9 asm shader class
    //!
    struct D3D9ShaderAsm : public D3D9BasicShader
    {
        enum
        {
            CONST_F = 1, //!< float const
            CONST_I = 2, //!< integer const
            CONST_B = 3, //!< bool const
        };

        //!
        //! Asm shader constant descriptor
        //!
        union D3DAsmConstDesc
        {
            //!
            //! shader constant descriptor as uint32
            //!
            uint32_t u32;

            struct
            {
                uint16_t type;  //!< should be one of CONST_F, CONST_I, CONST_B
                uint16_t index; //!< const index.
            };
        };

    protected:

        //!
        //! protected ctor
        //!
        D3D9ShaderAsm( ShaderType type) : D3D9BasicShader(type,LANG_D3D_ASM) {}
    };

    //!
    //! D3D9 asm vertex shader class
    //!
    class D3D9VtxShaderAsm : public D3D9ShaderAsm, public D3D9Resource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3D9VtxShaderAsm, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9VtxShaderAsm( D3D9Renderer & r )
            : D3D9ShaderAsm(VERTEX_SHADER)
            , D3D9Resource(r)
        { clear(); }
        virtual ~D3D9VtxShaderAsm() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const StrA & code );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() { mCode.clear(); mD3DShader = 0; }
        //@}

        // ********************************
        // from D3D9Resource
        // ********************************
    public:

        bool deviceCreate();
        bool deviceRestore() { return true; }
        void deviceDispose() {}
        void deviceDestroy();

        // ********************************
        // from D3D9BasicShader
        // ********************************
    public:

        void apply() const;
        void applyDirtyUniforms() const;

        // ********************************
        // from Shader
        // ********************************
    private:

        bool queryDeviceUniform( const char * name, HandleType & userData ) const;

        // ********************************
        // private variables
        // ********************************
    private:
        StrA                    mCode;
        LPDIRECT3DVERTEXSHADER9 mD3DShader;

        size_t mMaxConstF, mMaxConstI, mMaxConstB;

        // ********************************
        // private functions
        // ********************************
    private:
        bool analyzeUniforms( const DWORD * shaderFunction );
        GN_INLINE void applyUniform( LPDIRECT3DDEVICE9, const Uniform & ) const;
    };

    //!
    //! D3D9 asm pixel shader class
    //!
    class D3D9PxlShaderAsm : public D3D9ShaderAsm, public D3D9Resource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3D9PxlShaderAsm, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9PxlShaderAsm( D3D9Renderer & r )
            : D3D9ShaderAsm(PIXEL_SHADER)
            , D3D9Resource(r)
        { clear(); }
        virtual ~D3D9PxlShaderAsm() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const StrA & code );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() { mCode.clear(); mD3DShader = 0; }
        //@}

        // ********************************
        // from D3D9Resource
        // ********************************
    public:

        bool deviceCreate();
        bool deviceRestore() { return true; }
        void deviceDispose() {}
        void deviceDestroy();

        // ********************************
        // from D3D9BasicShader
        // ********************************
    public:

        void apply() const;
        void applyDirtyUniforms() const;

        // ********************************
        // from Shader
        // ********************************
    private:

        bool queryDeviceUniform( const char * name, HandleType & userData ) const;

        // ********************************
        // private variables
        // ********************************
    private:
        StrA                   mCode;
        LPDIRECT3DPIXELSHADER9 mD3DShader;

        size_t mMaxConstF, mMaxConstI, mMaxConstB;

        // ********************************
        // private functions
        // ********************************
    private:
        bool analyzeUniforms( const DWORD * );
        GN_INLINE void applyUniform( LPDIRECT3DDEVICE9, const Uniform & ) const;
    };

    // *************************************************************************
    // HLSL shaders
    // *************************************************************************

    //!
    //! Basic D3D9 HLSL shader class
    //!
    struct D3D9ShaderHlsl : public D3D9BasicShader
    {

    protected:

        //!
        //! protected ctor
        //!
        D3D9ShaderHlsl( ShaderType type) : D3D9BasicShader(type,LANG_D3D_ASM) {}

        //!
        //! apply uniform to D3D device
        //!
        void applyUniform( LPDIRECT3DDEVICE9 dev, LPD3DXCONSTANTTABLE table, const Uniform & u ) const
        {
            GN_GUARD_SLOW;

            GN_ASSERT( dev && table );

            D3DXHANDLE h = (D3DXHANDLE)u.userData;
            GN_ASSERT( h );

            switch( u.value.type )
            {
                case UVT_BOOL:
                    if( !u.value.bools.empty() )
                        GN_DX_CHECK( table->SetBoolArray( dev, h, (const BOOL*)&u.value.bools[0], (UINT)u.value.bools.size() ) );
                    break;

                case UVT_INT:
                    if( !u.value.ints.empty() )
                        GN_DX_CHECK( table->SetIntArray( dev, h, (const INT*)&u.value.ints[0], (UINT)u.value.ints.size() ) );
                    break;

                case UVT_FLOAT:
                    if( !u.value.floats.empty() )
                        GN_DX_CHECK( table->SetFloatArray( dev, h, &u.value.floats[0], (UINT)u.value.floats.size() ) );
                    break;

                case UVT_VECTOR4:
                    if( !u.value.vector4s.empty() )
                        GN_DX_CHECK( table->SetVectorArray( dev, h, (const D3DXVECTOR4*)&u.value.vector4s[0], (UINT)u.value.vector4s.size() ) );
                    break;

                case UVT_MATRIX44:
                    if( !u.value.matrix44s.empty() )
                        GN_DX_CHECK( table->SetMatrixTransposeArray( dev, h, (const D3DXMATRIX*)&u.value.matrix44s[0], (UINT)u.value.matrix44s.size() ) );
                    break;

                default:
                    // Program should not reach here.
                    GN_ERROR( "invalid uniform type!" );
                    GN_UNEXPECTED();
            }
            GN_UNGUARD_SLOW;
        }
    };

    //!
    //! D3D9 HLSL vertex shader class
    //!
    class D3D9VtxShaderHlsl : public D3D9ShaderHlsl, public D3D9Resource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3D9VtxShaderHlsl, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9VtxShaderHlsl( D3D9Renderer & r )
            : D3D9ShaderHlsl(VERTEX_SHADER)
            , D3D9Resource(r)
        { clear(); }
        virtual ~D3D9VtxShaderHlsl() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const StrA & code, const StrA & entry );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() { mCode.clear(); mConstTable = 0; mD3DShader = 0; }
        //@}

        // ********************************
        // from D3D9Resource
        // ********************************
    public:
        bool deviceCreate();
        bool deviceRestore() { return true; }
        void deviceDispose() {}
        void deviceDestroy();

        // ********************************
        // from D3D9BasicShader
        // ********************************
    public:

        void apply() const;
        void applyDirtyUniforms() const;

        // ********************************
        // from Shader
        // ********************************
    private:

        bool queryDeviceUniform( const char * name, HandleType & userData ) const;

        // ********************************
        // private variables
        // ********************************
    private:
        StrA                    mCode;
        StrA                    mEntry;
        LPD3DXCONSTANTTABLE     mConstTable;
        LPDIRECT3DVERTEXSHADER9 mD3DShader;

        // ********************************
        // private functions
        // ********************************
    private:
    };

    //!
    //! D3D9 HLSL pixel shader class
    //!
    class D3D9PxlShaderHlsl : public D3D9ShaderHlsl, public D3D9Resource, public StdClass
    {
         GN_DECLARE_STDCLASS( D3D9PxlShaderHlsl, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9PxlShaderHlsl( D3D9Renderer & r )
            : D3D9ShaderHlsl(PIXEL_SHADER)
            , D3D9Resource(r)
        { clear(); }
        virtual ~D3D9PxlShaderHlsl() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const StrA & code, const StrA & entry );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() { mCode.clear(); mConstTable = 0; mD3DShader = 0; }
        //@}

        // ********************************
        // from D3D9Resource
        // ********************************
    public:
        bool deviceCreate();
        bool deviceRestore() { return true; }
        void deviceDispose() {}
        void deviceDestroy();

        // ********************************
        // from D3D9BasicShader
        // ********************************
    public:

        void apply() const;
        void applyDirtyUniforms() const;

        // ********************************
        // from Shader
        // ********************************
    private:

        bool queryDeviceUniform( const char * name, HandleType & userData ) const;

        // ********************************
        // private variables
        // ********************************
    private:
        StrA                   mCode;
        StrA                   mEntry;
        LPD3DXCONSTANTTABLE    mConstTable;
        LPDIRECT3DPIXELSHADER9 mD3DShader;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of d3d9Shader.h
// *****************************************************************************
#endif // __GN_GFXD3D9_D3D9SHADER_H__
