Optimize GpuRaster Payload
===
Need to move majority of work to draw() and seal(). Leave only minimal amount of work in the final record function,since draw() and seal() are thread safe and can be called in parallel. The record method of the Payload, on the other hand, must run in serialized manner.

Transient Buffer and Texture Arena
===
Need to figure out how to manage transient data life time. To avoid wasting GPU memory, transient GPU data life time need to be minimized:
- Delay-allocated right before it is being using by rendering
- Released/Recycled right after the command buffer that references it is finished by GPU.
- A natual place is the playload class: it can be the one to allocate transcient buffer
- instead of generic transcient buffer, define dedicated classes for particular usage of transient data. such as:
  - upload buffer: upload data from CPU to GPU, mean to be read no more than once by GPU. common case is to copy data to another more permanent resource (texture or buffer).
    ```c++
        auto uploadBuffer = gpu->createUploadBuffer();
        auto mapped = uploadBufer->map();
        mapped.data = ...;
        mapped.unmap();
        gpu->copyBufferToSomething(uploadBuffer, something);
    ```
  - one time streaming data:
    ```c++
        auto streamingTexture = gpu->createStreamingTexture();
        auto mapped = streamingTexture->map();
        mapped.data = ...;
        mapped.unmap();
        gpu->drawTexture(streamingTexture, ...);
    ```
  - download buffer. copy GPU data back to CPU w/o pipeline stalling:
    ```c++
        auto downloadBuffer = createDownloadBuffer();
        auto id = gpu->copySomthingToBuffer(source_sources, downloadBuffer);
        id.finished([](){
            auto data = downloadBuffer->map();
            // read data here
        });
        gpu->pump();
    ```