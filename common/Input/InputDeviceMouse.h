#pragma once


#include "InputManager.h"


class InputDeviceMouse : public InputDevice
{
public:
    enum { MAX_BUTTONS = 4 };
    enum { CLICK_MAX_DISTANCE = 10 };       // max distance in pixels to detect click events
    enum { CLICK_TIMEOUT = 300 };           // single click timeout in ms
    enum { DOUBLE_CLICK_TIMEOUT = 500 };    // double click timeout in ms

    enum
    {
        BUTTON_STATE_RELEASED,
        BUTTON_STATE_PRESSED,
        BUTTON_STATE_JUST_PRESSED,
        BUTTON_STATE_JUST_RELEASED,
    };

    struct CursorPos
    {
        int x, y;
    };

    class Callback
    {
    public:
        virtual void OnMouseDown(int x, int y, int button) {}
        virtual void OnMouseUp(int x, int y, int button) {}
        virtual void OnMouseMove(int x, int y) {}
        virtual void OnMouseWheel(int delta) {}
        virtual void OnSingleClick(int x, int y, int button) {}
        virtual void OnDoubleClick(int x, int y, int button) {}
        virtual void OnLongPress(int x, int y, int button) {}
    };

    InputDeviceMouse(InputDeviceID id);
    virtual ~InputDeviceMouse();

    void Update() override;

    void LockCursor();
    void UnlockCursor();
    bool IsCursorLocked() const;
    const CursorPos& GetCursorPos() const;

    char GetButtonState(unsigned int button) const;
    bool GetButtonDown(unsigned int button) const;
    bool GetButtonPressed(unsigned int button) const;
    bool GetButtonReleased(unsigned int button) const;

    void AddCallback(Callback* func);
    void RemoveCallback(Callback* func);

    void OnMouseDown(unsigned int button);
    void OnMouseUp(unsigned int button);
    void OnMouseMove(int x, int y);
    void OnMouseWheel(int z);
    void OnCursorPositionChanged(int x, int y);

private:
    bool m_cursorLock = false;
    CursorPos m_cursorPos;
    CursorPos m_clickPos;
    bool m_clickDetected = false;
    size_t m_clickTimer = 0;
    std::set<Callback*> m_callbacks;
    char m_buttonState[MAX_BUTTONS] = {};
};
