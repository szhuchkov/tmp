#include "pch.h"
#include "RenderWorld.h"
#include "RenderDevice.h"
#include "BatchRender.h"
#include "MaterialManager.h"
#include "ShaderManager.h"


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
    m_entities.push_back(entity);
}


void RenderWorld::RemoveEntity(RenderEntity* entity)
{
    for (size_t i = 0; i < m_entities.size(); i++)
    {
        if (m_entities[i] == entity)
        {
            m_entities[i] = m_entities.back();
            m_entities.pop_back();
            break;
        }
    }
}


void RenderWorld::AddLight(RenderLight* light)
{
    m_lights.push_back(light);
}


void RenderWorld::RemoveLight(RenderLight* light)
{
    for (size_t i = 0; i < m_lights.size(); i++)
    {
        if (m_lights[i] == light)
        {
            m_lights[i] = m_lights.back();
            m_lights.pop_back();
            break;
        }
    }
}


void RenderWorld::Render()
{
    //TODO: remove default clear
    const float clearColor[] = { 0.4f, 0.6f, 0.8f, 1.0f };
    const float clearDepth = 1.0f;
    RenderDevice::GetInstance()->Clear(RenderDevice::CLEAR_COLOR | RenderDevice::CLEAR_DEPTH, clearColor, clearDepth, 0);

    RenderForContext(&m_mainContext);
}


void RenderWorld::CullEntities(RenderContext* context)
{
    context->visibleEntities.resize(m_entities.size());
    for (size_t i = 0; i < m_entities.size(); i++)
    {
        context->visibleEntities[i] = m_entities[i];
    }
}


void RenderWorld::CullLights(RenderContext* context)
{
    context->visibleLights.resize(m_lights.size());
    for (size_t i = 0; i < m_lights.size(); i++)
    {
        context->visibleLights[i] = m_lights[i];
    }
}


void RenderWorld::RenderForContext(RenderContext* context)
{
    context->entity = nullptr;
    context->light = nullptr;
    context->visibleEntities.clear();
    context->visibleLights.clear();

    CullLights(context);
    CullEntities(context);

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
        Matrix viewProj = view * proj;
        RenderDevice::GetInstance()->SetMatrix(RenderDevice::MATRIX_VIEW_PROJECTION, viewProj);
    }
    else
    {
        Matrix viewInverse;
        MatrixInverse(&viewInverse, &m_mainContext.camera->view);
        Matrix viewProj = m_mainContext.camera->view * m_mainContext.camera->proj;
        RenderDevice::GetInstance()->SetMatrix(RenderDevice::MATRIX_VIEW_PROJECTION, viewProj);
        RenderDevice::GetInstance()->SetMatrix(RenderDevice::MATRIX_VIEW_INVERSE, viewInverse);
    }

    // TODO: threads
    for (auto light : context->visibleLights)
    {
        context->light = light;

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
