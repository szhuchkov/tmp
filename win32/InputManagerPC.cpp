#include "pch.h"
#include "Engine.h"
#include "InputManagerPC.h"


#pragma comment(lib, "dinput8")
#pragma comment(lib, "dxguid")


#define SAFE_RELEASE(p)     { if (p) { p->Release(); p = nullptr; }}


InputManager* InputManager::GetInstance()
{
	return InputManagerPC::GetInstance();
}


InputManagerPC::InputManagerPC() :
    m_keyState(256),
    m_mouse(new InputDeviceMouse(-1)),
    m_keyboard(new InputDeviceKeyboard(-1))
{
}


InputManagerPC::~InputManagerPC()
{
}


bool InputManagerPC::Init()
{
	AddInputDevice(m_mouse.get());
	AddInputDevice(m_keyboard.get());

    // init cursor
    HWND hwnd = static_cast<HWND>(Engine::GetInstance()->GetWindow());
    RECT rect;
    GetWindowRect(hwnd, &rect);
    m_cursorInitialPos.x = (rect.left + rect.right) / 2;
    m_cursorInitialPos.y = (rect.bottom + rect.top) / 2;
    SetCursorPos(m_cursorInitialPos.x, m_cursorInitialPos.y);

    // init DirectInput
    HRESULT hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL);
    if (FAILED(hr))
    {
        LogPrintf("DirectInput8Create() failed: %u", hr);
        return false;
    }

    // init keyboard device
    hr = m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, NULL);
    if (FAILED(hr))
    {
        LogPrintf("Unable to create GUID_SysKeyboard, hr = %u", hr);
        return false;
    }

    // set data format for the keyb
    hr = m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(hr))
    {
        LogPrintf("SetDataFormat() failed for keyb, hr = %u", hr);
        return false;
    }

    // set coop level
    hr = m_pKeyboardDevice->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr))
    {
        LogPrintf("SetCooperativeLevel() failed for keyboard, hr = %u", hr);
        return false;
    }

    // acquire the keyb
    hr = m_pKeyboardDevice->Acquire();
    if (FAILED(hr))
    {
        LogPrintf("Acquire() failed for keyboard, hr = %u", hr);
        return false;
    }

    // get keyb initial state
    m_pKeyboardDevice->GetDeviceState(256, &m_keyState[0]);

    // create mouse device
    hr = m_pDI->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL);
    if (FAILED(hr))
    {
        LogPrintf("Unable to create GUID_SysMouse device, hr = %u", hr);
        return false;
    }

    // set data format for mouse
    hr = m_pMouseDevice->SetDataFormat(&c_dfDIMouse);
    if (FAILED(hr))
    {
        LogPrintf("SetDataFormat() failed for mouse, hr = %u", hr);
        return false;
    }

    // set coop level for mouse
    hr = m_pMouseDevice->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr))
    {
        LogPrintf("SetCooperativeLevel() failed for mouse, hr = %u", hr);
        return false;
    }

    // acquire mouse
    hr = m_pMouseDevice->Acquire();
    if (FAILED(hr))
    {
        LogPrintf("Acquire() failed for mouse, hr = %u", hr);
        return false;
    }

    // get initial mouse state
    m_pMouseDevice->GetDeviceState(sizeof(m_mouseState), &m_mouseState);

	return InputManager::Init();
}


void InputManagerPC::Shutdown()
{
    // remove virual devices
	RemoveInputDevice(m_mouse.get());
	RemoveInputDevice(m_keyboard.get());

    // unacquire and release mouse device
    if (m_pMouseDevice)
    {
        m_pMouseDevice->Unacquire();
        SAFE_RELEASE(m_pMouseDevice);
    }

    // unacquire and release keyb device
    if (m_pKeyboardDevice)
    {
        m_pKeyboardDevice->Unacquire();
        SAFE_RELEASE(m_pKeyboardDevice);
    }

    // release DirectInput
    SAFE_RELEASE(m_pDI);

	InputManager::Shutdown();
}


void InputManagerPC::Update()
{
    InputManager::Update();

    if (!m_focus)
        return;

    HWND hwnd = reinterpret_cast<HWND>(Engine::GetInstance()->GetWindow());
    POINT cursorPos;
    GetCursorPos(&cursorPos);

    if (m_cursorLocked)
    {
        SetCursorPos(m_cursorInitialPos.x, m_cursorInitialPos.y);
    }
    else
    {
        ScreenToClient(hwnd, &cursorPos);
        m_mouse->OnCursorPositionChanged(cursorPos.x, cursorPos.y);
    }

    if (m_mouse->IsCursorLocked() != m_cursorLocked)
    {
        m_cursorLocked = m_mouse->IsCursorLocked();
        if (m_cursorLocked)
        {
            m_cursorInitialPos = cursorPos;
        }
        else
        {
            ClientToScreen(hwnd, &m_cursorInitialPos);
            SetCursorPos(m_cursorInitialPos.x, m_cursorInitialPos.y);
        }
        ShowCursor(m_cursorLocked ? FALSE : TRUE);
    }

    UpdateMouse();
    UpdateKeyboard();
}


void InputManagerPC::UpdateMouse()
{
    DIMOUSESTATE state;
    HRESULT hr = m_pMouseDevice->GetDeviceState(sizeof(state), &state);
    if (hr == DIERR_NOTACQUIRED || hr == DIERR_INPUTLOST)
    {
        m_pMouseDevice->Acquire();
    }
    else
    {
        // x and y
        if (state.lX || state.lY)
        {
            LOG_INPUT_EVENT("MouseMove -> %d, %d", state.lX, state.lY);
            m_mouse->OnMouseMove(state.lX, state.lY);
        }

        // scroll
        if (state.lZ)
        {
            LOG_INPUT_EVENT("MouseScroll -> %d", state.lZ);
            m_mouse->OnMouseWheel(state.lZ > 0 ? 1 : -1);
        }

        // buttons
        for (int i = 0; i < ARRAY_SIZE(state.rgbButtons); i++)
        {
            auto oldState = m_mouseState.rgbButtons[i];
            auto newState = state.rgbButtons[i];
            if (newState != oldState)
            {
                LOG_INPUT_EVENT("MouseButton -> %d, %d", i, newState);
                if (newState & 0x80)
                    m_mouse->OnMouseDown(i);
                else
                    m_mouse->OnMouseUp(i);
            }
        }
        m_mouseState = state;
    }
}


void InputManagerPC::UpdateKeyboard()
{
    std::vector<BYTE> state(256);
    HRESULT hr = m_pKeyboardDevice->GetDeviceState(256, &state[0]);
    if (hr == DIERR_NOTACQUIRED || hr == DIERR_INPUTLOST)
    {
        m_pKeyboardDevice->Acquire();
    }
    else
    {
        for (size_t i = 0; i < 256; i++)
        {
            auto oldState = m_keyState[i];
            auto newState = state[i];
            if (oldState != newState)
            {
                if (newState & 0x80)
                    m_keyboard->OnKeyDown(i);
                else
                    m_keyboard->OnKeyUp(i);
                m_keyState[i] = newState;
            }
        }
    }
}


void InputManagerPC::OnKeyDown(int code)
{
	//m_keyboard->OnKeyDown(code);
}


void InputManagerPC::OnKeyUp(int code)
{
	m_keyboard->OnKeyUp(code);
}


void InputManagerPC::OnMouseDown(int button)
{
	m_mouse->OnMouseDown(button);
}


void InputManagerPC::OnMouseUp(int button)
{
	m_mouse->OnMouseUp(button);
}


void InputManagerPC::OnMouseMove(int x, int y)
{
	//LogPrintf("onMouseMove %d, %d", x, y);
    //x = x - m_cursorInitialPos.x;
    //y = y - m_cursorInitialPos.y;
    //if (m_mouse->IsCursorLocked() != m_cursorLocked)
   // {
    //    m_cursorLocked = m_mouse->IsCursorLocked();
        //ShowCursor(m_cursorLocked ? FALSE : TRUE);
    //}

	//m_mouse->OnMouseMove(x, y);
}


void InputManagerPC::OnMouseWheel(int z)
{
	m_mouse->OnMouseWheel(z);
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