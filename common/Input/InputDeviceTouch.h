#pragma once


#include "InputManager.h"


class InputDeviceTouch : public InputDevice
{
public:
    enum
    {
        POINTER_STATE_UP,
        POINTER_STATE_DOWN,
        POINTER_STATE_JUST_DOWN,
        POINTER_STATE_JUST_UP,
    };

    struct PointerPos
    {
        int x, y;
    };

    enum { MAX_POINTERS = 4 };

    class Callback
    {
    public:
        virtual void OnPointerDown(int x, int y, unsigned int pointer) {}
        virtual void OnPointerUp(int x, int y, unsigned int pointer) {}
        virtual void OnPointerMove(int x, int y, unsigned int pointer) {}
    };

	InputDeviceTouch();
	virtual ~InputDeviceTouch();

	void Update() override;

    std::vector<unsigned int> GetActivePointers() const;
    char GetPointerState(unsigned int pointer) const;
    bool GetPointerDown(unsigned int pointer) const;
    PointerPos GetPointerPos(unsigned int pointer) const;

	void OnPointerDown(int id, int x, int y);
	void OnPointerUp(int id, int x, int y);
	void OnPointerMove(int id, int x, int y);

private:
    int GetPointer(int id);
    int AllocPointer(int id);
    void ReleasePointer(int id);

private:
    char m_pointerState[MAX_POINTERS] = {};
    PointerPos m_pointerPos[MAX_POINTERS];
    int m_pointerID[MAX_POINTERS];
};
