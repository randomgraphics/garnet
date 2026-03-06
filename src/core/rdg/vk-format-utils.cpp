#include "pch.h"
#include "vk-format-utils.h"
#include <unordered_map>

namespace GN::rdg {

namespace {

using PF = gfx::img::PixelFormat;

#define MAKE_PF1(layout, sign, swizzle) PF::make(PF::LAYOUT_##layout, PF::SIGN_##sign, PF::SWIZZLE_##swizzle)

// Index = vk::Format value; value = corresponding PixelFormat. Order matches vk::Format enum.
constexpr gfx::img::PixelFormat VK_PF_MAPPING[] = {
    PF::UNKNOWN(),                    // 0   eUndefined
    MAKE_PF1(4_4, UNORM, XY01),       // 1   eR4G4UnormPack8
    PF::RGBA_4_4_4_4_UNORM(),         // 2   eR4G4B4A4UnormPack16
    PF::BGRA_4_4_4_4_UNORM(),         // 3   eB4G4R4A4UnormPack16
    PF::RGB_5_6_5_UNORM(),            // 4   eR5G6B5UnormPack16
    PF::BGR_5_6_5_UNORM(),            // 5   eB5G6R5UnormPack16
    PF::RGBA_5_5_5_1_UNORM(),         // 6   eR5G5B5A1UnormPack16
    PF::BGRA_5_5_5_1_UNORM(),         // 7   eB5G5R5A1UnormPack16
    PF::BGRA_5_5_5_1_UNORM(),         // 8   eA1R5G5B5UnormPack16
    PF::R_8_UNORM(),                  // 9   eR8Unorm
    PF::R_8_SNORM(),                  // 10  eR8Snorm
    PF::UNKNOWN(),                    // 11  eR8Uscaled
    PF::UNKNOWN(),                    // 12  eR8Sscaled
    PF::R_8_UINT(),                   // 13  eR8Uint
    PF::R_8_SINT(),                   // 14  eR8Sint
    MAKE_PF1(8, SRGB, X001),          // 15  eR8Srgb
    PF::RG_8_8_UNORM(),               // 16  eR8G8Unorm
    PF::RG_8_8_SNORM(),               // 17  eR8G8Snorm
    PF::UNKNOWN(),                    // 18  eR8G8Uscaled
    PF::UNKNOWN(),                    // 19  eR8G8Sscaled
    PF::RG_8_8_UINT(),                // 20  eR8G8Uint
    PF::RG_8_8_SINT(),                // 21  eR8G8Sint
    PF::UNKNOWN(),                    // 22  eR8G8Srgb
    PF::RGB_8_8_8_UNORM(),            // 23  eR8G8B8Unorm
    PF::RGB_8_8_8_SNORM(),            // 24  eR8G8B8Snorm
    PF::UNKNOWN(),                    // 25  eR8G8B8Uscaled
    PF::UNKNOWN(),                    // 26  eR8G8B8Sscaled
    PF::RGB_8_8_8_UINT(),             // 27  eR8G8B8Uint
    PF::RGB_8_8_8_SINT(),             // 28  eR8G8B8Sint
    PF::RGB_8_8_8_SRGB(),             // 29  eR8G8B8Srgb
    PF::BGR_8_8_8_UNORM(),            // 30  eB8G8R8Unorm
    PF::BGR_8_8_8_SNORM(),            // 31  eB8G8R8Snorm
    PF::UNKNOWN(),                    // 32  eB8G8R8Uscaled
    PF::UNKNOWN(),                    // 33  eB8G8R8Sscaled
    PF::BGR_8_8_8_UINT(),             // 34  eB8G8R8Uint
    PF::BGR_8_8_8_SINT(),             // 35  eB8G8R8Sint
    PF::BGR_8_8_8_SRGB(),             // 36  eB8G8R8Srgb
    PF::RGBA_8_8_8_8_UNORM(),         // 37  eR8G8B8A8Unorm
    PF::RGBA_8_8_8_8_SNORM(),         // 38  eR8G8B8A8Snorm
    PF::UNKNOWN(),                    // 39  eR8G8B8A8Uscaled
    PF::UNKNOWN(),                    // 40  eR8G8B8A8Sscaled
    PF::RGBA_8_8_8_8_UINT(),          // 41  eR8G8B8A8Uint
    PF::RGBA_8_8_8_8_SINT(),          // 42  eR8G8B8A8Sint
    PF::RGBA_8_8_8_8_SRGB(),          // 43  eR8G8B8A8Srgb
    PF::BGRA_8_8_8_8_UNORM(),         // 44  eB8G8R8A8Unorm
    PF::UNKNOWN(),                    // 45  eB8G8R8A8Snorm (no BGRA SNORM in rapid-image)
    PF::UNKNOWN(),                    // 46  eB8G8R8A8Uscaled
    PF::UNKNOWN(),                    // 47  eB8G8R8A8Sscaled
    PF::BGRA_8_8_8_8_UINT(),          // 48  eB8G8R8A8Uint
    PF::UNKNOWN(),                    // 49  eB8G8R8A8Sint (no BGRA SINT in rapid-image)
    PF::UNKNOWN(),                    // 50  eB8G8R8A8Srgb (no BGRA SRGB in rapid-image)
    PF::UNKNOWN(),                    // 51  eA8B8G8R8UnormPack32
    PF::UNKNOWN(),                    // 52  eA8B8G8R8SnormPack32
    PF::UNKNOWN(),                    // 53  eA8B8G8R8UscaledPack32
    PF::UNKNOWN(),                    // 54  eA8B8G8R8SscaledPack32
    PF::UNKNOWN(),                    // 55  eA8B8G8R8UintPack32
    PF::UNKNOWN(),                    // 56  eA8B8G8R8SintPack32
    PF::UNKNOWN(),                    // 57  eA8B8G8R8SrgbPack32
    PF::BGRA_10_10_10_2_UNORM(),      // 58  eA2R10G10B10UnormPack32
    PF::UNKNOWN(),                    // 59  eA2R10G10B10SnormPack32
    PF::UNKNOWN(),                    // 60  eA2R10G10B10UscaledPack32
    PF::UNKNOWN(),                    // 61  eA2R10G10B10SscaledPack32
    PF::RGBA_10_10_10_2_UINT(),       // 62  eA2R10G10B10UintPack32
    PF::RGBA_10_10_10_2_UINT(),       // 63  eA2R10G10B10SintPack32
    PF::RGBA_10_10_10_2_UNORM(),      // 64  eA2B10G10R10UnormPack32
    PF::UNKNOWN(),                    // 65  eA2B10G10R10SnormPack32
    PF::UNKNOWN(),                    // 66  eA2B10G10R10UscaledPack32
    PF::UNKNOWN(),                    // 67  eA2B10G10R10SscaledPack32
    PF::RGBA_10_10_10_2_UINT(),       // 68  eA2B10G10R10UintPack32
    PF::RGBA_10_10_10_2_UINT(),       // 69  eA2B10G10R10SintPack32
    PF::R_16_UNORM(),                 // 70  eR16Unorm
    PF::R_16_SNORM(),                 // 71  eR16Snorm
    PF::UNKNOWN(),                    // 72  eR16Uscaled
    PF::UNKNOWN(),                    // 73  eR16Sscaled
    PF::R_16_UINT(),                  // 74  eR16Uint
    PF::R_16_SINT(),                  // 75  eR16Sint
    PF::R_16_FLOAT(),                 // 76  eR16Sfloat
    PF::RG_16_16_UNORM(),             // 77  eR16G16Unorm
    PF::RG_16_16_SNORM(),             // 78  eR16G16Snorm
    PF::UNKNOWN(),                    // 79  eR16G16Uscaled
    PF::UNKNOWN(),                    // 80  eR16G16Sscaled
    PF::RG_16_16_UINT(),              // 81  eR16G16Uint
    PF::RG_16_16_SINT(),              // 82  eR16G16Sint
    PF::RG_16_16_FLOAT(),             // 83  eR16G16Sfloat
    PF::RGB_16_16_16_UNORM(),         // 84  eR16G16B16Unorm
    PF::RGB_16_16_16_SNORM(),         // 85  eR16G16B16Snorm
    PF::UNKNOWN(),                    // 86  eR16G16B16Uscaled
    PF::UNKNOWN(),                    // 87  eR16G16B16Sscaled
    PF::RGB_16_16_16_UINT(),          // 88  eR16G16B16Uint
    PF::RGB_16_16_16_SINT(),          // 89  eR16G16B16Sint
    PF::RGB_16_16_16_FLOAT(),         // 90  eR16G16B16Sfloat
    PF::RGBA_16_16_16_16_UNORM(),     // 91  eR16G16B16A16Unorm
    PF::RGBA_16_16_16_16_SNORM(),     // 92  eR16G16B16A16Snorm
    PF::UNKNOWN(),                    // 93  eR16G16B16A16Uscaled
    PF::UNKNOWN(),                    // 94  eR16G16B16A16Sscaled
    PF::RGBA_16_16_16_16_UINT(),      // 95  eR16G16B16A16Uint
    PF::RGBA_16_16_16_16_SINT(),      // 96  eR16G16B16A16Sint
    PF::RGBA_16_16_16_16_FLOAT(),     // 97  eR16G16B16A16Sfloat
    PF::R_32_UINT(),                  // 98  eR32Uint
    PF::R_32_SINT(),                  // 99  eR32Sint
    PF::R_32_FLOAT(),                 // 100 eR32Sfloat
    PF::RG_32_32_UINT(),              // 101 eR32G32Uint
    PF::RG_32_32_SINT(),              // 102 eR32G32Sint
    PF::RG_32_32_FLOAT(),             // 103 eR32G32Sfloat
    PF::RGB_32_32_32_UINT(),          // 104 eR32G32B32Uint
    PF::RGB_32_32_32_SINT(),          // 105 eR32G32B32Sint
    PF::RGB_32_32_32_FLOAT(),         // 106 eR32G32B32Sfloat
    PF::RGBA_32_32_32_32_UINT(),      // 107 eR32G32B32A32Uint
    PF::RGBA_32_32_32_32_SINT(),      // 108 eR32G32B32A32Sint
    PF::RGBA_32_32_32_32_FLOAT(),     // 109 eR32G32B32A32Sfloat
    PF::UNKNOWN(),                    // 110 eR64Uint
    PF::UNKNOWN(),                    // 111 eR64Sint
    PF::UNKNOWN(),                    // 112 eR64Sfloat
    PF::UNKNOWN(),                    // 113 eR64G64Uint
    PF::UNKNOWN(),                    // 114 eR64G64Sint
    PF::UNKNOWN(),                    // 115 eR64G64Sfloat
    PF::UNKNOWN(),                    // 116 eR64G64B64Uint
    PF::UNKNOWN(),                    // 117 eR64G64B64Sint
    PF::UNKNOWN(),                    // 118 eR64G64B64Sfloat
    PF::UNKNOWN(),                    // 119 eR64G64B64A64Uint
    PF::UNKNOWN(),                    // 120 eR64G64B64A64Sint
    PF::UNKNOWN(),                    // 121 eR64G64B64A64Sfloat
    PF::RGB_11_11_10_FLOAT(),         // 122 eB10G11R11UfloatPack32
    PF::UNKNOWN(),                    // 123 eE5B9G9R9UfloatPack32
    PF::R_16_UNORM(),                 // 124 eD16Unorm
    PF::UNKNOWN(),                    // 125 eX8D24UnormPack32
    PF::R_32_FLOAT(),                 // 126 eD32Sfloat
    PF::UNKNOWN(),                    // 127 eS8Uint
    PF::R_16_UNORM(),                 // 128 eD16UnormS8Uint
    PF::RG_24_UNORM_8_UINT(),         // 129 eD24UnormS8Uint
    PF::RGX_32_FLOAT_8_UINT_24(),     // 130 eD32SfloatS8Uint
    PF::BC1_UNORM(),                  // 131 eBc1RgbUnormBlock
    PF::BC1_SRGB(),                   // 132 eBc1RgbSrgbBlock
    PF::BC1_UNORM(),                  // 133 eBc1RgbaUnormBlock
    PF::BC1_SRGB(),                   // 134 eBc1RgbaSrgbBlock
    PF::BC2_UNORM(),                  // 135 eBc2UnormBlock
    PF::BC2_SRGB(),                   // 136 eBc2SrgbBlock
    PF::BC3_UNORM(),                  // 137 eBc3UnormBlock
    PF::BC3_SRGB(),                   // 138 eBc3SrgbBlock
    PF::BC4_UNORM(),                  // 139 eBc4UnormBlock
    PF::BC4_SNORM(),                  // 140 eBc4SnormBlock
    PF::BC5_UNORM(),                  // 141 eBc5UnormBlock
    PF::BC5_SNORM(),                  // 142 eBc5SnormBlock
    PF::BC6H_UNORM(),                 // 143 eBc6HUfloatBlock
    PF::BC6H_SNORM(),                 // 144 eBc6HSfloatBlock
    PF::BC7_UNORM(),                  // 145 eBc7UnormBlock
    PF::BC7_SRGB(),                   // 146 eBc7SrgbBlock
    PF::ETC2_UNORM(),                 // 147 eEtc2R8G8B8UnormBlock
    PF::ETC2_SRGB(),                  // 148 eEtc2R8G8B8SrgbBlock
    PF::ETC2_UNORM(),                 // 149 eEtc2R8G8B8A1UnormBlock
    PF::ETC2_UNORM(),                 // 150 eEtc2R8G8B8A1SrgbBlock
    PF::ETC2_EAC_UNORM(),             // 151 eEtc2R8G8B8A8UnormBlock
    PF::ETC2_EAC_SRGB(),              // 152 eEtc2R8G8B8A8SrgbBlock
    PF::UNKNOWN(),                    // 153 eEacR11UnormBlock
    PF::UNKNOWN(),                    // 154 eEacR11SnormBlock
    PF::UNKNOWN(),                    // 155 eEacR11G11UnormBlock
    PF::UNKNOWN(),                    // 156 eEacR11G11SnormBlock
    PF::ASTC_4x4_UNORM(),              // 157 eAstc4x4UnormBlock
    PF::ASTC_4x4_SRGB(),              // 158 eAstc4x4SrgbBlock
    PF::ASTC_5x4_UNORM(),              // 159 eAstc5x4UnormBlock
    PF::ASTC_5x4_SRGB(),              // 160 eAstc5x4SrgbBlock
    PF::ASTC_5x5_UNORM(),              // 161 eAstc5x5UnormBlock
    PF::ASTC_5x5_SRGB(),              // 162 eAstc5x5SrgbBlock
    PF::ASTC_6x5_UNORM(),              // 163 eAstc6x5UnormBlock
    PF::ASTC_6x5_SRGB(),              // 164 eAstc6x5SrgbBlock
    PF::ASTC_6x6_UNORM(),              // 165 eAstc6x6UnormBlock
    PF::ASTC_6x6_SRGB(),              // 166 eAstc6x6SrgbBlock
    PF::ASTC_8x5_UNORM(),              // 167 eAstc8x5UnormBlock
    PF::ASTC_8x5_SRGB(),              // 168 eAstc8x5SrgbBlock
    PF::ASTC_8x6_UNORM(),              // 169 eAstc8x6UnormBlock
    PF::ASTC_8x6_SRGB(),              // 170 eAstc8x6SrgbBlock
    PF::ASTC_8x8_UNORM(),              // 171 eAstc8x8UnormBlock
    PF::ASTC_8x8_SRGB(),              // 172 eAstc8x8SrgbBlock
    PF::ASTC_10x5_UNORM(),             // 173 eAstc10x5UnormBlock
    PF::ASTC_10x5_SRGB(),             // 174 eAstc10x5SrgbBlock
    PF::ASTC_10x6_UNORM(),             // 175 eAstc10x6UnormBlock
    PF::ASTC_10x6_SRGB(),             // 176 eAstc10x6SrgbBlock
    PF::ASTC_10x8_UNORM(),             // 177 eAstc10x8UnormBlock
    PF::ASTC_10x8_SRGB(),             // 178 eAstc10x8SrgbBlock
    PF::ASTC_10x10_UNORM(),            // 179 eAstc10x10UnormBlock
    PF::ASTC_10x10_SRGB(),             // 180 eAstc10x10SrgbBlock
    PF::ASTC_12x10_UNORM(),            // 181 eAstc12x10UnormBlock
    PF::ASTC_12x10_SRGB(),             // 182 eAstc12x10SrgbBlock
    PF::ASTC_12x12_UNORM(),            // 183 eAstc12x12UnormBlock
    PF::ASTC_12x12_SRGB(),             // 184 eAstc12x12SrgbBlock
    PF::UNKNOWN(),                    // 185 eG8B8G8R8422Unorm
    PF::UNKNOWN(),                    // 186 eB8G8R8G8422Unorm
    PF::UNKNOWN(),                    // 187 eG8B8R83Plane420Unorm
    PF::UNKNOWN(),                    // 188 eG8B8R82Plane420Unorm
    PF::UNKNOWN(),                    // 189 eG8B8R83Plane422Unorm
    PF::UNKNOWN(),                    // 190 eG8B8R82Plane422Unorm
    PF::UNKNOWN(),                    // 191 eG8B8R83Plane444Unorm
    PF::UNKNOWN(),                    // 192 eR10X6UnormPack16
    PF::UNKNOWN(),                    // 193 eR10X6G10X6Unorm2Pack16
    PF::UNKNOWN(),                    // 194 eR10X6G10X6B10X6A10X6Unorm4Pack16
    PF::UNKNOWN(),                    // 195 eG10X6B10X6G10X6R10X6422Unorm4Pack16
    PF::UNKNOWN(),                    // 196 eB10X6G10X6R10X6G10X6422Unorm4Pack16
    PF::BGRA_4_4_4_4_UNORM(),         // 197 eA4R4G4B4UnormPack16
    PF::BGRA_4_4_4_4_UNORM(),         // 198 eA4B4G4R4UnormPack16
    PF::ASTC_4x4_SFLOAT(),            // 199 eAstc4x4SfloatBlock
    PF::ASTC_5x4_SFLOAT(),            // 200 eAstc5x4SfloatBlock
    PF::ASTC_5x5_SFLOAT(),            // 201 eAstc5x5SfloatBlock
    PF::ASTC_6x5_SFLOAT(),            // 202 eAstc6x5SfloatBlock
    PF::ASTC_6x6_SFLOAT(),            // 203 eAstc6x6SfloatBlock
    PF::ASTC_8x5_SFLOAT(),            // 204 eAstc8x5SfloatBlock
    PF::ASTC_8x6_SFLOAT(),            // 205 eAstc8x6SfloatBlock
    PF::ASTC_8x8_SFLOAT(),            // 206 eAstc8x8SfloatBlock
    PF::ASTC_10x5_SFLOAT(),           // 207 eAstc10x5SfloatBlock
    PF::ASTC_10x6_SFLOAT(),           // 208 eAstc10x6SfloatBlock
    PF::ASTC_10x8_SFLOAT(),           // 209 eAstc10x8SfloatBlock
    PF::ASTC_10x10_SFLOAT(),          // 210 eAstc10x10SfloatBlock
    PF::ASTC_12x10_SFLOAT(),          // 211 eAstc12x10SfloatBlock
    PF::ASTC_12x12_SFLOAT(),          // 212 eAstc12x12SfloatBlock
    PF::UNKNOWN(),                    // 213 ePvrtc12BppUnormBlockIMG
    PF::UNKNOWN(),                    // 214 ePvrtc14BppUnormBlockIMG
    PF::UNKNOWN(),                    // 215 ePvrtc22BppUnormBlockIMG
    PF::UNKNOWN(),                    // 216 ePvrtc24BppUnormBlockIMG
    PF::UNKNOWN(),                    // 217 ePvrtc12BppSrgbBlockIMG
    PF::UNKNOWN(),                    // 218 ePvrtc14BppSrgbBlockIMG
    PF::UNKNOWN(),                    // 219 ePvrtc22BppSrgbBlockIMG
    PF::UNKNOWN(),                    // 220 ePvrtc24BppSrgbBlockIMG
    PF::UNKNOWN(),                    // 221 eR16G16S105NV
    PF::UNKNOWN(),                    // 222 eA4B4G4R4UnormPack16EXT
    PF::UNKNOWN(),                    // 223 eA4R4G4B4UnormPack16EXT
    PF::UNKNOWN(),                    // 224-255 (extension/unknown)
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
    PF::UNKNOWN(),
};

} // namespace

vk::Format pixelFormatToVkFormat(gfx::img::PixelFormat pf) {
    static std::unordered_map<uint32_t, vk::Format> map;
    if (map.empty()) GN_UNLIKELY {
        for (size_t i = 0; i < std::size(VK_PF_MAPPING); ++i) {
            if (VK_PF_MAPPING[i].u32 != PF::UNKNOWN().u32) {
                map[VK_PF_MAPPING[i].u32] = static_cast<vk::Format>(i);
            }
        }
    }
    auto it = map.find(pf.u32);
    return it != map.end() ? it->second : vk::Format::eUndefined;
}

gfx::img::PixelFormat vkFormatToPixelFormat(vk::Format vkFmt) {
    const size_t idx = static_cast<size_t>(vkFmt);
    if (idx < std::size(VK_PF_MAPPING)) {
        return VK_PF_MAPPING[idx];
    }
    return PF::UNKNOWN();
}

} // namespace GN::rdg
