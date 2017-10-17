#pragma once


typedef size_t InputDeviceID;


constexpr InputDeviceID INVALID_INPUT_DEVICE_ID = ~0;



enum InputEventType
{
	INPUT_EVENT_TYPE_BUTTON,
	INPUT_EVENT_TYPE_AXIS,
	INPUT_EVENT_TYPE_TOUCH,
};


struct InputEvent
{
	InputEventType type;
	InputDeviceID device;
	int index;
	union {
		struct {
			int state;
		} button;
		struct {
			float value;
		} axis;
		struct {
			int state;
			int x, y;
		} touch;
	} data;
};


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
	InputDeviceID GetDeviceID() const;

	virtual InputDeviceClass GetDeviceClass() const = 0;
	virtual const char* GetDeviceName() const = 0;

	virtual void Update() = 0;

	virtual bool IsConnected() const = 0;

	virtual int GetNumButtons() const = 0;
	virtual int GetButton(int button) const = 0;

	virtual int GetNumAxis() const = 0;
	virtual float GetAxis(int axis) const = 0;

private:
	friend class InputManager;
	InputDeviceID m_deviceID = INVALID_INPUT_DEVICE_ID;
	void SetDeviceID(InputDeviceID id);

private:
	void ClearEvents();
	size_t GetNumEvents() const;
	const InputEvent& GetEvent(size_t index) const;
	std::vector<InputEvent> m_events;

protected:
	void PushButtonEvent(int button, int state);
	void PushAxisEvent(int axis, float value);
	void PushTouchEvent(int pointer, int state, int x, int y);
};


typedef std::vector<InputDevice*> InputDeviceList;


class InputManager
{
public:
	static InputManager* GetInstance();

	virtual bool Init();
	virtual void Shutdown();
	virtual void Update();

	void ClearEvents();

	size_t GetNumDevices();
	InputDevice* GetDevice(size_t index);

	InputDevice* FindDeviceByID(InputDeviceID id);
	InputDeviceList FindDevicesByClass(InputDeviceClass cls);
	InputDeviceList FindDevicesByName(const char* name);
	InputDeviceList GetConnectedDevices();

	size_t GetNumEvents();
	const InputEvent& GetEvent(size_t index);

protected:
	InputManager();
	~InputManager();
	void AddInputDevice(InputDevice* device);
	void RemoveInputDevice(InputDevice* device);
	void PopEvents(InputDevice* device);
	void PushEvent(const InputEvent& e);

private:
	InputDeviceID m_newDeviceID = 0;
	std::vector<InputDevice*> m_devices;
	std::vector<InputEvent> m_events;
};