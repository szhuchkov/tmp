#pragma once


class SceneObjectData
{
public:
    void Clear();

    bool HasProperty(const char* name) const;

    // write values
    void SetBool(const char* name, bool value);
    void SetString(const char* name, const char* value);
    void SetString(const std::string& name, const std::string& value);
    void SetInt(const char* name, int value);
    void SetFloat(const char* name, float value);
    void SetVec2(const char* name, const Vector2& value);
    void SetVec3(const char* name, const Vector3& value);
    void SetVec4(const char* name, const Vector4& value);
    void SetMatrix(const char* name, const Matrix& value);

    // read values
    bool GetBool(const char* name) const;
    const char* GetString(const char* name) const;
    const char* GetString(const std::string& name) const;
    int GetInt(const char* name) const;
    float GetFloat(const char* name) const;
    Vector2 GetVec2(const char* name) const;
    Vector3 GetVec3(const char* name) const;
    Vector4 GetVec4(const char* name) const;
    Matrix GetMatrix(const char* name) const;

    // safe get values
    bool GetBool(const char* name, bool* dst) const;
    bool GetString(const char* name, std::string* dst) const;
    bool GetInt(const char* name, int* dst) const;
    bool GetFloat(const char* name, float* dst) const;
    bool GetVec2(const char* name, Vector2* dst) const;
    bool GetVec3(const char* name, Vector3* dst) const;
    bool GetVec4(const char* name, Vector4* dst) const;
    bool GetMatrix(const char* name, Matrix* dst) const;

private:
    std::unordered_map<std::string, std::string> m_items;
};
