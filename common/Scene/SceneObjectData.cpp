#include "pch.h"
#include "SceneObjectData.h"


void SceneObjectData::Clear()
{
    m_items.clear();
}


bool SceneObjectData::HasProperty(const char* name) const
{
    auto it = m_items.find(name);
    if (it != m_items.end())
        return true;
    return false;
}


void SceneObjectData::SetBool(const char* name, bool value)
{
    m_items[name] = value ? "true" : "false";
}


void SceneObjectData::SetString(const char* name, const char* value)
{
    m_items[name] = value ? value : "";
}


void SceneObjectData::SetInt(const char* name, int value)
{
    char buffer[256];
    sprintf(buffer, "%d", value);
    SetString(name, buffer);
}


void SceneObjectData::SetFloat(const char* name, float value)
{
    char buffer[256];
    sprintf(buffer, "%f", value);
    SetString(name, buffer);
}


void SceneObjectData::SetVec2(const char* name, const Vector2& value)
{
    char buffer[256];
    sprintf(buffer, "%f,%f", value.x, value.y);
    SetString(name, buffer);
}


void SceneObjectData::SetVec3(const char* name, const Vector3& value)
{
    char buffer[256];
    sprintf(buffer, "%f,%f,%f", value.x, value.y, value.z);
    SetString(name, buffer);
}


void SceneObjectData::SetVec4(const char* name, const Vector4& value)
{
    char buffer[256];
    sprintf(buffer, "%f,%f,%f,%f", value.x, value.y, value.z, value.w);
    SetString(name, buffer);
}


void SceneObjectData::SetMatrix(const char* name, const Matrix& value)
{
    const float* m = reinterpret_cast<const float*>(&value);
    char buffer[1024];
    sprintf(buffer, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
        m[0], m[1], m[2], m[3],
        m[4], m[5], m[6], m[7],
        m[8], m[9], m[10], m[11],
        m[12], m[13], m[14], m[15]);
    SetString(name, buffer);
}


bool SceneObjectData::GetBool(const char* name) const
{
    auto it = m_items.find(name);
    if (it != m_items.end())
        return it->second == "true";
    return false;
}


const char* SceneObjectData::GetString(const char* name) const
{
    auto it = m_items.find(name);
    if (it != m_items.end())
        return it->second.c_str();
    return "";
}


int SceneObjectData::GetInt(const char* name) const
{
    auto s = GetString(name);
    return std::atoi(s);
}


float SceneObjectData::GetFloat(const char* name) const
{
    auto s = GetString(name);
    return static_cast<float>(std::atof(s));
}


Vector2 SceneObjectData::GetVec2(const char* name) const
{
    Vector2 res(0, 0);
    auto s = GetString(name);
    sscanf(s, "%f,%f", &res.x, &res.y);
    return res;
}


Vector3 SceneObjectData::GetVec3(const char* name) const
{
    Vector3 res(0, 0, 0);
    auto s = GetString(name);
    sscanf(s, "%f,%f,%f", &res.x, &res.y, &res.z);
    return res;
}


Vector4 SceneObjectData::GetVec4(const char* name) const
{
    Vector4 res(0, 0, 0, 0);
    auto s = GetString(name);
    sscanf(s, "%f,%f,%f,%f", &res.x, &res.y, &res.z, &res.w);
    return res;
}


Matrix SceneObjectData::GetMatrix(const char* name) const
{
    Matrix res(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
    float* m = reinterpret_cast<float*>(&res);
    auto s = GetString(name);
    sscanf(s, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f",
        &m[0], &m[1], &m[2], &m[3],
        &m[4], &m[5], &m[6], &m[7],
        &m[8], &m[9], &m[10], &m[11],
        &m[12], &m[13], &m[14], &m[15]);
    return res;
}
