#include "pch.h"
#include <Render/BatchRender.h>
#include <Render/RenderWorld.h>
#include <Render/BaseMaterial.h>
#include <Render/RenderDevice.h>


class BatchRender::SortDrawSurfacesPred
{
public:
    SortDrawSurfacesPred(RenderContext* context)
    {
    }

    bool operator () (const DrawSurface* a, const DrawSurface* b)
    {
        // render layer
        if (a->entity->renderLayer < b->entity->renderLayer)
            return true;
        if (b->entity->renderLayer < a->entity->renderLayer)
            return false;

        // base material
        if (a->surface->material->base->GetSortIndex() < b->surface->material->base->GetSortIndex())
            return true;
        if (b->surface->material->base->GetSortIndex() < a->surface->material->base->GetSortIndex())
            return false;

        // material
        if (a->surface->material < b->surface->material)
            return true;
        if (b->surface->material < a->surface->material)
            return false;

        // distance factor
        if (a->distance < b->distance)
            return true;
        if (a->distance > b->distance)
            return false;

        return false;
    }
};


BatchRender::BatchRender()
{
}


BatchRender::~BatchRender()
{
}


bool BatchRender::Init()
{
    return true;
}


void BatchRender::Shutdown()
{
}


void BatchRender::SetRenderContext(RenderContext* context)
{
    m_context = context;
}


void BatchRender::AddDrawSurface(RenderContext* context)
{
    m_surfaces.resize(m_surfaces.size() + 1);
    auto& surf = m_surfaces.back();
    surf.entity = context->entity;
    surf.surface = context->surface;
}


void BatchRender::Execute()
{
    size_t numSurfaces = m_surfaces.size();
    m_sortSurfaces.resize(numSurfaces);
    for (size_t i = 0; i < numSurfaces; i++)
        m_sortSurfaces[i] = &m_surfaces[i];

    std::sort(m_sortSurfaces.begin(), m_sortSurfaces.end(), SortDrawSurfacesPred(m_context));

    RenderEntity* lastEntity = nullptr;
    RenderGeometry* lastGeometry = nullptr;
    RenderMaterial* lastMaterial = nullptr;
    BaseMaterial* lastBaseMaterial = nullptr;

    bool skipCurrentMaterial = false;
    for (auto s : m_sortSurfaces)
    {
        m_context->entity = s->entity;
        m_context->surface = s->surface;

        // setup base material
        if (lastBaseMaterial != s->surface->material->base)
        {
            if (lastBaseMaterial)
                lastBaseMaterial->End(m_context);
            lastBaseMaterial = s->surface->material->base;
            skipCurrentMaterial = !lastBaseMaterial->Begin(m_context);
        }

        // do skip if something goes wrong
        if (skipCurrentMaterial)
        {
            lastGeometry = nullptr;
            lastMaterial = nullptr;
            continue;
        }

        // setup entity
        if (lastEntity != s->entity)
        {
            lastEntity = s->entity;
            RenderDevice::GetInstance()->SetMatrix(RenderDevice::MATRIX_WORLD, lastEntity->bbox.GetTransform());
        }

        // setup geometry
        if (lastGeometry != s->entity->model->geometry)
        {
            lastGeometry = s->entity->model->geometry;

            RenderDevice::GetInstance()->SetVertexBuffer(lastGeometry->verts);
            RenderDevice::GetInstance()->SetIndexBuffer(lastGeometry->inds);
        }

        // setup material
        if (lastMaterial != s->surface->material)
        {
            lastMaterial = s->surface->material;
            lastBaseMaterial->SetMaterial(m_context, lastMaterial);
        }

        // now draw
        if (lastGeometry->inds)
        {
            RenderDevice::GetInstance()->DrawIndexedPrimitive(
                lastGeometry->primitive,
                s->surface->vertexOffset, s->surface->vertexCount,
                s->surface->indexOffset, s->surface->indexCount);
        }
        else
        {
            RenderDevice::GetInstance()->DrawPrimitive(
                lastGeometry->primitive, s->surface->vertexOffset, s->surface->vertexCount);
        }
    }

    m_surfaces.clear();
    m_sortSurfaces.clear();
}