#pragma once


struct Texture;
struct VertexBuffer;
struct IndexBuffer;
struct Shader;


class AABB;
class BoundingBox;


class DebugRender
{
public:
    // some limits
    enum
    {
        MAX_VERTS = 100000,
        MAX_INDS = 100000,
    };

    // draw vertex format
    struct DrawVertex
    {
        Vector3 pos;
        Vector2 tex;
        uint32_t color;
    };

    // singleton instance
    inline static DebugRender* GetInstance()
    {
        static DebugRender instance;
        return &instance;
    }

    bool Init();
    void Shutdown();
    void Render();
    void SetViewProj(const Matrix& viewProj);

    void DrawLine2D(const Vector2& p1, const Vector2& p2, uint32_t color);
    void DrawLines2D(const Vector2* points, size_t numPoints);
    void DrawLine3D(const Vector3& p1, const Vector3& p2, uint32_t color, const Matrix& transform);
    void DrawLines3D(const Vector3* points, size_t numPoints, uint32_t color, const Matrix& transform);
    void DrawMesh(const Vector3* verts, size_t numVerts, uint32_t color, const Matrix& transform);
    void DrawIndexedMesh(const Vector3* verts, const uint32_t* inds, size_t numInds, uint32_t color, const Matrix& transform);
    void DrawAABB(const AABB& box, uint32_t color, const Matrix& transform);
    void DrawBoundingBox(const BoundingBox& box, uint32_t color);
    void DrawImage(const Vector2& pos, const Vector2& size, Texture* tex);
    void DrawString(const Vector2& pos, const Vector2& size, const std::string& text);

private:
    // debug draw node types
    enum DrawNodeType
    {
        DRAW_LINES,
        DRAW_TRIANGLES,
        DRAW_FONT,
    };

    // debug draw node settings
    struct DrawNode
    {
        DrawNodeType    type;
        bool            transform;
        Texture*        texture;
        size_t          offsetVerts;
        size_t          numVerts;
        size_t          offsetInds;
        size_t          numInds;
    };

    DebugRender();
    ~DebugRender();

    DrawNode* AllocNode();
    DrawVertex* AllocVerts(size_t num);
    uint32_t* AllocInds(size_t num);

private:
    Shader* m_quadShader = nullptr;
    Shader* m_fontShader = nullptr;
    Shader* m_shapeShader = nullptr;
    Texture* m_fontTexture = nullptr;
    VertexBuffer* m_verts = nullptr;
    IndexBuffer* m_inds = nullptr;
    std::vector<DrawVertex> m_arrayVerts;
    std::vector<uint32_t> m_arrayInds;
    std::vector<DrawNode> m_nodes;
    Matrix m_viewProj = Matrix::IDENTITY;
};
