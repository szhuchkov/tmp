#include "pch.h"
#include "Engine.h"
#include "RenderDevice.h"


Engine::Engine()
{
}


Engine::~Engine()
{
}


bool Engine::Init(void* window, int width, int height, bool fullscreen)
{
	if (!RenderDevice::GetInstance()->Init(window, width, height, fullscreen))
	{
		LogPrintf("RenderDevice init failed");
		return false;
	}

	return true;
}


void Engine::Shutdown()
{
	RenderDevice::GetInstance()->Shutdown();
}


bool Engine::Update()
{
	if (!RenderDevice::GetInstance()->Update())
		return false;

	return true;
}


void Engine::Render()
{
	if (RenderDevice::GetInstance()->BeginFrame())
	{
		const float clearColor[] = { 0.4f, 0.6f, 0.8f, 1.0f };
		const float clearDepth = 1.0f;
		RenderDevice::GetInstance()->Clear(RenderDevice::CLEAR_COLOR | RenderDevice::CLEAR_DEPTH, clearColor, clearDepth, 0);
		RenderDevice::GetInstance()->EndFrame();
	}
}