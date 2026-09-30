#include "pch.h"
#include "model-internal.h"
#include "cel-model-frag.spv.h"
#include "cel-model-vert.spv.h"
#include "cel-outline-frag.spv.h"
#include "cel-outline-vert.spv.h"
#include "vk-shaders/model-material-ubo.h"
#include "vk-shaders/cel-shading-ubo.h"

#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_inverse.hpp>

static GN::Logger * sLogger = GN::getLogger("GN.fx2.cel-shading");

namespace GN::fx2 {

namespace {

void packUbo(const CelModelShading::Config & config, shader::CelShadingUBO & ubo) {
    ubo.shadowParams   = glm::vec4(config.shadowThreshold, config.shadowFeather, config.deepShadowThreshold, config.deepShadowFeather);
    ubo.shadowTint     = glm::vec4(config.shadowTint, 1.0f);
    ubo.deepShadowTint = glm::vec4(config.deepShadowTint, 1.0f);
    ubo.specularParams = glm::vec4(config.specularThreshold, config.specularShininess, config.specularIntensity, 0.0f);
    ubo.rimParams      = glm::vec4(config.rimThreshold, config.rimFeather, config.rimIntensity, 0.0f);
    ubo.rimTint        = glm::vec4(config.rimTint, 1.0f);
    ubo.outlineParams  = glm::vec4(config.outlineWidth, 0.0f, 0.0f, 0.0f);
    ubo.outlineColor   = config.outlineColor;
}

struct CelShadingData {
    AutoRef<gpu2::GpuShader> surfaceVs;
    AutoRef<gpu2::GpuShader> surfacePs;
    AutoRef<gpu2::GpuShader> outlineVs;
    AutoRef<gpu2::GpuShader> outlinePs;
    AutoRef<gpu2::Texture>   white;
    AutoRef<gpu2::Texture>   normal;
    AutoRef<gpu2::Texture>   black;
    AutoRef<gpu2::Texture>   metalRough;
    AutoRef<gpu2::Buffer>    celBuffer;
};

struct CelModelShadingAsset final : CelModelShading::Asset {
    GN_REGISTER_RUNTIME_TYPE(Asset);

    CelModelShading::Config   cfg;
    CelShadingData            data;
    AutoRef<gpu2::GpuPayload> gpuPayload;

    AutoRef<gpu2::GpuPayload> uploadPayload() const override { return gpuPayload; }

    const CelModelShading::Config & config() const override { return cfg; }

    void updateConfig(const CelModelShading::Config & newConfig) override {
        cfg = newConfig;
        if (data.celBuffer) {
            shader::CelShadingUBO ubo {};
            packUbo(cfg, ubo);
            data.celBuffer->setContent(ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(&ubo), sizeof(ubo)), 0);
        }
    }

    CelModelShadingAsset(CelModelShading::Config config, CelShadingData value)
        : Asset(TYPE_INFO(), "cel-shading"), cfg(std::move(config)), data(std::move(value)) {}
};

} // namespace

AutoRef<CelModelShading::Asset> CelModelShading::create(AutoRef<gpu2::GpuContext> gpu) { return create(gpu, Config {}); }

AutoRef<CelModelShading::Asset> CelModelShading::create(AutoRef<gpu2::GpuContext> gpu, const Config & config) {
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger, "CelModelShading::create: missing GPU context");
            return {};
        }

    CelShadingData data;
    data.surfaceVs = gpu2::GpuShader::create({.context = gpu, .name = "cel-model.vert", .binary = kCelModelVertSpv, .size = sizeof(kCelModelVertSpv)});
    data.surfacePs = gpu2::GpuShader::create({.context = gpu, .name = "cel-model.frag", .binary = kCelModelFragSpv, .size = sizeof(kCelModelFragSpv)});
    data.outlineVs = gpu2::GpuShader::create({.context = gpu, .name = "cel-outline.vert", .binary = kCelOutlineVertSpv, .size = sizeof(kCelOutlineVertSpv)});
    data.outlinePs = gpu2::GpuShader::create({.context = gpu, .name = "cel-outline.frag", .binary = kCelOutlineFragSpv, .size = sizeof(kCelOutlineFragSpv)});

    auto cnc = gpu2::GpuCnC::create({.gpu = gpu});
    if (!data.surfaceVs || !data.surfacePs || !data.outlineVs || !data.outlinePs || !cnc) GN_UNLIKELY return {};

    data.white      = makeSolidTexture(gpu, *cnc, "cel.white", {255, 255, 255, 255});
    data.normal     = makeSolidTexture(gpu, *cnc, "cel.normal", {128, 128, 255, 255});
    data.black      = makeSolidTexture(gpu, *cnc, "cel.black", {0, 0, 0, 255});
    data.metalRough = makeSolidTexture(gpu, *cnc, "cel.metal-rough", {0, 255, 255, 255});
    if (!data.white || !data.normal || !data.black || !data.metalRough) GN_UNLIKELY return {};

    data.celBuffer = gpu2::Buffer::create("cel.constants", {.context = gpu, .size = sizeof(shader::CelShadingUBO)});
    if (!data.celBuffer) GN_UNLIKELY return {};

    shader::CelShadingUBO ubo {};
    packUbo(config, ubo);
    cnc->uploadBuffer(data.celBuffer, 0, ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(&ubo), sizeof(ubo)));

    AutoRef<CelModelShadingAsset> result(new CelModelShadingAsset(config, std::move(data)));
    result->gpuPayload = cnc->seal();
    if (!result->gpuPayload) return {};
    return result;
}

gpu2::GpuRaster::DrawParameters CelModelShading::getDrawParams(const SharedShaderConstants::Snapshot & sscSnapshot, AutoRef<const Asset> shading,
                                                               AutoRef<const ModelAsset> modelBase, uint32_t primitiveIndex, const glm::mat4 & worldTransform) {
    const auto * model   = RuntimeType::cast<ModelAssetImpl>(modelBase.get());
    const auto * content = RuntimeType::cast<CelModelShadingAsset>(shading.get());
    if (!content || !model || primitiveIndex >= model->primitives.size()) GN_UNLIKELY return {};
    const ModelScene::Primitive & primitive = model->scene->primitives[primitiveIndex];
    if (primitive.material >= model->scene->materials.size() || primitive.material >= model->materialBuffers.size()) GN_UNLIKELY return {};
    const ModelScene::Material & material = model->scene->materials[primitive.material];

    gpu2::GpuRaster::DrawParameters draw;
    draw.vs                = content->data.surfaceVs;
    draw.ps                = content->data.surfacePs;
    draw.geometry          = model->primitives[primitiveIndex];
    draw.states.cullMode   = material.doubleSided ? gpu2::RasterState::CULL_NONE : gpu2::RasterState::CULL_BACK;
    draw.states.frontFace  = gpu2::RasterState::FRONT_CCW;
    draw.states.depthState = gpu2::RasterState::DepthState {gpu2::RasterState::Compare::LESS, true};

    const glm::mat4 normalTransform = glm::transpose(glm::inverse(worldTransform));
    struct PushConstants {
        glm::mat4 world;
        glm::mat4 normal;
    };
    const PushConstants constants {worldTransform, normalTransform};
    draw.immediates = referenceTo(new SimpleBlob<uint8_t>(sizeof(constants), reinterpret_cast<const uint8_t *>(&constants)));

    draw.resources.resize(2);
    draw.resources[0] = sscSnapshot.set0Resources;
    auto & set1       = draw.resources[1];
    set1.resize(7);
    auto bindTexture = [&](uint32_t binding, int32_t textureIndex, const AutoRef<gpu2::Texture> & fallback) {
        AutoRef<gpu2::Texture> texture = fallback;
        if (textureIndex >= 0 && static_cast<size_t>(textureIndex) < model->textures.size() && model->textures[textureIndex]) {
            texture = model->textures[textureIndex];
        }
        set1[binding].resize(1);
        set1[binding][0].resource = texture;
        set1[binding][0].setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
    };
    bindTexture(0, material.baseColorMap, content->data.white);
    bindTexture(1, material.normalMap, content->data.normal);
    bindTexture(2, material.emissiveMap, content->data.white);
    bindTexture(3, material.occlusionMap, content->data.white);
    bindTexture(4, material.metalRoughMap, content->data.metalRough);

    set1[5].resize(1);
    set1[5][0].resource = model->materialBuffers[primitive.material];
    set1[5][0].setBufferViewType(gpu2::GpuResourceView::BufferView::UNIFORM).setBufferViewOffset(0).setBufferViewSize(sizeof(shader::ModelMaterialUBO));

    set1[6].resize(1);
    set1[6][0].resource = content->data.celBuffer;
    set1[6][0].setBufferViewType(gpu2::GpuResourceView::BufferView::UNIFORM).setBufferViewOffset(0).setBufferViewSize(sizeof(shader::CelShadingUBO));

    return draw;
}

gpu2::GpuRaster::DrawParameters CelModelShading::getOutlineDrawParams(const SharedShaderConstants::Snapshot & sscSnapshot, AutoRef<const Asset> shading,
                                                                      AutoRef<const ModelAsset> modelBase, uint32_t primitiveIndex,
                                                                      const glm::mat4 & worldTransform) {
    const auto * model   = RuntimeType::cast<ModelAssetImpl>(modelBase.get());
    const auto * content = RuntimeType::cast<CelModelShadingAsset>(shading.get());
    if (!content || !model || primitiveIndex >= model->primitives.size()) GN_UNLIKELY return {};
    if (content->cfg.outlineWidth <= 0.0f) return {};

    const ModelScene::Primitive & primitive = model->scene->primitives[primitiveIndex];
    if (primitive.material >= model->scene->materials.size() || primitive.material >= model->materialBuffers.size()) GN_UNLIKELY return {};
    const ModelScene::Material & material = model->scene->materials[primitive.material];

    gpu2::GpuRaster::DrawParameters draw;
    draw.vs                = content->data.outlineVs;
    draw.ps                = content->data.outlinePs;
    draw.geometry          = model->primitives[primitiveIndex];
    draw.states.cullMode   = gpu2::RasterState::CULL_FRONT;
    draw.states.frontFace  = gpu2::RasterState::FRONT_CCW;
    draw.states.depthState = gpu2::RasterState::DepthState {gpu2::RasterState::Compare::LESS_EQUAL, true};

    const glm::mat4 normalTransform = glm::transpose(glm::inverse(worldTransform));
    struct PushConstants {
        glm::mat4 world;
        glm::mat4 normal;
    };
    const PushConstants constants {worldTransform, normalTransform};
    draw.immediates = referenceTo(new SimpleBlob<uint8_t>(sizeof(constants), reinterpret_cast<const uint8_t *>(&constants)));

    draw.resources.resize(2);
    draw.resources[0] = sscSnapshot.set0Resources;
    auto & set1       = draw.resources[1];
    set1.resize(7);

    auto bindTexture = [&](uint32_t binding, int32_t textureIndex, const AutoRef<gpu2::Texture> & fallback) {
        AutoRef<gpu2::Texture> texture = fallback;
        if (textureIndex >= 0 && static_cast<size_t>(textureIndex) < model->textures.size() && model->textures[textureIndex]) {
            texture = model->textures[textureIndex];
        }
        set1[binding].resize(1);
        set1[binding][0].resource = texture;
        set1[binding][0].setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
    };
    bindTexture(0, material.baseColorMap, content->data.white);
    bindTexture(1, material.normalMap, content->data.normal);
    bindTexture(2, material.emissiveMap, content->data.white);
    bindTexture(3, material.occlusionMap, content->data.white);
    bindTexture(4, material.metalRoughMap, content->data.metalRough);

    set1[5].resize(1);
    set1[5][0].resource = model->materialBuffers[primitive.material];
    set1[5][0].setBufferViewType(gpu2::GpuResourceView::BufferView::UNIFORM).setBufferViewOffset(0).setBufferViewSize(sizeof(shader::ModelMaterialUBO));

    set1[6].resize(1);
    set1[6][0].resource = content->data.celBuffer;
    set1[6][0].setBufferViewType(gpu2::GpuResourceView::BufferView::UNIFORM).setBufferViewOffset(0).setBufferViewSize(sizeof(shader::CelShadingUBO));

    return draw;
}

} // namespace GN::fx2
