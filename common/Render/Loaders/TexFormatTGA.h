#pragma once


class TexFormatTGA
{
public:
    ~TexFormatTGA();

    bool Load(const char* name);

    unsigned int width = 0;
    unsigned int height = 0;
    char* data = nullptr;
};
