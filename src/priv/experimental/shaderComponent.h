#include <garnet/GNgfx.h>

namespace GN { namespace gfx
{
    struct ShaderParameter
    {
        enum Type
        {
            TEXTURE,
            NUMBER,
            ARRAY,
        };

        enum Format
        {
            BOOL1,
            BOOL2,
            BOOL3,
            BOOL4,
            INT1,
            INT2,
            INT3,
            INT4,
            FLOAT1,
            FLOAT2,
            FLOAT3,
            FLOAT4,
            MAT4X4,
        };

        enum Direction
        {
            REQUIRED_INPUT,
            OPTIONAL_INPUT,
            OUTPUT,
        };

        Type      type;
        Format    format;
        StrA      name;
        Direction direction;
        StrA      description;
    };

    struct ShaderComponent
    {
        DynaArray<ShaderParameter> parameters;
        GpuProgramLanguage         lang;
        StrA                       code;
        StrA                       description;
    };

    ShaderComponent DIFFUSE_VS(
        // parameters
        {
            { ShaderParameter::NUMBER, ShaderParameter::MAX4X4, "pvw",          ShaderParameter::REQUIRED_INPUT },
            { ShaderParameter::NUMBER, ShaderParameter::MAX4X4, "world",        ShaderParameter::REQUIRED_INPUT },
            { ShaderParameter::NUMBER, ShaderParameter::MAX4X4, "wit",          ShaderParameter::REQUIRED_INPUT }, // inverse transpose of world.
            { ShaderParameter::NUMBER, ShaderParameter::FLOAT3, "position",     ShaderParameter::REQUIRED_INPUT },
            { ShaderParameter::NUMBER, ShaderParameter::FLOAT3, "normal",       ShaderParameter::OPTIONAL_INPUT },

            { ShaderParameter::NUMBER, ShaderParameter::FLOAT3, "hpos",         ShaderParameter::OUTPUT },
            { ShaderParameter::NUMBER, ShaderParameter::FLOAT3, "pos_world",    ShaderParameter::OUTPUT },
            { ShaderParameter::NUMBER, ShaderParameter::FLOAT3, "normal_world", ShaderParameter::OUTPUT },
        },
        // lang
        GpuProgramLanguage::HLSL9,
        // code
        "...",
        // description
        "Per-pixel diffuse lighting vertex shader"
    );

    ShaderComponent SKINNED_VS(
        // parameters
        {
            { ShaderParameter::NUMBER, ShaderParameter::FLOAT4, "joints",           ShaderParameter::REQUIRED_INPUT },
            { ShaderParameter::NUMBER, ShaderParameter::FLOAT3, "weights",          ShaderParameter::REQUIRED_INPUT },
            { ShaderParameter::ARRAY,  ShaderParameter::MAT4X4, "matries",          ShaderParameter::REQUIRED_INPUT },
            { ShaderParameter::NUMBER, ShaderParameter::FLOAT3, "point",            ShaderParameter::OPTIONAL_INPUT },
            { ShaderParameter::NUMBER, ShaderParameter::FLOAT3, "vector",           ShaderParameter::OPTIONAL_INPUT },

            { ShaderParameter::NUMBER, ShaderParameter::FLOAT3, "deformed_point",   ShaderParameter::OUTPUT },
            { ShaderParameter::NUMBER, ShaderParameter::FLOAT3, "deformed_vector",  ShaderParameter::OUTPUT },
        },
    );
}}
