#include "pch.h"
#include "InputManagerAndroid.h"


InputManager* InputManager::GetInstance()
{
	return InputManagerAndroid::GetInstance();
}


InputManagerAndroid::InputManagerAndroid()
{
}


InputManagerAndroid::~InputManagerAndroid()
{
}


bool InputManagerAndroid::Init()
{
	if (!InputManager::Init())
		return false;

	AddInputDevice(&m_touch);
	AddInputDevice(&m_keys);

	return true;
}


void InputManagerAndroid::Shutdown()
{
	RemoveInputDevice(&m_touch);
	RemoveInputDevice(&m_keys);
	InputManager::Shutdown();
}


void InputManagerAndroid::OnPointerDown(int pointer, int x, int y)
{
	m_touch.OnPointerDown(pointer, x, y);
	//LogPrintf(">>> OnPointerDown (%d, %d, %d)", pointer, x, y);
}


void InputManagerAndroid::OnPointerUp(int pointer, int x, int y)
{
	m_touch.OnPointerUp(pointer, x, y);
	//LogPrintf(">>> OnPointerUp (%d, %d, %d)", pointer, x, y);
}


void InputManagerAndroid::OnPointerMove(int pointer, int x, int y)
{
	m_touch.OnPointerMove(pointer, x, y);
}


void InputManagerAndroid::OnKeyDown(int code, int scanCode)
{
	m_keys.OnKeyDown(code);
	//LogPrintf(">>> OnKeyDown (%d, %d)", code, scanCode);
}


void InputManagerAndroid::OnKeyUp(int code, int scanCode)
{
	m_keys.OnKeyUp(code);
	//LogPrintf(">>> OnKeyUp (%d, %d)", code, scanCode);
}


void InputManagerAndroid::OnButtonDown(int deviceId, int code, int scanCode)
{
	LogPrintf(">>> OnButtonDown (%d, %d, %d)", deviceId, code, scanCode);
}


void InputManagerAndroid::OnButtonUp(int deviceId, int code, int scanCode)
{
	LogPrintf(">>> OnButtonUp (%d, %d, %d", deviceId, code, scanCode);
}

