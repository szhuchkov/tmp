#pragma once


class SceneObject;


class SceneObjectTransform
{
public:
    SceneObjectTransform(SceneObject* object);
    ~SceneObjectTransform();

    void CopyFrom(const SceneObjectTransform& other);

    const AffineTransform& GetPosition() const;
    void SetPosition(const AffineTransform& pos);

    const SceneObject* GetSceneObject() const;
    SceneObject* GetSceneObject();

    void AddChild(SceneObjectTransform* child);
    void RemoveChild(SceneObjectTransform* child);
    std::list<SceneObjectTransform*> GetChilds();

    void SetParent(SceneObjectTransform* parent);
    const SceneObjectTransform* GetParent() const;
    SceneObjectTransform* GetParent();

protected:
    SceneObject* m_object = nullptr;
    AffineTransform m_position;
    std::list<SceneObjectTransform*> m_childs;
    SceneObjectTransform* m_parent = nullptr;

private:
    SceneObjectTransform(const SceneObjectTransform& other) = default;
    SceneObjectTransform& operator = (const SceneObjectTransform& other) = default;
};
