#pragma once


#include "AABB.h"


class BoundingBox
{
public:
    BoundingBox() = default;
    BoundingBox(const BoundingBox& other) = default;
    BoundingBox(const AABB& box, const Matrix& transform);
    BoundingBox(const Vector3& mins, const Vector3& maxs, const Matrix& transform);

    const Matrix& GetTransform() const;
    void SetTransform(const Matrix& transform);
    const Vector3* GetCorners() const;
    const AABB& GetAABB() const;

private:
    void ComputeCorners();

private:
    AABB m_box;
    Matrix m_transform;
    bool m_upToDate = true;
    Vector3 m_corners[8];
};
