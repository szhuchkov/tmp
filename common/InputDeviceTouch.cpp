#include "pch.h"
#include "InputDeviceTouch.h"


InputDeviceTouch::InputDeviceTouch()
{
	for(int i = 0; i < MAX_POINTERS; i++)
	{
		m_pointerDown[i] = false;
		m_pointerX[i] = m_pointerY[i] = 0;
	}
}


InputDeviceTouch::~InputDeviceTouch()
{
}


InputDeviceClass InputDeviceTouch::GetDeviceClass() const
{
	return INPUT_DEVICE_CLASS_TOUCH;
}


const char * InputDeviceTouch::GetDeviceName() const
{
	return "Touch";
}


void InputDeviceTouch::Update()
{
}


bool InputDeviceTouch::IsConnected() const
{
	return true;
}


int InputDeviceTouch::GetNumButtons() const
{
	return 0;
}


int InputDeviceTouch::GetButton(int button) const
{
	return 0;
}


int InputDeviceTouch::GetNumAxis() const
{
	return 2 * MAX_POINTERS;
}


float InputDeviceTouch::GetAxis(int axis) const
{
	return static_cast<float>(axis & 1 ? m_pointerY[axis / 2] : m_pointerX[axis / 2]);
}


void InputDeviceTouch::OnPointerDown(int pointer, int x, int y)
{
	if (pointer >= MAX_POINTERS)
		return;

	if (!m_pointerDown[pointer])
	{
		m_pointerDown[pointer] = true;
		PushTouchEvent(pointer, POINTER_DOWN, x, y);
	}

	m_pointerX[pointer] = x;
	m_pointerY[pointer] = y;
}


void InputDeviceTouch::OnPointerUp(int pointer, int x, int y)
{
	if (pointer >= MAX_POINTERS)
		return;

	if (m_pointerDown[pointer])
	{
		m_pointerDown[pointer] = false;
		PushTouchEvent(pointer, POINTER_UP, x, y);
	}

	m_pointerX[pointer] = x;
	m_pointerY[pointer] = y;
}


void InputDeviceTouch::OnPointerMove(int pointer, int x, int y)
{
	if (pointer >= MAX_POINTERS)
		return;

	if (m_pointerDown[pointer])
	{
		if (m_pointerX[pointer] != x || m_pointerY[pointer] != y)
			PushTouchEvent(pointer, POINTER_MOVE, x, y);
	}

	m_pointerX[pointer] = x;
	m_pointerY[pointer] = y;
}
