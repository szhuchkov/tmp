#pragma once


class AABB
{
public:
    Vector3 mins, maxs;

    AABB();
    AABB(const AABB& other);
    AABB(const Vector3& mins, const Vector3& maxs);

    bool operator == (const AABB& other) const;
    bool operator != (const AABB& other) const;

    AABB& operator = (const AABB& aabb);

    void Invalidate();
    bool IsValid() const;
    void AddPoint(const Vector3& point);
    void AddAABB(const AABB& aabb);
    float Volume() const;
    Vector3 Size() const;
    std::vector<Vector3> GetCorners() const;
};


MATH_INLINE AABB::AABB()
{
}

MATH_INLINE AABB::AABB(const AABB& other)
{
    mins = other.mins;
    maxs = other.maxs;
}

MATH_INLINE AABB::AABB(const Vector3& mins, const Vector3& maxs)
{
    this->mins = mins;
    this->maxs = maxs;
}

MATH_INLINE bool AABB::operator == (const AABB& other) const
{
    return mins == other.mins && maxs == other.maxs;
}

MATH_INLINE bool AABB::operator != (const AABB& other) const
{
    return mins != other.mins || maxs != other.maxs;
}

MATH_INLINE AABB& AABB::operator = (const AABB& other)
{
    if (this != &other)
    {
        mins = other.mins;
        maxs = other.maxs;
    }
    return (*this);
}

MATH_INLINE void AABB::Invalidate()
{
    mins = Vector3(MATH_BIG_NUMBER, MATH_BIG_NUMBER, MATH_BIG_NUMBER);
    maxs = Vector3(-MATH_BIG_NUMBER, -MATH_BIG_NUMBER, -MATH_BIG_NUMBER);
}

MATH_INLINE bool AABB::IsValid() const
{
    return mins.x < maxs.x && mins.y < maxs.y && mins.z < maxs.z;
}

MATH_INLINE void AABB::AddPoint(const Vector3& point)
{
    mins.x = std::min(mins.x, point.x);
    mins.y = std::min(mins.y, point.y);
    mins.z = std::min(mins.z, point.z);
    maxs.x = std::max(maxs.x, point.x);
    maxs.y = std::max(maxs.y, point.y);
    maxs.z = std::max(maxs.z, point.z);
}

MATH_INLINE void AABB::AddAABB(const AABB& aabb)
{
    AddPoint(aabb.mins);
    AddPoint(aabb.maxs);
}


MATH_INLINE float AABB::Volume() const
{
    float dx = maxs.x - mins.x;
    float dy = maxs.y - mins.y;
    float dz = maxs.z - mins.z;
    return dx * dy * dz;
}

MATH_INLINE Vector3 AABB::Size() const
{
    return Vector3(maxs - mins);
}

MATH_INLINE std::vector<Vector3> AABB::GetCorners() const
{
    return {
        Vector3(mins.x, mins.y, mins.z),
        Vector3(mins.x, mins.y, maxs.z),
        Vector3(mins.x, maxs.y, mins.z),
        Vector3(mins.x, maxs.y, maxs.z),
        Vector3(maxs.x, mins.y, mins.z),
        Vector3(maxs.x, mins.y, maxs.z),
        Vector3(maxs.x, maxs.y, mins.z),
        Vector3(maxs.x, maxs.y, maxs.z),
    };
}
