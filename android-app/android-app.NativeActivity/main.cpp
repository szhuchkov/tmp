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


#if LOG_INPUT_EVENTS
#define LogInput LogPrintf
#else
#define LogInput	
#endif


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


struct engine {
	struct android_app* app;

	ASensorManager* sensorManager;
	const ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;

	int initialized;
	int animating;
	int quit;
};


/**
* Initialize an EGL context for the current display.
*/
static int engine_init_display(struct engine* engine) {
	LogPrintf(">>> Init...");

	int width = ANativeWindow_getWidth(engine->app->window);
	int height = ANativeWindow_getHeight(engine->app->window);

	if (!Engine::GetInstance()->Init(engine->app->window, width, height, false))
		return -1;

	if (!TestScene::GetInstance()->Init())
		return -1;

	return 0;
}


/**
* Tear down the EGL context currently associated with the display.
*/
static void engine_term_display(struct engine* engine) {
	LogPrintf(">>> Shutdown...");
	TestScene::GetInstance()->Shutdown();
	Engine::GetInstance()->Shutdown();
}


/**
* 
*/
static int engine_update_frame(struct engine* engine) {
	if (!Engine::GetInstance()->Update())
		return -1;

	size_t numEvents = InputManager::GetInstance()->GetNumEvents();
	if (numEvents)
		LogPrintf(">>> Num Events = %d", numEvents);

	for(size_t i = 0; i < numEvents; i++)
	{
		const auto& event = InputManager::GetInstance()->GetEvent(i);
		if (event.type == INPUT_EVENT_TYPE_BUTTON && event.index == AKEYCODE_BACK && !event.data.button.state)
		{
			LogPrintf(">>> EXIT!!!");
			engine->quit = 1;
		}
		else
		{
			LogPrintf(">>> ev: device = %d, type = %d, index = %d, %d", event.device, event.type, event.index, event.data.button.state);
		}
	}

	return 0;
}


/**
* Just the current frame in the display.
*/
static void engine_draw_frame(struct engine* engine) {
	if (RenderDevice::GetInstance()->BeginFrame())
	{
		constexpr float clearColor[] = { 0.4f, 0.6f, 0.8f, 1.0f };
		RenderDevice::GetInstance()->Clear(RenderDevice::CLEAR_COLOR | RenderDevice::CLEAR_DEPTH, clearColor, 1.0f, 0);
		TestScene::GetInstance()->Render();
		RenderDevice::GetInstance()->EndFrame();
	}
	//Engine::GetInstance()->Render();
}


/**
* Process the next input event.
*/
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
	int type = AInputEvent_getType(event);
	int deviceId = AInputEvent_getDeviceId(event);
	int source = AInputEvent_getSource(event);
	int sourceClass = source & AINPUT_SOURCE_CLASS_MASK;

	if (type == AINPUT_EVENT_TYPE_KEY) {
		int action = AKeyEvent_getAction(event);
		switch(source) {
		case AINPUT_SOURCE_KEYBOARD:
			{
				int code = AKeyEvent_getKeyCode(event);
				int scanCode = AKeyEvent_getScanCode(event);
				if (action == AKEY_EVENT_ACTION_DOWN)
					InputManagerAndroid::GetInstance()->OnKeyDown(code, scanCode);
				else if (action == AKEY_EVENT_ACTION_UP)
					InputManagerAndroid::GetInstance()->OnKeyUp(code, scanCode);
			}
			break;
		case AINPUT_SOURCE_DPAD:
		case AINPUT_SOURCE_GAMEPAD:
		case AINPUT_SOURCE_JOYSTICK:
		case AINPUT_SOURCE_MOUSE:
			{
				int code = AKeyEvent_getKeyCode(event);
				int scanCode = AKeyEvent_getScanCode(event);
				if (action == AKEY_EVENT_ACTION_DOWN)
					InputManagerAndroid::GetInstance()->OnButtonDown(deviceId, code, scanCode);
				else if (action == AKEY_EVENT_ACTION_UP)
					InputManagerAndroid::GetInstance()->OnButtonUp(deviceId, code, scanCode);
			}
			break;
		};
	}
	else if (type == AINPUT_EVENT_TYPE_MOTION) {
		switch(sourceClass){
		case AINPUT_SOURCE_CLASS_POINTER:
			{
				int numPointers = AMotionEvent_getPointerCount(event);
				int actionMasked = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
				int actionIndex = (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
				if (actionMasked == AMOTION_EVENT_ACTION_MOVE) {
					for (int i = 0; i < numPointers; i++) {
						int pointer = AMotionEvent_getPointerId(event, i);
						int x = AMotionEvent_getX(event, i);
						int y = AMotionEvent_getY(event, i);
						InputManagerAndroid::GetInstance()->OnPointerMove(pointer, x, y);
					}
				}
				else if (actionMasked == AMOTION_EVENT_ACTION_POINTER_DOWN || actionMasked == AMOTION_EVENT_ACTION_DOWN) {
					int pointer = AMotionEvent_getPointerId(event, actionIndex);
					int x = AMotionEvent_getX(event, pointer);
					int y = AMotionEvent_getY(event, pointer);
					InputManagerAndroid::GetInstance()->OnPointerDown(pointer, x, y);
				}
				else if (actionMasked == AMOTION_EVENT_ACTION_POINTER_UP || actionMasked == AMOTION_EVENT_ACTION_UP) {
					int pointer = AMotionEvent_getPointerId(event, actionIndex);
					int x = AMotionEvent_getX(event, pointer);
					int y = AMotionEvent_getY(event, pointer);
					InputManagerAndroid::GetInstance()->OnPointerUp(pointer, x, y);
				}
			}
			break;
		case AINPUT_SOURCE_CLASS_JOYSTICK:
			{
				int actionMasked = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
				int actionIndex = (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
				int pointer = AMotionEvent_getPointerId(event, actionIndex);
				float x = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_X, pointer);
				float y = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_Y, pointer);
				float rx = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_RX, pointer);
				float ry = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_RY, pointer);
			}
			break;
		}
	}

#if LOG_INPUT_EVENTS
	if (type == AINPUT_EVENT_TYPE_MOTION) {
		int action = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
		int pointer = (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
		int numPointers = AMotionEvent_getPointerCount(event);
		std::string pointerIdString = "";
		for(int pointerIndex = 0; pointerIndex < numPointers; pointerIndex++) {
			char buf[512] = { 0 };
			sprintf(buf, "%d", AMotionEvent_getPointerId(event, pointerIndex));
			pointerIdString += buf;
			if (pointerIndex != numPointers - 1)
				pointerIdString += ',';
		}
		const char* actionText = "UNKNOWN";
		switch(action){
		case AMOTION_EVENT_ACTION_DOWN:
			actionText = "AMOTION_EVENT_ACTION_DOWN";
			break;
		case AMOTION_EVENT_ACTION_UP:
			actionText = "AMOTION_EVENT_ACTION_UP";
			break;
		case AMOTION_EVENT_ACTION_MOVE:
			actionText = "AMOTION_EVENT_ACTION_MOVE";
			break;
		case AMOTION_EVENT_ACTION_CANCEL:
			actionText = "AMOTION_EVENT_ACTION_CANCEL";
			break;
		case AMOTION_EVENT_ACTION_OUTSIDE:
			actionText = "AMOTION_EVENT_ACTION_OUTSIDE";
			break;
		case AMOTION_EVENT_ACTION_POINTER_DOWN:
			actionText = "AMOTION_EVENT_ACTION_POINTER_DOWN";
			break;
		case AMOTION_EVENT_ACTION_POINTER_UP:
			actionText = "AMOTION_EVENT_ACTION_POINTER_UP";
			break;
		case AMOTION_EVENT_ACTION_HOVER_MOVE:
			actionText = "AMOTION_EVENT_ACTION_HOVER_MOVE";
			break;
		case AMOTION_EVENT_ACTION_SCROLL:
			actionText = "AMOTION_EVENT_ACTION_SCROLL";
			break;
		case AMOTION_EVENT_ACTION_HOVER_ENTER:
			actionText = "AMOTION_EVENT_ACTION_HOVER_ENTER";
			break;
		case AMOTION_EVENT_ACTION_HOVER_EXIT:
			actionText = "AMOTION_EVENT_ACTION_HOVER_EXIT";
			break;
		}
		const char* sourceText = "UNKNOWN";
		switch (source)
		{
		case AINPUT_SOURCE_KEYBOARD:
			sourceText = "AINPUT_SOURCE_KEYBOARD";
			break;
		case AINPUT_SOURCE_DPAD:
			sourceText = "AINPUT_SOURCE_DPAD";
			break;
		case AINPUT_SOURCE_GAMEPAD:
			sourceText = "AINPUT_SOURCE_GAMEPAD";
			break;
		case AINPUT_SOURCE_TOUCHSCREEN:
			sourceText = "AINPUT_SOURCE_TOUCHSCREEN";
			break;
		case AINPUT_SOURCE_MOUSE:
			sourceText = "AINPUT_SOURCE_MOUSE";
			break;
		case AINPUT_SOURCE_STYLUS:
			sourceText = "AINPUT_SOURCE_STYLUS";
			break;
		case AINPUT_SOURCE_TRACKBALL:
			sourceText = "AINPUT_SOURCE_TRACKBALL";
			break;
		case AINPUT_SOURCE_TOUCHPAD:
			sourceText = "AINPUT_SOURCE_TOUCHPAD";
			break;
		case AINPUT_SOURCE_TOUCH_NAVIGATION:
			sourceText = "AINPUT_SOURCE_TOUCH_NAVIGATION";
			break;
		case AINPUT_SOURCE_JOYSTICK:
			sourceText = "AINPUT_SOURCE_JOYSTICK";
		default:
			break;
		}
		const char* sourceClassText = "UNKNOWN";
		switch(sourceClass) {
		case AINPUT_SOURCE_CLASS_NONE:
			sourceClassText = "AINPUT_SOURCE_CLASS_NONE";
			break;
		case AINPUT_SOURCE_CLASS_BUTTON:
			sourceClassText = "AINPUT_SOURCE_CLASS_BUTTON";
			break;
		case AINPUT_SOURCE_CLASS_POINTER:
			sourceClassText = "AINPUT_SOURCE_CLASS_POINTER";
			break;
		case AINPUT_SOURCE_CLASS_NAVIGATION:
			sourceClassText = "AINPUT_SOURCE_CLASS_NAVIGATION";
			break;
		case AINPUT_SOURCE_CLASS_POSITION:
			sourceClassText = "AINPUT_SOURCE_CLASS_POSITION";
			break;
		case AINPUT_SOURCE_CLASS_JOYSTICK:
			sourceClassText = "AINPUT_SOURCE_CLASS_JOYSTICK";
			break;
		};
		float x = AMotionEvent_getX(event, 0);
		float y = AMotionEvent_getY(event, 0);
		LogInput("AINPUT_EVENT_TYPE_MOTION: (deviceId = %d, source = %s, sourceClass = %s, action = %s, pointer = %d, numPointers = %d (%s)) -> x = %f, y = %f",
			deviceId, sourceText, sourceClassText, actionText, pointer, numPointers, pointerIdString.c_str(), x, y);
		return 1;
	}
	else if (type == AINPUT_EVENT_TYPE_KEY) {
		auto action = AKeyEvent_getAction(event);
		auto code = AKeyEvent_getKeyCode(event);
		auto scanCode = AKeyEvent_getScanCode(event);
		if (action == AKEY_EVENT_ACTION_DOWN)
			LogInput("AKEY_EVENT_ACTION_DOWN: %d (scanCode = %d)", code, scanCode);
		else if (action == AKEY_EVENT_ACTION_UP)
			LogInput("AKEY_EVENT_ACTION_UP: %d (scanCode = %d)", code, scanCode);
		return 1;
	}
#endif
	return 0;
}

/**
* Process the next main command.
*/
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	struct engine* engine = (struct engine*)app->userData;
	switch (cmd) {
	case APP_CMD_SAVE_STATE:
		LogPrintf(">>> APP_CMD_SAVE_STATE");
		break;
	case APP_CMD_INIT_WINDOW:
		{
			LogPrintf(">>> APP_CMD_INIT_WINDOW");

			// The window is being shown, get it ready.
			if (engine->app->window != NULL)
			{
				if (engine->initialized)
				{
					if (!OGLDriverGLES::GetInstance()->OnWindowChanged(engine->app->window))
					{
						LogPrintf("OnWindowChanged() failed");
						engine->quit = 1;
					}
				}
				else
				{
					if (engine_init_display(engine))
					{
						engine->quit = 1;
					}
					else
					{
						engine->initialized = 1;
						engine_draw_frame(engine);
					}
				}
			}
		}
		break;
	case APP_CMD_TERM_WINDOW:
		LogPrintf(">>> APP_CMD_TERM_WINDOW");
		break;
	case APP_CMD_GAINED_FOCUS:
		{
			LogPrintf(">>> APP_CMD_GAINED_FOCUS");
			// When our app gains focus, we start monitoring the accelerometer.
			if (engine->accelerometerSensor != NULL) {
				ASensorEventQueue_enableSensor(engine->sensorEventQueue,
					engine->accelerometerSensor);
				// We'd like to get 60 events per second (in us).
				ASensorEventQueue_setEventRate(engine->sensorEventQueue,
					engine->accelerometerSensor, (1000L / 60) * 1000);
				engine->animating = 1;
			}
		}
		break;
	case APP_CMD_LOST_FOCUS:
		{
			LogPrintf(">>> APP_CMD_LOST_FOCUS");
			// When our app loses focus, we stop monitoring the accelerometer.
			// This is to avoid consuming battery while not being used.
			if (engine->accelerometerSensor != NULL) {
				ASensorEventQueue_disableSensor(engine->sensorEventQueue,
					engine->accelerometerSensor);
			}
			// Also stop animating.
			engine->animating = 0;
			engine_draw_frame(engine);
		}
		break;
	case APP_CMD_WINDOW_RESIZED:
		{
			LogPrintf(">>> APP_CMD_WINDOW_RESIZED");
			int width = ANativeWindow_getWidth(engine->app->window);
			int height = ANativeWindow_getHeight(engine->app->window);
			Engine::GetInstance()->Resize(width, height);
		}
		break;
	case APP_CMD_CONFIG_CHANGED:
		LogPrintf(">>> APP_CMD_CONFIG_CHANGED");
		break;
	case APP_CMD_START:
		LogPrintf(">>> APP_CMD_START");
		break;
	case APP_CMD_RESUME:
		LogPrintf(">>> APP_CMD_RESUME");
		break;
	case APP_CMD_PAUSE:
		LogPrintf(">>> APP_CMD_PAUSE");
		break;
	case APP_CMD_STOP:
		LogPrintf(">>> APP_CMD_STOP");
		break;
	case APP_CMD_DESTROY:
		LogPrintf(">>> APP_CMD_DESTROY");
		engine_term_display(engine);
		engine->initialized = 0;
		break;
	case APP_CMD_INPUT_CHANGED:
		LogPrintf(">>> APP_CMD_INPUT_CHANGED");
		break;
	default:
		LogPrintf(">>> APP_CMD: %d", cmd);
		break;
	}
}

/**
* This is the main entry point of a native application that is using
* android_native_app_glue.  It runs in its own thread, with its own
* event loop for receiving input events and doing other things.
*/
void android_main(struct android_app* state) {
	struct engine engine;

	memset(&engine, 0, sizeof(engine));
	state->userData = &engine;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;
	engine.app = state;

	// Prepare to monitor accelerometer
	engine.sensorManager = ASensorManager_getInstance();
	engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
		ASENSOR_TYPE_ACCELEROMETER);
	engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
		state->looper, LOOPER_ID_USER, NULL, NULL);

	if (state->savedState != NULL) {
	}

	engine.animating = 1;

	// loop waiting for stuff to do.

	LogPrintf(">>> Enter main loop");

	while (!engine.quit) {
		// Read all pending events.
		int ident;
		int events;
		struct android_poll_source* source;

		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
			(void**)&source)) >= 0) {

			// Process this event.
			if (source != NULL) {
				source->process(state, source);
			}

			// If a sensor has data, process it now.
			if (ident == LOOPER_ID_USER) {
				if (engine.accelerometerSensor != NULL) {
					ASensorEvent event;
					while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
						&event, 1) > 0) {
						//LogPrintf("accelerometer: x=%f y=%f z=%f",
						//	event.acceleration.x, event.acceleration.y,
						//	event.acceleration.z);
					}
				}
			}

			// Check if we are exiting.
			//if (state->destroyRequested != 0 && engine.initialized) {
			//	engine.initialized = 0;
			//	engine_term_display(&engine);
			//	return;
			//}
		}

		if (engine.initialized && engine.animating) {
			if (engine_update_frame(&engine)) {
				engine.quit = 1;
				break;
			}

			// Drawing is throttled to the screen update rate, so there
			// is no need to do timing here.
			engine_draw_frame(&engine);
		}
	}

	if (engine.initialized)
	{
		engine_term_display(&engine);
		engine.initialized = 0;
	}

	LogPrintf(">>> Exit main loop");
	exit(0);
}
