#pragma once


#if PLATFORM_PC
#include <gl\glew.h>
#endif


#if PLATFORM_ANDROID
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#define glClearDepth	glClearDepthf
#endif


class OGLDriver
{
public:
	static OGLDriver* GetInstance();

	virtual bool Init(void* window, int width, int height, bool fullscreen) = 0;
	virtual void Shutdown() = 0;
	virtual bool Update() = 0;
	virtual void SwapBuffers() = 0;

protected:
	OGLDriver() {}
	~OGLDriver() {}
};
