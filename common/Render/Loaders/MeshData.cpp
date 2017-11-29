#include "pch.h"
#include "MeshData.h"


MeshData::MeshData()
{
}


MeshData::~MeshData()
{
    delete[] m_raw;
}


bool MeshData::Load(const char* fileName)
{
    if (m_raw)
        Clear();

    m_raw = FS_LoadFile(fileName, &m_size);
    if (!m_raw)
        return false;

    char* ptr = m_raw;

    // header pointer
    m_header = reinterpret_cast<Header*>(ptr);
    ptr += sizeof(Header);

    // validate version
    if (m_header->version != VERSION)
    {
        LogPrintf("Invalid mesh data version %u, current is %u: '%s'",
            m_header->version, VERSION, fileName);
        return false;
    }

    if (!m_header->numVerts)
        LogPrintf("Warning: mesh '%s' has no verts", fileName);

    if (!m_header->format)
        LogPrintf("Warning: mesh '%s' has zero format", fileName);

    // vertex pointer
    m_verts = ptr;
    ptr += GetVertexSize() * m_header->numVerts;

    // index pointer
    m_inds = ptr;
    ptr += GetIndexSize() * m_header->numInds;

    // surfaces pointer
    if (m_header->numSurfaces)
    {
        m_surfaces = reinterpret_cast<Surface*>(ptr);
        ptr += sizeof(Surface) * m_header->numSurfaces;
    }

    // bones pointer
    if (m_header->numBones)
    {
        m_bones = reinterpret_cast<Bone*>(ptr);
        ptr += sizeof(Bone) * m_header->numBones;
    }

    return true;
}


void MeshData::Clear()
{
    delete[] m_raw;
    m_raw = nullptr;
    m_size = 0;
    m_header = nullptr;
    m_verts = nullptr;
    m_inds = nullptr;
    m_surfaces = nullptr;
    m_bones = nullptr;
}


uint32_t MeshData::GetFormat() const
{
    return m_header ? m_header->format : 0;
}


unsigned int MeshData::GetVertexSize() const
{
    // order should match vertex format bits
    static const unsigned int componentStride[] =
    {
        3 * sizeof(float), // position
        3 * sizeof(float), // normal
        2 * sizeof(float), // texcoord
        3 * sizeof(float), // tangent
        2 * sizeof(uint32_t), // tbn
        3 * sizeof(float), // bone weight
        4 * sizeof(uint8_t), // bone index
        4 * sizeof(uint8_t), // color
    };

    static const int numComponents = ARRAY_SIZE(componentStride);

    if (!m_header)
        return 0;

    // compute entire size of the active components
    unsigned int stride = 0;
    for (int i = 0; i < numComponents; i++)
    {
        if (m_header->format & (1 << i))
            stride += componentStride[i];
    }
    return stride;
}


unsigned int MeshData::GetIndexSize() const
{
    if (!m_header)
        return 0;

    return (m_header->format & INDEX_32) ? 4 : 2;
}


const char* MeshData::GetRaw() const
{
    return m_raw;
}


size_t MeshData::GetSize() const
{
    return m_size;
}


const MeshData::Header* MeshData::GetHeader() const
{
    return m_header;
}


size_t MeshData::GetNumVerts() const
{
    return m_header ? m_header->numVerts : 0;
}


const char* MeshData::GetVerts() const
{
    return m_verts;
}


size_t MeshData::GetNumInds() const
{
    return m_header ? m_header->numInds : 0;
}


const char* MeshData::GetInds() const
{
    return m_inds;
}


size_t MeshData::GetNumSurfaces() const
{
    return m_header ? m_header->numSurfaces : 0;
}


const MeshData::Surface* MeshData::GetSurfaces() const
{
    return m_surfaces;
}


size_t MeshData::GetNumBones() const
{
    return m_header ? m_header->numBones : 0;
}


const MeshData::Bone* MeshData::GetBones() const
{
    return m_bones;
}
