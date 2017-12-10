#pragma once


struct RenderEntity;
struct RenderLight;


class BoundingVolume
{
public:
    BoundingVolume() {}
    virtual ~BoundingVolume() {}

    virtual void AddEntity(RenderEntity* entity) = 0;
    virtual void RemoveEntity(RenderEntity* entity) = 0;
    virtual void AddLight(RenderLight* light) = 0;
    virtual void RemoveLight(RenderLight* light) = 0;

    virtual std::vector<RenderEntity*> CullEntities(const Frustum& frustum) = 0;
    virtual std::vector<RenderLight*> CullLights(const Frustum& frustum) = 0;
};
