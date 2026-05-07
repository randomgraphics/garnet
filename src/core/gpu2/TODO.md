Optimize GpuRaster Payload
===
Need to move majority of work to draw() and seal(). Leave only minimal amount of work in the final record function,since draw() and seal() are thread safe and can be called in parallel. The record method of the Payload, on the other hand, must run in serialized manner.

PSO Cache/Factory
===
GpuRaster should use a PSO Cache/Factory to minimize cost of creating PSO.


Transient Buffer and Texture Arena
===
Need to figure out how to manage transient data life time. To avoid wasting GPU memory, transient GPU data life time need to be minimized:
- Delay-allocated right before it is being using by rendering
- Released/Recycled right after the command buffer that references it is finished by GPU.
