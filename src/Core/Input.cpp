#include "Input.hpp"

namespace GravyEngine
{
    AxisKeys::AxisKeys()
    {

    }

    AxisKeys::AxisKeys(KeyCode positive, KeyCode negative)
    {
        this->positive = positive;
        this->negative = negative;
    }

    AxisInfo::AxisInfo()
    {

    }

    AxisInfo::AxisInfo(const std::string &name)
    {
        this->name = name;
    }

    void AxisInfo::AddKeys(KeyCode positive, KeyCode negative)
    {
        this->keys.push_back(AxisKeys(positive, negative));
    }

    std::unordered_map<KeyCode,KeyState> Input::keyStates;
    std::unordered_map<ButtonCode,KeyState> Input::buttonStates;
    std::unordered_map<std::string, AxisInfo> Input::keyToAxisMap;
    Vector2 Input::mousePosition = Vector2(0, 0);
    Vector2 Input::mousePositionOld = Vector2(0, 0);
    Vector2 Input::mouseDelta = Vector2(0, 0);
    Vector2 Input::scrollDirection = Vector2(0, 0);
    GLFWwindow *Input::pWindow = nullptr;
    bool Input::cursorEnabled = true;

    void Input::Initialize(GLFWwindow *window)
    {
        pWindow = window;

        keyStates[KeyCode::Unknown] = KeyState();
        keyStates[KeyCode::Space] = KeyState();
        keyStates[KeyCode::Apostrophe] = KeyState();
        keyStates[KeyCode::Comma] = KeyState();
        keyStates[KeyCode::Minus] = KeyState();
        keyStates[KeyCode::Period] = KeyState();
        keyStates[KeyCode::Slash] = KeyState();
        keyStates[KeyCode::Alpha0] = KeyState();
        keyStates[KeyCode::Alpha1] = KeyState();
        keyStates[KeyCode::Alpha2] = KeyState();
        keyStates[KeyCode::Alpha3] = KeyState();
        keyStates[KeyCode::Alpha4] = KeyState();
        keyStates[KeyCode::Alpha5] = KeyState();
        keyStates[KeyCode::Alpha6] = KeyState();
        keyStates[KeyCode::Alpha7] = KeyState();
        keyStates[KeyCode::Alpha8] = KeyState();
        keyStates[KeyCode::Alpha9] = KeyState();
        keyStates[KeyCode::SemiColon] = KeyState();
        keyStates[KeyCode::Equal] = KeyState();
        keyStates[KeyCode::A] = KeyState();
        keyStates[KeyCode::B] = KeyState();
        keyStates[KeyCode::C] = KeyState();
        keyStates[KeyCode::D] = KeyState();
        keyStates[KeyCode::E] = KeyState();
        keyStates[KeyCode::F] = KeyState();
        keyStates[KeyCode::G] = KeyState();
        keyStates[KeyCode::H] = KeyState();
        keyStates[KeyCode::I] = KeyState();
        keyStates[KeyCode::J] = KeyState();
        keyStates[KeyCode::K] = KeyState();
        keyStates[KeyCode::L] = KeyState();
        keyStates[KeyCode::M] = KeyState();
        keyStates[KeyCode::N] = KeyState();
        keyStates[KeyCode::O] = KeyState();
        keyStates[KeyCode::P] = KeyState();
        keyStates[KeyCode::Q] = KeyState();
        keyStates[KeyCode::R] = KeyState();
        keyStates[KeyCode::S] = KeyState();
        keyStates[KeyCode::T] = KeyState();
        keyStates[KeyCode::U] = KeyState();
        keyStates[KeyCode::V] = KeyState();
        keyStates[KeyCode::W] = KeyState();
        keyStates[KeyCode::X] = KeyState();
        keyStates[KeyCode::Y] = KeyState();
        keyStates[KeyCode::Z] = KeyState();
        keyStates[KeyCode::LeftBracket] = KeyState();
        keyStates[KeyCode::BackSlash] = KeyState();
        keyStates[KeyCode::RightBracket] = KeyState();
        keyStates[KeyCode::GraveAccent] = KeyState();
        keyStates[KeyCode::World1] = KeyState();
        keyStates[KeyCode::World2] = KeyState();
        keyStates[KeyCode::Escape] = KeyState();
        keyStates[KeyCode::Enter] = KeyState();
        keyStates[KeyCode::Tab] = KeyState();
        keyStates[KeyCode::Backspace] = KeyState();
        keyStates[KeyCode::Insert] = KeyState();
        keyStates[KeyCode::Delete] = KeyState();
        keyStates[KeyCode::Right] = KeyState();
        keyStates[KeyCode::Left] = KeyState();
        keyStates[KeyCode::Down] = KeyState();
        keyStates[KeyCode::Up] = KeyState();
        keyStates[KeyCode::PageUp] = KeyState();
        keyStates[KeyCode::PageDown] = KeyState();
        keyStates[KeyCode::Home] = KeyState();
        keyStates[KeyCode::End] = KeyState();
        keyStates[KeyCode::CapsLock] = KeyState();
        keyStates[KeyCode::ScrollLock] = KeyState();
        keyStates[KeyCode::NumLock] = KeyState();
        keyStates[KeyCode::PrintScreen] = KeyState();
        keyStates[KeyCode::Pause] = KeyState();
        keyStates[KeyCode::F1] = KeyState();
        keyStates[KeyCode::F2] = KeyState();
        keyStates[KeyCode::F3] = KeyState();
        keyStates[KeyCode::F4] = KeyState();
        keyStates[KeyCode::F5] = KeyState();
        keyStates[KeyCode::F6] = KeyState();
        keyStates[KeyCode::F7] = KeyState();
        keyStates[KeyCode::F8] = KeyState();
        keyStates[KeyCode::F9] = KeyState();
        keyStates[KeyCode::F10] = KeyState();
        keyStates[KeyCode::F11] = KeyState();
        keyStates[KeyCode::F12] = KeyState();
        keyStates[KeyCode::F13] = KeyState();
        keyStates[KeyCode::F14] = KeyState();
        keyStates[KeyCode::F15] = KeyState();
        keyStates[KeyCode::F16] = KeyState();
        keyStates[KeyCode::F17] = KeyState();
        keyStates[KeyCode::F18] = KeyState();
        keyStates[KeyCode::F19] = KeyState();
        keyStates[KeyCode::F20] = KeyState();
        keyStates[KeyCode::F21] = KeyState();
        keyStates[KeyCode::F22] = KeyState();
        keyStates[KeyCode::F23] = KeyState();
        keyStates[KeyCode::F24] = KeyState();
        keyStates[KeyCode::F25] = KeyState();
        keyStates[KeyCode::Keypad0] = KeyState();
        keyStates[KeyCode::Keypad1] = KeyState();
        keyStates[KeyCode::Keypad2] = KeyState();
        keyStates[KeyCode::Keypad3] = KeyState();
        keyStates[KeyCode::Keypad4] = KeyState();
        keyStates[KeyCode::Keypad5] = KeyState();
        keyStates[KeyCode::Keypad6] = KeyState();
        keyStates[KeyCode::Keypad7] = KeyState();
        keyStates[KeyCode::Keypad8] = KeyState();
        keyStates[KeyCode::Keypad9] = KeyState();
        keyStates[KeyCode::Decimal] = KeyState();
        keyStates[KeyCode::Divide] = KeyState();
        keyStates[KeyCode::Multiply] = KeyState();
        keyStates[KeyCode::Subtract] = KeyState();
        keyStates[KeyCode::Add] = KeyState();
        keyStates[KeyCode::KeypadEnter] = KeyState();
        keyStates[KeyCode::KeypadEqual] = KeyState();
        keyStates[KeyCode::LeftShift] = KeyState();
        keyStates[KeyCode::LeftControl] = KeyState();
        keyStates[KeyCode::LeftAlt] = KeyState();
        keyStates[KeyCode::LeftSuper] = KeyState();
        keyStates[KeyCode::RightShift] = KeyState();
        keyStates[KeyCode::RightControl] = KeyState();
        keyStates[KeyCode::RightAlt] = KeyState();
        keyStates[KeyCode::RightSuper] = KeyState();
        keyStates[KeyCode::Menu] = KeyState();

        buttonStates[ButtonCode::Left] = KeyState();
        buttonStates[ButtonCode::Right] = KeyState();
        buttonStates[ButtonCode::Middle] = KeyState();

        AxisInfo axisHorizontal("Horizontal");
        AxisInfo axisVertical("Vertical");
        AxisInfo axisPanning("Panning");

        axisHorizontal.AddKeys(KeyCode::D, KeyCode::A);
        axisVertical.AddKeys(KeyCode::W, KeyCode::S);
        axisPanning.AddKeys(KeyCode::R, KeyCode::F);
        
        RegisterAxis(axisHorizontal);
        RegisterAxis(axisVertical);
        RegisterAxis(axisPanning);
    }

    void Input::RegisterAxis(const AxisInfo &axisInfo)
    {
        if(keyToAxisMap.count(axisInfo.name) > 0)
            return;
        keyToAxisMap[axisInfo.name] = axisInfo;
    }

    void Input::OnStartFrame()
    {
        for(auto& k : keyStates)
        {
            KeyCode key = k.first;
            int state = k.second.state;

            if(state > 0)
            {
                if(keyStates[key].down == 0)
                {
                    keyStates[key].down = 1;
                    keyStates[key].pressed = 0;
                }
                else
                {
                    keyStates[key].down = 1;
                    keyStates[key].pressed = 1;
                }

                keyStates[key].up = 0;
            }
            else
            {
                if(keyStates[key].down == 1 || keyStates[key].pressed == 1)
                {
                    keyStates[key].down = 0;
                    keyStates[key].pressed = 0;
                    keyStates[key].up = 1;
                }
                else
                {
                    keyStates[key].down = 0;
                    keyStates[key].pressed = 0;
                    keyStates[key].up = 0;
                }
            }
        }

        for(auto& k : buttonStates)
        {
            ButtonCode button = k.first;
            int state = k.second.state;

            if(state > 0)
            {
                if(buttonStates[button].down == 0)
                {
                    buttonStates[button].down = 1;
                    buttonStates[button].pressed = 0;
                }
                else
                {
                    buttonStates[button].down = 1;
                    buttonStates[button].pressed = 1;
                }

                buttonStates[button].up = 0;
            }
            else
            {
                if(buttonStates[button].down == 1 || buttonStates[button].pressed == 1)
                {
                    buttonStates[button].down = 0;
                    buttonStates[button].pressed = 0;
                    buttonStates[button].up = 1;
                }
                else
                {
                    buttonStates[button].down = 0;
                    buttonStates[button].pressed = 0;
                    buttonStates[button].up = 0;
                }
            }
        }
    }

    void Input::OnEndFrame()
    {
        mouseDelta = Vector2(0.0f, 0.0f);
        scrollDirection = Vector2(0.0f, 0.0f);
    }

    void Input::SetMouseCursor(bool visible)
    {
        glfwSetInputMode(pWindow, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        cursorEnabled = visible;
    }

    void Input::ToggleMouseCursor()
    {
        SetMouseCursor(!cursorEnabled);
    }

    bool Input::IsCursorEnabled()
    {
        return cursorEnabled;
    }

    void Input::SetKeyState(KeyCode key, int state)
    {
        keyStates[key].state = state;
    }

    void Input::SetButtonState(ButtonCode button, int state)
    {
        buttonStates[button].state = state;
    }

    void Input::SetMousePosition(double x, double y)
    {
        double prevX = mousePosition.x;
        double prevY = mousePosition.y;

        mousePosition.x = x;
        mousePosition.y = y;

        mouseDelta.x = x - prevX;
        mouseDelta.y = y - prevY;
    }

    void Input::SetScrollDirection(double x, double y)
    {
        scrollDirection.x = static_cast<float>(x);
        scrollDirection.y = static_cast<float>(y);
    }

    Vector2 Input::GetScrollDirection()
    {
        return scrollDirection;
    }    

    bool Input::GetKey(KeyCode key)
    {
        KeyState state = keyStates[key];
        return state.down == 1 && state.pressed == 1;
    }

    bool Input::GetKeyDown(KeyCode key)
    {
        KeyState state = keyStates[key];
        return state.down == 1 && state.pressed == 0;
    }

    bool Input::GetKeyUp(KeyCode key)
    {
        KeyState state = keyStates[key];
        return state.up > 0;
    }

    float Input::GetAxis(const std::string &axis)
    {
        if (keyToAxisMap.count(axis) > 0)
        {
            for (size_t i = 0; i < keyToAxisMap[axis].keys.size(); i++)
            {
                if (GetKey(keyToAxisMap[axis].keys[i].positive))
                    return 1.0f;
                else if (GetKey(keyToAxisMap[axis].keys[i].negative))
                    return -1.0f;
            }
        }

        return 0.0f;
    }

    bool Input::GetButton(ButtonCode button)
    {
        KeyState state = buttonStates[button];
        return state.down == 1 && state.pressed == 1;
    }

    bool Input::GetButtonDown(ButtonCode button)
    {
        KeyState state = buttonStates[button];
        return state.down == 1 && state.pressed == 0;
    }

    bool Input::GetButtonUp(ButtonCode button)
    {
        KeyState state = buttonStates[button];
        return state.up > 0;
    }

    Vector2 Input::GetMousePosition()
    {
        return mousePosition;
    }

    Vector2 Input::GetMouseDelta()
    {
        return mouseDelta;
    }
};