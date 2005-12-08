//
//
// --------------------------------------------------------------------------------------
GN_INLINE void
GN::gfx::D3DPxlShaderAsm::applyUniform( LPDIRECT3DDEVICE9 dev, const Uniform & u ) const
{
    GN_GUARD_SLOW;

    D3DAsmConstDesc desc;

    desc.u32 = (uint32_t)u.userData;

    switch( desc.type )
    {
        case CONST_F :
            switch( u.type )
            {
                case UVT_FLOAT:
                    if( u.valueFloat.empty() ) return;
                    // FIXME : may read memory beyond the end of array, if (array_size % 4 != 0)
                    GN_DX_CHECK( dev->SetPixelShaderConstantF( desc.index, &u.valueFloat[0], (UINT)( u.valueFloat.size() + 3 ) / 4 ) );
                    break;

                case UVT_FLOAT4:
                    if( u.valueVector4.empty() ) return;
                    GN_DX_CHECK( dev->SetPixelShaderConstantF( desc.index, u.valueVector4[0], (UINT)u.valueVector4.size() ) );
                    break;

                case UVT_MATRIX44:
                    if( u.valueMatrix44.empty() ) return;
                    GN_DX_CHECK( dev->SetPixelShaderConstantF( desc.index, u.valueMatrix44[0][0], (UINT)(u.valueMatrix44.size()*4) ) );
                    break;

                case UVT_BOOL:
                case UVT_INT:
                    GN_ERROR( "Setting integer or boolean value to float shader register does not work." );
                    break;
            }
            break;

        case CONST_I :
            switch( u.type )
            {
                case UVT_INT:
                    if( u.valueFloat.empty() ) return;
                    // FIXME : may read memory beyond the end of array, if (array_size % 4 != 0)
                    GN_DX_CHECK( dev->SetPixelShaderConstantI( desc.index, (const int*)&u.valueInt[0], (UINT)( u.valueInt.size() + 3 ) / 4 ) );
                    break;

                case UVT_FLOAT:
                case UVT_FLOAT4:
                case UVT_MATRIX44:
                case UVT_BOOL:
                    GN_ERROR( "integer register accepts only integer value." );
                    break;
            }
            break;

        case CONST_B :
            switch( u.type )
            {
                case UVT_BOOL:
                    if( u.valueFloat.empty() ) return;
                    // FIXME : may read memory beyond the end of array, if (array_size % 4 != 0)
                    GN_DX_CHECK( dev->SetPixelShaderConstantB( desc.index, (const BOOL*)&u.valueBool[0], (UINT)u.valueBool.size() ) );
                    break;

                case UVT_FLOAT:
                case UVT_FLOAT4:
                case UVT_MATRIX44:
                case UVT_INT:
                    GN_ERROR( "Bool register accepts only boolean value." );
                    break;
            }
            break;

        default:
            // Program should not reach here.
            GN_ERROR( "invalid register type!" );
            GN_UNEXPECTED();
    }

    GN_UNGUARD_SLOW;
}

