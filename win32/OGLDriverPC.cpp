#include "pch.h"
#include "OGLDriverPC.h"


OGLDriver* OGLDriver::GetInstance()
{
	return OGLDriverPC::GetInstance();
}


OGLDriverPC::OGLDriverPC()
{
}


OGLDriverPC::~OGLDriverPC()
{
}


bool OGLDriverPC::Init(void* window, int width, int height, bool fullscreen)
{
	HWND hwnd = (HWND)window;

	m_hdc = GetDC(hwnd);
	if (!m_hdc)
	{
		LogPrintf("GetDC() failed");
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;

	int format = ChoosePixelFormat(m_hdc, &pfd);
	if (!format)
	{
		LogPrintf("ChoosePixelFormat() failed");
		return false;
	}

	if (!SetPixelFormat(m_hdc, format, &pfd))
	{
		LogPrintf("SetPixelFormat() failed");
		return false;
	}

	m_hrc = wglCreateContext(m_hdc);
	if (!m_hrc)
	{
		LogPrintf("wglCreateContext() failed");
		return false;
	}

	if (!wglMakeCurrent(m_hdc, m_hrc))
	{
		LogPrintf("wglMakeCurrent() failed");
		return false;
	}

	GLenum err = glewInit();
	if (err != GLEW_NO_ERROR)
	{
		LogPrintf("glewInit() failed: %u", err);
		return false;
	}

	return true;
}


void OGLDriverPC::Shutdown()
{
	if (m_hrc)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}

	if (m_hdc)
	{
		DeleteDC(m_hdc);
		m_hdc = NULL;
	}
}


bool OGLDriverPC::Update()
{
	return true;
}


void OGLDriverPC::SwapBuffers()
{
	::SwapBuffers(m_hdc);
}