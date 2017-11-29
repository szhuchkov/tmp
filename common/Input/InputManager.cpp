#include "pch.h"
#include "InputManager.h"


InputDeviceID InputDevice::GetDeviceID() const
{
	return m_deviceID;
}


void InputDevice::SetDeviceID(InputDeviceID id)
{
	m_deviceID = id;
}


void InputDevice::ClearEvents()
{
	m_events.clear();
}


size_t InputDevice::GetNumEvents() const
{
	return m_events.size();
}


const InputEvent& InputDevice::GetEvent(size_t index) const
{
	return m_events[index];
}


void InputDevice::PushButtonEvent(int button, int state)
{
	m_events.resize(m_events.size() + 1);
	auto& e = m_events.back();
	e.type = INPUT_EVENT_TYPE_BUTTON;
	e.device = m_deviceID;
	e.index = button;
	e.data.button.state = state;
}


void InputDevice::PushAxisEvent(int axis, float value)
{
	m_events.resize(m_events.size() + 1);
	auto& e = m_events.back();
	e.type = INPUT_EVENT_TYPE_AXIS;
	e.device = m_deviceID;
	e.index = axis;
	e.data.axis.value = value;
}


void InputDevice::PushTouchEvent(int pointer, int state, int x, int y)
{
	m_events.resize(m_events.size() + 1);
	auto& e = m_events.back();
	e.type = INPUT_EVENT_TYPE_TOUCH;
	e.device = m_deviceID;
	e.index = pointer;
	e.data.touch.x = x;
	e.data.touch.y = y;
}


InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}


bool InputManager::Init()
{
	return true;
}


void InputManager::Shutdown()
{
}


void InputManager::Update()
{
	for(auto device: m_devices)
	{
		device->Update();
		PopEvents(device);
	}
}


void InputManager::AddInputDevice(InputDevice* device)
{
	m_newDeviceID++;
	device->SetDeviceID(m_newDeviceID);

	m_devices.push_back(device);
}


void InputManager::RemoveInputDevice(InputDevice* device)
{
	auto index = std::find(m_devices.begin(), m_devices.end(), device);
	if (index != m_devices.end())
	{
		m_devices.erase(index);
	}
}


size_t InputManager::GetNumDevices()
{
	return m_devices.size();
}


InputDevice* InputManager::GetDevice(size_t index)
{
	return m_devices[index];
}


InputDevice* InputManager::FindDeviceByID(InputDeviceID id)
{
	for(auto device: m_devices)
	{
		if (device->GetDeviceID() == id)
			return device;
	}
	return nullptr;
}


InputDeviceList InputManager::FindDevicesByClass(InputDeviceClass cls)
{
	InputDeviceList res;
	for(auto device: m_devices)
	{
		if (device->GetDeviceClass() == cls)
			res.push_back(device);
	}
	return res;
}


InputDeviceList InputManager::FindDevicesByName(const char* name)
{
	InputDeviceList res;
	for(auto device: m_devices)
	{
		if (!strcmp(device->GetDeviceName(), name))
			res.push_back(device);
	}
	return res;
}


InputDeviceList InputManager::GetConnectedDevices()
{
	InputDeviceList res;
	for(auto device: m_devices)
	{
		if (device->IsConnected())
			res.push_back(device);
	}
	return res;
}


void InputManager::ClearEvents()
{
	m_events.clear();
}



void InputManager::PopEvents(InputDevice* device)
{
	for (size_t i = 0; i < device->GetNumEvents(); i++)
	{
		const auto& e = device->GetEvent(i);
		PushEvent(e);
	}
	device->ClearEvents();
}


void InputManager::PushEvent(const InputEvent& e)
{
	m_events.resize(m_events.size() + 1);
	m_events.back() = e;
}


size_t InputManager::GetNumEvents()
{
	return m_events.size();
}


const InputEvent& InputManager::GetEvent(size_t index)
{
	return m_events[index];
}
