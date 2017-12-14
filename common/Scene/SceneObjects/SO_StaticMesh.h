#pragma once


#include <Scene/SceneObject.h>
#include <Scene/SceneObjectTransform.h>
#include <Scene/SceneObjectCollision.h>
#include <Scene/SceneObjectData.h>
#include <Render/RenderWorld.h>
#include <Render/MaterialManager.h>
#include <Render/MeshManager.h>
#include <Render/TextureManager.h>
#include <Render/DebugRender.h>


class SO_StaticMesh : public SceneObject
{
public:
    static constexpr char* CLASS_NAME = "StaticMesh";

    SO_StaticMesh(int id) :
        SceneObject(id, CLASS_NAME)
    {
        CreateTransform();
        CreateCollision();

        m_entity.model = &m_model;
        m_entity.flags = RENDER_ENTITY_CAST_SHADOW;
        m_entity.renderLayer = 0;
        m_entity.bbox.SetTransform(GetTransform()->GetPosition().ToMatrix());

        m_model.geometry = &m_geometry;
    }

    ~SO_StaticMesh()
    {
        Free();
    }

    void Spawn() override
    {
        SceneObject::Spawn();
        Show();
    }

    void OnTransformChanged() override
    {
        SceneObject::OnTransformChanged();
        m_entity.bbox.SetTransform(GetTransform()->GetPosition().ToMatrix());
    }

    void Update(unsigned int dt) override
    {
        SceneObject::Update(dt);
        if (m_meshInstance)
        {
            DebugRender::GetInstance()->DrawBoundingBox(m_entity.bbox, 0xffff0000);
#if 1
            DebugRender::GetInstance()->DrawIndexedMesh(
                &m_meshInstance->posVerts[0],
                &m_meshInstance->posInds[0],
                m_meshInstance->posInds.size(),
                0xff00ff00,
                GetTransform()->GetPosition().ToMatrix());
#endif
        }
    }

    void Show()
    {
        if (!m_isVisible && m_meshInstance)
        {
            m_isVisible = true;
            RenderWorld::GetInstance()->AddEntity(&m_entity);
        }
    }

    void Hide()
    {
        if (m_isVisible)
        {
            m_isVisible = false;
            RenderWorld::GetInstance()->RemoveEntity(&m_entity);
        }
    }

    bool Load(const SceneObjectData& data) override
    {
        if (!SceneObject::Load(data))
            return false;

        auto meshName = data.GetString("MeshName");
        auto materialsName = data.GetString("MaterialsName");
        if (meshName && materialsName)
        {
            if (!Load(meshName, materialsName))
                return false;
        }

        return true;
    }

    bool Load(const char* meshName, const char* materialsName)
    {
        Free();

        if (!LoadMesh(meshName))
            return false;

        if (!LoadMaterials(materialsName))
            return false;

        if (!LinkMaterials())
            return false;

        //GetCollision()->CreateBox(m_meshInstance->bbox.mins, m_meshInstance->bbox.maxs);
        GetCollision()->CreateSphere(10.0f);

        return true;
    }

    void Free()
    {
        if (m_meshInstance)
        {
            MeshManager::GetInstance()->FreeMesh(m_meshInstance);
            m_meshInstance = nullptr;
        }

        if (m_materialLib)
        {
            MaterialManager::GetInstance()->FreeMaterials(m_materialLib);
            m_materialLib = nullptr;
        }

        // free materials
        for (size_t i = 0; i < m_materials.size(); i++)
        {
            // textures
            for (size_t j = 0; j < 4; j++)
            {
                TextureManager::GetInstance()->FreeTexture(m_materials[i].texmaps[j]);
            }
        }

        m_surfaces.clear();
        m_materials.clear();
    }

protected:
    bool LoadMesh(const char* name)
    {
        m_meshInstance = MeshManager::GetInstance()->LoadMesh(name);
        if (!m_meshInstance)
            return false;

        m_geometry.verts = m_meshInstance->verts;
        m_geometry.inds = m_meshInstance->inds;
        m_geometry.primitive = m_meshInstance->primitive;

        // clone surfaces
        m_surfaces.assign(m_meshInstance->surfaces.begin(), m_meshInstance->surfaces.end());

        m_model.surfaces.resize(m_surfaces.size());
        for (size_t i = 0; i < m_surfaces.size(); i++)
            m_model.surfaces[i] = &m_surfaces[i];

        m_entity.bbox = BoundingBox(m_meshInstance->bbox, Matrix::IDENTITY);

        return true;
    }

    bool LoadMaterials(const char* name)
    {
        m_materialLib = MaterialManager::GetInstance()->LoadMaterials(name);
        if (!m_materialLib)
            return false;

        size_t numMaterials = m_materialLib->materials.size();
        m_materials.resize(numMaterials);
        for (size_t i = 0; i < numMaterials; i++)
        {
            auto& src = m_materialLib->materials[i];
            auto& dst = m_materials[i];

            dst.flags = src.flags;
            dst.base = MaterialManager::GetInstance()->GetBaseMaterial(src.base.c_str());

            for (int j = 0; j < 4; j++)
            {
                if (!src.texmaps[j].empty())
                {
                    dst.texmaps[j] = TextureManager::GetInstance()->LoadTexture(src.texmaps[j].c_str());
                }
                else
                {
                    dst.texmaps[j] = nullptr;
                }
            }
        }

        return true;
    }

    bool LinkMaterials()
    {
        size_t numSurfaces = m_surfaces.size();
        size_t numMaterials = m_materials.size();

        for (size_t i = 0; i < m_surfaces.size(); i++)
        {
            auto& s = m_surfaces[i];
            auto idx = m_meshInstance->materialIndexes[i];
            if (idx >= m_materials.size())
            {
                LogPrintf("Invalid surface material index");
                return false;
            }
            s.material = &m_materials[idx];
        }

        return true;
    }

protected:
    bool m_isVisible = false;
    MeshInstance* m_meshInstance = nullptr;
    MaterialLibrary* m_materialLib = nullptr;
    RenderEntity m_entity;
    RenderModel m_model;
    RenderGeometry m_geometry;
    std::vector<RenderSurface> m_surfaces;
    std::vector<RenderMaterial> m_materials;
};