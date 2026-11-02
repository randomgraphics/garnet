---
name: garnet-android-docker
description: Verify Garnet Android builds with the project Docker image. Use when Android build verification is requested or required by an assignment.
---

# Garnet Android Docker Build

Use the project Docker image when local Android SDK/NDK setup is unavailable.

From the repo root:

```bash
docker run -it --rm \
  -v "$(pwd):/root/project" \
  -w /root/project \
  randomgraphics/garnet:00002 \
  bash -c "source env/garnet.rc && build.py -a d"
```

Use `-a p` or `-a r` for profile/release Android variants. Docker must be
installed and running. Success means the container command exits with status 0.

The image tag is pinned to [`env/docker/ubuntu/tag.txt`](../../../env/docker/ubuntu/tag.txt)
and should be updated together with that file when the project image changes.
See [`agent/knowledge/build-and-platforms.md`](../../knowledge/build-and-platforms.md)
for the configured Android API level and the distinction between CI configuration
and a verified build result.
