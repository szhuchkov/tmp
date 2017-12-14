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
    class MouseController : public InputDeviceMouse::Callback
    {
    public:
        MouseController(SO_FreeFlyCamera* camera, float sens, float moveSpeed) :
            m_camera(camera),
            m_sensitivity(sens),
            m_moveSpeed(moveSpeed)
        {
            InputManager::GetInstance()->GetSystemMouse()->LockCursor();
            InputManager::GetInstance()->GetSystemMouse()->AddCallback(this);
        }

        ~MouseController()
        {
            InputManager::GetInstance()->GetSystemMouse()->UnlockCursor();
            InputManager::GetInstance()->GetSystemMouse()->RemoveCallback(this);
        }

        void OnMouseMove(int x, int y) override
        {
            Vector2 angles = m_camera->GetAngles();
            angles.x += m_sensitivity * x;
            angles.y -= m_sensitivity * y;
            m_camera->SetAngles(angles);
        }

        void Update(float dt)
        {
            auto keyb = InputManager::GetInstance()->GetSystemKeyboard();
            Vector2 move(0, 0);
            if (keyb->GetKeyDown(KEY_CODE_W))
                move.x += m_moveSpeed * dt;
            if (keyb->GetKeyDown(KEY_CODE_S))
                move.x -= m_moveSpeed * dt;
            if (keyb->GetKeyDown(KEY_CODE_A))
                move.y -= m_moveSpeed * dt;
            if (keyb->GetKeyDown(KEY_CODE_D))
                move.y += m_moveSpeed * dt;

            if (keyb->GetKeyDown(KEY_CODE_LSHIFT))
                move *= 2.0f;
            
            Vector3 pos = m_camera->GetPosition();
            pos += m_moveSpeed * move.x * m_camera->GetDirection();
            pos += m_moveSpeed * move.y * m_camera->GetRightVector();
            m_camera->SetPosition(pos);
        }

    private:
        SO_FreeFlyCamera* m_camera;
        float m_sensitivity;
        float m_moveSpeed;
    };


    class GamepadController
    {
    public:
        GamepadController(GamepadController* camera)
        {
        }

        void Update(float dt)
        {
        }
    };


    class TouchController
    {
    public:
        TouchController(SO_FreeFlyCamera* camera)
        {
        }

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

        // create mouse + keyb controller
        if (InputManager::GetInstance()->GetSystemKeyboard() && InputManager::GetInstance()->GetSystemMouse())
            m_mouseController = std::make_unique<MouseController>(this, m_mouseSensitivity, m_moveSpeed);

        // create touch controller
        if (InputManager::GetInstance()->GetSystemTouch())
            m_touchController = std::make_unique<TouchController>(this);

        // create gamepad controller

        // init default matrices
        MatrixPerspective(&m_camera.proj, m_fov, m_aspect, 1.0f, 1000.0f);
        MatrixLookAt(&m_camera.view, &m_position, &(m_position + m_direction), &Vector3::UP);

        // set as current camera
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
    std::unique_ptr<MouseController> m_mouseController;
    std::unique_ptr<TouchController> m_touchController;
    std::unique_ptr<GamepadController> m_gamepadController;

    RenderCamera m_camera;
    Vector3 m_position;
    Vector3 m_direction;
    Vector3 m_right;
    Vector3 m_up;
    Vector2 m_angles;
    float m_fov = 80.0f;
    float m_aspect = 1.5f;
    float m_moveSpeed = 3.0f;
    float m_mouseSensitivity = 0.005f;
    int m_cursorPos[2] = { 0, 0 };
    bool m_updateView = false;
    bool m_updateProj = false;
};
