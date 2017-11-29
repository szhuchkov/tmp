#include "pch.h"
#include "TextureManager.h"
#include "RenderDevice.h"
#include "TexFormatTGA.h"
#include "TexFormatJPG.h"


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}


Texture* TextureManager::LoadTexture(const char* name)
{
    char ext[8] = {};

    const char* ptr = strrchr(name, '.');
    if (ptr && strlen(ptr) <= 8)
        strcpy(ext, ptr + 1);

    Texture* tex = nullptr;
    if (!strcmp(ext, "bmp"))
        tex = nullptr, LogPrintf("TODO: implement");
    else if (!strcmp(ext, "tga"))
        tex = Load<TexFormatTGA>(name);
    else if (!strcmp(ext, "dds"))
        tex = nullptr, LogPrintf("TODO: implement");
    else if (!strcmp(ext, "jpg") || !strcmp(ext, "jpeg"))
        tex = Load<TexFormatJPG>(name);
    else
        LogPrintf("Unsupported texture format: '%s'", name);

    if (tex)
    {
        TextureNode* node = new TextureNode();
        node->name = name;
        node->instances = 1;
        node->tex = tex;

        m_nameToNode[name] = node;
        m_texToNode[tex] = node;
    }
    else
    {
        LogPrintf("Unable to load texture '%s'", name);
    }

    return tex;
}


void TextureManager::DeleteTexture(Texture* texture)
{
    if (texture)
    {
        auto it = m_texToNode.find(texture);
        if (it != m_texToNode.end())
        {
            auto node = it->second;
            node->instances--;
            if (node->instances <= 0)
            {
                m_nameToNode.erase(m_nameToNode.find(node->name));
                m_texToNode.erase(it);

                RenderDevice::GetInstance()->DeleteTexture(node->tex);

                delete node;
            }
        }
    }
}


template <typename _LoaderType>
Texture* TextureManager::Load(const char* name)
{
    _LoaderType image;
    if (!image.Load(name))
        return nullptr;

    Texture* tex = RenderDevice::GetInstance()->CreateTexture2D(
        image.width, image.height, 1,
        RenderDevice::TEXF_A8R8G8B8, RenderDevice::TEXTURE_USAGE_DEFAULT);

    if (tex)
    {
        RenderDevice::GetInstance()->UpdateTexture2D(tex, 0, image.data);
    }

    return tex;
}
