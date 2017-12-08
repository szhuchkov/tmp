#include "pch.h"
#include <Render/BaseMaterial.h>
#include <Render/RenderWorld.h>
#include <Render/RenderDevice.h>
#include <Render/ShaderManager.h>
#include <Render/TextureManager.h>


BaseMaterial::BaseMaterial(const char* name) :
    m_name(name),
    m_shadings(MATERIAL_SHADING_COUNT, nullptr)
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
    m_shadings.clear();
}


const char* BaseMaterial::GetName() const
{
    return m_name.c_str();
}


bool BaseMaterial::SetShading(unsigned int shading, const char* vs, const char* ps)
{
    // get requested shader
    auto shader = GetShader(vs, ps);
    if (!shader)
        return false;

    // set new shading
    m_shadings[shading] = shader;

    // all fine
    return true;
}


bool BaseMaterial::SetShading(unsigned int shading, Shader* shader)
{
    // set new shading
    m_shadings[shading] = shader;

    // all fine
    return true;
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
    // setup shading
    if (m_shadings[context->shading] == nullptr)
        return false;

    RenderDevice::GetInstance()->SetShader(m_shadings[context->shading]);

    switch (context->shading)
    {
    case MATERIAL_SHADING_SKY_SHADOW:
    case MATERIAL_SHADING_SKY_LIGHT:
    {
        Vector4 lightColor1(context->light->color[0], context->light->color[1], context->light->color[2], 1.0f);
        Vector4 lightColor2(1.0f, 1.0f, 1.0f, 1.0f);
        SetUniform(0, lightColor1);
        SetUniform(1, lightColor2);

        SetMatrix(RenderDevice::MATRIX_LIGHT, context->light->position);
    }
    break;
    }

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
