#pragma once


#include "Render/OGLDriver.h"


class OGLDriverPC : public OGLDriver
{
public:
	inline static OGLDriverPC* GetInstance()
	{
		static OGLDriverPC instance;
		return &instance;
	}

	bool Init(void* window, int width, int height, bool fullscreen) override;
	void Shutdown() override;
	bool Update() override;
	void SwapBuffers() override;

private:
	OGLDriverPC();
	~OGLDriverPC();

private:
	HDC		m_hdc = NULL;
	HGLRC	m_hrc = NULL;
};
