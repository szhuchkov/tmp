#pragma once


#include <Scene/SceneObject.h>
#include <Scene/SceneObjectData.h>
#include <Render/RenderWorld.h>


class SO_SkyLight : public SceneObject
{
public:
    static constexpr char* CLASS_NAME = "SkyLight";

    SO_SkyLight(int id) :
        SceneObject(id, CLASS_NAME)
    {
        m_light.type = LIGHT_TYPE_SKY;
        m_light.flags = 0;
        m_light.color = Vector3(0, 0, 0);
    }

    ~SO_SkyLight()
    {
    }

    void Spawn() override
    {
        SceneObject::Spawn();
        Show();
    }

    bool Load(const SceneObjectData& data) override
    {
        if (!SceneObject::Load(data))
            return false;

        if (data.GetBool("CastShadows"))
            m_light.flags |= LIGHT_CAST_SHADOW;

        if (data.HasProperty("Color"))
            m_light.color = data.GetVec3("Color");

        return true;
    }

    void Kill() override
    {
        SceneObject::Kill();
        Hide();
    }

    void Show()
    {
        if (!m_isVisible)
        {
            m_isVisible = true;
            RenderWorld::GetInstance()->AddLight(&m_light);
        }
    }

    void Hide()
    {
        if (m_isVisible)
        {
            m_isVisible = false;
            RenderWorld::GetInstance()->RemoveLight(&m_light);
        }
    }

private:
    RenderLight m_light;
    bool m_isVisible = false;
};
