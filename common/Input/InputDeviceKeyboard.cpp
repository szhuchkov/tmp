#include "pch.h"
#include "InputDeviceKeyboard.h"


InputDeviceKeyboard::InputDeviceKeyboard()
{
	for (auto& key : m_keys)
		key = KEY_STATE_UP;
}


InputDeviceKeyboard::~InputDeviceKeyboard()
{
}


InputDeviceClass InputDeviceKeyboard::GetDeviceClass() const
{
	return INPUT_DEVICE_CLASS_KEYBOARD;
}


const char* InputDeviceKeyboard::GetDeviceName() const
{
	return "Keyboard";
}


int InputDeviceKeyboard::GetNumButtons() const
{
	return MAX_KEYS;
}


int InputDeviceKeyboard::GetButton(int index) const
{
	return m_keys[index];
}


int InputDeviceKeyboard::GetNumAxis() const
{
	return 0;
}


float InputDeviceKeyboard::GetAxis(int index) const
{
	return 0.0f;
}


void InputDeviceKeyboard::Update()
{
}


bool InputDeviceKeyboard::IsConnected() const
{
	return true;
}


void InputDeviceKeyboard::OnKeyDown(int key)
{
	if (key >= MAX_KEYS)
		return;

	if (m_keys[key] != KEY_STATE_DOWN)
	{
		m_keys[key] = KEY_STATE_DOWN;
		PushButtonEvent(key, KEY_STATE_DOWN);
	}
}


void InputDeviceKeyboard::OnKeyUp(int key)
{
	if (key >= MAX_KEYS)
		return;

	if (m_keys[key] != KEY_STATE_UP)
	{
		m_keys[key] = KEY_STATE_UP;
		PushButtonEvent(key, KEY_STATE_UP);
	}
}
