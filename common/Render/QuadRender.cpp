#include "pch.h"
#include "RenderDevice.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "QuadRender.h"


QuadRender::QuadRender() :
    m_position(0, 0, 1, 1)
{
}


QuadRender::~QuadRender()
{
}


bool QuadRender::Init()
{
    static const Vector3 verts[] =
    {
        { Vector3(0, 0, -1) },
        { Vector3(0, 1, -1) },
        { Vector3(1, 1, -1) },
        { Vector3(1, 0, -1) },
    };
    static const uint16_t inds[] =
    {
        0, 1, 2, 0, 2, 3,
    };

    m_verts = RenderDevice::GetInstance()->CreateVertexBuffer(RenderDevice::VERTEX_POSITION, 4, verts);
    if (!m_verts)
    {
        LogPrintf("Unable to create vertex buffer");
        return false;
    }

    m_inds = RenderDevice::GetInstance()->CreateIndexBuffer(RenderDevice::INDEX_16, 6, inds);
    if (!m_inds)
    {
        LogPrintf("Unable to create index buffer");
        return false;
    }

    m_shader = ShaderManager::GetInstance()->GetShader("Quad.vs", "Quad.ps");
    if (!m_shader)
    {
        LogPrintf("Unable to load shader");
        return false;
    }

    return true;
}


void QuadRender::Shutdown()
{
    if (m_verts)
    {
        RenderDevice::GetInstance()->DeleteVertexBuffer(m_verts);
        m_verts = nullptr;
    }

    if (m_inds)
    {
        RenderDevice::GetInstance()->DeleteIndexBuffer(m_inds);
        m_inds = nullptr;
    }
}


void QuadRender::SetTexture(Texture* tex)
{
    m_texture = tex;
}


void QuadRender::SetPosition(float x, float y, float w, float h)
{
    m_position = Vector4(x, y, w, h);
}


void QuadRender::Render()
{
    RenderDevice::GetInstance()->SetVertexBuffer(m_verts);
    RenderDevice::GetInstance()->SetIndexBuffer(m_inds);
    RenderDevice::GetInstance()->SetTexture(0, m_texture);
    RenderDevice::GetInstance()->SetShader(m_shader);
    RenderDevice::GetInstance()->SetUniform(0, m_position);
    RenderDevice::GetInstance()->DrawIndexedPrimitive(RenderDevice::PRIMITIVE_TRIANGLES, 0, 4, 0, 6);
}