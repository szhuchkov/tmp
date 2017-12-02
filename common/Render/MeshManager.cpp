#include "pch.h"
#include "MeshManager.h"
#include "RenderDevice.h"
#include "Loaders/MeshData.h"


MeshManager::MeshManager()
{
}


MeshManager::~MeshManager()
{
}


MeshInstance* MeshManager::LoadMesh(const char* name)
{
    //  find existing
    for (auto item : m_meshNodes)
    {
        if (item->name == name)
        {
            item->instances++;
            return &item->mesh;
        }
    }

    //  alloc new
    MeshNode* node = new MeshNode();
    node->name = name;
    node->instances = 1;
    node->mesh.verts = nullptr;
    node->mesh.inds = nullptr;
    node->mesh.primitive = RenderDevice::PRIMITIVE_TRIANGLES;
    node->mesh.instanceID = m_nextInstanceID++;
    m_meshNodes.push_back(node);

    // load data
    MeshData data;
    if (data.Load(name))
    {
        node->mesh.verts = RenderDevice::GetInstance()->CreateVertexBuffer(data.GetFormat(), data.GetNumVerts(), data.GetVerts());
        node->mesh.inds = RenderDevice::GetInstance()->CreateIndexBuffer(data.GetFormat(), data.GetNumInds(), data.GetInds());
        node->mesh.surfaces.resize(data.GetNumSurfaces());
        node->mesh.materialIndexes.resize(data.GetNumSurfaces());
        for (size_t i = 0; i < data.GetNumSurfaces(); i++)
        {
            auto src = data.GetSurfaces() + i;
            auto dst = &node->mesh.surfaces[i];

            dst->vertexOffset = src->vertexOffset;
            dst->vertexCount = src->vertexCount;
            dst->indexOffset = src->indexOffset;
            dst->indexCount = src->indexCount;
            dst->material = nullptr;

            node->mesh.materialIndexes[i] = src->materialIndex;
        }
    }
    else
    {
        LogPrintf("Unable to load mesh '%s'", name);
    }

    return &node->mesh;
}


void MeshManager::FreeMesh(MeshInstance* mesh)
{
    for (size_t i = 0; i < m_meshNodes.size(); i++)
    {
        auto item = m_meshNodes[i];
        if (mesh == &item->mesh)
        {
            item->instances--;
            if (!item->instances)
            {
                RenderDevice::GetInstance()->DeleteVertexBuffer(item->mesh.verts);
                RenderDevice::GetInstance()->DeleteIndexBuffer(item->mesh.inds);
                m_meshNodes[i] = m_meshNodes.back();
                delete item;
            }

            break;
        }
    }
}