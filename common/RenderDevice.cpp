#include "pch.h"
#include "RenderDevice.h"
#include "OGLDriver.h"


RenderDevice::RenderDevice()
{
}


RenderDevice::~RenderDevice()
{
}


bool RenderDevice::Init(void* window, int width, int height, bool fullscreen)
{
	if (!OGLDriver::GetInstance()->Init(window, width, height, fullscreen))
		return false;

	return true;
}


void RenderDevice::Shutdown()
{
	OGLDriver::GetInstance()->Shutdown();
}


bool RenderDevice::Update()
{
	if (!OGLDriver::GetInstance()->Update())
		return false;

	return true;
}


bool RenderDevice::BeginFrame()
{
	return true;
}


void RenderDevice::EndFrame()
{
	OGLDriver::GetInstance()->SwapBuffers();
}


void RenderDevice::Clear(unsigned int flags, const float* color, float depth, int stencil)
{
	GLbitfield mask = 0;

	if (flags & CLEAR_COLOR)
	{
		glClearColor(color[0], color[1], color[2], color[3]);
		mask |= GL_COLOR_BUFFER_BIT;
	}

	if (flags & CLEAR_DEPTH)
	{
		glClearDepth(depth);
		mask |= GL_DEPTH_BUFFER_BIT;
	}

	if (flags & CLEAR_STENCIL)
	{
		glClearStencil(stencil);
		mask |= GL_STENCIL_BUFFER_BIT;
	}

	glClear(mask);
}
