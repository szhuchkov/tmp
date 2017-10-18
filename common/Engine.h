#pragma once


class Engine
{
public:
	inline static Engine* GetInstance()
	{
		static Engine instance;
		return &instance;
	}

	bool Init(void* window, int width, int height, bool fullscreen);
	void Shutdown();
	bool Resize(int width, int height);
	bool Update();
	void Render();

private:
	Engine();
	~Engine();
};;
