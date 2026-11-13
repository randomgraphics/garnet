#version 460
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_ray_query : require
#extension GL_EXT_buffer_reference : require
#extension GL_EXT_buffer_reference2 : require
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require
#extension GL_EXT_shader_explicit_arithmetic_types_int16 : require

#include "bindless-shared-uniforms.h"
#include "bindless-pbr-material.h"
#include "bindless-sky-material.h"

struct GeometryData {
    uint64_t vertexAddress;
    uint64_t indexAddress;
    uint     vertexStride;
    uint     positionOffset;
    uint     normalOffset;
    uint     texcoordOffset;
    uint     materialIndex;
    uint     vertexCount;
    uint     indexCount;
    uint     indexFormat;
    mat4     objectToWorld;
};

layout(set = 0, binding = 1) uniform texture2D heapTextures[];
layout(set = 0, binding = 1) uniform textureCube heapCubemaps[];
layout(set = 0, binding = 5) uniform sampler heapSamplers[];
layout(set = 2, binding = 0) uniform accelerationStructureEXT sceneAccelerationStructure;
layout(set = 2, binding = 1, std430) readonly buffer GeometryTable { GeometryData geometries[]; };

layout(push_constant, std430) uniform PbrDraw {
    mat4  worldFromObject;
    uvec4 materialIndex;
}
draw;

layout(location = 0) in vec3 worldNormal;
layout(location = 1) flat in vec4 baseColor;
layout(location = 2) flat in vec4 emissive;
layout(location = 3) flat in vec4 factors;
layout(location = 4) flat in uvec4 textureIndices;
layout(location = 5) flat in uvec4 extraIndices;
layout(location = 6) in vec2 uv;
layout(location = 7) in vec3 worldPosition;
layout(location = 0) out vec4 outColor;

layout(buffer_reference, std430, buffer_reference_align = 4) readonly buffer FloatData { float values[]; };
layout(buffer_reference, std430, buffer_reference_align = 4) readonly buffer UIntData { uint values[]; };
layout(buffer_reference, std430, buffer_reference_align = 2) readonly buffer UShortData { uint16_t values[]; };

const float PI                    = 3.14159265359;
const uint  MAX_SECONDARY_BOUNCES = 2u;

struct Surface {
    vec3  position;
    vec3  normal;
    vec2  uv;
    vec4  albedo;
    vec3  emissive;
    float metallic;
    float roughness;
    float occlusion;
    uint  flags;
    bool  opaque;
};

vec4 sampleHeap(uint textureIndex, uint samplerIndex, vec2 coord) {
    return texture(sampler2D(heapTextures[nonuniformEXT(textureIndex)], heapSamplers[nonuniformEXT(samplerIndex)]), coord);
}

vec3 fresnelSchlick(float cosine, vec3 f0) { return f0 + (1.0 - f0) * pow(clamp(1.0 - cosine, 0.0, 1.0), 5.0); }

uint randomBits(inout uint state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

float randomFloat(inout uint state) { return float(randomBits(state) >> 8) * (1.0 / 16777216.0); }

uint pixelSeed(uvec2 pixel) {
    uint seed = pixel.x * 0x9e3779b9u ^ pixel.y * 0x85ebca6bu ^ sscUniforms.frameCounter * 0xc2b2ae35u;
    seed ^= seed >> 16;
    seed *= 0x7feb352du;
    seed ^= seed >> 15;
    return seed == 0u ? 1u : seed;
}

vec3 cosineHemisphere(vec3 normal, inout uint seed) {
    float r         = sqrt(randomFloat(seed));
    float phi       = 2.0 * PI * randomFloat(seed);
    vec3  helper    = abs(normal.y) < 0.999 ? vec3(0, 1, 0) : vec3(1, 0, 0);
    vec3  tangent   = normalize(cross(helper, normal));
    vec3  bitangent = cross(normal, tangent);
    return normalize(tangent * (r * cos(phi)) + bitangent * (r * sin(phi)) + normal * sqrt(max(0.0, 1.0 - r * r)));
}

vec3 fetchVec3(GeometryData geometry, uint vertex, uint byteOffset) {
    FloatData data = FloatData(geometry.vertexAddress + uint64_t(vertex) * geometry.vertexStride + byteOffset);
    return vec3(data.values[0], data.values[1], data.values[2]);
}

vec2 fetchVec2(GeometryData geometry, uint vertex, uint byteOffset) {
    FloatData data = FloatData(geometry.vertexAddress + uint64_t(vertex) * geometry.vertexStride + byteOffset);
    return vec2(data.values[0], data.values[1]);
}

uint triangleVertex(GeometryData geometry, uint primitive, uint corner) {
    if (geometry.indexCount == 0u) return primitive * 3u + corner;
    if (geometry.indexFormat == 1u) {
        UShortData indices = UShortData(geometry.indexAddress);
        return uint(indices.values[primitive * 3u + corner]);
    }
    UIntData indices = UIntData(geometry.indexAddress);
    return indices.values[primitive * 3u + corner];
}

Surface makeSecondarySurface(GeometryData geometry, uint primitive, vec2 barycentrics) {
    uint i0   = triangleVertex(geometry, primitive, 0u);
    uint i1   = triangleVertex(geometry, primitive, 1u);
    uint i2   = triangleVertex(geometry, primitive, 2u);
    vec3 bary = vec3(1.0 - barycentrics.x - barycentrics.y, barycentrics.x, barycentrics.y);
    vec3 p0   = fetchVec3(geometry, i0, geometry.positionOffset);
    vec3 p1   = fetchVec3(geometry, i1, geometry.positionOffset);
    vec3 p2   = fetchVec3(geometry, i2, geometry.positionOffset);
    vec3 n0   = fetchVec3(geometry, i0, geometry.normalOffset);
    vec3 n1   = fetchVec3(geometry, i1, geometry.normalOffset);
    vec3 n2   = fetchVec3(geometry, i2, geometry.normalOffset);
    vec2 uv0  = fetchVec2(geometry, i0, geometry.texcoordOffset);
    vec2 uv1  = fetchVec2(geometry, i1, geometry.texcoordOffset);
    vec2 uv2  = fetchVec2(geometry, i2, geometry.texcoordOffset);

    Surface surface;
    surface.position     = (geometry.objectToWorld * vec4(p0 * bary.x + p1 * bary.y + p2 * bary.z, 1.0)).xyz;
    mat3 normalTransform = transpose(inverse(mat3(geometry.objectToWorld)));
    surface.normal       = normalize(normalTransform * (n0 * bary.x + n1 * bary.y + n2 * bary.z));
    surface.uv           = uv0 * bary.x + uv1 * bary.y + uv2 * bary.z;

    PbrMaterialData material = pbrMaterialHeap.materials[geometry.materialIndex];
    surface.flags            = material.extraIndices.z;
    surface.albedo           = material.baseColor;
    if ((surface.flags & 1u) != 0u) surface.albedo *= sampleHeap(material.textureIndices.x, material.extraIndices.y, surface.uv);
    surface.emissive = material.emissive.rgb;
    if ((surface.flags & 4u) != 0u) surface.emissive *= sampleHeap(material.textureIndices.z, material.extraIndices.y, surface.uv).rgb;
    surface.occlusion =
        (surface.flags & 8u) != 0u ? mix(1.0, sampleHeap(material.textureIndices.w, material.extraIndices.y, surface.uv).r, material.factors.w) : 1.0;
    surface.metallic  = material.factors.x;
    surface.roughness = material.factors.y;
    if ((surface.flags & 16u) != 0u) {
        vec4 metalRoughness = sampleHeap(material.extraIndices.x, material.extraIndices.y, surface.uv);
        surface.roughness *= metalRoughness.g;
        surface.metallic *= metalRoughness.b;
    }
    surface.metallic  = clamp(surface.metallic, 0.0, 1.0);
    surface.roughness = clamp(surface.roughness, 0.04, 1.0);
    surface.opaque    = material.extraIndices.w != 0u;

    if ((surface.flags & 2u) != 0u) {
        vec3  edge1       = p1 - p0;
        vec3  edge2       = p2 - p0;
        vec2  uvEdge1     = uv1 - uv0;
        vec2  uvEdge2     = uv2 - uv0;
        float determinant = uvEdge1.x * uvEdge2.y - uvEdge1.y * uvEdge2.x;
        if (abs(determinant) > 1e-10) {
            vec3 tangent   = normalize(normalTransform * (edge1 * uvEdge2.y - edge2 * uvEdge1.y) / determinant);
            vec3 bitangent = normalize(normalTransform * (-edge1 * uvEdge2.x + edge2 * uvEdge1.x) / determinant);
            vec3 mapped    = sampleHeap(material.textureIndices.y, material.extraIndices.y, surface.uv).xyz * 2.0 - 1.0;
            mapped.xy *= material.factors.z;
            surface.normal = normalize(mat3(tangent, bitangent, surface.normal) * mapped);
        }
    }
    return surface;
}

Surface makePrimarySurface() {
    Surface surface;
    surface.position = worldPosition;
    surface.normal   = normalize(worldNormal);
    if (!gl_FrontFacing) surface.normal = -surface.normal;
    surface.uv     = uv;
    surface.albedo = baseColor;
    surface.flags  = extraIndices.z;
    if ((surface.flags & 1u) != 0u) surface.albedo *= sampleHeap(textureIndices.x, extraIndices.y, uv);
    surface.emissive = emissive.rgb;
    if ((surface.flags & 4u) != 0u) surface.emissive *= sampleHeap(textureIndices.z, extraIndices.y, uv).rgb;
    surface.occlusion = (surface.flags & 8u) != 0u ? mix(1.0, sampleHeap(textureIndices.w, extraIndices.y, uv).r, factors.w) : 1.0;
    surface.metallic  = factors.x;
    surface.roughness = factors.y;
    if ((surface.flags & 16u) != 0u) {
        vec4 metalRoughness = sampleHeap(extraIndices.x, extraIndices.y, uv);
        surface.roughness *= metalRoughness.g;
        surface.metallic *= metalRoughness.b;
    }
    surface.metallic  = clamp(surface.metallic, 0.0, 1.0);
    surface.roughness = clamp(surface.roughness, 0.04, 1.0);
    surface.opaque    = extraIndices.w != 0u;
    if ((surface.flags & 2u) != 0u) {
        vec3  dpdx = dFdx(worldPosition), dpdy = dFdy(worldPosition);
        vec2  duvdx = dFdx(uv), duvdy = dFdy(uv);
        vec3  p1 = cross(dpdy, surface.normal), p2 = cross(surface.normal, dpdx);
        vec3  tangent   = p1 * duvdx.x + p2 * duvdy.x;
        vec3  bitangent = p1 * duvdx.y + p2 * duvdy.y;
        float scale     = max(dot(tangent, tangent), dot(bitangent, bitangent));
        vec3  mapped    = sampleHeap(textureIndices.y, extraIndices.y, uv).xyz * 2.0 - 1.0;
        mapped.xy *= factors.z;
        if (scale > 1e-20) surface.normal = normalize(mat3(tangent * inversesqrt(scale), bitangent * inversesqrt(scale), surface.normal) * mapped);
    }
    return surface;
}

vec3 evaluatePbrBrdf(Surface surface, vec3 viewDirection, vec3 lightDirection) {
    float nl = max(dot(surface.normal, lightDirection), 0.0);
    float nv = max(dot(surface.normal, viewDirection), 1e-4);
    if (nl <= 0.0) return vec3(0.0);

    vec3 halfVector = normalize(lightDirection + viewDirection);
    float nh = max(dot(surface.normal, halfVector), 0.0);
    float vh = max(dot(viewDirection, halfVector), 0.0);
    float alpha = surface.roughness * surface.roughness;
    float alpha2 = alpha * alpha;
    float distribution = alpha2 / max(PI * pow(nh * nh * (alpha2 - 1.0) + 1.0, 2.0), 1e-6);
    float k = (surface.roughness + 1.0) * (surface.roughness + 1.0) / 8.0;
    float geometry = nv / (nv * (1.0 - k) + k) * nl / (nl * (1.0 - k) + k);
    vec3 f0 = mix(vec3(0.04), max(surface.albedo.rgb, vec3(0.0)), surface.metallic);
    vec3 fresnel = fresnelSchlick(vh, f0);
    vec3 specular = distribution * geometry * fresnel / max(4.0 * nv * max(nl, 1e-4), 1e-5);
    vec3 diffuse = (1.0 - fresnel) * (1.0 - surface.metallic) * surface.albedo.rgb / PI;
    return diffuse + specular;
}

vec3 sampleGgxHalfVector(vec3 normal, float roughness, inout uint seed) {
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float u1 = randomFloat(seed);
    float u2 = randomFloat(seed);
    float phi = 2.0 * PI * u1;
    float cosTheta = sqrt((1.0 - u2) / max(1.0 + (alpha2 - 1.0) * u2, 1e-7));
    float sinTheta = sqrt(max(1.0 - cosTheta * cosTheta, 0.0));
    vec3 helper = abs(normal.y) < 0.999 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(helper, normal));
    vec3 bitangent = cross(normal, tangent);
    return normalize(tangent * (sinTheta * cos(phi)) + bitangent * (sinTheta * sin(phi)) + normal * cosTheta);
}

bool samplePbrDirection(Surface surface, vec3 viewDirection, inout uint seed, out vec3 direction, out vec3 throughput) {
    float opacity = surface.opaque ? 1.0 : clamp(surface.albedo.a, 0.0, 1.0);
    if (randomFloat(seed) >= opacity) {
        direction = refract(-viewDirection, surface.normal, 1.0 / 1.5);
        if (dot(direction, direction) < 1e-10) direction = reflect(-viewDirection, surface.normal);
        throughput = vec3(1.0);
        return true;
    }

    vec3 f0 = mix(vec3(0.04), max(surface.albedo.rgb, vec3(0.0)), surface.metallic);
    vec3 viewFresnel = fresnelSchlick(max(dot(surface.normal, viewDirection), 0.0), f0);
    float specularProbability = clamp(max(max(viewFresnel.r, viewFresnel.g), viewFresnel.b), 0.05, 0.95);
    bool sampleSpecular = randomFloat(seed) < specularProbability;
    if (sampleSpecular) {
        vec3 halfVector = sampleGgxHalfVector(surface.normal, surface.roughness, seed);
        direction = reflect(-viewDirection, halfVector);
    } else {
        direction = cosineHemisphere(surface.normal, seed);
    }

    float nl = max(dot(surface.normal, direction), 0.0);
    if (nl <= 0.0) {
        throughput = vec3(0.0);
        return false;
    }

    vec3 halfVector = normalize(viewDirection + direction);
    float nh = max(dot(surface.normal, halfVector), 0.0);
    float vh = max(dot(viewDirection, halfVector), 0.0);
    float alpha = surface.roughness * surface.roughness;
    float alpha2 = alpha * alpha;
    float distribution = alpha2 / max(PI * pow(nh * nh * (alpha2 - 1.0) + 1.0, 2.0), 1e-6);
    float pdfSpecular = distribution * nh / max(4.0 * vh, 1e-7);
    float pdfDiffuse = nl / PI;
    float mixturePdf = specularProbability * pdfSpecular + (1.0 - specularProbability) * pdfDiffuse;
    vec3 brdf = evaluatePbrBrdf(surface, viewDirection, direction);
    // The opacity probability cancels its coverage-scaled BSDF weight; include the lobe PDF.
    throughput = brdf * nl / max(mixturePdf, 1e-7);
    return true;
}

vec3 directLighting(Surface surface, vec3 viewDirection) {
    vec3 result = vec3(0.0);
    for (uint i = 0u; i < min(sscUniforms.numLights, uint(MAX_SHARED_LIGHTS)); ++i) {
        DirectLightUniform light = sscUniforms.lights[i];
        bool directional = int(light.positionOrDir.w + 0.5) == int(SHARED_LIGHT_TYPE_DIRECTIONAL);
        vec3 delta = directional ? -light.positionOrDir.xyz : light.positionOrDir.xyz - surface.position;
        float distanceToLight = length(delta);
        vec3 lightDirection = delta / max(distanceToLight, 1e-20);
        float nl = max(dot(surface.normal, lightDirection), 0.0);
        if (nl <= 0.0) continue;

        rayQueryEXT shadowQuery;
        float maxDistance = directional ? 1e30 : max(distanceToLight - 0.002, 0.001);
        rayQueryInitializeEXT(shadowQuery, sceneAccelerationStructure, gl_RayFlagsOpaqueEXT | gl_RayFlagsTerminateOnFirstHitEXT, 0xff,
                              surface.position + surface.normal * 0.001, 0.001, lightDirection, maxDistance);
        while (rayQueryProceedEXT(shadowQuery)) {}
        if (rayQueryGetIntersectionTypeEXT(shadowQuery, true) != gl_RayQueryCommittedIntersectionNoneEXT) continue;

        float attenuation = directional ? 1.0 : 1.0 / max(distanceToLight * distanceToLight, 1e-4);
        if (!directional && light.colorAndRange.w > 0.0) attenuation *= pow(clamp(1.0 - distanceToLight / light.colorAndRange.w, 0.0, 1.0), 2.0);
        vec3 brdf = evaluatePbrBrdf(surface, viewDirection, lightDirection);
        result += brdf * max(light.colorAndRange.rgb, vec3(0.0)) * attenuation * nl;
    }
    // Match raster PBR: ambient occlusion modulates reflected light, not emission.
    return result * surface.occlusion + surface.emissive;
}
vec3 sampleSky(SkyMaterialData sky, vec3 direction) {
    vec3 skyColor = texture(samplerCube(heapCubemaps[nonuniformEXT(sky.textureIndices.x)], heapSamplers[nonuniformEXT(sky.samplerIndices.x)]), direction).rgb *
                    sky.factors.x;
    return max(skyColor, vec3(sky.factors.y));
}

// All indirect diffuse/specular rays share this incoming-radiance trace and bounce loop.
vec3 traceIncomingRadiance(Surface primary, SkyMaterialData sky, vec3 viewDirection, inout uint seed) {
    vec3 direction;
    vec3 throughput;
    if (!samplePbrDirection(primary, viewDirection, seed, direction, throughput)) return vec3(0.0);
    vec3 jitter   = dFdx(primary.position) * (randomFloat(seed) - 0.5) + dFdy(primary.position) * (randomFloat(seed) - 0.5);
    vec3 origin   = primary.position + jitter + primary.normal * 0.001;
    vec3 radiance = vec3(0.0);
    for (uint bounce = 0u; bounce < MAX_SECONDARY_BOUNCES; ++bounce) {
        rayQueryEXT query;
        rayQueryInitializeEXT(query, sceneAccelerationStructure, gl_RayFlagsOpaqueEXT | gl_RayFlagsTerminateOnFirstHitEXT, 0xff, origin, 0.001, direction,
                              sscUniforms.farPlane);
        while (rayQueryProceedEXT(query)) {}
        if (rayQueryGetIntersectionTypeEXT(query, true) == gl_RayQueryCommittedIntersectionNoneEXT) {
            radiance += throughput * sampleSky(sky, direction);
            break;
        }

        uint         geometryIndex = rayQueryGetIntersectionGeometryIndexEXT(query, true);
        GeometryData geometry      = geometries[geometryIndex];
        Surface      hit =
            makeSecondarySurface(geometry, rayQueryGetIntersectionPrimitiveIndexEXT(query, true), rayQueryGetIntersectionBarycentricsEXT(query, true));
        float distance = rayQueryGetIntersectionTEXT(query, true);
        hit.position   = origin + direction * distance;
        if (dot(hit.normal, direction) > 0.0) hit.normal = -hit.normal;
        vec3 hitViewDirection = normalize(sscUniforms.cameraPosition.xyz - hit.position);
        radiance += throughput * directLighting(hit, hitViewDirection);

        if (bounce + 1u >= MAX_SECONDARY_BOUNCES) break;
        vec3 nextDirection;
        vec3 bounceThroughput;
        if (!samplePbrDirection(hit, hitViewDirection, seed, nextDirection, bounceThroughput)) break;
        direction = nextDirection;
        throughput *= bounceThroughput;        origin = hit.position + hit.normal * 0.001;
    }
    return radiance;
}

void main() {
    Surface primary       = makePrimarySurface();
    vec3    viewDirection = normalize(sscUniforms.cameraPosition.xyz - primary.position);
    vec3    color         = directLighting(primary, viewDirection);
    uint    skyIndex      = draw.materialIndex.y;
    if (skyIndex != 0xffffffffu) {
        SkyMaterialData sky  = skyMaterialHeap.materials[skyIndex];
        uint            seed = pixelSeed(uvec2(gl_FragCoord.xy));
        color += traceIncomingRadiance(primary, sky, viewDirection, seed);
    }
    color = max(color, vec3(0.0)) * max(sscUniforms.exposure, 0.0);
    color = color / (color + vec3(1.0));
    // Alpha carries the progressive sample weight and primary surface coverage.
    float sampleWeight = 1.0 / float(sscUniforms.frameCounter + 1u);
    float coverage     = primary.opaque ? 1.0 : primary.albedo.a;
    outColor           = vec4(color, sampleWeight * coverage);
}
