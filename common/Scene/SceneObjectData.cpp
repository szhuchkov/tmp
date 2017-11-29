#include "pch.h"
#include "SceneObjectData.h"


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
    const float* ptr = reinterpret_cast<const float*>(&value);
    std::string buffer;
    for (int i = 0; i < 16; i++)
    {
        buffer += std::to_string(ptr[i]);
        if (i != 15)
            buffer += ',';
    }
    SetString(name, buffer.c_str());
}


const char* SceneObjectData::GetString(const char* name) const
{
    auto it = m_items.find(name);
    if (it != m_items.end())
        return it->second.c_str();
    return "";
}
