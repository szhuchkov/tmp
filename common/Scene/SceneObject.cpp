#include "pch.h"
#include "SceneObject.h"
#include "SceneObjectTransform.h"
#include "SceneObjectCollision.h"
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


void SceneObject::CreateTransform()
{
    m_transform = std::make_unique<SceneObjectTransform>(this);
}


const SceneObjectTransform* SceneObject::GetTransform() const
{
    return m_transform.get();
}


SceneObjectTransform* SceneObject::GetTransform()
{
    return m_transform.get();
}


void SceneObject::OnTransformChanged()
{
    if (m_collision)
        m_collision->SetPosition(m_transform->GetPosition());
}


void SceneObject::CreateCollision()
{
    m_collision = std::make_unique<SceneObjectCollision>(this);
}


const SceneObjectCollision* SceneObject::GetCollision() const
{
    return m_collision.get();
}


SceneObjectCollision* SceneObject::GetCollision()
{
    return m_collision.get();
}
