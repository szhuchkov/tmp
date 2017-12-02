#include "pch.h"
#include "MaterialManager.h"
#include "BaseMaterial.h"
#include "Loaders/MaterialLoader.h"

#include "Materials/M_Quad.h"
#include "Materials/M_BasicMaterial.h"


MaterialManager::MaterialManager()
{
}


MaterialManager::~MaterialManager()
{
}


bool MaterialManager::Init()
{
    // add basic materials
    AddBaseMaterial(new M_Quad());
    AddBaseMaterial(new M_BasicMaterial());

    // now initialize all
    int numErrors = 0;
    for (auto item : m_baseMaterials)
    {
        auto mtr = item.second;
        if (!mtr->Init())
        {
            LogPrintf("Unable to initialize material '%s'", mtr->GetName());
            numErrors++;
        }
    }

    if (numErrors)
    {
        LogPrintf("%d materials failed to initialize", numErrors);
        return false;
    }

    return true;
}


void MaterialManager::Shutdown()
{
    for (auto item : m_baseMaterials)
    {
        auto mtr = item.second;
        mtr->Shutdown();
        delete mtr;
    }
    m_baseMaterials.clear();

    for (auto item : m_materialLibs)
    {
        delete item.second;
    }
    m_materialLibs.clear();
}


void MaterialManager::AddBaseMaterial(BaseMaterial* mtr)
{
    m_baseMaterials[mtr->GetName()] = mtr;
}


BaseMaterial* MaterialManager::GetBaseMaterial(const char* name)
{
    if (!name || strlen(name) == 0)
    {
        LogPrintf("Warning: using default base material");
        name = "BasicMaterial";
    }

    auto it = m_baseMaterials.find(name);
    if (it != m_baseMaterials.end())
        return it->second;

    return nullptr;
}


MaterialLibrary* MaterialManager::LoadMaterials(const char* name)
{
    auto it = m_materialLibs.find(name);
    if (it != m_materialLibs.end())
        return it->second;

    auto lib = MaterialLoader::Load(name);
    m_materialLibs[name] = lib;

    return lib;
}


void MaterialManager::FreeMaterials(MaterialLibrary* lib)
{
}