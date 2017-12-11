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

        if (!SetShading(MATERIAL_SHADING_UNLIT, "BasicMaterial.vs", "BasicMaterial.ps") ||
            !SetShading(MATERIAL_SHADING_DEPTH_WRITE, "BasicMaterialDepth.vs", "BasicMaterialDepth.ps") ||
            !SetShading(MATERIAL_SHADING_SKY_LIGHT, "BasicMaterialSkyLight.vs", "BasicMaterial.ps") ||
            !SetShading(MATERIAL_SHADING_SKY_SHADOW, "BasicMaterialSkyShadow.vs", "BasicMaterialShadow.ps"))
            return false;

        return true;
    }
};
