#pragma once


#include "InputManager.h"


class InputDeviceTouch : public InputDevice
{
public:
	enum
	{
		POINTER_UP,
		POINTER_DOWN,
		POINTER_MOVE,
	};

	InputDeviceTouch();
	virtual ~InputDeviceTouch();

	InputDeviceClass GetDeviceClass() const override;
	const char* GetDeviceName() const override;

	void Update() override;

	bool IsConnected() const override;

	int GetNumButtons() const override;
	int GetButton(int button) const override;

	int GetNumAxis() const override;
	float GetAxis(int axis) const override;

	void OnPointerDown(int pointer, int x, int y);
	void OnPointerUp(int pointer, int x, int y);
	void OnPointerMove(int pointer, int x, int y);

private:
	static const int MAX_POINTERS = 4;
	bool m_pointerDown[MAX_POINTERS];
	int m_pointerX[MAX_POINTERS];
	int m_pointerY[MAX_POINTERS];
};
