#pragma once


class RenderDevice
{
public:
	inline static RenderDevice* GetInstance()
	{
		static RenderDevice instance;
		return &instance;
	}

	enum ClearFlags
	{
		CLEAR_COLOR = 1,
		CLEAR_DEPTH = 2,
		CLEAR_STENCIL = 4,
	};

	bool Init(void* window, int width, int height, bool fullscreen);
	void Shutdown();
	bool Update();

	bool BeginFrame();
	void EndFrame();

	void Clear(unsigned int mask, const float* color, float depth, int stencil);

private:
	RenderDevice();
	~RenderDevice();
};
