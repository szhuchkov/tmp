/*
* Copyright (C) 2010 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/


#include "Engine.h"
#include "InputManagerAndroid.h"
#include "RenderDevice.h"
#include "OGLDriverGLES.h"
#include "..\..\tests\TestScene.h"


#define LOG_INPUT_EVENTS	1
#define LOG_APP_EVENTS		1


#if LOG_INPUT_EVENTS
#define LogInput LogPrintf
#else
#define LogInput	
#endif


#if LOG_APP_EVENTS
#define LogAppEvent	LogPrintf
#else
#define LogAppEvent
#endif


struct AndroidAppContext
{
	struct android_app* app;

	int initialized;
	int running;
	int quit;
};


void _LogPrintf(const char* file, int line, const char* format, ...)
{
	va_list args;
	va_start(args, format);

	char buffer[10000] = { 0 };
	int offset = 0;

	offset += sprintf(&buffer[offset], "%s(%d): ", file, line);
	offset += vsprintf(&buffer[offset], format, args);
	buffer[offset++] = '\n';
	buffer[offset++] = 0;

	va_end(args);

	printf("%s", buffer);
	__android_log_print(ANDROID_LOG_INFO, "AGE", "%s", buffer);
}


static bool AppInit(AndroidAppContext* context)
{
	LogPrintf(">>> Init...");

	int width = ANativeWindow_getWidth(context->app->window);
	int height = ANativeWindow_getHeight(context->app->window);

	if (!Engine::GetInstance()->Init(context->app->window, width, height, false))
		return false;

	if (!TestScene::GetInstance()->Init())
		return false;

	context->initialized = 1;

	return true;
}


static void AppShutdown(AndroidAppContext* context)
{
	LogPrintf(">>> Shutdown...");
	context->initialized = 0;

	TestScene::GetInstance()->Shutdown();
	Engine::GetInstance()->Shutdown();
}


static bool AppUpdate(AndroidAppContext* context)
{
	if (!Engine::GetInstance()->Update())
		return false;

	size_t numEvents = InputManager::GetInstance()->GetNumEvents();
	for(size_t i = 0; i < numEvents; i++)
	{
		const auto& event = InputManager::GetInstance()->GetEvent(i);
		if (event.type == INPUT_EVENT_TYPE_BUTTON && event.index == AKEYCODE_BACK && !event.data.button.state)
		{
			LogPrintf(">>> EXIT!!!");
			context->quit = 1;
		}
		else
		{
			LogPrintf(">>> ev: device = %d, type = %d, index = %d, %d", event.device, event.type, event.index, event.data.button.state);
		}
	}

	return true;
}


static void AppRender(AndroidAppContext* context)
{
	if (RenderDevice::GetInstance()->BeginFrame())
	{
		constexpr float clearColor[] = { 0.4f, 0.6f, 0.8f, 1.0f };
		RenderDevice::GetInstance()->Clear(RenderDevice::CLEAR_COLOR | RenderDevice::CLEAR_DEPTH, clearColor, 1.0f, 0);
		TestScene::GetInstance()->Render();
		RenderDevice::GetInstance()->EndFrame();
	}
	//Engine::GetInstance()->Render();
}


#if LOG_INPUT_EVENTS
static void LogInputEvent(AInputEvent* event)
{
	// get common info
	int type = AInputEvent_getType(event);
	int deviceId = AInputEvent_getDeviceId(event);
	int source = AInputEvent_getSource(event);
	int sourceClass = source & AINPUT_SOURCE_CLASS_MASK;

	if (type == AINPUT_EVENT_TYPE_MOTION)
	{
		int action = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
		int pointer = (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
		int numPointers = AMotionEvent_getPointerCount(event);

		// build pointer list string
		std::string pointerIdString = "";
		for(int pointerIndex = 0; pointerIndex < numPointers; pointerIndex++)
		{
			char buf[512] = { 0 };
			sprintf(buf, "%d", AMotionEvent_getPointerId(event, pointerIndex));
			pointerIdString += buf;
			if (pointerIndex != numPointers - 1)
				pointerIdString += ',';
		}

		// convert action to text
		const char* actionText = "UNKNOWN";
		switch(action)
		{
		case AMOTION_EVENT_ACTION_DOWN:			actionText = "AMOTION_EVENT_ACTION_DOWN";			break;
		case AMOTION_EVENT_ACTION_UP:			actionText = "AMOTION_EVENT_ACTION_UP";				break;
		case AMOTION_EVENT_ACTION_MOVE:			actionText = "AMOTION_EVENT_ACTION_MOVE";			break;
		case AMOTION_EVENT_ACTION_CANCEL:		actionText = "AMOTION_EVENT_ACTION_CANCEL";			break;
		case AMOTION_EVENT_ACTION_OUTSIDE:		actionText = "AMOTION_EVENT_ACTION_OUTSIDE";		break;
		case AMOTION_EVENT_ACTION_POINTER_DOWN:	actionText = "AMOTION_EVENT_ACTION_POINTER_DOWN";	break;
		case AMOTION_EVENT_ACTION_POINTER_UP:	actionText = "AMOTION_EVENT_ACTION_POINTER_UP";		break;
		case AMOTION_EVENT_ACTION_HOVER_MOVE:	actionText = "AMOTION_EVENT_ACTION_HOVER_MOVE";		break;
		case AMOTION_EVENT_ACTION_SCROLL:		actionText = "AMOTION_EVENT_ACTION_SCROLL";			break;
		case AMOTION_EVENT_ACTION_HOVER_ENTER:	actionText = "AMOTION_EVENT_ACTION_HOVER_ENTER";	break;
		case AMOTION_EVENT_ACTION_HOVER_EXIT:	actionText = "AMOTION_EVENT_ACTION_HOVER_EXIT";		break;
		}

		// convert source to text
		const char* sourceText = "UNKNOWN";
		switch (source)
		{
		case AINPUT_SOURCE_KEYBOARD:			sourceText = "AINPUT_SOURCE_KEYBOARD";			break;
		case AINPUT_SOURCE_DPAD:				sourceText = "AINPUT_SOURCE_DPAD";				break;
		case AINPUT_SOURCE_GAMEPAD:				sourceText = "AINPUT_SOURCE_GAMEPAD";			break;
		case AINPUT_SOURCE_TOUCHSCREEN:			sourceText = "AINPUT_SOURCE_TOUCHSCREEN";		break;
		case AINPUT_SOURCE_MOUSE:				sourceText = "AINPUT_SOURCE_MOUSE";				break;
		case AINPUT_SOURCE_STYLUS:				sourceText = "AINPUT_SOURCE_STYLUS";			break;
		case AINPUT_SOURCE_TRACKBALL:			sourceText = "AINPUT_SOURCE_TRACKBALL";			break;
		case AINPUT_SOURCE_TOUCHPAD:			sourceText = "AINPUT_SOURCE_TOUCHPAD";			break;
		case AINPUT_SOURCE_TOUCH_NAVIGATION:	sourceText = "AINPUT_SOURCE_TOUCH_NAVIGATION";	break;
		case AINPUT_SOURCE_JOYSTICK:			sourceText = "AINPUT_SOURCE_JOYSTICK";			break;
		}

		// convert source class to text
		const char* sourceClassText = "UNKNOWN";
		switch(sourceClass)
		{
		case AINPUT_SOURCE_CLASS_NONE:			sourceClassText = "AINPUT_SOURCE_CLASS_NONE";		break;
		case AINPUT_SOURCE_CLASS_BUTTON:		sourceClassText = "AINPUT_SOURCE_CLASS_BUTTON";		break;
		case AINPUT_SOURCE_CLASS_POINTER:		sourceClassText = "AINPUT_SOURCE_CLASS_POINTER";	break;
		case AINPUT_SOURCE_CLASS_NAVIGATION:	sourceClassText = "AINPUT_SOURCE_CLASS_NAVIGATION";	break;
		case AINPUT_SOURCE_CLASS_POSITION:		sourceClassText = "AINPUT_SOURCE_CLASS_POSITION";	break;
		case AINPUT_SOURCE_CLASS_JOYSTICK:		sourceClassText = "AINPUT_SOURCE_CLASS_JOYSTICK";	break;
		};

		// useless in most cases
		float x = AMotionEvent_getX(event, 0);
		float y = AMotionEvent_getY(event, 0);

		LogInput("AINPUT_EVENT_TYPE_MOTION: (deviceId = %d, source = %s, sourceClass = %s, action = %s, pointer = %d, numPointers = %d (%s)) -> x = %f, y = %f",
			deviceId, sourceText, sourceClassText, actionText, pointer, numPointers, pointerIdString.c_str(), x, y);
	}
	else if (type == AINPUT_EVENT_TYPE_KEY)
	{
		auto action = AKeyEvent_getAction(event);
		auto code = AKeyEvent_getKeyCode(event);
		auto scanCode = AKeyEvent_getScanCode(event);
		if (action == AKEY_EVENT_ACTION_DOWN)
			LogInput("AKEY_EVENT_ACTION_DOWN: %d (scanCode = %d)", code, scanCode);
		else if (action == AKEY_EVENT_ACTION_UP)
			LogInput("AKEY_EVENT_ACTION_UP: %d (scanCode = %d)", code, scanCode);
	}
}
#endif


static int32_t AppHandleInput(android_app* app, AInputEvent* event)
{
	int32_t res = 0;

	// get common info
	int type = AInputEvent_getType(event);
	int deviceId = AInputEvent_getDeviceId(event);
	int source = AInputEvent_getSource(event);
	int sourceClass = source & AINPUT_SOURCE_CLASS_MASK;

	// process event
	if (type == AINPUT_EVENT_TYPE_KEY)
	{
		int action = AKeyEvent_getAction(event);
		switch(source)
		{
		case AINPUT_SOURCE_KEYBOARD:
			{
				// keyboard events - as keys
				int code = AKeyEvent_getKeyCode(event);
				int scanCode = AKeyEvent_getScanCode(event);
				if (action == AKEY_EVENT_ACTION_DOWN)
					InputManagerAndroid::GetInstance()->OnKeyDown(code, scanCode);
				else if (action == AKEY_EVENT_ACTION_UP)
					InputManagerAndroid::GetInstance()->OnKeyUp(code, scanCode);
				res = 1;
			}
			break;
		case AINPUT_SOURCE_DPAD:
		case AINPUT_SOURCE_GAMEPAD:
		case AINPUT_SOURCE_JOYSTICK:
		case AINPUT_SOURCE_MOUSE:
			{
				// gamepad, joystick, etc events as buttons
				int code = AKeyEvent_getKeyCode(event);
				int scanCode = AKeyEvent_getScanCode(event);
				if (action == AKEY_EVENT_ACTION_DOWN)
					InputManagerAndroid::GetInstance()->OnButtonDown(deviceId, code, scanCode);
				else if (action == AKEY_EVENT_ACTION_UP)
					InputManagerAndroid::GetInstance()->OnButtonUp(deviceId, code, scanCode);
				res = 1;
			}
			break;
		};
	}
	else if (type == AINPUT_EVENT_TYPE_MOTION)
	{
		switch(sourceClass)
		{
		case AINPUT_SOURCE_CLASS_POINTER:
			{
				// pointer events - handle all pointers
				int numPointers = AMotionEvent_getPointerCount(event);
				int actionMasked = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
				int actionIndex = (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
				if (actionMasked == AMOTION_EVENT_ACTION_MOVE)
				{
					for (int pointerIndex = 0; pointerIndex < numPointers; pointerIndex++)
					{
						int pointer = AMotionEvent_getPointerId(event, pointerIndex);
						int x = AMotionEvent_getX(event, pointerIndex);
						int y = AMotionEvent_getY(event, pointerIndex);
						InputManagerAndroid::GetInstance()->OnPointerMove(pointer, x, y);
					}
				}
				else if (actionMasked == AMOTION_EVENT_ACTION_POINTER_DOWN || actionMasked == AMOTION_EVENT_ACTION_DOWN)
				{
					int pointer = AMotionEvent_getPointerId(event, actionIndex);
					int x = AMotionEvent_getX(event, pointer);
					int y = AMotionEvent_getY(event, pointer);
					InputManagerAndroid::GetInstance()->OnPointerDown(pointer, x, y);
				}
				else if (actionMasked == AMOTION_EVENT_ACTION_POINTER_UP || actionMasked == AMOTION_EVENT_ACTION_UP)
				{
					int pointer = AMotionEvent_getPointerId(event, actionIndex);
					int x = AMotionEvent_getX(event, pointer);
					int y = AMotionEvent_getY(event, pointer);
					InputManagerAndroid::GetInstance()->OnPointerUp(pointer, x, y);
				}
				res = 1;
			}
			break;
		case AINPUT_SOURCE_CLASS_JOYSTICK:
			{
				//TODO: figure out how not to get every axis every frame
				int actionMasked = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
				int actionIndex = (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
				int pointer = AMotionEvent_getPointerId(event, actionIndex);
				float x = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_X, pointer);
				float y = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_Y, pointer);
				float rx = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_RX, pointer);
				float ry = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_RY, pointer);
				res = 1;
			}
			break;
		}
	}

#if LOG_INPUT_EVENTS
	LogInputEvent(event);
#endif
	return res;
}


static void AppHandleCmd(struct android_app* app, int32_t cmd)
{
	AndroidAppContext* context = static_cast<AndroidAppContext*>(app->userData);
	switch (cmd)
	{
	case APP_CMD_SAVE_STATE:
		LogAppEvent(">>> APP_CMD_SAVE_STATE");
		break;
	case APP_CMD_INIT_WINDOW:
		{
			LogAppEvent(">>> APP_CMD_INIT_WINDOW");

			// The window is being shown, get it ready.
			if (context->app->window != NULL)
			{
				// preserve GLES context if already initialized
				if (context->initialized)
				{
					if (!OGLDriverGLES::GetInstance()->OnWindowChanged(context->app->window))
					{
						LogPrintf("OnWindowChanged() failed");
						context->quit = 1;
					}
				}
				else
				{
					// full initialization
					if (!AppInit(context))
					{
						context->quit = 1;
					}
					else
					{
						AppRender(context);
					}
				}
			}
		}
		break;
	case APP_CMD_TERM_WINDOW:
		LogAppEvent(">>> APP_CMD_TERM_WINDOW");
		break;
	case APP_CMD_GAINED_FOCUS:
		{
			LogAppEvent(">>> APP_CMD_GAINED_FOCUS");
			context->running = 1;
		}
		break;
	case APP_CMD_LOST_FOCUS:
		{
			LogAppEvent(">>> APP_CMD_LOST_FOCUS");
			context->running = 0;
			AppRender(context);
		}
		break;
	case APP_CMD_WINDOW_RESIZED:
		{
			LogAppEvent(">>> APP_CMD_WINDOW_RESIZED");
			int width = ANativeWindow_getWidth(context->app->window);
			int height = ANativeWindow_getHeight(context->app->window);
			Engine::GetInstance()->Resize(width, height);
		}
		break;
	case APP_CMD_CONFIG_CHANGED:
		LogAppEvent(">>> APP_CMD_CONFIG_CHANGED");
		break;
	case APP_CMD_START:
		LogAppEvent(">>> APP_CMD_START");
		break;
	case APP_CMD_RESUME:
		LogAppEvent(">>> APP_CMD_RESUME");
		break;
	case APP_CMD_PAUSE:
		LogAppEvent(">>> APP_CMD_PAUSE");
		break;
	case APP_CMD_STOP:
		LogAppEvent(">>> APP_CMD_STOP");
		break;
	case APP_CMD_DESTROY:
		LogAppEvent(">>> APP_CMD_DESTROY");
		AppShutdown(context);
		break;
	case APP_CMD_INPUT_CHANGED:
		LogAppEvent(">>> APP_CMD_INPUT_CHANGED");
		break;
	default:
		LogAppEvent(">>> APP_CMD: %d", cmd);
		break;
	}
}

/**
* This is the main entry point of a native application that is using
* android_native_app_glue.  It runs in its own thread, with its own
* event loop for receiving input events and doing other things.
*/
void android_main(struct android_app* state)
{
	AndroidAppContext context;

	memset(&context, 0, sizeof(context));
	state->userData = &context;
	state->onAppCmd = AppHandleCmd;
	state->onInputEvent = AppHandleInput;
	context.app = state;

	if (state->savedState != NULL)
	{
	}

	context.running = 1;

	// loop waiting for stuff to do.

	LogPrintf(">>> Enter main loop");

	while (!context.quit)
	{
		// Read all pending events.
		int ident;
		int events;
		struct android_poll_source* source;

		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while ((ident = ALooper_pollAll(context.running ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
		{
			// Process this event.
			if (source != NULL)
				source->process(state, source);

			// Check if we are exiting.
			//if (state->destroyRequested != 0 && engine.initialized) {
			//	engine.initialized = 0;
			//	engine_term_display(&engine);
			//	return;
			//}
		}

		if (context.initialized && context.running)
		{
			if (!AppUpdate(&context))
			{
				context.quit = 1;
				break;
			}

			// Drawing is throttled to the screen update rate, so there
			// is no need to do timing here.
			AppRender(&context);
		}
	}

	if (context.initialized)
	{
		AppShutdown(&context);
	}

	LogPrintf(">>> Exit main loop");
	exit(0);
}
