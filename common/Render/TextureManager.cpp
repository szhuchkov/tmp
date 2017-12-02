#include "pch.h"
#include "TextureManager.h"
#include "RenderDevice.h"
#include "Loaders/TexFormatTGA.h"
#include "Loaders/TexFormatJPG.h"


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}


Texture* TextureManager::LoadTexture(const char* name, bool genMipmaps)
{
    char ext[8] = {};

    const char* ptr = strrchr(name, '.');
    if (ptr && strlen(ptr) <= 8)
        strcpy(ext, ptr + 1);

    Texture* tex = nullptr;
    if (!strcmp(ext, "bmp"))
        tex = nullptr, LogPrintf("TODO: implement");
    else if (!strcmp(ext, "tga"))
        tex = Load<TexFormatTGA>(name, genMipmaps);
    else if (!strcmp(ext, "dds"))
        tex = nullptr, LogPrintf("TODO: implement");
    else if (!strcmp(ext, "jpg") || !strcmp(ext, "jpeg"))
        tex = Load<TexFormatJPG>(name, genMipmaps);
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


void TextureManager::FreeTexture(Texture* texture)
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


bool IsPowOf2(unsigned int value)
{
    for (int i = 0; i < 32; i++)
    {
        if (value & (1U << i))
        {
            if (value > (1U << i))
                return false;
            else
                return true;
        }
    }
    return true;
}


template <typename _LoaderType>
Texture* TextureManager::Load(const char* name, bool genMipmaps)
{
    _LoaderType image;
    if (!image.Load(name))
        return nullptr;

    // check if actually can generate mips
    if (genMipmaps)
    {
        if (image.width != image.height || !IsPowOf2(image.width))
        {
            LogPrintf("Unable to generate mipmaps for texture '%s'", name);
            genMipmaps = false;
        }
    }

    // compute number of levels
    unsigned int levels = 1;
    if (genMipmaps)
    {
        auto w = image.width;
        auto h = image.height;
        do
        {
            levels++;
            w = std::max<unsigned int>(1, w / 2);
            h = std::max<unsigned int>(1, h / 2);
        } while (w > 1 || h > 1);
    }

    Texture* tex = RenderDevice::GetInstance()->CreateTexture2D(
        image.width, image.height, levels,
        RenderDevice::TEXF_A8R8G8B8, RenderDevice::TEXTURE_USAGE_DEFAULT);

    if (tex)
    {
        RenderDevice::GetInstance()->UpdateTexture2D(tex, 0, image.data);

        if (genMipmaps)
        {
            std::vector<uint32_t> buffers[2];
            buffers[0].resize(image.width * image.height);
            buffers[1].resize(image.width * image.height);
            memcpy(&buffers[0][0], image.data, 4 * image.width * image.height);

            unsigned int w = image.width;
            unsigned int h = image.height;
            unsigned int src = 0;
            for (unsigned int i = 1; i < levels; i++)
            {
                unsigned int dst = (src + 1) & 1;
                GenMipLevel(w, h, buffers[src], buffers[dst]);
                w = std::max<unsigned int>(1, w / 2);
                h = std::max<unsigned int>(1, h / 2);
                src = dst;
                RenderDevice::GetInstance()->UpdateTexture2D(tex, i, &buffers[dst][0]);
            }
        }
    }

    return tex;
}


static uint32_t MixColors(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4)
{
    uint32_t res = 0;
    unsigned char* pr = reinterpret_cast<unsigned char*>(&res);
    unsigned char* p1 = reinterpret_cast<unsigned char*>(&c1);
    unsigned char* p2 = reinterpret_cast<unsigned char*>(&c2);
    unsigned char* p3 = reinterpret_cast<unsigned char*>(&c3);
    unsigned char* p4 = reinterpret_cast<unsigned char*>(&c4);
    pr[0] = (p1[0] + p2[0] + p3[0] + p4[0]) / 4;
    pr[1] = (p1[1] + p2[1] + p3[1] + p4[1]) / 4;
    pr[2] = (p1[2] + p2[2] + p3[2] + p4[2]) / 4;
    pr[3] = (p1[3] + p2[3] + p3[3] + p4[3]) / 4;
    return res;
}


void TextureManager::GenMipLevel(unsigned int w, unsigned int h, const std::vector<uint32_t>& src, std::vector<uint32_t>& dst)
{
    unsigned int dstW = std::max<unsigned int>(1, w / 2);
    unsigned int dstH = std::max<unsigned int>(1, h / 2);
    dst.resize(dstW * dstH);
    uint32_t* pdst = &dst[0];

    //TODO: non-squared
    for (size_t y = 0; y < dstH; y++)
    {
        for (size_t x = 0; x < dstW; x++)
        {
            const auto& c1 = src[(2 * y + 0) * w + 2 * x + 0];
            const auto& c2 = src[(2 * y + 0) * w + 2 * x + 1];
            const auto& c3 = src[(2 * y + 1) * w + 2 * x + 0];
            const auto& c4 = src[(2 * y + 1) * w + 2 * x + 1];
            *pdst++ = MixColors(c1, c2, c3, c4);
        }
    }
}
