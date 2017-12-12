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

    void* GetWindow();

    size_t GetTime();

private:
	Engine();
	~Engine();

    void* m_window = nullptr;
    size_t m_startTime = 0;
    size_t m_currTime = 0;
};;
