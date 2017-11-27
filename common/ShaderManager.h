#pragma once


struct Shader;


class ShaderManager
{
public:
    inline static ShaderManager* GetInstance()
    {
        static ShaderManager instance;
        return &instance;
    }

    bool Init();
    void Shutdown();

    Shader* GetShader(const char* vs, const char* ps, const char* gs = nullptr);

private:
    ShaderManager();
    ~ShaderManager();

    bool PreloadShaders();

private:
    struct ShaderNode
    {
        std::string     vs;
        std::string     ps;
        std::string     gs;
        Shader*         shader;
    };

    std::list<ShaderNode> m_shaders;
    bool m_preload = false;
};
