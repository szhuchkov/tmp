#include "pch.h"
#include <Render/RenderWorld.h>
#include <Render/RenderDevice.h>
#include <Render/BatchRender.h>
#include <Render/MaterialManager.h>
#include <Render/ShaderManager.h>
#include <Render/BV/BoundingVolume.h>
#include <Render/BV/LinearVolume.h>


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

    return true;
}


void RenderWorld::Shutdown()
{
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
}


void RenderWorld::RenderForContext(RenderContext* context)
{
    context->entity = nullptr;
    context->light = nullptr;
    context->visibleEntities.clear();
    context->visibleLights.clear();

    Matrix viewProj;
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
        RenderDevice::GetInstance()->SetMatrix(RenderDevice::MATRIX_VIEW_PROJECTION, viewProj);
    }
    else
    {
        Matrix viewInverse;
        MatrixInverse(&viewInverse, &m_mainContext.camera->view);
        viewProj = m_mainContext.camera->view * m_mainContext.camera->proj;
        RenderDevice::GetInstance()->SetMatrix(RenderDevice::MATRIX_VIEW_PROJECTION, viewProj);
        RenderDevice::GetInstance()->SetMatrix(RenderDevice::MATRIX_VIEW_INVERSE, viewInverse);
    }

    context->frustum.FromViewProj(viewProj);
    context->visibleEntities = m_bv->CullEntities(context->frustum);
    context->visibleLights = m_bv->CullLights(context->frustum);

    // TODO: threads
    for (auto light : context->visibleLights)
    {
        context->light = light;
        SetupMaterialShading(context);

        // batch from here
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

        // to here
        BatchRender::GetInstance()->Execute();
    }
}


void RenderWorld::SetupMaterialShading(RenderContext* context)
{
    if (!context->light)
    {
        context->shading = MATERIAL_SHADING_UNLIT;
        return;
    }

    bool hasShadow = 0 != (context->light->flags & LIGHT_CAST_SHADOW);

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