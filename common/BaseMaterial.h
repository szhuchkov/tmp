#pragma once


struct Shader;
struct Texture;
struct RenderContext;
struct RenderMaterial;


class BaseMaterial
{
public:
    BaseMaterial(const char* name);
    virtual ~BaseMaterial();

    const char* GetName() const;

    int GetSortIndex() const;

    virtual bool Init();
    virtual void Shutdown();
    virtual bool Begin(RenderContext* context);
    virtual void End(RenderContext* context);
    virtual void SetMaterial(RenderMaterial* material);

protected:
    void SetSortIndex(int index);
    void SetShader(Shader* shader);
    void SetTexture(unsigned int index, Texture* texture);
    void SetUniform(unsigned int index, const Vector4& value);
    void SetMatrix(unsigned int index, const Matrix& value);

    Shader* GetShader(const char* vs, const char* ps, const char* gs = nullptr);

private:
    int m_sortIndex = 0;
    std::string m_name;
};
