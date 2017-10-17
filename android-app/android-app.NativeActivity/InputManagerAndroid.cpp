#include "pch.h"
#include "InputManagerAndroid.h"


InputManager* InputManager::GetInstance()
{
	return InputManagerAndroid::GetInstance();
}