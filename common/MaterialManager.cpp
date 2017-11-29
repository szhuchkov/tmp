#include "pch.h"
#include "MaterialManager.h"
#include "BaseMaterial.h"

#include "M_Quad.h"
#include "M_BasicMaterial.h"


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
    for (auto item : m_materials)
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
    for (auto item : m_materials)
    {
        auto mtr = item.second;
        mtr->Shutdown();
        delete mtr;
    }
    m_materials.clear();
}


void MaterialManager::AddBaseMaterial(BaseMaterial* mtr)
{
    m_materials[mtr->GetName()] = mtr;
}


BaseMaterial* MaterialManager::GetBaseMaterial(const char* name)
{
    auto it = m_materials.find(name);
    if (it != m_materials.end())
        return it->second;
    return nullptr;
}