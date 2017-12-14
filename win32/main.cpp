#include "pch.h"
#include <Engine.h>
#include <Render/RenderDevice.h>
#include <Scene/Scene.h>
#include "InputManagerPC.h"


#define WND_NAME		"AGE RenderWindow"
#define WND_CLASS_NAME	WND_NAME


static HINSTANCE	cl_hInstance = NULL;
static HWND			cl_hWnd = NULL;
static bool			cl_quit = false;
static uint64_t     sys_timerBase = 0;
static uint64_t     sys_timerFreq = 1;


static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
    case WM_ACTIVATE:
    {
        switch (wParam)
        {
        case WA_INACTIVE:
            InputManagerPC::GetInstance()->OnFocusChanged(false);
            break;
        default:
            InputManagerPC::GetInstance()->OnFocusChanged(true);
            break;
        }
    }
    break;
	case WM_CLOSE:
		cl_quit = true;
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int showCmd)
{
    HWND hbw = FindWindow("StartMenuSizingFrame", NULL);
    if (hbw)
    {
        if (!CloseWindow(hbw))
            LogPrintf("CloseWindow()");
        if (!DestroyWindow(hbw))
            LogPrintf("DestroyWindow()");
        if (!MoveWindow(hbw, -1, -1, 0, 0, FALSE))
            LogPrintf("MoveWindow()");
    }
	cl_hInstance = hInstance;

    // init system timer
    QueryPerformanceCounter((LARGE_INTEGER*)&sys_timerBase);
    QueryPerformanceFrequency((LARGE_INTEGER*)&sys_timerFreq);

	// init some vars
	CVar_Set("ScreenWidth", 1280);
	CVar_Set("ScreenHeight", 720);
	CVar_Set("Fullscreen", 0);

	// get screen size from the config
	int width = CVar_Get<int>("ScreenWidth");
	int height = CVar_Get<int>("ScreenHeight");

	// setup window class
	WNDCLASS wndClass;
	memset(&wndClass, 0, sizeof(wndClass));
	wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = WND_CLASS_NAME;
	RegisterClass(&wndClass);

	// window style
	DWORD wndStyle = WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	// adjust window size
	RECT wndRect;
	SetRect(&wndRect, 0, 0, width, height);
	AdjustWindowRect(&wndRect, wndStyle, FALSE);
	int windowWidth = wndRect.right - wndRect.left;
	int windowHeight = wndRect.bottom - wndRect.top;
	int x = (GetSystemMetrics(SM_CXFULLSCREEN) - windowWidth) / 2;
	int y = (GetSystemMetrics(SM_CYFULLSCREEN) - windowHeight) / 2;

	// create window
	cl_hWnd = CreateWindow(WND_CLASS_NAME, WND_NAME, wndStyle, x, y, windowWidth, windowHeight, GetDesktopWindow(), NULL, hInstance, NULL);
	if (!cl_hWnd)
	{
		LogPrintf("CreateWindow() failed");
		return -1;
	}

	ShowWindow(cl_hWnd, SW_NORMAL);
	UpdateWindow(cl_hWnd);

	// initialize engine
	if (!Engine::GetInstance()->Init(cl_hWnd, width, height, false))
	{
		LogPrintf("Engine init failed");
		Engine::GetInstance()->Shutdown();
		return -1;
	}

    if (!Scene::GetInstance()->Load("test.scene"))
    {
        LogPrintf("Unable to load test scene");
        Engine::GetInstance()->Shutdown();
        return -1;
    }

	// run main loop
	while (!cl_quit)
	{
		// pump window messages
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (!Engine::GetInstance()->Update())
			break;

		auto mouseDeviceID = InputManager::GetInstance()->FindDevicesByClass(INPUT_DEVICE_CLASS_MOUSE).front()->GetDeviceID();

		Engine::GetInstance()->Render();
	}

	// shutdown
    Scene::GetInstance()->Clear();
	Engine::GetInstance()->Shutdown();

	// cleanup vars
	CVar_Clear();

	return 0;
}


size_t Sys_Milliseconds()
{
    uint64_t curr;
    QueryPerformanceCounter((LARGE_INTEGER*)&curr);
    return (size_t)((curr - sys_timerBase) * 1000 / sys_timerFreq);
}


void _LogRaw(const char* format, ...)
{
    char buffer[10000] = { 0 };
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    strcat(buffer, "\n");
    OutputDebugString(buffer);
}


void _LogPrintf(const char* file, int line, const char* format, ...)
{
	va_list args;
	va_start(args, format);

	char buffer[10000] = { 0 };
	int offset = 0;

	// VS double click navigation support
	offset += sprintf(&buffer[offset], "%s(%d): ", file, line);
	offset += vsprintf(&buffer[offset], format, args);
	buffer[offset++] = '\n';
	buffer[offset++] = 0;

	va_end(args);

	// dump to the console and debug output
	printf("%s", buffer);
	OutputDebugString(buffer);
}