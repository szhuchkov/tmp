#pragma once


class MeshData
{
public:
    static const uint32_t VERSION = 100;

    static const uint32_t VERTEX_POSITION = (1 << 0);
    static const uint32_t VERTEX_NORMAL = (1 << 1);
    static const uint32_t VERTEX_TEXCOORD = (1 << 2);
    static const uint32_t VERTEX_TANGENT = (1 << 3);
    static const uint32_t VERTEX_TBN = (1 << 4);
    static const uint32_t VERTEX_BONE_WEIGHT = (1 << 5);
    static const uint32_t VERTEX_BONE_INDEX = (1 << 6);
    static const uint32_t VERTEX_COLOR = (1 << 7);

    static const uint32_t INDEX_32 = (1 << 20);

    static const uint32_t INVALID_MATERIAL = ~0UL;

    struct Header
    {
        uint32_t version;
        uint32_t format;
        uint32_t numVerts;
        uint32_t numInds;
        uint32_t numSurfaces;
        uint32_t numBones;
    };

    struct Surface
    {
        uint32_t vertexOffset;
        uint32_t vertexCount;
        uint32_t indexOffset;
        uint32_t indexCount;
        uint32_t materialIndex;
    };

    struct BoneTransform
    {
        float orientation[3][3];
        float translation[3];
    };

    struct Bone
    {
        char name[128];
        BoneTransform initialPose;
    };

    MeshData();
    ~MeshData();

    bool Load(const char* name);
    void Clear();

    const char* GetRaw() const;
    size_t GetSize() const;
    const Header* GetHeader() const;

    unsigned int GetVertexSize() const;
    unsigned int GetIndexSize() const;

    uint32_t GetFormat() const;

    size_t GetNumVerts() const;
    const char* GetVerts() const;

    size_t GetNumInds() const;
    const char* GetInds() const;

    size_t GetNumSurfaces() const;
    const Surface* GetSurfaces() const;

    size_t GetNumBones() const;
    const Bone* GetBones() const;

    bool HasNormals() const;
    bool HasTangents() const;
    bool HasBones() const;

private:
    char* m_raw = nullptr;
    size_t m_size = 0;
    Header* m_header = nullptr;
    char* m_verts = nullptr;
    char* m_inds = nullptr;
    Surface* m_surfaces = nullptr;
    Bone* m_bones = nullptr;
};
