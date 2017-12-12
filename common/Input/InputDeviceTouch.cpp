#include "pch.h"
#include "InputDeviceTouch.h"


InputDeviceTouch::~InputDeviceTouch()
{
}


void InputDeviceTouch::Update()
{
    // update state for pointers
    for (int i = 0; i < MAX_POINTERS; i++)
    {
        if (m_pointerState[i] == POINTER_STATE_JUST_UP)
            m_pointerState[i] = POINTER_STATE_UP;
        else if (m_pointerState[i] == POINTER_STATE_JUST_DOWN)
            m_pointerState[i] = POINTER_STATE_DOWN;
    }
}


std::vector<unsigned int> InputDeviceTouch::GetActivePointers() const
{
    std::vector<unsigned int> pointers;
    for (unsigned int i = 0; i < MAX_POINTERS; i++)
    {
        if (m_pointerState[i] == POINTER_STATE_DOWN)
            pointers.push_back(i);
    }
    return pointers;
}


InputDeviceTouch::PointerPos InputDeviceTouch::GetPointerPos(unsigned int pointer) const
{
    return m_pointerPos[pointer];
}


char InputDeviceTouch::GetPointerState(unsigned int pointer) const
{
    return m_pointerState[pointer];
}


bool InputDeviceTouch::GetPointerDown(unsigned int pointer) const
{
    return m_pointerState[pointer] == POINTER_STATE_DOWN;
}


void InputDeviceTouch::OnPointerDown(int id, int x, int y)
{
    auto pointer = AllocPointer(id);
    if (pointer != -1)
    {
        m_pointerID[pointer] = id;
        m_pointerPos[pointer] = { x, y };
        m_pointerState[pointer] = POINTER_STATE_JUST_DOWN;
    }
}


void InputDeviceTouch::OnPointerUp(int id, int x, int y)
{
    auto pointer = GetPointer(id);
    if (pointer != -1)
    {
        m_pointerState[pointer] = POINTER_STATE_JUST_UP;
        ReleasePointer(id);
    }
}


int InputDeviceTouch::AllocPointer(int id)
{
    for (int i = 0; i < MAX_POINTERS; i++)
    {
        if (m_pointerID[i] == -1)
            return i;
    }
    return -1;
}


void InputDeviceTouch::ReleasePointer(int id)
{
    for (int i = 0; i < MAX_POINTERS; i++)
    {
        if (m_pointerID[i] == id)
        {
            m_pointerID[i] = -1;
            break;
        }
    }
}


int InputDeviceTouch::GetPointer(int id)
{
    for (int i = 0; i < MAX_POINTERS; i++)
    {
        if (m_pointerID[i] == id)
            return i;
    }
    return -1;
}