#include "pch.h"
#include "InputManager.h"


InputDevice::InputDevice(InputDeviceClass cls, InputDeviceID id, const std::string& name) :
    m_deviceClass(cls),
    m_deviceID(id),
    m_deviceName(name)
{
}


InputDeviceID InputDevice::GetDeviceID() const
{
	return m_deviceID;
}


void InputDevice::SetDeviceID(InputDeviceID id)
{
	m_deviceID = id;
}


InputDeviceClass InputDevice::GetDeviceClass() const
{
    return m_deviceClass;
}


const char* InputDevice::GetDeviceName() const
{
    return m_deviceName.c_str();
}


void InputDevice::SetConnected(bool connected)
{
    m_isConnected = connected;
}


bool InputDevice::IsConnected() const
{
    return m_isConnected;
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


InputDeviceMouse* InputManager::GetSystemMouse()
{
    auto devices = FindDevicesByClass(INPUT_DEVICE_CLASS_MOUSE);
    return devices.empty() ? nullptr : reinterpret_cast<InputDeviceMouse*>(devices[0]);
}


InputDeviceKeyboard* InputManager::GetSystemKeyboard()
{
    auto devices = FindDevicesByClass(INPUT_DEVICE_CLASS_KEYBOARD);
    return devices.empty() ? nullptr : reinterpret_cast<InputDeviceKeyboard*>(devices[0]);
}


InputDeviceTouch* InputManager::GetSystemTouch()
{
    auto devices = FindDevicesByClass(INPUT_DEVICE_CLASS_TOUCH);
    return devices.empty() ? nullptr : reinterpret_cast<InputDeviceTouch*>(devices[0]);
}
