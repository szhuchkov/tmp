#pragma once


#include "Render/RenderDevice.h"
#include "Render/RenderWorld.h"
#include "Render/TextureManager.h"


class TestScene
{
private:
    const char* TEXTURE_NAME = "checker.jpg";

    struct DrawVertex
    {
        float x, y, z;
        float u, v;
    };

private:
    TestScene()
    {
    }

    ~TestScene()
    {
    }

public:
    inline static TestScene* GetInstance()
    {
        static TestScene instance;
        return &instance;
    }

    uint32_t RandomColor()
    {
        int r = std::rand() & 0xff;
        int g = std::rand() & 0xff;
        int b = std::rand() & 0xff;
        return 0xff000000 | (r << 16) | (g << 8) | (b << 0);
    }

    bool Init()
    {
        static const DrawVertex verts[] =
        {
            { -0.5f, -0.5f, 0.0f, 0.0f, 1.0f },
            { -0.5f,  0.5f, 0.0f, 0.0f, 0.0f },
            {  0.5f,  0.5f, 0.0f, 1.0f, 0.0f },
            {  0.5f, -0.5f, 0.0f, 1.0f, 1.0f },
        };

        static const uint16_t inds[] =
        {
            0, 1, 2, 0, 2, 3,
        };

        // create light
        m_light.flags = 0;
        MatrixIdentity(&m_light.position);
        RenderWorld::GetInstance()->AddLight(&m_light);

        // create texture
#if 1
        m_texture = TextureManager::GetInstance()->LoadTexture(TEXTURE_NAME);
#else
        std::vector<uint32_t> pixels(16 * 16);
        for (size_t i = 0; i < 16 * 16; i++)
            pixels[i] = RandomColor();
        m_texture = RenderDevice::GetInstance()->CreateTexture2D(16, 16, 1,
            RenderDevice::TEXF_A8R8G8B8, RenderDevice::TEXTURE_USAGE_DEFAULT);
        RenderDevice::GetInstance()->UpdateTexture2D(m_texture, 0, &pixels[0]);
#endif

        // create surface
        m_surface.vertexOffset = 0;
        m_surface.vertexCount = ARRAY_SIZE(verts);
        m_surface.indexOffset = 0;
        m_surface.indexCount = ARRAY_SIZE(inds);
        m_surface.material = &m_material;

        // create geometry
        m_geometry.primitive = RenderDevice::PRIMITIVE_TRIANGLES;
        m_geometry.verts = RenderDevice::GetInstance()->CreateVertexBuffer(
            RenderDevice::VERTEX_XYZ | RenderDevice::VERTEX_TEXCOORD, m_surface.vertexCount, verts);
        m_geometry.inds = RenderDevice::GetInstance()->CreateIndexBuffer(
            RenderDevice::INDEX_16, m_surface.indexCount, inds);
        m_model.geometry = &m_geometry;
        m_model.surfaces.push_back(&m_surface);

        // create material
        m_material.base = RenderWorld::GetInstance()->GetBaseMaterial("Quad");
        m_material.flags = 0;
        m_material.texmaps[0] = m_texture;
        m_material.texmaps[1] = nullptr;
        m_material.texmaps[2] = nullptr;
        m_material.texmaps[3] = nullptr;

        // create entity
        m_entity.model = &m_model;
        m_entity.flags = 0;
        m_entity.renderLayer = 0;
        MatrixIdentity(&m_entity.position);
        RenderWorld::GetInstance()->AddEntity(&m_entity);

        // setup camera
        Vector3 camPos(0, 0, -1);
        Vector3 camTarget(0, 0, 0);
        Vector3 camUp(0, 1, 0);
        MatrixLookAt(&m_camera.view, &camPos, &camTarget, &camUp);
        MatrixPerspective(&m_camera.proj, 80.0f, 1.5f, 1.0f, 1000.0f);
        RenderWorld::GetInstance()->SetCamera(&m_camera);

        return true;
    }

    void Shutdown()
    {
#if 1
        TextureManager::GetInstance()->DeleteTexture(m_texture);
#else
        RenderDevice::GetInstance()->DeleteTexture(m_texture);
#endif
        RenderDevice::GetInstance()->DeleteVertexBuffer(m_geometry.verts);
        RenderDevice::GetInstance()->DeleteIndexBuffer(m_geometry.inds);
        RenderWorld::GetInstance()->RemoveLight(&m_light);
        RenderWorld::GetInstance()->RemoveEntity(&m_entity);
    }

private:
    Texture* m_texture = nullptr;
    RenderCamera m_camera;
    RenderLight m_light;
    RenderEntity m_entity;
    RenderGeometry m_geometry;
    RenderModel m_model;
    RenderSurface m_surface;
    RenderMaterial m_material;
};
