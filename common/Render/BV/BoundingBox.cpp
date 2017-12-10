#include "pch.h"
#include "BoundingBox.h"


BoundingBox::BoundingBox(const AABB& aabb, const Matrix& transform) :
    m_box(aabb),
    m_transform(transform),
    m_upToDate(false)
{
}


BoundingBox::BoundingBox(const Vector3& mins, const Vector3& maxs, const Matrix& transform) :
    m_box(mins, maxs),
    m_transform(transform),
    m_upToDate(false)
{
}


const Matrix& BoundingBox::GetTransform() const
{
    return m_transform;
}


void BoundingBox::SetTransform(const Matrix& transform)
{
    m_transform = transform;
    m_upToDate = false;
}


const Vector3* BoundingBox::GetCorners() const
{
    auto self = const_cast<BoundingBox*>(this);
    if (!self->m_upToDate)
    {
        self->m_upToDate = true;
        self->ComputeCorners();
    }
    return &m_corners[0];
}


const AABB& BoundingBox::GetAABB() const
{
    return m_box;
}


void BoundingBox::ComputeCorners()
{
    m_corners[0] = Vector3(m_box.mins.x, m_box.mins.y, m_box.mins.z);
    m_corners[1] = Vector3(m_box.mins.x, m_box.mins.y, m_box.maxs.z);
    m_corners[2] = Vector3(m_box.mins.x, m_box.maxs.y, m_box.mins.z);
    m_corners[3] = Vector3(m_box.mins.x, m_box.maxs.y, m_box.maxs.z);
    m_corners[4] = Vector3(m_box.maxs.x, m_box.mins.y, m_box.mins.z);
    m_corners[5] = Vector3(m_box.maxs.x, m_box.mins.y, m_box.maxs.z);
    m_corners[6] = Vector3(m_box.maxs.x, m_box.maxs.y, m_box.mins.z);
    m_corners[7] = Vector3(m_box.maxs.x, m_box.maxs.y, m_box.maxs.z);
}