#ifndef __GN_GFX_ASE_H__
#define __GN_GFX_ASE_H__
// *****************************************************************************
/// \file
/// \brief   ASE loader
/// \author  chenli@@REDMOND (2009.1.16)
// *****************************************************************************

namespace GN {
namespace gfx {
///
/// ASE map
///
struct AseMap {
    std::string     name;
    std::string     class_;
    int      subno;
    float    amount;
    std::string     bitmap;
    std::string     type;
    Vector2f offset;
    Vector2f tiling;
    float    angle;
    float    blur;
    float    blur_offset;
    float    noise_amt;
    float    noise_size;
    int      noise_level;
    float    noise_phase;
    std::string     filter;
};

///
/// ASE material
///
struct AseMaterial {
    std::string     name;
    std::string     class_;
    Vector3f ambient, diffuse, specular;
    float    shine;
    float    shinestrength;
    float    transparency;
    float    wiresize;
    std::string     shading;
    float    xp_falloff;
    float    selfillum;
    std::string     falloff;
    std::string     xp_type;
    AseMap   mapdiff;
    AseMap   mapbump;
};

struct AseMesh : public gfx::MeshResourceDesc {
    std::string      parent;    ///< parent mesh
    std::string      name;      ///< mesh name
    Matrix44f transform; ///< transformation
    Vector3f  pos;       ///< position
    Vector3f  rotaxis;   ///< rotate axis
    float     rotangle;  ///< rotation angle in radian
    Vector3f  scale;     ///< scale factors
    Boxf      selfbbox;  ///< bounding box of myself
    Boxf      treebbox;  ///< bounding box of myself and my children.
};

///
/// ase mesh subset
///
struct AseMeshSubset : public gfx::MeshResourceSubset {
    uint32_t matid;  ///< index into AseScene::materials array
    uint32_t meshid; ///< index into AseScene::meshes array
};

///
/// ASE scene. Include multiple materials and meshes, no animation.
///
struct AseScene : public NoCopy {
    DynaArray<AseMaterial>   materials;
    DynaArray<AseMesh>       meshes;
    DynaArray<AutoRef<Blob>> meshdata; ///< store mesh data
    DynaArray<AseMeshSubset> subsets;
    Boxf                     bbox; ///< bounding box of the whole scene

    /// dtor
    ~AseScene() { clear(); }

    /// clear the scene
    void clear();

    /// load ASE scene from file. Exising content in the scene will be discarded.
    bool loadFromFile(File & file);
};
} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_ASE_H__
