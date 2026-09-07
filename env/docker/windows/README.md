# Windows build and CIT image

Target image: `randomgraphics/garnet-windows:00001`, read from `tag.txt`.
Keep revision 00001 throughout this branch; increment only after the previous
revision has merged to master. The rebuilt full Windows Server image replaces
the earlier Server Core contents under the same tag. Clean debug/profile/release
builds and full CIT passed on 2026-09-07: each variant passed 208 unit tests,
116 internal tests (2 presentation skips), and all three GPU smoke checks.
Published and confirmed by pull: digest
`sha256:45a4d9e7e5f3a5986dfdb117769d4323b7930c56e43e6ef5bec2380752e09d9d`.

The image uses Microsoft's full `windows/server:ltsc2022` base for graphics
runtime APIs and DirectX GPU support. Use a compatible Windows Docker host,
process isolation, and a hydrated checkout on that host. Linux Docker cannot
build or run this image. Allow ample disk space for Windows and Visual Studio.

```powershell
git submodule update --init --recursive --depth 1
git lfs pull
git submodule foreach --recursive git lfs pull
./env/docker/windows/build.ps1
# Full clean build plus lint, unit/internal tests, and GPU smoke samples per variant:
./env/docker/windows/verify.ps1 -RunCit -Gpu
# Developer shell with the host's graphics devices exposed:
./env/docker/windows/launch.ps1 -Interactive -Gpu
```

GPU access is assigned when a container starts: `-Gpu` adds
`--device class/5B45201D-F2F2-4F3B-85BB-30FF1F953599`. It requires a compatible
host GPU/driver. Microsoft's supported hardware acceleration path is DirectX;
Vulkan ICD availability and execution must be verified separately. Installing
the Vulkan SDK alone does not establish a usable Vulkan device. Full CIT
runs the GPU samples without filtering or converting failures into skips.

On the local Intel UHD 770 / NVIDIA RTX 4090 host, both adapters can create
Direct3D 11 devices inside the container. Windows Docker exposes the entire
GPU interface class; it does not provide a per-adapter `--gpus device=...`
selector. Applications select among the exposed adapters. The NVIDIA Vulkan
ICD currently reports no physical devices in this container, including when
selected explicitly as ContainerUser; Vulkan workloads use Intel UHD 770.
The Intel container driver lacks `VK_KHR_swapchain`: offscreen rendering,
copy and compute work, while two existing presentation tests report skips.

The image includes VS 2026 Build Tools (stable channel) and its recommended
Windows SDK, Git 2.55.0.5, LFS 3.8.0, CMake 4.4.3, Python 3.14.7, and official
LunarG Vulkan SDK 1.4.357.0. Python requirements include Black for CIT lint.
Packages without Python 3.14 wheels build from source with MSVC; OpenEXR
3.4.15 was verified through this path. The image records resolved packages at
`C:\image\python-packages.txt` and checks Black, pip consistency, and OpenGL
runtime loading during installation. The VS channel and Python requirement
ranges float; refresh explicit installer pins for each new image revision.

`verify.ps1` runs debug, profile, and release in fresh disposable containers,
with isolated output at `C:\garnet-build`. With `-RunCit`, it runs `cit.py`
against those outputs after each build. Omitting `-RunCit` checks compilation
only. On success, ignored `verification.json` records the image ID, variants,
and whether CIT and GPU sharing were requested. `upload.ps1` checks the image
ID and tag and requires successful full CIT with GPU sharing before publication.

After full verification, publish with `upload.ps1`, pull the image, and record
its digest. Then add and verify the CircleCI Windows matrix before retiring
the existing GitHub Windows workflow. Resume from `agent/WINDOWS_DOCKER_CI.txt`.

References:
- [Microsoft container base image guidance](https://learn.microsoft.com/en-us/virtualization/windowscontainers/manage-containers/container-base-images)
- [Microsoft GPU container requirements](https://learn.microsoft.com/en-us/virtualization/windowscontainers/deploy-containers/gpu-acceleration)
- [Microsoft Build Tools containers](https://learn.microsoft.com/en-us/visualstudio/install/build-tools-container?view=visualstudio)
