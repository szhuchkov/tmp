#pragma once


class SceneObjectTransform;
class SceneObjectCollision;
class SceneObjectData;


class SceneObject
{
public:
    // object dest
    const char* GetClsName() const;
    int GetID() const;
    bool IsAllive() const;

    // transform component
    const SceneObjectTransform* GetTransform() const;
    SceneObjectTransform* GetTransform();
    virtual void OnTransformChanged();

    // collision component
    const SceneObjectCollision* GetCollision() const;
    SceneObjectCollision* GetCollision();
    //TODO: collision callbacks

    // overrides
    virtual void Spawn();
    virtual void Update(unsigned int dt);
    virtual void Kill();

    // serrialization
    virtual bool Load(const SceneObjectData& data);
    virtual bool Save(SceneObjectData& data);

private:
    std::string m_className;
    int m_id = -1;
    bool m_isAllive = true;
    unsigned int m_killTimeout = ~0;

    // components
    std::unique_ptr<SceneObjectTransform> m_transform;
    std::unique_ptr<SceneObjectCollision> m_collision;

protected:
    friend class SceneFactory;
    SceneObject(int id, const char* clsName);
    virtual ~SceneObject();
    void CreateTransform();
    void CreateCollision();
};
