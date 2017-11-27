#include "pch.h"
#include "Engine.h"
#include "FileSystem.h"
#include "RenderDevice.h"
#include "InputManager.h"
#include "RenderWorld.h"
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

    // init render world
    if (!RenderWorld::GetInstance()->Init())
    {
        LogPrintf("RenderWorld init failed");
        return false;
    }

	return true;
}


void Engine::Shutdown()
{
    RenderWorld::GetInstance()->Shutdown();
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
        RenderWorld::GetInstance()->Render();
		RenderDevice::GetInstance()->EndFrame();
	}
}