#include "pch.h"
#include "Frustum.h"
#include "AABB.h"
#include "BoundingBox.h"


Frustum::Frustum(const Matrix& view, const Matrix& proj)
{
    FromViewProj(view, proj);
}


void Frustum::FromViewProj(const Matrix& view, const Matrix& proj)
{
    FromViewProj(view * proj);
}


void Frustum::FromViewProj(const Matrix& viewProj)
{
    auto mat = reinterpret_cast<const float*>(&viewProj);

    // Left Plane
    m_planes[PLANE_LEFT].x = mat[3] + mat[0];
    m_planes[PLANE_LEFT].y = mat[7] + mat[4];
    m_planes[PLANE_LEFT].z = mat[11] + mat[8];
    m_planes[PLANE_LEFT].w = mat[15] + mat[12];

    // Right Plane
    m_planes[PLANE_RIGHT].x = mat[3] - mat[0];
    m_planes[PLANE_RIGHT].y = mat[7] - mat[4];
    m_planes[PLANE_RIGHT].z = mat[11] - mat[8];
    m_planes[PLANE_RIGHT].w = mat[15] - mat[12];

    // Bottom Plane
    m_planes[PLANE_BOTTOM].x = mat[3] + mat[1];
    m_planes[PLANE_BOTTOM].y = mat[7] + mat[5];
    m_planes[PLANE_BOTTOM].z = mat[11] + mat[9];
    m_planes[PLANE_BOTTOM].w = mat[15] + mat[13];

    // Top Plane
    m_planes[PLANE_TOP].x = mat[3] - mat[1];
    m_planes[PLANE_TOP].y = mat[7] - mat[5];
    m_planes[PLANE_TOP].z = mat[11] - mat[9];
    m_planes[PLANE_TOP].w = mat[15] - mat[13];

    // Near Plane
    m_planes[PLANE_NEAR].x = mat[3] + mat[2];
    m_planes[PLANE_NEAR].y = mat[7] + mat[6];
    m_planes[PLANE_NEAR].z = mat[11] + mat[10];
    m_planes[PLANE_NEAR].w = mat[15] + mat[14];

    // Far Plane
    m_planes[PLANE_FAR].x = mat[3] - mat[2];
    m_planes[PLANE_FAR].y = mat[7] - mat[6];
    m_planes[PLANE_FAR].z = mat[11] - mat[10];
    m_planes[PLANE_FAR].w = mat[15] - mat[14];
}


int Frustum::GetNumPlanes() const
{
    return m_numPlanes;
}


const Plane* Frustum::GetPlanes() const
{
    return &m_planes[0];
}


const Vector3* Frustum::GetCorners() const
{
    return &m_corners[0];
}


void Frustum::SetPlanes(const Plane* planes, int num)
{
    for (int i = 0; i < num; i++)
        m_planes[i] = planes[i];
    m_numPlanes = num;
}


CullType CullBox(const BoundingBox& bbox, const Frustum& frustum)
{
    auto planes = frustum.GetPlanes();
    auto corners = bbox.GetCorners();
    int total = 0;
    for (int planeIndex = 0; planeIndex < frustum.GetNumPlanes(); planeIndex++)
    {
        auto plane = planes[planeIndex];
        int inside = 0;
        for (int cornerIndex = 0; cornerIndex < 8; cornerIndex++)
        {
            auto point = corners[cornerIndex];
            if (PointOnPlaneSide(point, plane))
                inside++;
        }
        if (inside == 0)
            return CULL_OUT;
        if (inside == 8)
            total++;
    }
    if (total == 6)
        return CULL_IN;
    return CULL_CLIP;
}


CullType CullBox(const AABB& bbox, const Frustum& frustum)
{
    auto corners = bbox.GetCorners();
    auto planes = frustum.GetPlanes();
    int total = 0;
    for (int planeIndex = 0; planeIndex < frustum.GetNumPlanes(); planeIndex++)
    {
        auto plane = planes[planeIndex];
        int inside = 0;
        for (int cornerIndex = 0; cornerIndex < 8; cornerIndex++)
        {
            auto point = corners[cornerIndex];
            if (PointOnPlaneSide(point, plane))
                inside++;
        }
        if (inside == 0)
            return CULL_OUT;
    }
    if (total == 6)
        return CULL_IN;
    return CULL_CLIP;
}