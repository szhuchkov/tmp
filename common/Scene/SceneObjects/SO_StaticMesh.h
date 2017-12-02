#pragma once


#include <Scene/SceneObject.h>
#include <Scene/SceneObjectData.h>
#include <Render/RenderWorld.h>
#include <Render/MaterialManager.h>
#include <Render/MeshManager.h>
#include <Render/TextureManager.h>


class SO_StaticMesh : public SceneObject
{
public:
    static constexpr char* CLASS_NAME = "StaticMesh";

    SO_StaticMesh(int id) :
        SceneObject(id, CLASS_NAME)
    {
        m_entity.model = &m_model;
        m_entity.flags = 0;
        m_entity.renderLayer = 0;

        m_model.geometry = &m_geometry;

        MatrixIdentity(&m_entity.position);
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

    bool Load(const SceneObjectData& data)
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

        m_surfaces.clear();

        // free materials
        for (size_t i = 0; i < m_materials.size(); i++)
        {
            // textures
            for (size_t j = 0; j < 4; j++)
            {
                TextureManager::GetInstance()->FreeTexture(m_materials[i].texmaps[j]);
            }
        }
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