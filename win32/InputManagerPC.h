#pragma once


#include <Input/InputManager.h>
#include <Input/InputDeviceMouse.h>
#include <Input/InputDeviceKeyboard.h>


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

    void OnFocusChanged(bool focus);
	void OnKeyDown(int code);
	void OnKeyUp(int code);
	void OnMouseDown(int button);
	void OnMouseUp(int button); 
	void OnMouseMove(int x, int y);
	void OnMouseWheel(int z);

private:
	InputManagerPC();
	~InputManagerPC();

    POINT m_cursorInitialPos;
    bool m_cursorLocked = false;
    bool m_focus = true;

	InputDeviceMouse m_mouse;
	InputDeviceKeyboard m_keyboard;
};
