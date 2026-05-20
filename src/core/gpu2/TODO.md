Optimize GpuRaster and GpuCnc Payload
===
Need to move majority of work to draw/dispatch/copy() and seal(). Leave only minimal amount of work in the final record function,since draw/dispatch/copy/() and seal() are thread safe and can be called in parallel. The record method of the Payload, on the other hand, must run in serialized manner.

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