#pragma once


#include "InputManager.h"


class InputDeviceKeyboard : public InputDevice
{
public:
	enum
	{
		KEY_STATE_UP,
		KEY_STATE_DOWN,
	};

	InputDeviceKeyboard();
	virtual ~InputDeviceKeyboard();

	InputDeviceClass GetDeviceClass() const override;
	const char* GetDeviceName() const override;

	void Update() override;

	bool IsConnected() const override;

	int GetNumButtons() const override;
	int GetButton(int button) const override;

	int GetNumAxis() const override;
	float GetAxis(int axis) const override;

	void OnKeyDown(int key);
	void OnKeyUp(int key);

private:
	static const int MAX_KEYS = 512;
	int m_keys[MAX_KEYS];
};
