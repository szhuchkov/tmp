#pragma once


class BaseMaterial;


struct MaterialInstance
{
    std::string     base;
    std::string     texmaps[4];
    unsigned int    flags;
};


struct MaterialLibrary
{
    std::vector<MaterialInstance> materials;
};


class MaterialManager
{
public:
    inline static MaterialManager* GetInstance()
    {
        static MaterialManager instance;
        return &instance;
    }

    bool Init();
    void Shutdown();

    BaseMaterial* GetBaseMaterial(const char* name);
    MaterialLibrary* LoadMaterials(const char* name);
    void FreeMaterials(MaterialLibrary* lib);

private:
    MaterialManager();
    ~MaterialManager();

    void AddBaseMaterial(BaseMaterial* mtr);

private:
    std::unordered_map<std::string, BaseMaterial*> m_baseMaterials;
    std::unordered_map<std::string, MaterialLibrary*> m_materialLibs;
};
