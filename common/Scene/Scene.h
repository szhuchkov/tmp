#pragma once


class Ray;
class SceneObject;


class Scene
{
public:
    inline static Scene* GetInstance()
    {
        static Scene instance;
        return &instance;
    }

    struct RayIntersection
    {
        SceneObject*    object;
        float           distance;
        Vector3         normal;
    };

    void Clear();
    void Update(unsigned int dt);
    bool Load(const char* name);

    SceneObject* CreateObject(const char* clsName);
    std::vector<SceneObject*> EnumObjects();
    void AddObject(SceneObject* obj);
    void RemoveObject(SceneObject* obj);

    std::vector<RayIntersection> RayHitObjects(const Ray& ray, bool sortByDistance = false);

private:
    Scene();
    ~Scene();

private:
    std::vector<SceneObject*> m_objects;
    int m_nextObjectID = 1;
};
