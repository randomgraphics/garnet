#include "ray-tracing-pipeline.h"

namespace GN::viewer {

bool RayTracingPipeline::prepare(gpu2::DescriptorHeap & heap, gpu2::CnC & uploads, const DynaArray<fx2::PbrRayTracingScene::Mesh> & meshes) {
    mScene.clear();
    mKernel.clear();
    if (meshes.empty()) return false;

    mKernel = fx2::RayTraceKernel::create(heap, uploads);
    if (!mKernel) return false;
    mScene = fx2::PbrRayTracingScene::create(heap, uploads, meshes);
    if (!mScene) mKernel.clear();
    return available();
}

bool RayTracingPipeline::resize(AutoRef<gpu2::GpuContext> gpu, gpu2::DescriptorHeap & heap, gpu2::CnC & uploads, uint32_t width, uint32_t height) {
    if (!gpu || !width || !height) return false;
    if (mColor && mWidth == width && mHeight == height) return true;

    auto color = gpu2::Texture::create(
        "viewer.rt-color",
        {.context    = gpu,
         .descriptor = gpu2::Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA_16_16_16_16_FLOAT()).setDimensions(width, height).setLevels(1)});
    if (!color) return false;

    fx2::UnlitMaterial::Parameters parameters;
    parameters.colorMap = gpu2::GpuResourceView {color}.setImageViewType(gpu2::GpuResourceView::ImageView::SAMPLED);
    parameters.toneMapWithExposure = true;
    auto material       = fx2::UnlitMaterial::create(heap, uploads, parameters);
    if (!material) return false;

    if (mCompositeGeometry.vertices.empty()) {
        const float vertices[]   = {-1, -1, 0.5f, 0, 0, 1, -1, 0.5f, 1, 0, 1, 1, 0.5f, 1, 1, -1, -1, 0.5f, 0, 0, 1, 1, 0.5f, 1, 1, -1, 1, 0.5f, 0, 1};
        auto        vertexBuffer = gpu2::Buffer::create("viewer.rt-composite-vertices", {.context = gpu, .size = sizeof(vertices)});
        if (!vertexBuffer) return false;
        uploads.recordUploadBuffer(vertexBuffer, 0, {reinterpret_cast<const uint8_t *>(vertices), sizeof(vertices)});
        mCompositeGeometry.vertices.push_back({.buffer = vertexBuffer, .offset = 0, .stride = 5 * sizeof(float)});
        mCompositeGeometry.format.attributes.push_back(
            {.location = fx2::UnlitMaterial::POSITION_LOCATION, .binding = 0, .offset = 0, .format = gpu2::RasterGeometry::AttributeFormat::F32_3});
        mCompositeGeometry.format.attributes.push_back({.location = fx2::UnlitMaterial::TEXCOORD_LOCATION,
                                                        .binding  = 0,
                                                        .offset   = 3 * sizeof(float),
                                                        .format   = gpu2::RasterGeometry::AttributeFormat::F32_2});
        mCompositeGeometry.vertexCount = 6;
    }

    mColor             = std::move(color);
    mCompositeMaterial = std::move(material);
    mWidth             = width;
    mHeight            = height;
    mSampleCount       = 0;
    return true;
}

bool RayTracingPipeline::record(gpu2::Raster & raster, const AutoRef<fx2::SharedShaderConstants::UniformState> & uniforms,
                                const gpu2::RasterGeometry & geometry, const gpu2::RasterState * state, const glm::mat4 & transform,
                                const AutoRef<fx2::SkyMaterial> & sky, const AutoRef<fx2::PbrMaterial> & material) const {
    if (!available() || !material) return false;
    fx2::RayTraceKernel::DrawParameters draw {{raster, uniforms, geometry, state}, mScene->accelerationStructure(), transform, sky, material.get()};
    return mKernel->record(draw);
}

void RayTracingPipeline::bindSceneResources(gpu2::GpuResourceTable & resources) const {
    if (!available()) return;
    resources.resize(3);
    resources[2].resize(2);
    gpu2::GpuResourceView accelerationStructure;
    accelerationStructure.setAccelerationStructure(mScene->accelerationStructure());
    resources[2][0].append(std::move(accelerationStructure));
    resources[2][1].append(mScene->geometryTable());
}

bool RayTracingPipeline::configureTarget(gpu2::RasterTarget & target, const AutoRef<gpu2::Texture> & sharedDepth, bool cameraMoved) {
    if (!mColor || !sharedDepth) return false;
    if (cameraMoved) mSampleCount = 0;
    const bool continueAccumulation = mSampleCount > 0;
    target.setColorTarget(0, gpu2::GpuResourceView {mColor});
    target.setDepthStencilTarget(gpu2::GpuResourceView {sharedDepth}).setClearDepth(1.0f).setClearColor(0, 0, 0, 0);
    target.loadColor = continueAccumulation;
    target.loadDepth = continueAccumulation;
    if (continueAccumulation) {
        auto & blend   = target.colorTargets[0].blendState;
        blend.colorSrc = gpu2::RasterTarget::BlendState::SRC_ALPHA;
        blend.colorDst = gpu2::RasterTarget::BlendState::INV_SRC_ALPHA;
        blend.alphaSrc = gpu2::RasterTarget::BlendState::ONE;
        blend.alphaDst = gpu2::RasterTarget::BlendState::INV_SRC_ALPHA;
    }
    return true;
}

bool RayTracingPipeline::recordComposite(gpu2::Raster & raster, const AutoRef<fx2::SharedShaderConstants::UniformState> & uniforms,
                                         const glm::mat4 & inverseViewProjection, fx2::UnlitKernel & kernel) const {
    if (!mCompositeMaterial || mCompositeGeometry.vertexCount == 0) return false;
    gpu2::RasterState state;
    state.cullMode   = gpu2::RasterState::CULL_NONE;
    state.depthState = gpu2::RasterState::DepthState {gpu2::RasterState::Compare::ALWAYS, false};
    fx2::UnlitMaterial::DrawParameters draw {{raster, uniforms, mCompositeGeometry, &state}, inverseViewProjection};
    return kernel.record(*mCompositeMaterial, draw);
}

} // namespace GN::viewer
