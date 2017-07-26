#include "pch.h"
#include "Engine.h"


#define WND_NAME		"AGE RenderWindow"
#define WND_CLASS_NAME	WND_NAME


static HINSTANCE	cl_hInstance = NULL;
static HWND			cl_hWnd = NULL;
static bool			cl_quit = false;


static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		cl_quit = true;
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int showCmd)
{
	cl_hInstance = hInstance;

	WNDCLASS wndClass;
	memset(&wndClass, 0, sizeof(wndClass));
	wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = WND_CLASS_NAME;
	RegisterClass(&wndClass);

	DWORD wndStyle = WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	int width = 1024;
	int height = 768;

	RECT wndRect;
	SetRect(&wndRect, 0, 0, width, height);
	AdjustWindowRect(&wndRect, wndStyle, FALSE);

	width = wndRect.right - wndRect.left;
	height = wndRect.bottom - wndRect.top;

	int x = (GetSystemMetrics(SM_CXFULLSCREEN) - width) / 2;
	int y = (GetSystemMetrics(SM_CYFULLSCREEN) - height) / 2;

	cl_hWnd = CreateWindow(WND_CLASS_NAME, WND_NAME, wndStyle, x, y, width, height, GetDesktopWindow(), NULL, hInstance, NULL);
	if (!cl_hWnd)
	{
		LogPrintf("CreateWindow() failed");
		return -1;
	}

	ShowWindow(cl_hWnd, SW_NORMAL);
	UpdateWindow(cl_hWnd);

	if (!Engine::GetInstance()->Init(cl_hWnd, 1024, 768, false))
	{
		LogPrintf("Engine init failed");
		Engine::GetInstance()->Shutdown();
		return -1;
	}

	while (!cl_quit)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (!Engine::GetInstance()->Update())
			break;

		Engine::GetInstance()->Render();
	}

	Engine::GetInstance()->Shutdown();

	return 0;
}


void _LogPrintf(const char* file, int line, const char* format, ...)
{
	va_list args;
	va_start(args, format);

	char buffer[10000] = { 0 };
	int offset = 0;

	offset += sprintf(&buffer[offset], "%s(%d): ", file, line);
	offset += vsprintf(&buffer[offset], format, args);
	buffer[offset++] = '\n';
	buffer[offset++] = 0;

	va_end(args);

	printf(buffer);
	OutputDebugString(buffer);
}