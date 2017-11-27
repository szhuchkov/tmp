#include "pch.h"
#include "SceneFactory.h"
#include "SceneObject.h"

#include <SceneObjects\SO_EmptyObject.h>


SceneObject* SceneFactory::CreateObject(int id, const char* clsName)
{
    SceneObject* res = nullptr;
    if (!strcmp(clsName, "EmptyObject"))
        res = new SO_EmptyObject(id, clsName);
    return res;
}


void SceneFactory::DeleteObject(SceneObject* obj)
{
    delete obj;
}
