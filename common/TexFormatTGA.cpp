#include "pch.h"
#include "TexFormatTGA.h"


#pragma pack(push, 1)
struct TGA_HEADER
{
    uint8_t     idLength;
    uint8_t     colorMapType;
    uint8_t     imageType;

    // color map spec
    uint16_t    cMapStart;
    uint16_t    cMapLength;
    uint8_t     cMapDepth;

    // image spec
    uint16_t    xOffset;
    uint16_t    yOffset;
    uint16_t    width;
    uint16_t    height;
    uint8_t     bitsPerPixel;
    uint8_t     imageDescriptor;
};
#pragma pack(pop)


TexFormatTGA::~TexFormatTGA()
{
    delete[] data;
}


bool TexFormatTGA::Load(const char* name)
{
    char* raw = FS_LoadFile(name, nullptr);
    if (!raw)
        return false;

    char* ptr = raw;
    auto header = reinterpret_cast<TGA_HEADER*>(ptr);
    ptr += sizeof(TGA_HEADER);

    // validate header
    bool isValid = true;
    if (header->imageType != 2)
    {
        LogPrintf("Only RGB files supported");
        isValid = false;
    }

    if (header->bitsPerPixel != 24 && header->bitsPerPixel != 32)
    {
        LogPrintf("Only 24 and 32 bit files supported");
        isValid = false;
    }

    if (isValid)
    {
        ptr += header->idLength;

        width = header->width;
        height = header->height;
        data = new char[4 * width * height];

        if (header->bitsPerPixel == 32)
        {
            memcpy(data, ptr, 4 * width * height);
            ptr += 4 * width * height;
        }
        else if (header->bitsPerPixel == 24)
        {
            char *src = ptr;
            char* dst = data;
            for (unsigned int row = 0; row < height; row++)
            {
                for (unsigned int col = 0; col < width; col++)
                {
                    *dst++ = *src++;
                    *dst++ = *src++;
                    *dst++ = *src++;
                    *dst++ = char(0xff);
                }
            }
        }
        else
        {
            //...
        }
    }
    else
    {
        LogPrintf("Unsupported TGA file: '%s'", name);
    }

    delete[] raw;

    return true;
}
