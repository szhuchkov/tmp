#include "pch.h"
#include <Render/RenderWorld.h>
#include <Render/RenderDevice.h>
#include <Render/BatchRender.h>
#include <Render/MaterialManager.h>
#include <Render/ShaderManager.h>
#include <Render/BV/BoundingVolume.h>
#include <Render/BV/LinearVolume.h>
#include <Render/QuadRender.h>


RenderWorld::RenderWorld()
{
    m_mainContext.camera = nullptr;
    m_mainContext.entity = nullptr;
    m_mainContext.light = nullptr;
}


RenderWorld::~RenderWorld()
{
}


BaseMaterial* RenderWorld::GetBaseMaterial(const char* name)
{
    auto mtr = MaterialManager::GetInstance()->GetBaseMaterial(name);
    if (!mtr)
    {
        LogPrintf("Warning: base material '%s' not found", name);
    }
    return mtr;
}


Texture* RenderWorld::GetShadowMap()
{
    //return RenderDevice::GetInstance()->GetRenderTargetColor(m_shadowTarget);
    return m_shadowMap;
}


bool RenderWorld::Init()
{
    // init shader manager
    if (!ShaderManager::GetInstance()->Init())
    {
        LogPrintf("ShaderManager init failed");
        return false;
    }

    // init material manager
    if (!MaterialManager::GetInstance()->Init())
    {
        LogPrintf("MaterialManager init failed");
        return false;
    }

    // init batch render
    if (!BatchRender::GetInstance()->Init())
    {
        LogPrintf("BatchRender init failed");
        return false;
    }

    // init bounding volume
    m_bv = std::make_unique<LinearVolume>();

    // create shadowmap texture
    m_shadowTarget = RenderDevice::GetInstance()->CreateRenderTarget(1024, 1024, RenderDevice::TEXF_A8R8G8B8, RenderDevice::TEXF_D16);
    if (!m_shadowTarget)
    {
        LogPrintf("Unable to create shadowmap texture");
        return false;
    }

    m_shadowMap = RenderDevice::GetInstance()->GetRenderTargetDepth(m_shadowTarget);

    // create debug shadow render
    m_debugShadowRender = new QuadRender();
    if (!m_debugShadowRender->Init())
    {
        LogPrintf("Unable to create debug shadow render");
        return false;
    }

    m_debugShadowRender->SetTexture(RenderDevice::GetInstance()->GetRenderTargetColor(m_shadowTarget));
   // m_debugShadowRender->SetTexture(m_shadowMap);
    m_debugShadowRender->SetPosition(0, 0, 0.3f, 0.3f);

    return true;
}


void RenderWorld::Shutdown()
{
    RenderDevice::GetInstance()->DeleteRenderTarget(m_shadowTarget);
    m_shadowTarget = nullptr;
    m_shadowMap = nullptr;

    if (m_debugShadowRender)
    {
        m_debugShadowRender->Shutdown();
        delete m_debugShadowRender;
        m_debugShadowRender = nullptr;
    }

    BatchRender::GetInstance()->Shutdown();
    MaterialManager::GetInstance()->Shutdown();
    ShaderManager::GetInstance()->Shutdown();
}


void RenderWorld::SetCamera(RenderCamera* camera)
{
    m_mainContext.camera = camera;
}


void RenderWorld::AddEntity(RenderEntity* entity)
{
    m_bv->AddEntity(entity);
}


void RenderWorld::RemoveEntity(RenderEntity* entity)
{
    m_bv->RemoveEntity(entity);
}


void RenderWorld::AddLight(RenderLight* light)
{
    m_bv->AddLight(light);
}


void RenderWorld::RemoveLight(RenderLight* light)
{
    m_bv->RemoveLight(light);
}


void RenderWorld::Render()
{
    //TODO: remove default clear
    const float clearColor[] = { 0.4f, 0.6f, 0.8f, 1.0f };
    const float clearDepth = 1.0f;
    RenderDevice::GetInstance()->Clear(RenderDevice::CLEAR_COLOR | RenderDevice::CLEAR_DEPTH, clearColor, clearDepth, 0);
    
    RenderForContext(&m_mainContext);
    
    RenderDevice::GetInstance()->SetRenderTarget(nullptr);
    m_debugShadowRender->Render();
}


void RenderWorld::RenderForContext(RenderContext* context)
{
    context->entity = nullptr;
    context->light = nullptr;
    context->visibleEntities.clear();
    context->visibleLights.clear();

    Matrix viewProj, viewInverse;
    if (!m_mainContext.camera)
    {
        static float angle = 0.0f;
        angle += 0.00007f;
        Vector3 camPos(8 * sinf(angle), 1.8f, 8 * cosf(angle));
        Vector3 camTarget(0, 0, 0);
        Vector3 camUp(0, 1, 0);

        Matrix view;
        MatrixLookAt(&view, &camPos, &camTarget, &camUp);
        Matrix proj;
        MatrixPerspective(&proj, 80.0f, 1280.0f / 720.0f, 1.0f, 100.0f);
        viewProj = view * proj;
        MatrixInverse(&viewInverse, &view);
    }
    else
    {
        MatrixInverse(&viewInverse, &m_mainContext.camera->view);
        viewProj = m_mainContext.camera->view * m_mainContext.camera->proj;
    }

    context->frustum.FromViewProj(viewProj);
    context->visibleEntities = m_bv->CullEntities(context->frustum);
    context->visibleLights = m_bv->CullLights(context->frustum);

    // TODO: threads
    for (auto light : context->visibleLights)
    {
        context->light = light;

        // begin shadow pass
        if (light->flags & LIGHT_SHADOWS)
        {
            RenderShadowsForLight(context);
            Matrix shadowBiasMatrix(
                0.5f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.5f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.5f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f);
            m_mainContext.shadowMatrix = m_shadowContext.shadowMatrix * shadowBiasMatrix;
        }

        SetupMaterialShading(context);

        // begin light pass
        RenderDevice::GetInstance()->SetMatrix(RenderDevice::MATRIX_VIEW_PROJECTION, viewProj);
        RenderDevice::GetInstance()->SetMatrix(RenderDevice::MATRIX_VIEW_INVERSE, viewInverse);

        BatchRender::GetInstance()->SetRenderContext(context);

        for (auto entity : context->visibleEntities)
        {
            context->entity = entity;

            for (auto surf : context->entity->model->surfaces)
            {
                context->surface = surf;
                BatchRender::GetInstance()->AddDrawSurface(context);
            }
        }

        // end light pass
        BatchRender::GetInstance()->Execute();
    }
}


void RenderWorld::RenderShadowsForLight(RenderContext* context)
{
    auto light = context->light;

    m_shadowContext.camera = nullptr;
    m_shadowContext.light = light;
    m_shadowContext.pass = RENDER_PASS_SHADOW;
    m_shadowContext.shading = MATERIAL_SHADING_DEPTH_WRITE;

    // compute frustum
    m_shadowContext.frustum.FromViewProj(light->position);

    // cull entities
    m_shadowContext.visibleEntities = m_bv->GetAllEntities();// m_bv->CullEntities(m_shadowContext.frustum);

    // setup shadow matrices
    Matrix view;
    view = light->position;
    Matrix proj;
    MatrixOrtho(&proj, 20, 20, -100, 100);
    Matrix viewProj;
    viewProj = view * proj;
    Matrix viewInverse;
    MatrixInverse(&viewInverse, &view);
    m_shadowContext.shadowMatrix = viewProj;

    RenderDevice::GetInstance()->SetMatrix(RenderDevice::MATRIX_VIEW_PROJECTION,viewProj);
    RenderDevice::GetInstance()->SetMatrix(RenderDevice::MATRIX_VIEW_INVERSE, viewInverse);

    // begin shadow pass
    RenderDevice::GetInstance()->SetRenderTarget(m_shadowTarget);
    float clearColor[] = { 1, 1, 1, 1 };
    float clearDepth = 1.0f;
    RenderDevice::GetInstance()->Clear(RenderDevice::CLEAR_COLOR | RenderDevice::CLEAR_DEPTH, clearColor, clearDepth, 0);

    // change face direction
    RenderDevice::GetInstance()->SetCullMode(RenderDevice::CULL_MODE_FRONT);

    // batch rendering
    BatchRender::GetInstance()->SetRenderContext(&m_shadowContext);
    for (auto entity : m_shadowContext.visibleEntities)
    {
        // skip entities with no shadows
        if (0 == (entity->flags & RENDER_ENTITY_CAST_SHADOW))
            continue;

        m_shadowContext.entity = entity;
        for (auto surf : m_shadowContext.entity->model->surfaces)
        {
            m_shadowContext.surface = surf;
            BatchRender::GetInstance()->AddDrawSurface(&m_shadowContext);
        }
    }
    BatchRender::GetInstance()->Execute();
    RenderDevice::GetInstance()->SetRenderTarget(nullptr);

    // restore cull mode
    RenderDevice::GetInstance()->SetCullMode(RenderDevice::CULL_MODE_BACK);
}


void RenderWorld::SetupMaterialShading(RenderContext* context)
{
    if (!context->light)
    {
        context->shading = MATERIAL_SHADING_UNLIT;
        return;
    }

    bool hasShadow = 0 != (context->light->flags & LIGHT_SHADOWS);

    switch (context->light->type)
    {
    case LIGHT_TYPE_SKY:
        context->shading = (hasShadow ? MATERIAL_SHADING_SKY_SHADOW : MATERIAL_SHADING_SKY_LIGHT);
        break;
    case LIGHT_TYPE_POINT:
        context->shading = (hasShadow ? MATERIAL_SHADING_POINT_SHADOW : MATERIAL_SHADING_POINT_LIGHT);
        break;
    case LIGHT_TYPE_SPOT:
        context->shading = (hasShadow ? MATERIAL_SHADING_SPOT_SHADOW : MATERIAL_SHADING_SPOT_LIGHT);
        break;
    default:
        context->shading = MATERIAL_SHADING_UNLIT;
        break;
    }
}