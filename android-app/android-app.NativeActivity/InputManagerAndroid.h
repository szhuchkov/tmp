#pragma once


#include "Input/InputManager.h"
#include "Input/InputDeviceTouch.h"
#include "Input/InputDeviceKeyboard.h"


class InputManagerAndroid : public InputManager
{
public:
	inline static InputManagerAndroid* GetInstance()
	{
		static InputManagerAndroid instance;
		return &instance;
	}

	bool Init() override;
	void Shutdown() override;

	void OnPointerDown(int pointer, int x, int y);
	void OnPointerUp(int pointer, int x, int y);
	void OnPointerMove(int pointer, int x, int y);

	void OnKeyDown(int code, int scanCode);
	void OnKeyUp(int code, int scanCode);

	void OnAxis(int device, int axis, float value);
	void OnButtonDown(int device, int code, int scanCode);
	void OnButtonUp(int device, int code, int scanCode);

private:
	InputManagerAndroid();
	~InputManagerAndroid();

	InputDeviceTouch m_touch;
	InputDeviceKeyboard m_keys;
};
