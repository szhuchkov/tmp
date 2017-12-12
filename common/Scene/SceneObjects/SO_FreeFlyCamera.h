#pragma once


#include <Scene/SceneObject.h>
#include <Scene/SceneObjectData.h>
#include <Render/RenderWorld.h>
#include <Render/RenderDevice.h>
#include <Input/InputDeviceMouse.h>
#include <Input/InputDeviceKeyboard.h>


class SO_FreeFlyCamera : public SceneObject
{
private:
    class MouseController
    {
    public:
        MouseController(SO_FreeFlyCamera* camera, InputDeviceMouse* mouse, InputDeviceKeyboard* keyboard, float sens, float moveSpeed) :
            m_camera(camera),
            m_mouse(mouse),
            m_keyboard(keyboard),
            m_sensitivity(sens),
            m_moveSpeed(moveSpeed)
        {
            mouse->LockCursor();
        }

        ~MouseController()
        {
            m_mouse->UnlockCursor();
        }

        void Update(float dt)
        {
            auto cursorPos = m_mouse->GetCursorPos();

            if (cursorPos.x != 0 || cursorPos.y != 0)
            {
                Vector2 angles = m_camera->GetAngles();
                angles.x += m_sensitivity * cursorPos.x;
                angles.y -= m_sensitivity * cursorPos.y;
                m_camera->SetAngles(angles);
            }

            Vector2 move(0, 0);
            if (m_keyboard->GetKeyDown('W'))
                move.x += m_moveSpeed * dt;
            if (m_keyboard->GetKeyDown('S'))
                move.x -= m_moveSpeed * dt;
            if (m_keyboard->GetKeyDown('A'))
                move.y -= m_moveSpeed * dt;
            if (m_keyboard->GetKeyDown('D'))
                move.y += m_moveSpeed * dt;

            if (std::abs(move.x) > 0.0001f || std::abs(move.y) > 0.001f)
            {
                Vector3 pos = m_camera->GetPosition();
                pos += m_moveSpeed * move.x * m_camera->GetDirection();
                pos += m_moveSpeed * move.y * m_camera->GetRightVector();
                m_camera->SetPosition(pos);
            }
        }

    private:
        SO_FreeFlyCamera* m_camera;
        InputDeviceMouse* m_mouse;
        InputDeviceKeyboard* m_keyboard;
        InputDeviceMouse::CursorPos m_cursorPos;
        float m_sensitivity;
        float m_moveSpeed;
    };


    class GamepadController
    {
    public:
        void Update(float dt)
        {
        }
    };


    class TouchController
    {
    public:
        void Update(float dt)
        {
        }
    };

public:
    static constexpr char* CLASS_NAME = "FreeFlyCamera";

    static constexpr char* FOV_PARAM_NAME = "Fov";
    static constexpr char* MOUSE_SENS_PARAM_NAME = "MouseSens";
    static constexpr char* MOVE_SPEED_PARAM_NAME = "MoveSpeed";
    static constexpr char* POSITION_PARAM_NAME = "Position";

    SO_FreeFlyCamera(int id) :
        SceneObject(id, CLASS_NAME)
    {
        m_position = Vector3::ZERO;
        m_direction = Vector3::FORWARD;
        m_right = Vector3::RIGHT;
        m_up = Vector3::UP;
        m_angles = Vector2(0, 0);
    }

    ~SO_FreeFlyCamera()
    {
    }

    bool Load(const SceneObjectData& data) override
    {
        if (!SceneObject::Load(data))
            return false;

        data.GetFloat(FOV_PARAM_NAME, &m_fov);
        data.GetFloat(MOUSE_SENS_PARAM_NAME, &m_mouseSensitivity);
        data.GetFloat(MOVE_SPEED_PARAM_NAME, &m_moveSpeed);
        data.GetVec3(POSITION_PARAM_NAME, &m_position);

        return true;
    }

    void Spawn() override
    {
        SceneObject::Spawn();

        InputDeviceMouse* mouse = nullptr;
        InputDeviceKeyboard* keyb = nullptr;

        auto mouseList = InputManager::GetInstance()->FindDevicesByClass(INPUT_DEVICE_CLASS_MOUSE);
        if (!mouseList.empty())
            mouse = static_cast<InputDeviceMouse*>(mouseList[0]);

        auto keybList = InputManager::GetInstance()->FindDevicesByClass(INPUT_DEVICE_CLASS_KEYBOARD);
        if (!keybList.empty())
            keyb = static_cast<InputDeviceKeyboard*>(keybList[0]);

        if (mouse && keyb)
            m_mouseController = new MouseController(this, mouse, keyb, m_mouseSensitivity, m_moveSpeed);

        MatrixPerspective(&m_camera.proj, m_fov, m_aspect, 1.0f, 1000.0f);
        MatrixLookAt(&m_camera.view, &m_position, &(m_position + m_direction), &Vector3::UP);

        RenderWorld::GetInstance()->SetCamera(&m_camera);
    }

    void Update(unsigned int ms) override
    {
        SceneObject::Update(ms);

        float dt = 0.001f * ms;

        if (m_mouseController)
            m_mouseController->Update(dt);
        if (m_touchController)
            m_touchController->Update(dt);
        if (m_gamepadController)
            m_gamepadController->Update(dt);

        auto screenSize = RenderDevice::GetInstance()->GetScreenSize();
        if (std::abs(m_aspect - screenSize.aspect) > 0.01f)
            m_updateProj = true;

        if (m_updateView)
        {
            m_updateView = false;

            float sa = std::sin(m_angles.x);
            float ca = std::cos(m_angles.x);
            float sb = std::sin(m_angles.y);
            float cb = std::cos(m_angles.y);

            m_direction.x = sa * cb;
            m_direction.y = sb;
            m_direction.z = ca * cb;
            Vec3Normalize(&m_direction, &m_direction);

            Vec3Cross(&m_right, &Vector3::UP, &m_direction);
            Vec3Normalize(&m_right, &m_right);

            Vec3Cross(&m_up, &m_direction, &m_right);

            MatrixLookAt(&m_camera.view, &m_position, &(m_position + m_direction), &Vector3::UP);
        }

        if (m_updateProj)
        {
            m_updateProj = false;
            MatrixPerspective(&m_camera.proj, m_fov, m_aspect, 1.0f, 1000.0f);
        }
    }

    const Vector2& GetAngles() const
    {
        return m_angles;
    }

    void SetAngles(const Vector2& a)
    {
        m_updateView = true;
        m_angles = a;
    }

    const Vector3& GetPosition() const
    {
        return m_position;
    }

    void SetPosition(const Vector3& pos)
    {
        m_updateView = true;
        m_position = pos;
    }

    const Vector3& GetDirection() const
    {
        return m_direction;
    }

    const Vector3& GetRightVector() const
    {
        return m_right;
    }

    const Vector3& GetUpVector() const
    {
        return m_up;
    }

private:
    MouseController* m_mouseController = nullptr;
    TouchController* m_touchController = nullptr;
    GamepadController* m_gamepadController = nullptr;

    RenderCamera m_camera;
    Vector3 m_position;
    Vector3 m_direction;
    Vector3 m_right;
    Vector3 m_up;
    Vector2 m_angles;
    float m_fov = 80.0f;
    float m_aspect = 1.5f;
    float m_moveSpeed = 0.5f;
    float m_mouseSensitivity = 0.005f;
    int m_cursorPos[2] = { 0, 0 };
    bool m_updateView = false;
    bool m_updateProj = false;
};
