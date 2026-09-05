# Media assets

Large resource groups live in the [Asset Foundry dataset](https://huggingface.co/datasets/chenlirandom/asset-foundry), mounted here as the shallow `asset-foundry` Git submodule. The Garnet commit pins the dataset revision.

Initialize assets with Git LFS installed:

```bash
git lfs install
git submodule update --init --recursive
```

If the checkout was created with LFS downloads disabled, materialize this dataset explicitly:

```bash
git -C media/asset-foundry lfs pull
```

These assets download from Hugging Face. Other Garnet LFS files and existing GitHub submodules still download from their respective origins.

| Resource group | Dataset path relative to `media/asset-foundry` |
| --- | --- |
| Bad Salzbrunn environment map, source EXR, and generated IBL maps | `image/envmap/bad-salzbrunn-walking-hall/` |
| DamagedHelmet mesh, material, and all textures | `model/DamagedHelmet/` |
| Humanoid ASCII and binary FBX models | `model/humanoid/` |
| Complete font collection | `font/` |
| All current digital-forge content, including the Speeder Getaway model | `model/character/`, `LICENSE`, and `provenance/digital-forge/` |

Use paths such as `media::asset-foundry/image/envmap/bad-salzbrunn-walking-hall/skybox-cube.dds` and `media::asset-foundry/model/DamagedHelmet/DamagedHelmet.gltf`. The `font::` filesystem also searches `media::asset-foundry/font`, so existing font names keep working.

The dataset's `provenance/migration.json` records source commits, original paths, byte sizes, and SHA-256 hashes. Related resources move as a group so that glTF and material-relative references remain valid. Small unrelated assets remain in this repository.
