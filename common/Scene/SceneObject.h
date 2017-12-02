#pragma once


class SceneObjectData;


class SceneObject
{
public:
    const char* GetClsName() const;
    int GetID() const;
    bool IsAllive() const;

    virtual void Spawn();
    virtual void Update(unsigned int dt);
    virtual void Kill();

    virtual bool Load(const SceneObjectData& data);
    virtual bool Save(SceneObjectData& data);

private:
    std::string m_className;
    int m_id = -1;
    bool m_isAllive = true;
    unsigned int m_killTimeout = ~0;

protected:
    friend class SceneFactory;
    SceneObject(int id, const char* clsName);
    virtual ~SceneObject();
};
