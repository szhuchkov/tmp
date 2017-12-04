#include "pch.h"
#include "Engine.h"
#include "InputManagerPC.h"


InputManager* InputManager::GetInstance()
{
	return InputManagerPC::GetInstance();
}


InputManagerPC::InputManagerPC()
{
}


InputManagerPC::~InputManagerPC()
{
}


bool InputManagerPC::Init()
{
	AddInputDevice(&m_mouse);
	AddInputDevice(&m_keyboard);

    HWND hwnd = static_cast<HWND>(Engine::GetInstance()->GetWindow());
    RECT rect;
    GetWindowRect(hwnd, &rect);
    m_cursorInitialPos.x = (rect.left + rect.right) / 2;
    m_cursorInitialPos.y = (rect.bottom + rect.top) / 2;
    SetCursorPos(m_cursorInitialPos.x, m_cursorInitialPos.y);

	return InputManager::Init();
}


void InputManagerPC::Shutdown()
{
	RemoveInputDevice(&m_mouse);
	RemoveInputDevice(&m_keyboard);
	InputManager::Shutdown();
}


void InputManagerPC::Update()
{
    if (!m_focus)
        return;

    POINT cursorPos;
    GetCursorPos(&cursorPos);

    int cx = cursorPos.x - m_cursorInitialPos.x;
    int cy = cursorPos.y - m_cursorInitialPos.y;

    if (m_mouse.IsCursorLocked() != m_cursorLocked)
    {
        m_cursorLocked = m_mouse.IsCursorLocked();
        ShowCursor(m_cursorLocked ? FALSE : TRUE);
    }

    if (m_cursorLocked)
    {
        if (cx != 0 || cy != 0)
            SetCursorPos(m_cursorInitialPos.x, m_cursorInitialPos.y);

        auto pos = m_mouse.GetCursorPos();
        if (pos.x != cx || pos.y != cy)
            m_mouse.OnMouseMove(cx, cy);
    }
    else
    {
        m_mouse.OnMouseMove(cursorPos.x, cursorPos.y);
    }

	InputManager::Update();
}


void InputManagerPC::OnKeyDown(int code)
{
	m_keyboard.OnKeyDown(code);
}


void InputManagerPC::OnKeyUp(int code)
{
	m_keyboard.OnKeyUp(code);
}


void InputManagerPC::OnMouseDown(int button)
{
	m_mouse.OnMouseDown(button);
}


void InputManagerPC::OnMouseUp(int button)
{
	m_mouse.OnMouseUp(button);
}


void InputManagerPC::OnMouseMove(int x, int y)
{
	//LogPrintf("onMouseMove %d, %d", x, y);
    //x = x - m_cursorInitialPos.x;
    //y = y - m_cursorInitialPos.y;
    //if (m_mouse.IsCursorLocked() != m_cursorLocked)
   // {
    //    m_cursorLocked = m_mouse.IsCursorLocked();
        //ShowCursor(m_cursorLocked ? FALSE : TRUE);
    //}

	//m_mouse.OnMouseMove(x, y);
}


void InputManagerPC::OnMouseWheel(int z)
{
	m_mouse.OnMouseWheel(z);
}


void InputManagerPC::OnFocusChanged(bool focus)
{
    if (m_focus == focus)
        return;

    m_focus = focus;
    if (m_focus)
    {
        SetCursorPos(m_cursorInitialPos.x, m_cursorInitialPos.y);
        ShowCursor(m_cursorLocked ? FALSE : TRUE);
    }
    else
    {
        ShowCursor(TRUE);
    }
}