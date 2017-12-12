#include "pch.h"
#include "Scene.h"
#include "SceneObject.h"
#include "SceneObjectData.h"
#include "SceneObjectTransform.h"
#include "SceneObjectCollision.h"
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


std::vector<SceneObject*> Scene::EnumObjects()
{
    return m_objects;
}


std::vector<Scene::RayIntersection> Scene::RayHitObjects(const Ray& ray, bool sortByDistance)
{
    std::vector<RayIntersection> res;
    for (auto item : m_objects)
    {
        auto collision = item->GetCollision();
        if (collision)
        {
            auto src = collision->TraceRay(ray);
            if (src.distance > 0.0f)
            {
                RayIntersection dst;
                dst.object = item;
                dst.normal = src.normal;
                dst.distance = src.distance;
                res.push_back(dst);
            }
        }
    }
    if (sortByDistance && res.size() > 1)
    {
        std::sort(res.begin(), res.end(), [](const RayIntersection& a, const RayIntersection& b) {
            return a.distance < b.distance;
        });
    }
    return res;
}