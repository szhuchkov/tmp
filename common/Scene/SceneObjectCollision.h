#pragma once


class SceneObject;


class SceneObjectCollision
{
public:
    class CollisionShape;

    struct RayIntersection
    {
        float       distance;
        Vector3     normal;
    };

    SceneObjectCollision(SceneObject* object);
    ~SceneObjectCollision();

    void CreateSphere(const float radius);
    void CreateBox(const Vector3& mins, const Vector3& maxs);

    const SceneObject* GetSceneObject() const;
    SceneObject* GetSceneObject();

    const AffineTransform& GetPosition() const;
    void SetPosition(const AffineTransform& pos);

    bool IsEmpty() const;

    RayIntersection TraceRay(const Ray& ray) const;

private:
    SceneObjectCollision(const SceneObjectCollision& other) = default;
    SceneObjectCollision& operator = (const SceneObjectCollision& other) = default;

private:
    SceneObject* m_object;
    AffineTransform m_position;
    std::unique_ptr<CollisionShape> m_shape;
};
