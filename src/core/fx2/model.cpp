#include "pch.h"

GN::fx2::ModelScene::SourceFormat GN::fx2::ModelScene::sourceFormatFromPath(const StrA & path) {
    StrA extension = fs::extName(path);
    extension.toLower();

    if (extension == ".fbx") return SourceFormat::FBX;
    if (extension == ".gltf") return SourceFormat::GLTF;
    if (extension == ".glb") return SourceFormat::GLB;
    if (extension == ".stl") return SourceFormat::STL;
    return SourceFormat::UNKNOWN;
}
