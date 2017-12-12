#pragma once

#define LOGGER_NO_INPUT
#if defined _DEBUG && !defined LOGGER_NO_INPUT
#define LOG_INPUT_EVENT     LogPrintf
#else
#define LOG_INPUT_EVENT(...)
#endif


typedef size_t InputDeviceID;
constexpr InputDeviceID INVALID_INPUT_DEVICE_ID = ~0;


class InputDeviceMouse;
class InputDeviceKeyboard;
class InputDeviceTouch;
class InputDeviceJoystick;
class InputDeviceGamepad;


enum InputDeviceClass
{
	INPUT_DEVICE_CLASS_UNKNOWN,
	INPUT_DEVICE_CLASS_KEYBOARD,
	INPUT_DEVICE_CLASS_MOUSE,
	INPUT_DEVICE_CLASS_TOUCH,
	INPUT_DEVICE_CLASS_GAMEPAD,
};


class InputDevice
{
public:
    InputDevice(InputDeviceClass cls, InputDeviceID id, const std::string& name);

    InputDeviceClass GetDeviceClass() const;
    InputDeviceID GetDeviceID() const;
    const char* GetDeviceName() const;
    bool IsConnected() const;

	virtual void Update() = 0;

protected:
    void SetConnected(bool connected);
    void SetDeviceName(const std::string& name);

private:
	friend class InputManager;
	void SetDeviceID(InputDeviceID id);

private:
    InputDeviceID m_deviceID;
    InputDeviceClass m_deviceClass;
    std::string m_deviceName;
    bool m_isConnected = true;
};


typedef std::vector<InputDevice*> InputDeviceList;


class InputManager
{
public:
	static InputManager* GetInstance();

	virtual bool Init();
	virtual void Shutdown();
	virtual void Update();

	size_t GetNumDevices();
	InputDevice* GetDevice(size_t index);

    InputDeviceKeyboard* GetSystemKeyboard();
    InputDeviceMouse* GetSystemMouse();
    InputDeviceTouch* GetSystemTouch();

	InputDevice* FindDeviceByID(InputDeviceID id);
	InputDeviceList FindDevicesByClass(InputDeviceClass cls);
	InputDeviceList FindDevicesByName(const char* name);
	InputDeviceList GetConnectedDevices();

protected:
	InputManager();
	~InputManager();

	void AddInputDevice(InputDevice* device);
	void RemoveInputDevice(InputDevice* device);

private:
	InputDeviceID m_newDeviceID = 0;
	std::vector<InputDevice*> m_devices;
};