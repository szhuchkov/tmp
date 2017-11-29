#pragma once


struct RenderContext;
struct RenderEntity;
struct RenderSurface;


class BatchRender
{
public:
    inline static BatchRender* GetInstance()
    {
        static BatchRender instance;
        return &instance;
    }

    bool Init();
    void Shutdown();
    void Execute();
    void SetRenderContext(RenderContext* context);
    void AddDrawSurface(RenderContext* context);

private:
    BatchRender();
    ~BatchRender();

private:
    struct DrawSurface
    {
        RenderEntity*   entity;
        RenderSurface*  surface;
        float           distance;
    };

    class SortDrawSurfacesPred;

    RenderContext* m_context = nullptr;
    std::vector<DrawSurface> m_surfaces;
    std::vector<DrawSurface*> m_sortSurfaces;
};
