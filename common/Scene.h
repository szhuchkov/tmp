#pragma once


class SceneObject;


class Scene
{
public:
    inline static Scene* GetInstance()
    {
        static Scene instance;
        return &instance;
    }

    void Clear();
    void Update(unsigned int dt);
    bool Load(const char* name);

    SceneObject* CreateObject(const char* clsName);
    void AddObject(SceneObject* obj);
    void RemoveObject(SceneObject* obj);

private:
    Scene();
    ~Scene();

private:
    std::vector<SceneObject*> m_objects;
    int m_nextObjectID = 1;
};
