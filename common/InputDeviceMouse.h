#pragma once


#include "InputManager.h"


class InputDeviceMouse : public InputDevice
{
public:
	enum
	{
		BUTTON_STATE_UP,
		BUTTON_STATE_DOWN,
	};

	InputDeviceMouse();
	virtual ~InputDeviceMouse();

	InputDeviceClass GetDeviceClass() const override;
	const char* GetDeviceName() const override;

	void Update() override;

	bool IsConnected() const override;

	int GetNumButtons() const override;
	int GetButton(int button) const override;

	int GetNumAxis() const override;
	float GetAxis(int axis) const override;

	void OnMouseDown(int button);
	void OnMouseUp(int button);
	void OnMouseMove(int x, int y);
	void OnMouseWheel(int z);

private:
	static const int MAX_MOUSE_BUTTONS = 4;
	int m_buttons[MAX_MOUSE_BUTTONS];
	static const int MAX_MOUSE_AXIS = 3;
	int m_axis[MAX_MOUSE_AXIS];
};
