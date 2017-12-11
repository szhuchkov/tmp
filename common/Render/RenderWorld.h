#pragma once


#include <Render/BV/BoundingBox.h>
#include <Render/BV/Frustum.h>


struct VertexBuffer;
struct IndexBuffer;
struct RenderTarget;
struct Shader;
struct Texture;


class BaseMaterial;
class BoundingVolume;
class QuadRender;


enum RenderLayer
{
    RENDER_LAYER_SOLID,
    RENDER_LAYER_DECALS,
    RENDER_LAYER_ALPHA_BLEND,
    RENDER_LAYER_USER0,
};


enum LightFlags
{
    LIGHT_SHADOWS = 1,
};


enum LightType
{
    LIGHT_TYPE_NONE,
    LIGHT_TYPE_AMBIENT,
    LIGHT_TYPE_SKY,
    LIGHT_TYPE_DIRECTIONAL,
    LIGHT_TYPE_POINT,
    LIGHT_TYPE_SPOT,
    LIGHT_TYPE_PROJECTOR,
    LIGHT_TYPE_FLARE,
};


enum RenderEntityFlags
{
    RENDER_ENTITY_CAST_SHADOW = 1,
};


enum MaterialFlags
{
    MATERIAL_FLAG_TRANSPARENT   = 1,
    MATERIAL_FLAG_TWOSIDED      = 2,
};


struct RenderCamera
{
    Matrix                      view;
    Matrix                      proj;
};


struct RenderGeometry
{
    VertexBuffer*               verts;
    IndexBuffer*                inds;
    unsigned int                primitive;
};


struct RenderMaterial
{
    BaseMaterial*               base;
    Texture*                    texmaps[4];
    unsigned int                flags;
};


struct RenderSurface
{
    unsigned int                vertexOffset;
    unsigned int                vertexCount;
    unsigned int                indexOffset;
    unsigned int                indexCount;
    RenderMaterial*             material;
};


struct RenderModel
{
    RenderGeometry*             geometry;
    std::vector<RenderSurface*> surfaces;
};


struct RenderEntity
{
    RenderModel*                model;
    unsigned int                renderLayer;
    unsigned int                flags;
    BoundingBox                 bbox;
    void*                       bvdata; // bounding volume custom data
};


struct RenderLight
{
    LightType                   type;
    unsigned int                flags;
    Vector3                     color;
    Matrix                      position;
    void*                       bvdata; // bounding volume custom data
};


enum RenderPass
{
    RENDER_PASS_NONE,
    RENDER_PASS_SHADOW,
    RENDER_PASS_COLOR,
    RENDER_PASS_ALPHA,
};


enum MaterialShading
{
    MATERIAL_SHADING_UNLIT,
    MATERIAL_SHADING_DEPTH_WRITE,
    MATERIAL_SHADING_SKY_LIGHT,
    MATERIAL_SHADING_SKY_SHADOW,
    MATERIAL_SHADING_SPOT_LIGHT,
    MATERIAL_SHADING_SPOT_SHADOW,
    MATERIAL_SHADING_POINT_LIGHT,
    MATERIAL_SHADING_POINT_SHADOW,
    MATERIAL_SHADING_GBUFFER,
    MATERIAL_SHADING_COUNT,
};


struct RenderContext
{
    RenderPass                  pass = RENDER_PASS_NONE;
    MaterialShading             shading = MATERIAL_SHADING_UNLIT;
    RenderCamera*               camera = nullptr;
    RenderLight*                light = nullptr;
    RenderEntity*               entity = nullptr;
    RenderSurface*              surface = nullptr;

    Frustum                     frustum;

    Matrix                      shadowMatrix;

    std::vector<RenderEntity*>  visibleEntities;
    std::vector<RenderLight*>   visibleLights;
};


class RenderWorld
{
public:
    inline static RenderWorld* GetInstance()
    {
        static RenderWorld instance;
        return &instance;
    }

    bool Init();
    void Shutdown();
    void Render();
    void SetCamera(RenderCamera* camera);
    void AddEntity(RenderEntity* entity);
    void RemoveEntity(RenderEntity* entity);
    void AddLight(RenderLight* light);
    void RemoveLight(RenderLight* light);

    BaseMaterial* GetBaseMaterial(const char* name);
    Texture* GetShadowMap();

private:
    RenderWorld();
    ~RenderWorld();

    void RenderForContext(RenderContext* context);
    void RenderShadowsForLight(RenderContext* context);
    void SetupMaterialShading(RenderContext* context);

private:
    RenderContext m_mainContext;
    RenderContext m_shadowContext;
    std::unique_ptr<BoundingVolume> m_bv;
    RenderTarget* m_shadowTarget = nullptr;
    Texture* m_shadowMap = nullptr;
    QuadRender* m_debugShadowRender = nullptr;
};
