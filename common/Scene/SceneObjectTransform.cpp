#include "pch.h"
#include "SceneObject.h"
#include "SceneObjectTransform.h"


SceneObjectTransform::SceneObjectTransform(SceneObject* obj) :
    m_object(obj)
{
    AffineIdentity(&m_position);
}


SceneObjectTransform::~SceneObjectTransform()
{
}


const AffineTransform& SceneObjectTransform::GetPosition() const
{
    return m_position;
}


void SceneObjectTransform::SetPosition(const AffineTransform& pos)
{
    m_position = pos;
    m_object->OnTransformChanged();
}


const SceneObject* SceneObjectTransform::GetSceneObject() const
{
    return m_object;
}


SceneObject* SceneObjectTransform::GetSceneObject()
{
    return m_object;
}