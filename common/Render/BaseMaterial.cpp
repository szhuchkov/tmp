#include "pch.h"
#include <Render/BaseMaterial.h>
#include <Render/RenderWorld.h>
#include <Render/RenderDevice.h>
#include <Render/ShaderManager.h>
#include <Render/TextureManager.h>


BaseMaterial::BaseMaterial(const char* name) :
    m_name(name)
{
}


BaseMaterial::~BaseMaterial()
{
}


bool BaseMaterial::Init()
{
    return true;
}


void BaseMaterial::Shutdown()
{
}


const char* BaseMaterial::GetName() const
{
    return m_name.c_str();
}


void BaseMaterial::SetSortIndex(int index)
{
    m_sortIndex = index;
}


int BaseMaterial::GetSortIndex() const
{
    return m_sortIndex;
}


bool BaseMaterial::Begin(RenderContext* context)
{
    return true;
}


void BaseMaterial::End(RenderContext* context)
{
}


void BaseMaterial::SetMaterial(RenderMaterial* material)
{
    // bind textures
    for (int i = 0; i < 4; i++)
    {
        RenderDevice::GetInstance()->SetTexture(i, material->texmaps[i]);
    }

    //TODO: two-sided materials
    if (material->flags & (1 << MATERIAL_FLAG_TWOSIDED))
    {
    }
    else
    {
    }
}


Shader* BaseMaterial::GetShader(const char* vs, const char* ps, const char* gs)
{
    return ShaderManager::GetInstance()->GetShader(vs, ps, gs);
}


Texture* BaseMaterial::LoadTexture(const char* name)
{
    return TextureManager::GetInstance()->LoadTexture(name);
}


void BaseMaterial::SetShader(Shader* shader)
{
    RenderDevice::GetInstance()->SetShader(shader);
}


void BaseMaterial::SetTexture(unsigned int index, Texture* value)
{
    RenderDevice::GetInstance()->SetTexture(index, value);
}


void BaseMaterial::SetUniform(unsigned int index, const Vector4& value)
{
    RenderDevice::GetInstance()->SetUniform(index, value);
}


void BaseMaterial::SetMatrix(unsigned int index, const Matrix& value)
{
    RenderDevice::GetInstance()->SetMatrix(index, value);
}
