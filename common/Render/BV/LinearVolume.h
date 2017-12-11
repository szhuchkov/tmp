#pragma once


#include "BoundingVolume.h"


class LinearVolume : public BoundingVolume
{
public:
    void AddEntity(RenderEntity* entity) override
    {
        m_entities.push_back(entity);
    }

    void RemoveEntity(RenderEntity* entity)
    {
        auto ptr = std::find(m_entities.begin(), m_entities.end(), entity);
        if (ptr != m_entities.end())
        {
            *ptr = m_entities.back();
            m_entities.pop_back();
        }
    }

    void AddLight(RenderLight* light) override
    {
        m_lights.push_back(light);
    }

    void RemoveLight(RenderLight* light) override
    {
        auto ptr = std::find(m_lights.begin(), m_lights.end(), light);
        if (ptr != m_lights.end())
        {
            *ptr = m_lights.back();
            m_lights.pop_back();
        }
    }

    std::vector<RenderEntity*> CullEntities(const Frustum& frustum) override
    {
        std::vector<RenderEntity*> entities;
        entities.reserve(m_entities.size());
        for (auto item : m_entities)
        {
            if (CullBox(item->bbox, frustum))
                entities.push_back(item);
        }
        return entities;
    }

    std::vector<RenderLight*> CullLights(const Frustum& frustum) override
    {
        std::vector<RenderLight*> lights;
        lights.reserve(m_lights.size());
        for (auto item : m_lights)
        {
            lights.push_back(item);
        }
        return lights;
    }

    std::vector<RenderEntity*> GetAllEntities() override
    {
        return m_entities;
    }

    std::vector<RenderLight*> GetAllLights() override
    {
        return m_lights;
    }

protected:
    std::vector<RenderEntity*> m_entities;
    std::vector<RenderLight*> m_lights;
};
