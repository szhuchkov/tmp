#include "pch.h"
#include "InputDeviceKeyboard.h"


InputDeviceKeyboard::InputDeviceKeyboard(InputDeviceID id) :
    InputDevice(INPUT_DEVICE_CLASS_KEYBOARD, id, "SystemKeyboard"),
    m_keyState(256, KEY_STATE_RELEASED)
{
}


InputDeviceKeyboard::~InputDeviceKeyboard()
{
}


void InputDeviceKeyboard::Update()
{
    for (size_t i = 0; i < MAX_KEYS; i++)
    {
        if (m_keyState[i] == KEY_STATE_JUST_PRESSED)
            m_keyState[i] = KEY_STATE_PRESSED;
        else if (m_keyState[i] == KEY_STATE_JUST_RELEASED)
            m_keyState[i] = KEY_STATE_RELEASED;
    }
}


void InputDeviceKeyboard::AddCallback(Callback* func)
{
    m_callbacks.insert(func);
}


void InputDeviceKeyboard::RemoveCallback(Callback* func)
{
    auto it = m_callbacks.find(func);
    if (it != m_callbacks.end())
        m_callbacks.erase(it);
}


void InputDeviceKeyboard::OnKeyDown(unsigned int code)
{
    if (m_keyState[code] != KEY_STATE_PRESSED)
    {
        LOG_INPUT_EVENT("OnKeyDown(%u)", code);
        m_keyState[code] = KEY_STATE_JUST_PRESSED;
        for (auto item : m_callbacks)
            item->OnKeyDown(code);
    }
}


void InputDeviceKeyboard::OnKeyUp(unsigned int code)
{
    if (m_keyState[code] != KEY_STATE_RELEASED)
    {
        LOG_INPUT_EVENT("OnKeyUp(%u)", code);
        m_keyState[code] = KEY_STATE_JUST_RELEASED;
        for (auto item : m_callbacks)
            item->OnKeyUp(code);
    }
}


bool InputDeviceKeyboard::GetKeyDown(unsigned int code) const
{
    return m_keyState[code] == KEY_STATE_PRESSED;
}


bool InputDeviceKeyboard::GetKeyPressed(unsigned int code) const
{
    return m_keyState[code] == KEY_STATE_JUST_PRESSED;
}


bool InputDeviceKeyboard::GetKeyReleased(unsigned int code) const
{
    return m_keyState[code] == KEY_STATE_JUST_RELEASED;
}
