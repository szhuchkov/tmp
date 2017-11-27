#pragma once


class BaseMaterial;


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

    BaseMaterial* GetMaterial(const char* name);

private:
    MaterialManager();
    ~MaterialManager();

    void AddMaterial(BaseMaterial* mtr);

private:
    std::unordered_map<std::string, BaseMaterial*> m_materials;
};
