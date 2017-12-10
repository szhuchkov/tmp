#pragma once


#include "BoundingVolume.h"


class LinearVolume : public BoundingVolume
{
public:
    virtual void AddEntity(RenderEntity* entity)
    {
        m_entities.push_back(entity);
    }

    virtual void RemoveEntity(RenderEntity* entity)
    {
        auto ptr = std::find(m_entities.begin(), m_entities.end(), entity);
        if (ptr != m_entities.end())
        {
            *ptr = m_entities.back();
            m_entities.pop_back();
        }
    }

    virtual void AddLight(RenderLight* light)
    {
        m_lights.push_back(light);
    }

    virtual void RemoveLight(RenderLight* light)
    {
        auto ptr = std::find(m_lights.begin(), m_lights.end(), light);
        if (ptr != m_lights.end())
        {
            *ptr = m_lights.back();
            m_lights.pop_back();
        }
    }

    virtual std::vector<RenderEntity*> CullEntities(const Frustum& frustum)
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

    virtual std::vector<RenderLight*> CullLights(const Frustum& frustum)
    {
        std::vector<RenderLight*> lights;
        lights.reserve(m_lights.size());
        for (auto item : m_lights)
        {
            lights.push_back(item);
        }
        return lights;
    }

protected:
    std::vector<RenderEntity*> m_entities;
    std::vector<RenderLight*> m_lights;
};
