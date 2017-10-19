#pragma once


#include "OGLDriver.h"


class OGLDriverGLES : public OGLDriver
{
public:
	inline static OGLDriverGLES* GetInstance()
	{
		static OGLDriverGLES instance;
		return &instance;
	}

	bool Init(void* window, int width, int height, bool fullscreen) override;
	void Shutdown() override;
	bool Update() override;
	void SwapBuffers() override;

	bool OnWindowChanged(ANativeWindow* window);

private:
	OGLDriverGLES();
	~OGLDriverGLES();

private:
	EGLDisplay	m_display = EGL_NO_DISPLAY;
	EGLSurface	m_surface = EGL_NO_SURFACE;
	EGLContext	m_context = EGL_NO_CONTEXT;
	EGLConfig	m_config;
	int			m_width;
	int			m_height;
};
