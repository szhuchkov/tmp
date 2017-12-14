#include "pch.h"
#include <Engine.h>
#include <FileSystem.h>
#include <Render/RenderDevice.h>
#include <Render/RenderWorld.h>
#include <Render/DebugRender.h>
#include <Input/InputManager.h>
#include <Scene/Scene.h>


Engine::Engine()
{
}


Engine::~Engine()
{
}


bool Engine::Init(void* window, int width, int height, bool fullscreen)
{
    m_window = window;

    // init timer
    m_startTime = Sys_Milliseconds();

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

    // init debug render
    if (!DebugRender::GetInstance()->Init())
    {
        LogPrintf("DebugRender init failed");
        return false;
    }

	return true;
}


void Engine::Shutdown()
{
    DebugRender::GetInstance()->Shutdown();
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
    // update timer
    auto time = Sys_Milliseconds();
    auto currTime = time - m_startTime;
    auto deltaTime = currTime - m_currTime;
    m_currTime = currTime;

    // update render device
	if (!RenderDevice::GetInstance()->Update())
		return false;

    // update input manager
	InputManager::GetInstance()->Update();

    // update scene
    Scene::GetInstance()->Update(deltaTime);

	return true;
}


void Engine::Render()
{
	if (RenderDevice::GetInstance()->BeginFrame())
	{
        RenderWorld::GetInstance()->Render();
        DebugRender::GetInstance()->Render();
		RenderDevice::GetInstance()->EndFrame();
	}
}


void* Engine::GetWindow()
{
    return m_window;
}


size_t Engine::GetTime()
{
    return m_currTime;
}