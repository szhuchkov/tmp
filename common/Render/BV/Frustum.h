#pragma once


class AABB;
class BoundingBox;
enum CullType;


class Frustum
{
public:
    enum
    {
        PLANE_LEFT,
        PLANE_RIGHT,
        PLANE_BOTTOM,
        PLANE_TOP,
        PLANE_NEAR,
        PLANE_FAR,
    };

    Frustum() = default;
    Frustum(const Frustum& other) = default;
    Frustum(const Matrix& view, const Matrix& proj);

    void FromViewProj(const Matrix& view, const Matrix& proj);
    void FromViewProj(const Matrix& viewProj);

    int GetNumPlanes() const;
    const Plane* GetPlanes() const;
    void SetPlanes(const Plane* planes, int num);
    const Vector3* GetCorners() const;

private:
    Plane m_planes[6];
    int m_numPlanes = 6;
    Vector3 m_corners[8];
};


CullType CullBox(const AABB& bbox, const Frustum& frustum);
CullType CullBox(const BoundingBox& bbox, const Frustum& frustum);