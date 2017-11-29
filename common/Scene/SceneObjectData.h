#pragma once


class SceneObjectData
{
public:
    void Clear();

    bool HasProperty(const char* name) const;

    // write values
    void SetString(const char* name, const char* value);
    void SetInt(const char* name, int value);
    void SetFloat(const char* name, float value);
    void SetVec2(const char* name, const Vector2& value);
    void SetVec3(const char* name, const Vector3& value);
    void SetVec4(const char* name, const Vector4& value);
    void SetMatrix(const char* name, const Matrix& value);

    // read values
    const char* GetString(const char* name) const;
    int GetInt(const char* name) const;
    float GetFloat(const char* name) const;
    Vector2 GetVec2(const char* name) const;
    Vector3 GetVec3(const char* name) const;
    Vector4 GetVec4(const char* name) const;
    Matrix GetMatrix(const char* name) const;

private:
    std::unordered_map<std::string, std::string> m_items;
};
