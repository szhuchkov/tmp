#pragma once


#include "../BaseMaterial.h"


class M_Quad : public BaseMaterial
{
public:
    M_Quad() : BaseMaterial("Quad")
    {
    }

    bool Init() override
    {
        if (!BaseMaterial::Init())
            return false;

        if (!SetShading(MATERIAL_SHADING_UNLIT, "Quad.vs", "Quad.ps"))
            return false;

        return true;
    }
};
