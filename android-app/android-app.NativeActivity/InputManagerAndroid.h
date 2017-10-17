#pragma once


#include "InputManager.h"


class InputManagerAndroid : public InputManager
{
public:
	inline static InputManagerAndroid* GetInstance()
	{
		static InputManagerAndroid instance;
		return &instance;
	}

private:
	InputManagerAndroid()
	{
	}

	~InputManagerAndroid()
	{
	}
};
