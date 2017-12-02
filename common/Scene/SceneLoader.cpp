#include "pch.h"
#include "SceneLoader.h"
#include "Scene.h"
#include "SceneObject.h"
#include "SceneObjectData.h"
#include <tinyxml/tinyxml2.h>


using namespace tinyxml2;


bool SceneLoader::Load(const char* fileName)
{
    char* xml = FS_LoadFile(fileName, nullptr);
    if (!xml)
    {
        LogPrintf("Unable to open '%s'", fileName);
        return false;
    }

    XMLDocument doc;
    auto err = doc.Parse(xml);
    delete xml;

    if (err != XML_SUCCESS)
    {
        LogPrintf("Error while parsing '%s'", fileName);
        delete xml;
        return false;
    }

    // obtain root 'Scene' element
    auto sceneElement = doc.FirstChildElement("Scene");
    if (!sceneElement)
    {
        LogPrintf("Not a valid scene file: '%s'", fileName);
        return false;
    }

    // enum objects inside 'Scene'
    std::vector<SceneObject*> spawnObjects;
    for (auto objElement = sceneElement->FirstChildElement(); objElement != nullptr; objElement = objElement->NextSiblingElement())
    {
        // get all properties
        SceneObjectData data;
        for (auto prop = objElement->FirstChildElement(); prop != nullptr; prop = prop->NextSiblingElement())
        {
            auto propName = prop->Value();
            auto propValue = prop->GetText();
            data.SetString(propName, propValue);
        }

        // create and load object
        auto clsName = data.GetString("ClassName");
        if (clsName)
        {
            auto obj = Scene::GetInstance()->CreateObject(clsName);
            if (obj)
            {
                if (!obj->Load(data))
                {
                    LogPrintf("Unable to load object '%s' from '%s'", clsName, fileName);
                    obj->Kill();
                }
                else
                {
                    spawnObjects.push_back(obj);
                }
            }
            else
            {
                LogPrintf("Unable to create object '%s' from '%s'", clsName, fileName);
            }
        }
        else
        {
            LogPrintf("Object with no ClassName in '%s'", fileName);
        }
    }

    // spawn objects
    for (auto obj : spawnObjects)
        obj->Spawn();

    return true;
}