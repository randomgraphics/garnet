---
name: run-android-docker
description: Verifies Android build using the project's Docker image (randomgraphics/garnet:latest). Use when the user asks to verify Android build, run Android in docker, or when a task requires Android docker verification.
---

# Android build verification (Docker)

## When to use

Use the provided Docker image to verify that the project builds for Android. Use when a task says "Android: docker ... build.py -a d (must pass)" or when you need to confirm Android build without a local NDK/SDK.

## Command (run from repo root on Linux/macOS)

```bash
docker run -it --rm \
  -v "$(pwd):/root/project" \
  -w /root/project \
  randomgraphics/garnet:latest \
  bash -c "source env/garnet.rc && build.py -a d"
```

**Other variants:** Replace `build.py -a d` with `build.py -a p` or `build.py -a r` for profile/release.

**Run tests in same container (optional):** If the image has a Linux build, you can run lint+test after Android build:

```bash
docker run -it --rm \
  -v "$(pwd):/root/project" \
  -w /root/project \
  randomgraphics/garnet:latest \
  bash -c "source env/garnet.rc && build.py d && env/bin/cit.py -t"
```

## Requirements

- Docker installed and running.
- Repo root as current directory so `$(pwd)` mounts the project.
- Image `randomgraphics/garnet:latest` (same as used in .circleci/config.yml for build-android-*).

## Verification

Success = container runs and `build.py -a d` (or -a p / -a r) completes with exit code 0. No need to run an APK on a device unless the user asks.
