#include "pch.h"
#include "garnet/gfx/fatModel.h"
#include <set>

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.FatModel");

// *****************************************************************************
// FatVertexBuffer
// *****************************************************************************

const uint32 GN::gfx::FatVertexBuffer::MAX_TEXCOORDS = (uint32) (GN::gfx::FatVertexBuffer::TEXCOORD_LAST - GN::gfx::FatVertexBuffer::TEXCOORD0);
const uint32 GN::gfx::FatVertexBuffer::POS_NORMAL_TEX =
    (1 << GN::gfx::FatVertexBuffer::POSITION) | (1 << GN::gfx::FatVertexBuffer::NORMAL) | (1 << GN::gfx::FatVertexBuffer::TEXCOORD0);
const uint32 GN::gfx::FatVertexBuffer::POS_NORMAL_TEX_SKINNING = (1 << GN::gfx::FatVertexBuffer::POSITION) | (1 << GN::gfx::FatVertexBuffer::NORMAL) |
                                                                 (1 << GN::gfx::FatVertexBuffer::TEXCOORD0) | (1 << GN::gfx::FatVertexBuffer::JOINT_ID) |
                                                                 (1 << GN::gfx::FatVertexBuffer::JOINT_WEIGHT);

//
//
// -----------------------------------------------------------------------------
const char * const GN::gfx::FatVertexBuffer::SEMANTIC_NAMES[] = {
    "POSITION", "NORMAL",   "TANGENT",      "BINORMAL", "TEXCOORD0", "TEXCOORD1", "TEXCOORD2", "TEXCOORD3", "TEXCOORD4", "TEXCOORD5", "TEXCOORD6", "TEXCOORD7",
    "ALBEDO",   "JOINT_ID", "JOINT_WEIGHT", "CUSTOM0",  "CUSTOM1",   "CUSTOM2",   "CUSTOM3",   "CUSTOM4",   "CUSTOM5",   "CUSTOM6",   "CUSTOM7",
};
GN_CASSERT(GN_ARRAY_COUNT(GN::gfx::FatVertexBuffer::SEMANTIC_NAMES) == GN::gfx::FatVertexBuffer::NUM_SEMANTICS);

//
//
// -----------------------------------------------------------------------------
GN::gfx::FatVertexBuffer::Semantic GN::gfx::FatVertexBuffer::sString2Semantic(const char * s) {
    for (int i = 0; i < (int) NUM_SEMANTICS; ++i) {
        if (0 == str::compare(s, SEMANTIC_NAMES[i])) { return (Semantic) i; }
    }
    return INVALID;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::FatVertexBuffer::resize(uint32 layout, uint32 count) {
    if (0 == layout || 0 == count) {
        clear();
        return true;
    }

    // update data pointer and format
    for (int i = 0; i < (int) NUM_SEMANTICS; ++i) {
        if ((1 << i) & layout) {
            mElements[i].resize(count);
        } else {
            mElements[i].clear();
            mFormats[i] = PixelFormat::UNKNOWN();
        }
    }

    mLayout = layout;
    mCount  = count;

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::FatVertexBuffer::beginVertices(uint32_t layout, uint32_t estimatedCount) {
    if (!mFatVertex.empty()) {
        GN_ERROR(sLogger)("redundant call to beginVertices()");
        return false;
    }

    clear(); // cleanup existing vertices

    // reserve memory
    for (auto i = 0; i < NUM_SEMANTICS; ++i) {
        mFormats[i] = PixelFormat::UNKNOWN();
        if (0 == ((1 << i) & layout)) continue;
        mElements[i].reserve(estimatedCount);
    }
    mLayout = layout;
    mFatVertex.resize(NUM_SEMANTICS);

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::FatVertexBuffer::addVertexElement(int semantic, const VertexElement & value) {
    if (mFatVertex.empty()) {
        GN_ERROR(sLogger)("addVertexElement() can only be called between beginVertices() and endVertices().");
        return;
    }
    if (semantic < 0 || semantic >= NUM_SEMANTICS) {
        GN_ERROR(sLogger)("invalid semantic.");
        return;
    }

    mFatVertex[semantic] = value;

    if (POSITION == semantic) {
        for (int i = 0; i < NUM_SEMANTICS; ++i) {
            if (0 == ((1 << i) & mLayout)) continue;
            mElements[i].append(mFatVertex[i]);
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::FatVertexBuffer::endVertices() {
    if (mFatVertex.empty()) { GN_ERROR(sLogger)("Redundant call to endVertices()"); }
    mFatVertex.purge();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::FatVertexBuffer::GenerateMeshVertexFormat(MeshVertexFormat & mvf) const {
    mvf.clear();

    uint8 offset = 0;
    for (int i = 0; i < (int) NUM_SEMANTICS; ++i) {
        if ((1 << i) & mLayout) {
            MeshVertexElement & e = mvf.elements[mvf.numElements];

            e.format = mFormats[i];
            e.stream = 0;
            e.offset = offset;
            e.setSemantic(SEMANTIC_NAMES[i]);

            offset += e.format.bytesPerBlock();
            ++mvf.numElements;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::FatVertexBuffer::GenerateVertexStream(const MeshVertexFormat & mvf, size_t stream, size_t stride, void * buffer, size_t bufferSize) const {
    // Check parameters
    uint32 numStreams = mvf.calcNumStreams();
    if (stream >= numStreams) {
        GN_ERROR(sLogger)("Invalid stream.");
        return false;
    }

    uint32 minStride = mvf.calcStreamStride((uint32) stream);
    if (0 == stride) {
        stride = minStride;
    } else if (stride < minStride) {
        GN_ERROR(sLogger)("Stride is too small.");
        return false;
    }

    if (NULL == buffer) {
        GN_ERROR(sLogger)("Null buffer pointer.");
        return false;
    }

    size_t minBufferSize = stride * mCount;
    if (bufferSize < minBufferSize) {
        GN_ERROR(sLogger)("Buffer size is too small.");
        return false;
    }

    Semantic semantics[MeshVertexFormat::MAX_VERTEX_ELEMENTS];
    for (size_t i = 0; i < mvf.numElements; ++i) {
        const char * s = mvf.elements[i].semantic;
        if (str::isEmpty(s)) {
            s            = "[EMPTY]";
            semantics[i] = INVALID;
        } else {
            semantics[i] = sString2Semantic(s);
        }
        if (INVALID == semantics[i]) { GN_WARN(sLogger)("unsupport semantic: %s", s); }
    }

    // Copy vertex data
    for (size_t j = 0; j < mvf.numElements; ++j) {
        const MeshVertexElement & e = mvf.elements[j];

        size_t size = e.format.bytesPerBlock();
        if (0 == size) continue;

        GN_ASSERT((e.offset + size) <= stride);

        SafeArrayAccessor<uint8> dst((uint8 *) buffer + e.offset, (mCount * stride) - e.offset);

        if (semantics[j] != INVALID) {
            SafeArrayAccessor<const VertexElement> src(mElements[semantics[j]].rawptr(), mCount);

            for (size_t i = 0; i < mCount; ++i) {
                memcpy(dst.subrange(0, size), src.subrange(0, 1), size);
                src += 1;
                dst += stride;
            }
        } else {
            for (size_t i = 0; i < mCount; ++i) {
                memset(dst.subrange(0, size), 0, size);
                dst += stride;
            }
        }
    }

    return true;
}

// *****************************************************************************
// FatSkeleton
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static void sPrintFatJointRecursivly(StrA & s, const FatJoint * joints, uint32 count, uint32 root, uint32 depth) {
    for (uint32 i = 0; i < depth; ++i) { s += "  "; }

    if (root < count) {
        s += str::format("(%d) %s\n", depth, joints[root].name.rawptr());

        for (uint32 i = joints[root].child; i != FatJoint::NO_JOINT; i = joints[i].sibling) { sPrintFatJointRecursivly(s, joints, count, i, depth + 1); }
    } else {
        s += str::format("(%d) ERROR: joint index out of range: %d\n", depth, root);
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::FatSkeleton::printJointHierarchy(StrA & s) const {
    if (joints.empty()) {
        s = "[Empty skeleton]";
    } else {
        return sPrintFatJointRecursivly(s, joints.rawptr(), joints.size(), root, 0);
    }
}

// *****************************************************************************
// FatModel
// *****************************************************************************

struct SkinnedVertexKey {
    uint32 vertexIndex; //< Index into vertex array
    uint32 joints[4];   //< Remapped joint ID that associated with this vertex.

    bool operator==(const SkinnedVertexKey & rhs) const {
        return vertexIndex == rhs.vertexIndex && joints[0] == rhs.joints[0] && joints[1] == rhs.joints[1] && joints[2] == rhs.joints[2] &&
               joints[3] == rhs.joints[3];
    }

    static uint64 hash(const SkinnedVertexKey & k) {
        uint64 h = ((uint64) k.vertexIndex) | (((uint64) (k.joints[0] | (k.joints[1] << 16))) << 32) | (((uint64) (k.joints[2] | (k.joints[3] << 16))) << 48);
        return h;
    }
};

namespace std {
template<>
struct hash<SkinnedVertexKey> {
    size_t operator()(const SkinnedVertexKey & k) const { return (size_t) SkinnedVertexKey::hash(k); }
};
} // namespace std

typedef std::unordered_map<SkinnedVertexKey, uint32> SkinnedVertexMap;

//
//
// -----------------------------------------------------------------------------
static void sJointSet2JointArray(DynaArray<uint32, uint32> & jarray, const std::set<uint32> & jset) {
    jarray.clear();
    for (std::set<uint32>::const_iterator i = jset.begin(); i != jset.end(); ++i) { jarray.append(*i); }
}

//
// Split subsets in the mesh to ensure that every subset uses no more than
// specific number of joints.
//
// Return true if there's at least one subset that gets split
// -----------------------------------------------------------------------------
static bool sSplitFatMeshSubsets(FatMesh & mesh, uint32 maxJointsPerSubset) {
    // ignore mesh without skeleton
    if (FatMesh::NO_SKELETON == mesh.skeleton) return false;

    // ignore mesh without skin information in vertex.
    const uint32 * joints  = (uint32 *) mesh.vertices.getElementData(FatVertexBuffer::JOINT_ID);
    float *        weights = (float *) mesh.vertices.getElementData(FatVertexBuffer::JOINT_WEIGHT);
    if (NULL == joints || NULL == weights) return false;

    // cache mesh index buffer (might be NULL)
    uint32 * indices = mesh.indices.rawptr();

    // Temporary array to hold newly created subset.
    DynaArray<FatMeshSubset> newSubsets;

    //
    // Loop through all subsets in the mesh. See if any of them need to be
    // split into smaller pieces to meet the joint number threshold. All
    // newly created subsets are stored in array "newSubsets".
    //
    for (uint32 s = 0; s < mesh.subsets.size(); ++s) {
        // This is the subset that we're currently looping through
        FatMeshSubset & subset = mesh.subsets[s];

        // This is to see if we have created a new subset.
        FatMeshSubset * newsub = NULL;

        // Loop through each triangle in the subset. Accumulate joints on the way.
        // Whenever number of joints exceeds the threshold, creates a new subset
        // and reset the joint count.

        std::set<uint32> accumulatedJoints;

        uint32 faceCount = (indices ? subset.numidx : subset.numvtx) / 3;

        for (uint32 i = 0; i < faceCount; ++i) {
            // Collect joint of this face (triangle)
            std::set<uint32> newJoints;
            for (uint32 j = 0; j < 3; ++j) {
                uint32         vertexIndex  = subset.basevtx + (indices ? indices[subset.startidx + i * 3 + j] : (i * 3 + j));
                const uint32 * vertexJoints = joints + vertexIndex * 4;
                if (FatJoint::NO_JOINT != vertexJoints[0]) newJoints.insert(vertexJoints[0]);
                if (FatJoint::NO_JOINT != vertexJoints[1]) newJoints.insert(vertexJoints[1]);
                if (FatJoint::NO_JOINT != vertexJoints[2]) newJoints.insert(vertexJoints[2]);
                if (FatJoint::NO_JOINT != vertexJoints[3]) newJoints.insert(vertexJoints[3]);
            }

            // Merge current joints and new joints into a temporary set.
            std::set<uint32> tempJointSet(accumulatedJoints);
            tempJointSet.insert(newJoints.begin(), newJoints.end());

            // See if the size of the merged joint set exceeds the limit.
            if (tempJointSet.size() > maxJointsPerSubset) {
                //
                // There's no eough space to hold the new face in current subset. We have to
                // create a new subset and put the newJoints into it. And if it is the first
                // time we split, we also need to remember the current face index, which we
                // will use later to adjust the original subset.
                //

                if (NULL == newsub) {
                    // This is the first time we split. We need to adjust the original subset
                    // to remove all faces that will been moved to new subsets. We have
                    // remembered the orignal face count in variable "faceCount". So this should
                    // not affect the looping.
                    if (indices) {
                        GN_ASSERT((i * 3) > subset.startidx);
                        subset.numidx = i * 3 - subset.startidx;
                    } else {
                        GN_ASSERT((i * 3) > subset.basevtx);
                        subset.numvtx = i * 3 - subset.basevtx;
                    }

                    // The accumualted joint set should contain joints and only joints that are
                    // used by the subset
                    sJointSet2JointArray(subset.joints, accumulatedJoints);
                } else {
                    // We have split the subset before. It's time to finalize the previous split
                    // subset by filling in the number of vertices or indices in the subset.
                    if (indices) {
                        newsub->numidx = i * 3 - newsub->startidx;
                    } else {
                        newsub->numvtx = i * 3 - newsub->basevtx;
                    }

                    // copy accumulated joints to the new subset.
                    sJointSet2JointArray(newsub->joints, accumulatedJoints);
                }

                // Now, create a new subset.
                newSubsets.resize(newSubsets.size() + 1);
                newsub = &newSubsets.back();

                // Initialize the new subset. Note that we don't know the final vertex and
                // index count yet.
                newsub->material = subset.material;
                if (indices) {
                    newsub->basevtx  = subset.basevtx;
                    newsub->numvtx   = subset.numvtx;
                    newsub->startidx = i * 3;
                    newsub->numidx   = 0xbadbeef; // don't know this yet.
                } else {
                    newsub->basevtx  = i * 3;
                    newsub->numvtx   = 0xbadbeef; // don't know this yet.
                    newsub->startidx = 0;
                    newsub->numidx   = 0;
                }

                // We just split a new subset. So we need to reset the joint set to start
                // a new round of accumulation towards the joint count threshold.
                accumulatedJoints = newJoints;
            } else {
                // Not yet. Let's add the new joints into accumulated joint set then continue.
                accumulatedJoints = tempJointSet;
            }
        }

        // If we have split the subset at least once, ...
        if (NULL != newsub) {
            // So we need to finalize the last new subset, in the same way we finalize other
            // subsets.
            if (indices) {
                newsub->numidx = faceCount * 3 - newsub->startidx;
            } else {
                newsub->numvtx = faceCount * 3 - newsub->basevtx;
            }

            // copy accumulated joints to the new subset.
            sJointSet2JointArray(newsub->joints, accumulatedJoints);
        }
    }

    // We have gone through all subsets in the mesh and make sure every single of them meet
    // the joint threshold (anyone that doesn't has been split). Now it is time to add all
    // newly created subsets back to the mesh.
    mesh.subsets.append(newSubsets);

    return !newSubsets.empty();
}

//
//
// -----------------------------------------------------------------------------
static int sCompareUInt32(const void * a, const void * b) {
    uint32 ia = *(const uint32 *) a;
    uint32 ib = *(const uint32 *) b;

    if (ia < ib)
        return -1;
    else if (ia == ib)
        return 0;
    else
        return 1;
}

//
// Remap joint ID: joint id -> array index.
// The function assums that the joint ID does exist in the array,
// or is FatJoint::NO_JOINT.
// -----------------------------------------------------------------------------
static inline uint32 sRemapJoint(const DynaArray<uint32, uint32> & joints, uint32 id) {
    if (FatJoint::NO_JOINT == id) return FatJoint::NO_JOINT;

    const uint32 * begin    = joints.begin();
    const uint32 * end      = joints.end();
    const uint32 * iterator = std::lower_bound(begin, end, id);

    // Make sure we did find it.
    GN_ASSERT(begin <= iterator && iterator < end);

    return (uint32) (iterator - begin);
}

//
//
// -----------------------------------------------------------------------------
static bool sRemapFatMeshJointID(FatMesh & mesh) {
    // Ignore non skinned mesh
    uint32 * originalJoints = (uint32 *) mesh.vertices.getElementData(FatVertexBuffer::JOINT_ID);
    if (NULL == originalJoints) return true;

    bool indexedMesh = !mesh.indices.empty();

    SkinnedVertexMap vmap(indexedMesh ? mesh.indices.size() : mesh.vertices.getVertexCount());

    DynaArray<SkinnedVertexKey, uint32> newVertices;
    DynaArray<uint32, uint32>           newIndices;

    for (uint32 isubset = 0; isubset < mesh.subsets.size(); ++isubset) {
        // Refernece the subset that we're currently working on
        FatMeshSubset & subset = mesh.subsets[isubset];

        // Sort joint array of the subset. So that we can binary search it later.
        qsort(subset.joints.rawptr(), subset.joints.size(), sizeof(uint32), sCompareUInt32);

        // Remember how many new indices we have now. We'll use that later.
        uint32 currentNewIndexCount = newIndices.size();

        // Loop throuhg all faces.
        uint32 start, end;
        if (indexedMesh) {
            start = subset.startidx;
            end   = subset.startidx + subset.numidx;
        } else {
            start = subset.basevtx;
            end   = subset.basevtx + subset.numvtx;
        }
        for (uint32 i = start; i < end; ++i) {
            uint32 vertexIndex;

            if (indexedMesh) {
                vertexIndex = mesh.indices[i] + subset.basevtx;
            } else {
                vertexIndex = i;
            }

            SkinnedVertexKey key;
            key.vertexIndex = vertexIndex;
            key.joints[0]   = sRemapJoint(subset.joints, originalJoints[vertexIndex * 4 + 0]);
            key.joints[1]   = sRemapJoint(subset.joints, originalJoints[vertexIndex * 4 + 1]);
            key.joints[2]   = sRemapJoint(subset.joints, originalJoints[vertexIndex * 4 + 2]);
            key.joints[3]   = sRemapJoint(subset.joints, originalJoints[vertexIndex * 4 + 3]);

            // Is this an existing key or new key?
            auto inserted            = vmap.insert({key, (uint32) vmap.size()});
            auto isNewVertex         = inserted.second;
            auto remappedVertexIndex = inserted.first->second;
            if (isNewVertex) {
                // If it is a new key, add it to the new vertex array.
                GN_ASSERT(newVertices.size() == remappedVertexIndex);
                newVertices.append(key);
            }

            // Store remapped vertex index into index buffer.
            newIndices.append(remappedVertexIndex);
        }

        // We've iterate through all vertices in the subset. Let's see how many new
        // indices we gathered for this subset. And setup the index range of the
        // subset accordingly
        subset.startidx = currentNewIndexCount;
        subset.numidx   = newIndices.size() - currentNewIndexCount;
    }

    // We have looped through all subsets in the mesh. And gathered all new vertices and indices.
    // It's time to update vertex count information for each subset.
    for (uint32 isubset = 0; isubset < mesh.subsets.size(); ++isubset) {
        FatMeshSubset & subset = mesh.subsets[isubset];
        subset.basevtx         = 0;
        subset.numvtx          = newVertices.size();
    }

    // Now let's make a new vertex buffer using remaped joint ID.
    FatVertexBuffer newvb;
    if (!newvb.resize(mesh.vertices.getLayout(), newVertices.size())) {
        GN_ERROR(sLogger)("Out of memory.");
        return false;
    }
    for (uint32 s = 0; s < FatVertexBuffer::NUM_SEMANTICS; ++s) {
        const uint32 * oldval = (const uint32 *) mesh.vertices.getElementData(s);
        if (NULL == oldval) continue;

        newvb.setElementFormat(s, mesh.vertices.getElementFormat(s));

        uint32 * newval = (uint32 *) newvb.getElementData(s);
        GN_ASSERT(newval);

        if (s == FatVertexBuffer::JOINT_ID) {
            // Use remapped joint ID in new VB.
            for (uint32 i = 0; i < newVertices.size(); ++i) {
                const SkinnedVertexKey & key = newVertices[i];

                uint32 newOffset      = i * 4;
                newval[newOffset + 0] = key.joints[0];
                newval[newOffset + 1] = key.joints[1];
                newval[newOffset + 2] = key.joints[2];
                newval[newOffset + 3] = key.joints[3];
            }
        } else {
            // If it is not joint ID, just copy it from old VB
            for (uint32 i = 0; i < newVertices.size(); ++i) {
                const SkinnedVertexKey & key = newVertices[i];

                GN_ASSERT(key.vertexIndex < mesh.vertices.getVertexCount());

                uint32 oldOffset = key.vertexIndex * 4;
                uint32 newOffset = i * 4;

                newval[newOffset + 0] = oldval[oldOffset + 0];
                newval[newOffset + 1] = oldval[oldOffset + 1];
                newval[newOffset + 2] = oldval[oldOffset + 2];
                newval[newOffset + 3] = oldval[oldOffset + 3];
            }
        }
    }

    // Replace mesh vb and ib with new data. And we are done.
    mesh.vertices = std::move(newvb);
    mesh.indices  = std::move(newIndices);

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::FatModel::calcBoundingBox() {
    bbox.clear();
    for (uint32 i = 0; i < meshes.size(); ++i) { Boxf::sGetUnion(bbox, bbox, meshes[i].bbox); }
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::FatModel::splitSkinnedMesh(uint32 maxJointsPerSubset) {
    GN_SCOPE_PROFILER(FatModel_splitSkinnedMesh, "Split skinned mesh in FatModel.");

    if (maxJointsPerSubset < 12) {
        GN_ERROR(sLogger)("The minimal MAX_JOINTS_PER_SUBSET is 12.");
        maxJointsPerSubset = 12;
    }

    //
    // Loop through all meshes
    //
    for (uint32 i = 0; i < meshes.size(); ++i) {
        auto & mesh = meshes[i];

        // Split subsets in the mesh to ensure that every subset uses no more than
        // specific number of joints. Create new subsets when necessary.
        sSplitFatMeshSubsets(mesh, maxJointsPerSubset);

        // Now remap joint ID for all vertices in the mesh. When this function fails,
        // the mesh could be left in undefined state.
        if (!sRemapFatMeshJointID(mesh)) { return false; }
    }

    // We have gone through all meshes. Job is well done.
    return true;
}
