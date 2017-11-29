#pragma once


#include "UnitTest.h"
#include "Scene\SceneObjectData.h"


//=====================================
// SceneObjectDataTest
// Unit test for named value serrialization
//=====================================
class SceneObjectDataTest : public UnitTest
{
public:
    // float cmp precission
    const float eps = 0.001f;

    // name of the test
    const char* GetName() override
    {
        return "SceneObjectDataTest";
    }

    // compare structs of float
    // @return true if NOT equal
    template <typename _T>
    bool NotEqual(const _T& a, const _T& b)
    {
        const float* pa = reinterpret_cast<const float*>(&a);
        const float* pb = reinterpret_cast<const float*>(&b);

        // num floats in a struct
        int dim = sizeof(_T) / sizeof(float);

        // compare as float arrays
        for (int i = 0; i < dim; i++)
        {
            if (std::abs(pa[i] - pb[i]) > eps)
                return true;
        }
        return false;
    }

    // random float number
    float R()
    {
        float sign = std::rand() & 1 ? 1.0f : -1.0f;
        return sign * std::rand() / RAND_MAX;
    }

    // run test
    // @return true if succeeded
    bool Run() override
    {
        SceneObjectData data;

        // generate random values for each value type
        int i = 13;
        float f = R();
        const char* s = "Test string";
        Vector2 v2(R(), R());
        Vector3 v3(R(), R(), R());
        Vector4 v4(R(), R(), R(), R());
        Matrix m(
            R(), R(), R(), R(),
            R(), R(), R(), R(),
            R(), R(), R(), R(),
            R(), R(), R(), R());

        // write values
        data.SetInt("i", i);
        data.SetFloat("f", f);
        data.SetString("s", s);
        data.SetVec2("v2", v2);
        data.SetVec3("v3", v3);
        data.SetVec4("v4", v4);
        data.SetMatrix("m", m);

        // check value exists
        if (!data.HasProperty("i") ||
            !data.HasProperty("f") ||
            !data.HasProperty("s") ||
            !data.HasProperty("v2") ||
            !data.HasProperty("v3") ||
            !data.HasProperty("v4") ||
            !data.HasProperty("m"))
            return false;

        // read values and cmp to ref
        if (data.GetInt("i") != i)
            return false;
        if (strcmp(data.GetString("s"), s))
            return false;
        if (NotEqual(data.GetFloat("f"), f))
            return false;
        if (NotEqual(data.GetVec2("v2"), v2))
            return false;
        if (NotEqual(data.GetVec3("v3"), v3))
            return false;
        if (NotEqual(data.GetVec4("v4"), v4))
            return false;
        if (NotEqual(data.GetMatrix("m"), m))
            return false;

        // clear values
        data.Clear();
        if (data.HasProperty("i"))
            return false;

        return true;
    }
};
