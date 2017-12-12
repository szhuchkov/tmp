#pragma once


#include "InputManager.h"
#include "KeyCodes.h"


class InputDeviceKeyboard : public InputDevice
{
public:
    enum
    {
        KEY_STATE_RELEASED,
        KEY_STATE_PRESSED,
        KEY_STATE_JUST_PRESSED,
        KEY_STATE_JUST_RELEASED,
    };

    enum { MAX_KEYS = 256 };

    class Callback
    {
    public:
        virtual void OnKeyDown(unsigned int code) {}
        virtual void OnKeyUp(unsigned int code) {}
    };

	InputDeviceKeyboard(InputDeviceID id);
	virtual ~InputDeviceKeyboard();

	void Update() override;

    void AddCallback(Callback* func);
    void RemoveCallback(Callback* func);

    char GetKeyState(unsigned int code) const;
    bool GetKeyDown(unsigned int code) const;
    bool GetKeyPressed(unsigned int code) const;
    bool GetKeyReleased(unsigned int code) const;

	void OnKeyDown(unsigned int code);
	void OnKeyUp(unsigned int code);

private:
    std::set<Callback*> m_callbacks;
    std::vector<char> m_keyState;
};
