#pragma once


class TexFormatJPG
{
public:
    TexFormatJPG();
    ~TexFormatJPG();

    bool Load(const char* name);

    unsigned int width = 0;
    unsigned int height = 0;
    char* data = nullptr;

private:
    class Context;
    Context* m_context = nullptr;
};
