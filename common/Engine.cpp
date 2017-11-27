#include "pch.h"
#include "Engine.h"
#include "FileSystem.h"
#include "RenderDevice.h"
#include "InputManager.h"
#include "Scene.h"


Engine::Engine()
{
}


Engine::~Engine()
{
}


bool Engine::Init(void* window, int width, int height, bool fullscreen)
{
	// init file system
	if (!FileSystem::GetInstance()->Init())
	{
		LogPrintf("FileSystem init failed");
		return false;
	}

	// init render device
	if (!RenderDevice::GetInstance()->Init(window, width, height, fullscreen))
	{
		LogPrintf("RenderDevice init failed");
		return false;
	}

	// init input manager
	if (!InputManager::GetInstance()->Init())
	{
		LogPrintf("InputManager init failed");
		return false;
	}

	return true;
}


void Engine::Shutdown()
{
	InputManager::GetInstance()->Shutdown();
	RenderDevice::GetInstance()->Shutdown();
	FileSystem::GetInstance()->Shutdown();
}


bool Engine::Resize(int width, int height)
{
	LogPrintf(">>> Resize(%d, %d)", width, height);

	if (!RenderDevice::GetInstance()->Resize(width, height))
		return false;

	return true;
}


bool Engine::Update()
{
	if (!RenderDevice::GetInstance()->Update())
		return false;

	InputManager::GetInstance()->ClearEvents();
	InputManager::GetInstance()->Update();

    Scene::GetInstance()->Update(15);

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