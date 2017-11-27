#pragma once


#include "SceneObject.h"


class SO_EmptyObject : public SceneObject
{
public:
    SO_EmptyObject(int id, const char* clsName) :
        SceneObject(id, clsName)
    {
    }
};
