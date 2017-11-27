#include "pch.h"
#include "Scene.h"
#include "SceneObject.h"
#include "SceneLoader.h"
#include "SceneFactory.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}


void Scene::Update(unsigned int dt)
{
    // update objects
    for (auto item : m_objects)
        item->Update(dt);

    // remove dead
    for (size_t i = 0; i < m_objects.size(); i++)
    {
        auto obj = m_objects[i];
        if (!obj->IsAllive())
        {
            m_objects[i] = m_objects.back();
            m_objects.pop_back();
            i--;
            SceneFactory::DeleteObject(obj);
        }
    }
}


void Scene::Clear()
{
    for (auto item : m_objects)
        SceneFactory::DeleteObject(item);
    m_objects.clear();
}


bool Scene::Load(const char* name)
{
    Clear();

    if (!SceneLoader::Load(name))
        return false;

    return true;
}


void Scene::AddObject(SceneObject* obj)
{
    m_objects.push_back(obj);
}


void Scene::RemoveObject(SceneObject* obj)
{
    for (size_t i = 0; i < m_objects.size(); i++)
    {
        if (m_objects[i] == obj)
        {
            m_objects[i] = m_objects.back();
            m_objects.pop_back();
            break;
        }
    }
}


SceneObject* Scene::CreateObject(const char* clsName)
{
    auto id = m_nextObjectID++;
    auto obj = SceneFactory::CreateObject(id, clsName);
    if (obj)
    {
        AddObject(obj);
    }
    return obj;
}
