#pragma once


#include "../BaseMaterial.h"
#include "../RenderWorld.h"


class M_BasicMaterial : public BaseMaterial
{
public:
    enum
    {
        SHADOW_PASS,
        COLOR_PASS,
    };

    M_BasicMaterial() :
        BaseMaterial("BasicMaterial")
    {
    }

    bool Init() override
    {
        if (!BaseMaterial::Init())
            return false;

        m_shadowShader = GetShader("BasicMaterialShadow.vs", "BasicMaterialShadow.ps");
        if (!m_shadowShader)
            return false;

        m_colorShader = GetShader("BasicMaterial.vs", "BasicMaterial.ps");
        if (!m_colorShader)
            return false;

        return true;
    }

    bool Begin(RenderContext* context) override
    {
        if (!BaseMaterial::Begin(context))
            return false;

        if (context->pass == RENDER_PASS_SHADOW)
            SetShader(m_shadowShader);
        else
            SetShader(m_colorShader);

        return true;
    }

private:
    Shader* m_shadowShader = nullptr;
    Shader* m_colorShader = nullptr;
};
