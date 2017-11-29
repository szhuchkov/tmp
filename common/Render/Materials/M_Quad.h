#pragma once


#include "../BaseMaterial.h"


class M_Quad : public BaseMaterial
{
public:
    M_Quad() :
        BaseMaterial("Quad")
    {
    }

    bool Init() override
    {
        if (!BaseMaterial::Init())
            return false;

        m_quadShader = GetShader("Quad.vs", "Quad.ps");
        if (!m_quadShader)
            return false;

        return true;
    }

    bool Begin(RenderContext* context) override
    {
        if (!BaseMaterial::Begin(context))
            return false;

        SetShader(m_quadShader);

        return true;
    }

private:
    Shader* m_quadShader = nullptr;
};
