#pragma once


#include "Input/InputManager.h"
#include "Input/InputDeviceMouse.h"
#include "Input/InputDeviceKeyboard.h"


class InputManagerPC : public InputManager
{
public:
	inline static InputManagerPC* GetInstance()
	{
		static InputManagerPC instance;
		return &instance;
	}

	bool Init() override;
	void Shutdown() override;
	void Update() override;

	void OnKeyDown(int code);
	void OnKeyUp(int code);
	void OnMouseDown(int button);
	void OnMouseUp(int button); 
	void OnMouseMove(int x, int y);
	void OnMouseWheel(int z);

private:
	InputManagerPC();
	~InputManagerPC();

	InputDeviceMouse m_mouse;
	InputDeviceKeyboard m_keyboard;
};
