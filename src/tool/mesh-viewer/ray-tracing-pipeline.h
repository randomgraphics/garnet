#pragma once

#include <garnet/GNfx2.h>

namespace GN::viewer {

/// Owns the mesh-viewer's FX2 ray tracing kernel and its prepared PBR scene.
///
/// The viewer supplies its existing geometry and materials once, then records each
/// visible mesh through record(). GPU payload creation and scene acceleration details
/// stay out of the application loop.
class RayTracingPipeline {
    AutoRef<fx2::RayTraceKernel>     mKernel;
    AutoRef<fx2::PbrRayTracingScene> mScene;
    AutoRef<gpu2::Texture>           mColor;
    AutoRef<fx2::UnlitMaterial>      mCompositeMaterial;
    gpu2::RasterGeometry             mCompositeGeometry;
    uint32_t                         mWidth = 0, mHeight = 0, mSampleCount = 0, mSampleLimit = 256;

public:
    /// Prepare a ray tracing scene from the same PBR objects used by raster rendering.
    /// Returns false when the scene is empty, contains a non-PBR material, or FX2 cannot
    /// create its acceleration structure. The caller submits the upload recorder after setup.
    bool prepare(gpu2::DescriptorHeap & heap, gpu2::CnC & uploads, const DynaArray<fx2::PbrRayTracingScene::Mesh> & meshes);

    /// Ensure persistent offscreen targets and the fullscreen composite are sized for this frame.
    bool resize(AutoRef<gpu2::GpuContext> gpu, gpu2::DescriptorHeap & heap, gpu2::CnC & uploads, uint32_t width, uint32_t height);

    /// Whether prepare() produced a usable scene and kernel.
    bool available() const { return mKernel && mScene; }

    /// Add the prepared scene's typed pass bindings to FX2's shared uniform resources.
    void bindSceneResources(gpu2::GpuResourceTable & resources) const;

    /// Configure the offscreen target with the viewer's same-size depth texture. Camera
    /// movement clears prior samples; a stationary camera blends to the quality cap.
    bool configureTarget(gpu2::RasterTarget & target, const AutoRef<gpu2::Texture> & sharedDepth, bool cameraMoved);

    /// Whether another sample is needed for the current camera view.
    bool shouldTrace() const { return available() && mSampleCount < mSampleLimit; }

    /// Set the maximum accumulated samples for each unchanged view.
    /// A zero limit disables ray tracing; callers should provide a positive value.
    void setSampleLimit(uint32_t sampleLimit) { mSampleLimit = sampleLimit; }

    /// Number of completed samples for the current camera view.
    uint32_t sampleCount() const { return mSampleCount; }

    /// Discard accumulation when another rendering mode replaces the shared depth contents.
    void resetAccumulation() { mSampleCount = 0; }

    /// Mark one successfully sealed ray-tracing sample as complete.
    void finishSample() {
        if (mSampleCount < mSampleLimit) ++mSampleCount;
    }

    /// Draw the last ray-traced sample image as a fullscreen surface.
    bool recordComposite(gpu2::Raster & raster, const AutoRef<fx2::SharedShaderConstants::UniformState> & uniforms, const glm::mat4 & inverseViewProjection,
                         fx2::UnlitKernel & kernel) const;

    /// Append one mesh's ray traced PBR draw to the caller's raster pass.
    bool record(gpu2::Raster & raster, const AutoRef<fx2::SharedShaderConstants::UniformState> & uniforms, const gpu2::RasterGeometry & geometry,
                const gpu2::RasterState * state, const glm::mat4 & transform, const AutoRef<fx2::SkyMaterial> & sky,
                const AutoRef<fx2::PbrMaterial> & material) const;
};

} // namespace GN::viewer
