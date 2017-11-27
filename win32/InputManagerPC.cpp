#include "pch.h"
#include "InputManagerPC.h"


InputManager* InputManager::GetInstance()
{
	return InputManagerPC::GetInstance();
}


InputManagerPC::InputManagerPC()
{
}


InputManagerPC::~InputManagerPC()
{
}


bool InputManagerPC::Init()
{
	AddInputDevice(&m_mouse);
	AddInputDevice(&m_keyboard);
	return InputManager::Init();
}


void InputManagerPC::Shutdown()
{
	RemoveInputDevice(&m_mouse);
	RemoveInputDevice(&m_keyboard);
	InputManager::Shutdown();
}


void InputManagerPC::Update()
{
	InputManager::Update();
}


void InputManagerPC::OnKeyDown(int code)
{
	m_keyboard.OnKeyDown(code);
}


void InputManagerPC::OnKeyUp(int code)
{
	m_keyboard.OnKeyUp(code);
}


void InputManagerPC::OnMouseDown(int button)
{
	m_mouse.OnMouseDown(button);
}


void InputManagerPC::OnMouseUp(int button)
{
	m_mouse.OnMouseUp(button);
}


void InputManagerPC::OnMouseMove(int x, int y)
{
	//LogPrintf("onMouseMove %d, %d", x, y);
	m_mouse.OnMouseMove(x, y);
}


void InputManagerPC::OnMouseWheel(int z)
{
	m_mouse.OnMouseWheel(z);
}

