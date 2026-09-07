# Windows build image (draft, unverified)

Target image: `randomgraphics/garnet-windows:00001`, read from `tag.txt`.
No image has been built or published yet. Continue from
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
powershell -ExecutionPolicy Bypass -File dev/docker/windows/build.ps1
powershell -ExecutionPolicy Bypass -File dev/docker/windows/verify.ps1
docker login
powershell -ExecutionPolicy Bypass -File dev/docker/windows/upload.ps1
```

The image preinstalls VS 2022 C++ Build Tools, Windows SDK 22621, Git with LFS,
CMake 3.31.6, Python 3.12.10, the repo Python requirements in a venv, and Vulkan
from `env/vulkan-sdk-version.txt`. The VS bootstrapper and Python requirement
ranges are floating; image revisions identify the resulting environment.
Review installer versions and checksums before treating this draft as final.

`verify.ps1` runs debug, profile and release using the project build wrapper,
with fresh output directories inside disposable containers. On success it
writes ignored `verification.json`; `upload.ps1` checks that its image ID and
tag still match. This verifies compilation, matching existing Windows CI.
GPU runtime tests require additional host/container graphics support and are
not claimed by this check. The checkout and its assets must be hydrated first.

```powershell
# Pull the published revision on another Windows host.
./dev/docker/windows/pull.ps1
# Open a developer shell, then initialize the Garnet environment if needed.
./dev/docker/windows/launch.ps1 -Interactive
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
