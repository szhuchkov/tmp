#pragma once


class SceneObject;


class SceneFactory
{
public:
    static SceneObject* CreateObject(int id, const char* clsName);
    static void DeleteObject(SceneObject* obj);
};
