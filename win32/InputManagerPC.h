#pragma once


#include <Input/InputManager.h>
#include <Input/InputDeviceMouse.h>
#include <Input/InputDeviceKeyboard.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>


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

    void UpdateMouse();
    void UpdateKeyboard();

private:
    POINT m_cursorInitialPos;
    bool m_cursorLocked = false;
    bool m_focus = true;

    std::unique_ptr<InputDeviceMouse> m_mouse;
    std::unique_ptr<InputDeviceKeyboard> m_keyboard;

    IDirectInput8* m_pDI = nullptr;
    IDirectInputDevice8* m_pMouseDevice = nullptr;
    IDirectInputDevice8* m_pKeyboardDevice = nullptr;
    std::vector<BYTE> m_keyState;
    DIMOUSESTATE m_mouseState;
};
