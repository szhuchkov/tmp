#pragma once


struct MaterialLibrary;


class MaterialLoader
{
public:
    static MaterialLibrary* Load(const char* name);
};
