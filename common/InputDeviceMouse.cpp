#include "pch.h"
#include "InputDeviceMouse.h"


InputDeviceMouse::InputDeviceMouse()
{
	for (auto& button : m_buttons)
		button = BUTTON_STATE_UP;

	for (auto& axis : m_axis)
		axis = 0;
}


InputDeviceMouse::~InputDeviceMouse()
{
}


InputDeviceClass InputDeviceMouse::GetDeviceClass() const
{
	return INPUT_DEVICE_CLASS_MOUSE;
}


const char* InputDeviceMouse::GetDeviceName() const
{
	return "Mouse";
}


void InputDeviceMouse::Update()
{
}


bool InputDeviceMouse::IsConnected() const
{
	return true;
}


int InputDeviceMouse::GetNumButtons() const
{
	return MAX_MOUSE_BUTTONS;
}


int InputDeviceMouse::GetButton(int index) const
{
	return m_buttons[index];
}


int InputDeviceMouse::GetNumAxis() const
{
	return MAX_MOUSE_AXIS;
}


float InputDeviceMouse::GetAxis(int index) const
{
	return static_cast<float>(m_axis[index]);
}


void InputDeviceMouse::OnMouseDown(int button)
{
	if (button >= MAX_MOUSE_BUTTONS)
		return;

	if (m_buttons[button] != BUTTON_STATE_DOWN)
	{
		m_buttons[button] = BUTTON_STATE_DOWN;
		PushButtonEvent(button, BUTTON_STATE_DOWN);
	}
}


void InputDeviceMouse::OnMouseUp(int button)
{
	if (button >= MAX_MOUSE_BUTTONS)
		return;

	if (m_buttons[button] != BUTTON_STATE_UP)
	{
		m_buttons[button] = BUTTON_STATE_UP;
		PushButtonEvent(button, BUTTON_STATE_UP);
	}
}


void InputDeviceMouse::OnMouseMove(int x, int y)
{
	if (m_axis[0] != x || m_axis[1] != y)
	{
		if (m_axis[0] != x)
		{
			m_axis[0] = x;
			PushAxisEvent(0, static_cast<float>(x));
		}

		if (m_axis[1] != y)
		{
			m_axis[1] = y;
			PushAxisEvent(1, static_cast<float>(y));
		}

		PushTouchEvent(0, 0, x, y);
	}
}


void InputDeviceMouse::OnMouseWheel(int z)
{
	if (m_axis[2] != z)
	{
		m_axis[2] = z;
		PushAxisEvent(2, static_cast<float>(z));
	}
}