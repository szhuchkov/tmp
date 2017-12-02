#include "pch.h"
#include "SceneObject.h"
#include "SceneObjectData.h"


SceneObject::SceneObject(int id, const char* className) :
    m_className(className),
    m_id(id)
{
}


SceneObject::~SceneObject()
{
}


const char* SceneObject::GetClsName() const
{
    return m_className.c_str();
}


int SceneObject::GetID() const
{
    return m_id;
}


bool SceneObject::IsAllive() const
{
    return m_isAllive;
}


bool SceneObject::Load(const SceneObjectData& data)
{
    return true;
}


bool SceneObject::Save(SceneObjectData& data)
{
    data.SetString("ClassName", GetClsName());
    return true;
}


void SceneObject::Update(unsigned int dt)
{
    if (m_killTimeout != ~0)
    {
        if (m_killTimeout > dt)
            m_killTimeout -= dt;
        else
            Kill();
    }
}


void SceneObject::Kill()
{
    m_isAllive = false;
}


void SceneObject::Spawn()
{
}
