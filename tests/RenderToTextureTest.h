#pragma once


#include "UnitTest.h"
#include "TestScene.h"
#include "Render/RenderDevice.h"


class RenderToTextureTest : public UnitTest
{
public:
    RenderToTextureTest()
    {
    }

    ~RenderToTextureTest()
    {
    }

    const char* GetName() override
    {
        return "RenderToTexture";
    }

    bool Run() override
    {
        m_target = RenderDevice::GetInstance()->CreateRenderTarget(256, 256, RenderDevice::TEXF_A8R8G8B8, RenderDevice::TEXF_D24);
        if (!m_target)
            return false;

        RenderDevice::GetInstance()->DeleteRenderTarget(m_target);

        return true;
    }

private:
    RenderTarget* m_target = nullptr;
};
