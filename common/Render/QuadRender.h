#pragma once


struct Texture;
struct Shader;
struct VertexBuffer;
struct IndexBuffer;


class QuadRender
{
public:
    QuadRender();
    ~QuadRender();

    bool Init();
    void Shutdown();
    void Render();
    void SetTexture(Texture* texture);
    void SetPosition(float x, float y, float w, float h);

private:
    VertexBuffer* m_verts = nullptr;
    IndexBuffer* m_inds = nullptr;
    Shader* m_shader = nullptr;
    Texture* m_texture = nullptr;
    Vector4 m_position;
};
