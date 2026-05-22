## Raster DrawBundle and Payload optimization.

- Create a new class DrawBundle that is to record a series of draws for certain GpuRaster class. Multiple draw bundles
  can record draw calls in parallel. Once seal is called, information of all draw bundles are connected/packed together
  to become the final payload od the GpuRaster class.
- Refactor code to move most of the heavy lifting logic from Payload's record-for-submit method back to draw/dispatch/copy and seal.
  the goal is to keep payload's record-for-submit as lightweight as possible.

## Shader Hot Reload

Add `GpuShader::reload()` for live shader reloading:

- Valid only for shaders created via `GpuShader::load()` (no-op for binary-blob `create()`).
- On success, invalidates all cached PSOs that reference the shader; they are rebuilt
  transparently on the next `GpuRaster::seal()` / submit.
- Must not be called while GPU work referencing the shader is in flight.

Transient Buffer and Texture Arena
===
Need to figure out how to manage transient data life time. To avoid wasting GPU memory, transient GPU data life time need to be minimized.
- Current plan is not supporting this behavior at GPU2 level. Leave it to higher level code.
- Delay GPU memory allocation as much as possible.
- Released/Recycled automatically after the command buffer that references it is finished by GPU.
- Typical user scenarios:
  - Upload/stream data via map/unmap, one CPU copy cost (to host visible GPU memory), one GPU read cost. Buffer life time is from when buffer is mapped to rendering is done:
    ```c++
        // .
        auto mappableUploadBuffer = gpu->createStreamingBuffer();
        auto mapped = uploadBufer->map(); // this is where the GPU buffer is actually allocated and mapped.
        mapped.data = ...;
        mapped.unmap();
        gpu->copyBufferToSomething(uploadBuffer, something);
    ```
  - Upload/Stream data via callback. One CPU copy cost (to host visible GPU memory), one GPU read cost. GPU buffer time is from when rendering starts to rendering finished (minimized).
    ```c++
        auto streamingTexture = gpu->createStreamingTexture([](void * gpuBuffer, ...){
          // Copy content to gpuBuffer. Will be called right before drawTexture()
        });
        gpu->drawTexture(streamingTexture, ...);
    ```
  - Download buffer. copy GPU data back to CPU w/o pipeline stalling:
    ```c++
        auto downloadBuffer = createDownloadBuffer();
        auto id = gpu->copySomthingToBuffer(source_sources, downloadBuffer);
        id.finished([](){
            auto data = downloadBuffer->map();
            // read data here
        });
    ```