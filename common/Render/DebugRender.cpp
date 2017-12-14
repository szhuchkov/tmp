#include "pch.h"
#include "DebugRender.h"
#include "RenderDevice.h"
#include "ShaderManager.h"
#include <Render/BV/AABB.h>
#include <Render/BV/BoundingBox.h>


DebugRender::DebugRender()
{
}


DebugRender::~DebugRender()
{
}


bool DebugRender::Init()
{
    // init vertex buffer
    m_verts = RenderDevice::GetInstance()->CreateVertexBuffer(
        RenderDevice::VERTEX_POSITION | RenderDevice::VERTEX_TEXCOORD |
        RenderDevice::VERTEX_COLOR | RenderDevice::VERTEX_DYNAMIC,
        MAX_VERTS, nullptr);
    if (!m_verts)
    {
        LogPrintf("Unable to create vertex buffer");
        return false;
    }

    // init index buffer
    m_inds = RenderDevice::GetInstance()->CreateIndexBuffer(
        RenderDevice::INDEX_32 | RenderDevice::INDEX_DYNAMIC, MAX_INDS, nullptr);
    if (!m_inds)
    {
        LogPrintf("Unable to create index buffer");
        return false;
    }

    // load font shader
    m_fontShader = ShaderManager::GetInstance()->GetShader("DebugShape.vs", "DebugTex.ps");
    if (!m_fontShader)
    {
        LogPrintf("Unable to load font shader");
        return false;
    }

    // load quad shader
    m_quadShader = ShaderManager::GetInstance()->GetShader("DebugShape.vs", "DebugTex.ps");
    if (!m_quadShader)
    {
        LogPrintf("Unable to load quad shader");
        return false;
    }

    // load shape shader
    m_shapeShader = ShaderManager::GetInstance()->GetShader("DebugShape.vs", "DebugColor.ps");
    if (!m_shapeShader)
    {
        LogPrintf("Unable to load line2D shader");
        return false;
    }

    return true;
}


void DebugRender::Shutdown()
{
    RenderDevice::GetInstance()->DeleteVertexBuffer(m_verts);
    m_verts = nullptr;

    RenderDevice::GetInstance()->DeleteIndexBuffer(m_inds);
    m_inds = nullptr;
}


void DebugRender::Render()
{
    if (m_nodes.empty() ||
        m_arrayVerts.empty() ||
        m_arrayInds.empty())
        return;

    auto device = RenderDevice::GetInstance();

    // update buffers
    device->UpdateVertexBuffer(m_verts, &m_arrayVerts[0], 0, sizeof(DrawVertex) * m_arrayVerts.size());
    device->UpdateIndexBuffer(m_inds, &m_arrayInds[0], 0, sizeof(uint32_t) * m_arrayInds.size());

    // bind buffer
    device->SetVertexBuffer(m_verts);
    device->SetIndexBuffer(m_inds);
    device->SetMatrix(RenderDevice::MATRIX_WORLD, Matrix::IDENTITY);

    // draw debug nodes
    for (auto& item : m_nodes)
    {
        // common settings
        if (item.transform)
            device->SetMatrix(RenderDevice::MATRIX_VIEW_PROJECTION, m_viewProj);
        else
            device->SetMatrix(RenderDevice::MATRIX_VIEW_PROJECTION, Matrix::IDENTITY);

        // node settings
        unsigned int primitiveType = RenderDevice::PRIMITIVE_POINTS;
        switch (item.type)
        {
        case DRAW_LINES:
            {
                primitiveType = RenderDevice::PRIMITIVE_LINES;
                device->SetShader(m_shapeShader);
                device->SetTexture(0, nullptr);
            }
            break;
        case DRAW_TRIANGLES:
            {
                primitiveType = RenderDevice::PRIMITIVE_TRIANGLES;
                device->SetShader(m_quadShader);
                device->SetTexture(0, item.texture);
            }
            break;
        case DRAW_FONT:
            {
                primitiveType = RenderDevice::PRIMITIVE_TRIANGLES;
                device->SetShader(m_fontShader);
                device->SetTexture(0, m_fontTexture);
            }
            break;
        default:
            break;
        }

        // draw
        device->DrawIndexedPrimitive(primitiveType, item.offsetVerts, item.numVerts, item.offsetInds, item.numInds);
    }

    // clear cache
    m_nodes.clear();
    m_arrayVerts.clear();
    m_arrayInds.clear();
}


void DebugRender::SetViewProj(const Matrix& viewProj)
{
    m_viewProj = viewProj;
}


DebugRender::DrawNode* DebugRender::AllocNode()
{
    m_nodes.resize(m_nodes.size() + 1);
    return &(m_nodes.back());
}


DebugRender::DrawVertex* DebugRender::AllocVerts(size_t count)
{
    size_t offset = m_arrayVerts.size();
    m_arrayVerts.resize(offset + count);
    return &m_arrayVerts[offset];
}


uint32_t* DebugRender::AllocInds(size_t count)
{
    size_t offset = m_arrayInds.size();
    m_arrayInds.resize(offset + count);
    return &m_arrayInds[offset];
}


void DebugRender::DrawLine2D(const Vector2& p1, const Vector2& p2, uint32_t color)
{
    auto node = AllocNode();
    node->type = DRAW_LINES;
    node->transform = false;
    node->texture = nullptr;
    node->offsetVerts = m_arrayVerts.size();
    node->offsetInds = m_arrayInds.size();
    node->numVerts = 2;
    node->numInds = 2;

    auto verts = AllocVerts(2);
    verts[0].pos = Vector3(p1.x, p1.y, 0);
    verts[0].color = color;
    verts[1].pos = Vector3(p2.x, p2.y, 0);
    verts[1].color = color;

    auto inds = AllocInds(2);
    inds[0] = node->offsetVerts;
    inds[1] = node->offsetVerts + 1;
}


void DebugRender::DrawIndexedMesh(const Vector3* pos, const uint32_t* inds, size_t numInds, uint32_t color, const Matrix& transform)
{
    // alloc node
    auto node = AllocNode();
    node->type = DRAW_LINES;
    node->transform = true;
    node->texture = nullptr;
    node->offsetVerts = m_arrayVerts.size();
    node->offsetInds = m_arrayInds.size();
    node->numInds = numInds;
    // set later
    node->numVerts = 0;

    // copy inds

    // compute number of verts
    size_t maxIndex = 0;
    auto dstInds = AllocInds(numInds);
    for (size_t i = 0; i < numInds; i++)
    {
        dstInds[i] = node->offsetVerts + inds[i];
        maxIndex = std::max(maxIndex, inds[i]);
    }

    // copy verts
    node->numVerts = maxIndex + 1;
    auto dstVerts = AllocVerts(node->numVerts);
    for (size_t i = 0; i < node->numVerts; i++)
    {
        MatrixTransformCoord(&dstVerts[i].pos, &pos[i], &transform);
        //dstVerts[i].pos = pos[i];
        dstVerts[i].color = color;
    }
}


void DebugRender::DrawBoundingBox(const BoundingBox& bbox, uint32_t color)
{
    auto c = bbox.GetCorners();
    std::vector<uint32_t> inds({
        0, 1, 1, 3, 3, 2, 2, 0,
        4, 5, 5, 7, 7, 6, 6, 4,
        0, 4, 1, 5, 2, 6, 3, 7,
    });
    DrawIndexedMesh(&c[0], &inds[0], inds.size(), color, Matrix::IDENTITY);
}