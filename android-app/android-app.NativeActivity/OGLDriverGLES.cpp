#include "pch.h"
#include "OGLDriverGLES.h"


OGLDriver* OGLDriver::GetInstance()
{
	return OGLDriverGLES::GetInstance();
}


OGLDriverGLES::OGLDriverGLES()
{
}


OGLDriverGLES::~OGLDriverGLES()
{
}


bool OGLDriverGLES::Init(void* pwnd, int width, int height, bool fullscreen)
{
	ANativeWindow* window = (ANativeWindow*)pwnd;

	m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (m_display == EGL_NO_DISPLAY)
	{
		LogPrintf("eglGetDisplay() failed");
		return false;
	}

	if (!eglInitialize(m_display, 0, 0))
	{
		LogPrintf("eglInitialize() failed");
		return false;
	}

	const EGLint configAttribs[] = {
		EGL_LEVEL, 0,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE,
	};

	EGLint numConfigs;
	eglChooseConfig(m_display, configAttribs, &m_config, 1, &numConfigs);

	LogPrintf("Num EGL configs: %d", numConfigs);

	EGLint format;
	eglGetConfigAttrib(m_display, m_config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(window, 0, 0, format);

	m_surface = eglCreateWindowSurface(m_display, m_config, window, NULL);
	if (m_surface == EGL_NO_SURFACE)
	{
		LogPrintf("eglCreateWindowSurface() failed");
		return false;
	}

	GLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE,
	};

	m_context = eglCreateContext(m_display, m_config, NULL, contextAttribs);
	if (m_context == EGL_NO_CONTEXT)
	{
		LogPrintf("eglCreateContext() failed");
		return false;
	}

	if (!eglMakeCurrent(m_display, m_surface, m_surface, m_context))
	{
		LogPrintf("eglMakeCurrent() failed");
		return false;
	}

	eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_width);
	eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_height);

	LogPrintf("GL_VENDOR = %s", glGetString(GL_VENDOR));
	LogPrintf("GL_RENDERER = %s", glGetString(GL_RENDERER));
	LogPrintf("GL_VERSION = %s", glGetString(GL_VERSION));
	LogPrintf("GL_SHADING_LANGUAGE_VERSION = %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	return true;
}


void OGLDriverGLES::Shutdown()
{
	if (m_display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

		if (m_context != EGL_NO_CONTEXT)
			eglDestroyContext(m_display, m_context);

		if (m_surface != EGL_NO_SURFACE)
			eglDestroySurface(m_display, m_surface);

		eglTerminate(m_display);

		m_display = EGL_NO_DISPLAY;
		m_surface = EGL_NO_SURFACE;
		m_context = EGL_NO_CONTEXT;
	}
}


bool OGLDriverGLES::Update()
{
	return true;
}


bool OGLDriverGLES::OnWindowChanged(ANativeWindow* window)
{
	if (m_surface)
	{
		eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroySurface(m_display, m_surface);

		EGLint format;
		eglGetConfigAttrib(m_display, m_config, EGL_NATIVE_VISUAL_ID, &format);

		ANativeWindow_setBuffersGeometry(window, 0, 0, format);

		m_surface = eglCreateWindowSurface(m_display, m_config, window, NULL);
		if (m_surface == EGL_NO_SURFACE)
		{
			LogPrintf("eglCreateWindowSurface() failed");
			return false;
		}

		if (!eglMakeCurrent(m_display, m_surface, m_surface, m_context))
		{
			LogPrintf("eglMakeCurrent() failed");
			return false;
		}

		eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_width);
		eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_height);
	}
	return true;
}


void OGLDriverGLES::SwapBuffers()
{
	eglSwapBuffers(m_display, m_surface);
}