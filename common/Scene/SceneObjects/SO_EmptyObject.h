#pragma once


#include <Scene/SceneObject.h>


class SO_EmptyObject : public SceneObject
{
public:
    static constexpr char* CLASS_NAME = "EmptyObject";

    SO_EmptyObject(int id) :
        SceneObject(id, CLASS_NAME)
    {
    }
};
