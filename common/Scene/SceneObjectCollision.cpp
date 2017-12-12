#include "pch.h"
#include "SceneObjectCollision.h"


class SceneObjectCollision::CollisionShape
{
public:
    virtual SceneObjectCollision::RayIntersection TraceRay(const Ray& ray) = 0;
};


class SceneObjectCollisionSphere : public SceneObjectCollision::CollisionShape
{
public:
    SceneObjectCollisionSphere(float radius) :
        m_radius(radius * radius)
    {
    }

    virtual SceneObjectCollision::RayIntersection TraceRay(const Ray& ray)
    {
        SceneObjectCollision::RayIntersection res = { -1.0f, Vector3::ZERO };
        Vector3 dir;
        Vec3Normalize(&dir, &ray.direction);
        float L = Vec3LengthSq(&ray.origin);
        float tc = -Vec3Dot(&ray.origin, &dir);
        if (tc < 0.0f)
            return res;

        float d2 = tc * tc - L;
        if (d2 > m_radius)
            return res;
        float t1c = std::sqrtf(m_radius - d2);

        float t = tc < t1c ? tc + t1c : tc - t1c;
        res.distance = t;
        return res;
    }

private:
    float m_radius;
};


class SceneObjectCollisionBox : public SceneObjectCollision::CollisionShape
{
public:
    SceneObjectCollisionBox(const Vector3& mins, const Vector3& maxs) :
        m_mins(mins),
        m_maxs(maxs)
    {
    }

    virtual SceneObjectCollision::RayIntersection TraceRay(const Ray& ray)
    {
        SceneObjectCollision::RayIntersection res = { -1.0f, Vector3::ZERO };
        return res;
    }

private:
    Vector3 m_mins, m_maxs;
};


SceneObjectCollision::SceneObjectCollision(SceneObject* obj) :
    m_object(obj),
    m_position(AffineTransform::IDENTITY)
{
}


SceneObjectCollision::~SceneObjectCollision()
{
}


bool SceneObjectCollision::IsEmpty() const
{
    return !m_shape;
}


SceneObjectCollision::RayIntersection SceneObjectCollision::TraceRay(const Ray& ray) const
{
    RayIntersection res = { -1.0f, Vector3::ZERO };
    if (m_shape)
    {
        Matrix inv = m_position.ToMatrix();
        MatrixInverse(&inv, &inv);

        Ray tmRay;
        MatrixTransformCoord(&tmRay.origin, &ray.origin, &inv);
        MatrixTransformNormal(&tmRay.direction, &ray.direction, &inv);
        res = m_shape->TraceRay(tmRay);
    }
    return res;
}


void SceneObjectCollision::SetPosition(const AffineTransform& pos)
{
    m_position = pos;
}


const AffineTransform& SceneObjectCollision::GetPosition() const
{
    return m_position;
}


const SceneObject* SceneObjectCollision::GetSceneObject() const
{
    return m_object;
}


SceneObject* SceneObjectCollision::GetSceneObject()
{
    return m_object;
}


void SceneObjectCollision::CreateSphere(float radius)
{
    m_shape = std::make_unique<SceneObjectCollisionSphere>(radius);
}


void SceneObjectCollision::CreateBox(const Vector3& mins, const Vector3& maxs)
{
    m_shape = std::make_unique<SceneObjectCollisionBox>(mins, maxs);
}
