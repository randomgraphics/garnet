# Windows build image

Target image: `randomgraphics/garnet-windows:00001`, read from `tag.txt`.
The image passed debug, profile and release container builds on 2026-09-07.
Published and confirmed by pull on the same date. Registry digest:
`sha256:011aa786856c4f07152157d10d5f8c7db35f5d50529d632daafae8c3aab8e12d`.
Continue from
`agent/WINDOWS_DOCKER_CI.txt` on a Windows host.

Use a Windows Server 2022 Docker engine in Windows container mode. The scripts
use process isolation and bind-mount the checkout, so the checkout must exist
on the Docker host. Linux Docker cannot build or run this image.
Allow ample disk space for Windows, Visual Studio and image layers.

From a Windows checkout with Git and Git LFS available on the host:

```powershell
git submodule update --init --recursive --depth 1
git lfs pull
git submodule foreach --recursive git lfs pull
powershell -ExecutionPolicy Bypass -File env/docker/windows/build.ps1
powershell -ExecutionPolicy Bypass -File env/docker/windows/verify.ps1
docker login
powershell -ExecutionPolicy Bypass -File env/docker/windows/upload.ps1
```

The image targets current stable releases checked on 2026-09-07: VS 2026
Build Tools (stable channel) with its recommended Windows SDK, Git 2.55.0.5
with LFS 3.8.0, CMake 4.4.3, Python 3.14.7, and LunarG Vulkan SDK 1.4.357.0.
Python packages resolve to the latest compatible versions from the repository
requirements. Packages without Python 3.14 wheels build from source using MSVC;
OpenEXR currently needs this path. Resolved packages are recorded inside the
image at `C:\image\python-packages.txt`. Installation runs `pip check`.
Dependency installation and Python package checks pass in Windows Server Core.
The VS channel and Python requirement ranges float; image revisions identify
the resulting environment. Refresh explicit installer pins for new revisions.
Vulkan uses the official unattended installer and lives at `C:\VulkanSDK\current`;
its Dockerfile default is independent of the older SDK pin used by existing CI.

`verify.ps1` runs debug, profile and release using the project build wrapper,
with fresh output directories inside disposable containers. On success it
writes ignored `verification.json`; `upload.ps1` checks that its image ID and
tag still match. This verifies compilation, matching existing Windows CI.
GPU runtime tests require additional host/container graphics support and are
not claimed by this check. The checkout and its assets must be hydrated first.

```powershell
# Pull the published revision on another Windows host.
./env/docker/windows/pull.ps1
# Open a developer shell, then initialize the Garnet environment if needed.
./env/docker/windows/launch.ps1 -Interactive
# Within the container:
. env\garnet.ps1
```

After publication, add a debug/profile/release matrix to `.circleci/config.yml`
using `windows-server-2022-gui:current`, `windows.medium`, and PowerShell.
The VM checks out and hydrates the repository, runs `pull.ps1`, then runs
`launch.ps1` with `ci-build.ps1 -Variant <variant>`. Check native command exit
codes explicitly in PowerShell. Keep the existing GitHub Windows workflow
until the CircleCI jobs pass, then retire it.

References:
- [CircleCI Windows container execution](https://circleci.com/docs/guides/execution-managed/using-windows/)
- [Microsoft Build Tools containers](https://learn.microsoft.com/en-us/visualstudio/install/build-tools-container?view=visualstudio)
