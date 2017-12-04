#include "pch.h"
#include <Scene/SceneFactory.h>
#include <Scene/SceneObject.h>
#include <Scene/SceneObjects/SO_EmptyObject.h>
#include <Scene/SceneObjects/SO_SkyLight.h>
#include <Scene/SceneObjects/SO_StaticMesh.h>
#include <Scene/SceneObjects/SO_FreeFlyCamera.h>


SceneObject* SceneFactory::CreateObject(int id, const char* clsName)
{
    SceneObject* res = nullptr;
#define MAKE_SCENE_OBJECT(cls)    (strcmp(clsName, cls::CLASS_NAME) ? nullptr : new cls(id))
    if (!res) res = MAKE_SCENE_OBJECT(SO_EmptyObject);
    if (!res) res = MAKE_SCENE_OBJECT(SO_SkyLight);
    if (!res) res = MAKE_SCENE_OBJECT(SO_StaticMesh);
    if (!res) res = MAKE_SCENE_OBJECT(SO_FreeFlyCamera);
#undef MAKE_SCENE_OBJECT
    return res;
}


void SceneFactory::DeleteObject(SceneObject* obj)
{
    delete obj;
}
