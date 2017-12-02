#include "pch.h"
#include "MaterialLoader.h"
#include "Render/MaterialManager.h"
#include "Render/RenderWorld.h"
#include <tinyxml/tinyxml2.h>


using namespace tinyxml2;


MaterialLibrary* MaterialLoader::Load(const char* fileName)
{
    // load file
    char* xml = FS_LoadFile(fileName, nullptr);
    if (!xml)
    {
        LogPrintf("Unable to load material library '%s'", fileName);
        return false;
    }

    // parse and release file data
    XMLDocument doc;
    auto res = doc.Parse(xml);
    delete[] xml;
    if (res != XML_SUCCESS)
    {
        LogPrintf("Error while parsing material library '%s'", fileName);
        return nullptr;
    }

    // get root element
    XMLElement* libElement = doc.FirstChildElement("MaterialLibrary");
    if (!libElement)
    {
        LogPrintf("Root element not found in '%s'", fileName);
        return nullptr;
    }

    // alloc
    MaterialLibrary* lib = new MaterialLibrary();
    for (auto mtrElement = libElement->FirstChildElement("Material"); mtrElement != nullptr; mtrElement = mtrElement->NextSiblingElement())
    {
        // get all params
        std::unordered_map<std::string, std::string> params;
        for (auto paramElement = mtrElement->FirstChildElement(); paramElement != nullptr; paramElement = paramElement->NextSiblingElement())
        {
            params[paramElement->Value()] = paramElement->GetText();
        }

        // setup material from params
        lib->materials.resize(lib->materials.size() + 1);
        auto& inst = lib->materials.back();

        // get base material
        inst.base = params["BaseMaterial"];

        // get texmaps
        inst.texmaps[0] = params["Texture0"];
        inst.texmaps[1] = params["Texture1"];
        inst.texmaps[2] = params["Texture2"];
        inst.texmaps[3] = params["Texture3"];

        // get flags
        inst.flags = 0;
        if (params["SwoSided"] == "true")
            inst.flags |= MATERIAL_FLAG_TWOSIDED;
        if (params["Transparent"] == "true")
            inst.flags |= MATERIAL_FLAG_TRANSPARENT;
    }

    return lib;
}