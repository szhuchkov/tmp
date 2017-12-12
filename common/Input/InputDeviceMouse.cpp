#include "pch.h"
#include "InputDeviceMouse.h"


InputDeviceMouse::InputDeviceMouse(InputDeviceID id) :
    InputDevice(INPUT_DEVICE_CLASS_MOUSE, id, "SystemMouse")
{
}


InputDeviceMouse::~InputDeviceMouse()
{
}


void InputDeviceMouse::Update()
{
}


bool InputDeviceMouse::IsCursorLocked() const
{
    return m_cursorLock;
}


void InputDeviceMouse::LockCursor()
{
    m_cursorLock = true;
}


void InputDeviceMouse::UnlockCursor()
{
    m_cursorLock = false;
}


void InputDeviceMouse::AddCallback(Callback* func)
{
    m_callbacks.insert(func);
}


void InputDeviceMouse::RemoveCallback(Callback* func)
{
    auto it = m_callbacks.find(func);
    if (it != m_callbacks.end())
        m_callbacks.erase(it);
}


void InputDeviceMouse::OnMouseDown(unsigned int button)
{
    if (m_buttonState[button] != BUTTON_STATE_PRESSED)
    {
        if (button == 0)
        {
            auto timer = Sys_Milliseconds();
            auto timeout = timer - m_clickTimer;

            if (timeout > CLICK_TIMEOUT)
            {
                // initiate click event
                m_clickDetected = false;
                m_clickPos = m_cursorPos;
                m_clickTimer = timer;
            }
            else
            {
                // initiate double click event
                if (m_clickDetected)
                    m_clickTimer = timer;
            }
        }

        m_buttonState[button] = BUTTON_STATE_JUST_PRESSED;
        for (auto item : m_callbacks)
            item->OnMouseDown(m_cursorPos.x, m_cursorPos.y, button);
	}
}


void InputDeviceMouse::OnMouseUp(unsigned int button)
{
    if (m_buttonState[button] != BUTTON_STATE_RELEASED)
	{
        if (button == 0)
        {
            auto timer = Sys_Milliseconds();
            auto timeout = timer - m_clickTimer;

            // detect click events
            if (std::abs(m_clickPos.x - m_cursorPos.x) < CLICK_MAX_DISTANCE &&
                std::abs(m_clickPos.y - m_cursorPos.y) < CLICK_MAX_DISTANCE)
            {
                // detect double click
                if (m_clickDetected && timeout < DOUBLE_CLICK_TIMEOUT)
                {
                    LOG_INPUT_EVENT("OnDoubleClick(%d, %d)", m_clickPos.x, m_clickPos.y);

                    for (auto item : m_callbacks)
                        item->OnDoubleClick(m_cursorPos.x, m_cursorPos.y, 0);

                    m_clickDetected = false;
                }
                else if (!m_clickDetected && timeout < CLICK_TIMEOUT)
                {
                    LOG_INPUT_EVENT("OnSingleClick(%d, %d)", m_clickPos.x, m_clickPos.y);
                    
                    for (auto item : m_callbacks)
                        item->OnSingleClick(m_cursorPos.x, m_cursorPos.y, 0);

                    m_clickTimer = timer;
                    m_clickDetected = true;
                }
                else
                {
                    m_clickDetected = false;
                }
            }
            else
            {
                m_clickDetected = false;
            }
        }

        m_buttonState[button] = BUTTON_STATE_JUST_RELEASED;
        for (auto item : m_callbacks)
            item->OnMouseUp(m_cursorPos.x, m_cursorPos.y, button);
	}
}


void InputDeviceMouse::OnMouseMove(int x, int y)
{
    for (auto item : m_callbacks)
        item->OnMouseMove(x, y);
}


void InputDeviceMouse::OnMouseWheel(int z)
{
    for (auto item : m_callbacks)
        item->OnMouseWheel(z);
}


const InputDeviceMouse::CursorPos& InputDeviceMouse::GetCursorPos() const
{
    return m_cursorPos;
}


char InputDeviceMouse::GetButtonState(unsigned int button) const
{
    return m_buttonState[button];
}


bool InputDeviceMouse::GetButtonDown(unsigned int button) const
{
    return m_buttonState[button] == BUTTON_STATE_PRESSED;
}


bool InputDeviceMouse::GetButtonPressed(unsigned int button) const
{
    return m_buttonState[button] == BUTTON_STATE_JUST_PRESSED;
}


bool InputDeviceMouse::GetButtonReleased(unsigned int button) const
{
    return m_buttonState[button] == BUTTON_STATE_JUST_RELEASED;
}


void InputDeviceMouse::OnCursorPositionChanged(int x, int y)
{
    m_cursorPos = { x, y };
}