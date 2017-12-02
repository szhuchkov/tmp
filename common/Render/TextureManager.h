#pragma once


struct Texture;


class TextureManager
{
public:
    inline static TextureManager* GetInstance()
    {
        static TextureManager instance;
        return &instance;
    }

    Texture* LoadTexture(const char* name, bool genMipmaps = true);
    void FreeTexture(Texture* tex);

private:
    TextureManager();
    ~TextureManager();

    template <typename _LoaderType>
    Texture* Load(const char* name, bool genMips);

    void GenMipLevel(unsigned int w, unsigned int h, const std::vector<uint32_t>& src, std::vector<uint32_t>& dst);

private:
    struct TextureNode
    {
        Texture*    tex;
        int         instances;
        std::string name;
    };

    std::unordered_map<std::string, TextureNode*> m_nameToNode;
    std::unordered_map<Texture*, TextureNode*> m_texToNode;
};
