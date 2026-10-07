#include "scene-renderer.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>
#include <sstream>

namespace GN::viewer {

using namespace gpu2;

namespace {

AutoRef<Texture> loadTexture(AutoRef<GpuContext> gpu, GpuCnC & uploads, const ModelScene::Texture & source) {
    gfx::img::Image image;
    if (!source.path.empty()) {
        auto file = fs::openFile(source.path, std::ios::in | std::ios::binary);
        if (!file) return {};
        image = gfx::img::Image::load(file->input(), source.path.c_str());
    } else {
        std::string        bytes(reinterpret_cast<const char *>(source.embeddedData.data()), source.embeddedData.size());
        std::istringstream stream(bytes);
        image = gfx::img::Image::load(stream, source.mimeType.c_str());
    }
    if (image.empty()) return {};

    Texture::Descriptor descriptor;
    descriptor.setFormat(image.format())
        .setDimensions(image.width(), image.height(), image.depth())
        .setFaces(static_cast<uint32_t>(image.desc().faces))
        .setLevels(static_cast<uint32_t>(image.desc().levels));
    using PF        = gfx::img::PixelFormat;
    const auto sign = source.srgb ? PF::SIGN_GNORM : PF::SIGN_UNORM;
    if (descriptor.format.sign0 == PF::SIGN_UNORM || descriptor.format.sign0 == PF::SIGN_GNORM) descriptor.format.sign0 = sign;
    if (descriptor.format.sign12 == PF::SIGN_UNORM || descriptor.format.sign12 == PF::SIGN_GNORM) descriptor.format.sign12 = sign;

    auto texture = Texture::create("viewer.texture", {.context = gpu, .descriptor = descriptor});
    if (!texture) return {};
    uploads.recordUploadImage(texture, image);
    return texture;
}

} // namespace

bool RenderModel::prepare(AutoRef<GpuContext> gpu, GpuCnC & geometryUploads, bindless::CnC & materialUploads, AutoRef<fx2::bindless::PbrKernel> pbrKernel,
                          AutoRef<fx2::bindless::LambertianKernel> lambertianKernel, AutoRef<fx2::bindless::UnlitKernel> unlitKernel, const ModelScene & scene,
                          Shading choice, const glm::mat4 & modelTransform) {
    DynaArray<ModelInstance> collected;
    if (!collectModelInstances(scene, collected)) return false;

    DynaArray<AutoRef<Texture>> textures;
    textures.resize(scene.textures.size());
    auto textureView = [&](int32_t index, GpuResourceView & view) {
        if (index < 0) return true;
        if (static_cast<size_t>(index) >= textures.size()) return false;
        if (!textures[index]) textures[index] = loadTexture(gpu, geometryUploads, scene.textures[index]);
        if (!textures[index]) return false;
        view.resource = textures[index];
        view.setImageViewType(GpuResourceView::ImageView::SAMPLED);
        return true;
    };

    for (const auto & source : scene.materials) {
        Material material;
        material.shading = choice == Shading::IMPORTED ? (source.workflow == ModelScene::MaterialWorkflow::UNLIT ? Shading::UNLIT : Shading::PBR) : choice;
        // Cel shading has no bindless kernel yet; use PBR for imported/forced CEL until it is restored.
        if (material.shading == Shading::CEL) material.shading = Shading::PBR;
        material.doubleSided = source.doubleSided;

        GpuResourceView baseColor, normal, emissive, occlusion, metalRough;
        if (!textureView(source.baseColorMap, baseColor) || !textureView(source.normalMap, normal) || !textureView(source.emissiveMap, emissive) ||
            !textureView(source.occlusionMap, occlusion) || !textureView(source.metalRoughMap, metalRough))
            return false;

        switch (material.shading) {
        case Shading::PBR: {
            auto parameters          = pbrKernel->defaultMaterialParameters();
            parameters.baseColor     = source.baseColor;
            parameters.emissive      = source.emissive;
            parameters.metallic      = source.metallic;
            parameters.roughness     = source.roughness;
            parameters.opaque        = source.alphaMode == ModelScene::AlphaMode::OPAQUE;
            parameters.baseColorMap  = baseColor;
            parameters.normalMap     = normal;
            parameters.emissiveMap   = emissive;
            parameters.occlusionMap  = occlusion;
            parameters.metalRoughMap = metalRough;
            material.pbr             = pbrKernel->createMaterial(materialUploads, parameters);
            if (!material.pbr) return false;
            break;
        }
        case Shading::LAMBERTIAN: {
            auto parameters      = lambertianKernel->defaultMaterialParameters();
            parameters.color     = source.baseColor;
            parameters.emissive  = source.emissive;
            parameters.colorMap  = baseColor;
            parameters.normalMap = normal;
            material.lambertian  = lambertianKernel->createMaterial(materialUploads, parameters);
            if (!material.lambertian) return false;
            break;
        }
        case Shading::UNLIT: {
            auto parameters     = unlitKernel->defaultMaterialParameters();
            parameters.color    = source.baseColor;
            parameters.emissive = source.emissive;
            parameters.colorMap = baseColor;
            material.unlit      = unlitKernel->createMaterial(materialUploads, parameters);
            if (!material.unlit) return false;
            break;
        }
        case Shading::IMPORTED:
        case Shading::CEL:
            return false;
        }
        materials.append(std::move(material));
    }

    DynaArray<RasterGeometry> meshes;
    for (const auto & primitive : scene.primitives) {
        if (primitive.vertices.empty() || primitive.indices.empty() || primitive.material >= scene.materials.size()) return false;
        for (auto index : primitive.indices)
            if (index >= primitive.vertices.size()) return false;

        const auto & sourceMaterial = scene.materials[primitive.material];
        if (!primitive.texcoords && (sourceMaterial.baseColorMap >= 0 || sourceMaterial.normalMap >= 0 || sourceMaterial.emissiveMap >= 0 ||
                                     sourceMaterial.occlusionMap >= 0 || sourceMaterial.metalRoughMap >= 0))
            return false;

        auto vb = Buffer::create("viewer.vertices", {.context = gpu, .size = primitive.vertices.size() * sizeof(ModelScene::Vertex)});
        auto ib = Buffer::create("viewer.indices", {.context = gpu, .size = primitive.indices.size() * sizeof(uint32_t)});
        if (!vb || !ib) return false;
        geometryUploads.recordUploadBuffer(
            vb, 0, {reinterpret_cast<const uint8_t *>(primitive.vertices.data()), primitive.vertices.size() * sizeof(ModelScene::Vertex)});
        geometryUploads.recordUploadBuffer(ib, 0, {reinterpret_cast<const uint8_t *>(primitive.indices.data()), primitive.indices.size() * sizeof(uint32_t)});

        RasterGeometry geometry;
        geometry.vertices.push_back({.buffer = vb, .offset = 0, .stride = sizeof(ModelScene::Vertex)});
        geometry.indices     = {.buffer = ib, .offset = 0, .stride = sizeof(uint32_t)};
        geometry.vertexCount = static_cast<uint32_t>(primitive.vertices.size());
        geometry.indexCount  = static_cast<uint32_t>(primitive.indices.size());
        using V              = ModelScene::Vertex;
        using F              = RasterGeometry::AttributeFormat;
        geometry.format.attributes.push_back({.location = 0, .binding = 0, .offset = offsetof(V, position), .format = F::F32_3});
        geometry.format.attributes.push_back({.location = 1, .binding = 0, .offset = offsetof(V, normal), .format = F::F32_3});
        geometry.format.attributes.push_back({.location = 2, .binding = 0, .offset = offsetof(V, texcoord), .format = F::F32_2});
        meshes.append(std::move(geometry));
    }

    for (const auto & instance : collected) {
        const auto  transform   = modelTransform * instance.transform;
        const float determinant = glm::determinant(transform);
        if (!std::isfinite(determinant) || determinant == 0) return false;
        instances.append({meshes[instance.primitive], scene.primitives[instance.primitive].material, transform});
    }
    return true;
}

bool SceneRenderer::prepare(AutoRef<GpuContext> gpu, const ModelScene & source, Shading shading, float environmentLuminance) {
    // Each material allocates one descriptor per map it owns, and every kernel adds its own
    // fallback descriptors, so size the heap from the import rather than guessing a constant.
    const uint32_t heapCapacity = static_cast<uint32_t>(std::min<uint64_t>(64 + source.materials.size() * 6, 1u << 20));
    heap                        = bindless::DescriptorHeap::create("viewer.heap", {.gpu = gpu, .capacity = heapCapacity, .materialCapacity = 4 * 1024 * 1024});
    ssc                         = fx2::bindless::SharedShaderConstants::create({.gpu = gpu, .uniformCapacity = 64 * 1024, .streamingCapacity = 64 * 1024});
    if (!heap || !ssc) return false;

    auto initialization  = bindless::CnC::create("viewer.material-init", {.gpu = gpu, .heap = heap});
    auto geometryUploads = GpuCnC::create({.gpu = gpu});
    if (!initialization || !geometryUploads) return false;
    pbr        = fx2::bindless::PbrKernel::create(*heap, *initialization);
    lambertian = fx2::bindless::LambertianKernel::create(*heap, *initialization);
    unlit      = fx2::bindless::UnlitKernel::create(*heap, *initialization);
    sky        = fx2::bindless::SkyKernel::create(*heap, *initialization);
    if (!pbr || !lambertian || !unlit || !sky) return false;

    const auto env = [](AutoRef<Texture> texture) {
        GpuResourceView view;
        if (texture) {
            view.resource = texture;
            view.setImageViewType(GpuResourceView::ImageView::SAMPLED);
        }
        return view;
    };
    auto skyParameters      = sky->defaultMaterialParameters();
    skyParameters.skyboxMap = env(Texture::load({.context = gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/skybox-cube.dds"}));
    skyParameters.irradianceMap =
        env(Texture::load({.context = gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/irradiance.dds"}));
    skyParameters.prefilteredMap =
        env(Texture::load({.context = gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/prefiltered.dds"}));
    skyParameters.brdfLut = env(Texture::load({.context = gpu, .filename = "media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/brdf_lut.dds"}));
    skyParameters.luminanceScale = environmentLuminance;
    skyMaterial                  = sky->createMaterial(*initialization, skyParameters);
    if (!skyMaterial) return false;

    const auto  center       = (source.bounds.minimum + source.bounds.maximum) * 0.5f;
    const auto  transform    = glm::translate(glm::mat4(1), -center);
    const float width        = std::max(glm::length(source.bounds.maximum - source.bounds.minimum) * 0.5f, 0.001f) * 0.004f;
    auto        boundsSource = ModelScene::createDebugVisualization(source.bounds, width, true, false);
    auto        axesSource   = ModelScene::createDebugVisualization(source.bounds, width, false, true);
    if (!boundsSource || !axesSource || !model.prepare(gpu, *geometryUploads, *initialization, pbr, lambertian, unlit, source, shading, transform) ||
        !bounds.prepare(gpu, *geometryUploads, *initialization, pbr, lambertian, unlit, *boundsSource, Shading::UNLIT, transform) ||
        !axes.prepare(gpu, *geometryUploads, *initialization, pbr, lambertian, unlit, *axesSource, Shading::UNLIT, transform))
        return false;

    // One-shot initialization may block: nothing is presented until every material and
    // geometry upload has landed, so later frames record draws without a producer payload.
    GpuContext::SubmitParameters submit("viewer.initialize");
    submit.appendWork(geometryUploads->seal());
    submit.appendWork(initialization->seal());
    gpu->submit(submit);
    gpu->waitForIdle();
    return true;
}

AutoRef<fx2::bindless::SharedShaderConstants::UniformState> SceneRenderer::updateUniforms(bindless::CnC & uploads, const FrameParameters & frame) {
    fx2::bindless::SharedUniforms uniforms;
    const auto                    cameraToWorld = glm::translate(glm::mat4(1), frame.eye) * glm::mat4_cast(frame.orientation);
    uniforms.viewMatrix                         = glm::inverse(cameraToWorld);
    uniforms.projMatrix =
        glm::perspectiveRH_ZO(glm::radians(frame.fovDegrees), static_cast<float>(frame.width) / std::max(1u, frame.height), frame.nearPlane, frame.farPlane);
    uniforms.projMatrix[1][1] *= -1; // Vulkan clip space
    uniforms.projViewMatrix   = uniforms.projMatrix * uniforms.viewMatrix;
    uniforms.cameraPosition   = glm::vec4(frame.eye, 1);
    uniforms.renderTargetSize = glm::vec2(static_cast<float>(frame.width), static_cast<float>(frame.height));
    uniforms.nearPlane        = frame.nearPlane;
    uniforms.farPlane         = frame.farPlane;
    uniforms.exposure         = frame.exposure;
    uniforms.frameCounter     = frame.frame;
    uniforms.frameDurationMs  = frame.frameDurationMs;
    return ssc->recordUniformUpdate(uploads, {reinterpret_cast<const uint8_t *>(&uniforms), sizeof(uniforms)});
}

size_t SceneRenderer::drawCount(bool showBounds, bool showAxes) const {
    return model.instances.size() + (showBounds ? bounds.instances.size() : 0) + (showAxes ? axes.instances.size() : 0) + 1;
}

bool SceneRenderer::recordModel(const RenderModel & source, bindless::Raster & raster,
                                const AutoRef<fx2::bindless::SharedShaderConstants::UniformState> & state) const {
    for (const auto & instance : source.instances) {
        const auto & material = source.materials[instance.material];
        RasterState  states;
        states.cullMode   = material.doubleSided ? RasterState::CULL_NONE : RasterState::CULL_BACK;
        states.frontFace  = glm::determinant(instance.transform) < 0 ? RasterState::FRONT_CW : RasterState::FRONT_CCW;
        states.depthState = RasterState::DepthState {RasterState::Compare::LESS, true};
        switch (material.shading) {
        case Shading::PBR: {
            fx2::bindless::PbrMaterial::DrawParameters draw {{raster, state, instance.geometry, &states}, instance.transform, skyMaterial};
            if (!material.pbr->record(draw)) return false;
            break;
        }
        case Shading::LAMBERTIAN: {
            fx2::bindless::LambertianMaterial::DrawParameters draw {{raster, state, instance.geometry, &states}, instance.transform};
            if (!material.lambertian->record(draw)) return false;
            break;
        }
        case Shading::UNLIT: {
            fx2::bindless::UnlitMaterial::DrawParameters draw {{raster, state, instance.geometry, &states}, instance.transform};
            if (!material.unlit->record(draw)) return false;
            break;
        }
        case Shading::IMPORTED:
        case Shading::CEL:
            return false;
        }
    }
    return true;
}

bool SceneRenderer::record(bindless::Raster & raster, const AutoRef<fx2::bindless::SharedShaderConstants::UniformState> & state, bool showBounds,
                           bool showAxes) const {
    if (!recordModel(model, raster, state)) return false;
    if (showBounds && !recordModel(bounds, raster, state)) return false;
    if (showAxes && !recordModel(axes, raster, state)) return false;
    return skyMaterial->record({raster, state});
}

} // namespace GN::viewer
