// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include "scene/Node.hpp"
#include "math/Vector2.hpp"
#include "math/Size2.hpp"
#include "input/Input.hpp"

namespace ouzel
{
    enum EventModifiers
    {
        SHIFT_DOWN          = 0x0001,
        ALT_DOWN            = 0x0002,
        CONTROL_DOWN        = 0x0004,
        SUPER_DOWN          = 0x0008, // Windows key on Windows, Command key on macOS
        FUNCTION_DOWN       = 0x0010,
        LEFT_MOUSE_DOWN     = 0x0020,
        RIGHT_MOUSE_DOWN    = 0x0040,
        MIDDLE_MOUSE_DOWN   = 0x0080,
    };

    struct KeyboardEvent
    {
        uint32_t modifiers = 0;
        input::KeyboardKey key = input::KeyboardKey::NONE;
    };

    struct MouseEvent
    {
        uint32_t modifiers = 0;
        input::MouseButton button = input::MouseButton::NONE;
        Vector2 difference;
        Vector2 position;
        Vector2 scroll;
    };

    struct TouchEvent
    {
        uint64_t touchId = 0;
        Vector2 difference;
        Vector2 position;
    };

    struct GamepadEvent
    {
        input::Gamepad* gamepad = nullptr;
        input::GamepadButton button = input::GamepadButton::NONE;
        bool pressed = false;
        bool previousPressed = false;
        float value = 0.0f;
        float previousValue = 0.0f;
    };

    class Window;

    struct WindowEvent
    {
        Window* window = nullptr;
        Size2 size;
        float contentScale = 1.0f;
        std::string title;
        bool fullscreen = false;
        uint32_t screenId = 0;
    };

    struct SystemEvent
    {
        enum class Orientation
        {
            UNKNOWN,
            PORTRAIT,
            PORTRAIT_UPSIDE_DOWN,
            LANDSCAPE_LEFT,
            LANDSCAPE_RIGHT,
            FACE_UP,
            FACE_DOWN
        };

        Orientation orientation;
        std::string filename;
    };

    struct UIEvent
    {
        scene::Node* node;
        uint64_t touchId = 0;
        Vector2 difference;
        Vector2 position;
        Vector3 localPosition;
    };

    struct AnimationEvent
    {
        scene::Component* component;
    };

    struct UserEvent
    {
        std::vector<std::string> parameters;
    };

    struct Event
    {
        enum class Type
        {
            // keyboard events
            KEY_PRESS,
            KEY_RELEASE,
            KEY_REPEAT,

            // mouse events
            MOUSE_PRESS,
            MOUSE_RELEASE,
            MOUSE_SCROLL,
            MOUSE_MOVE,

            // touch events
            TOUCH_BEGIN,
            TOUCH_MOVE,
            TOUCH_END,
            TOUCH_CANCEL,

            // gamepad events
            GAMEPAD_CONNECT,
            GAMEPAD_DISCONNECT,
            GAMEPAD_BUTTON_CHANGE,

            // window events
            WINDOW_SIZE_CHANGE,
            WINDOW_TITLE_CHANGE,
            WINDOW_FULLSCREEN_CHANGE,
            WINDOW_CONTENT_SCALE_CHANGE,
            WINDOW_SCREEN_CHANGE,

            // system events
            ENGINE_START, // engine started running (sent only once)
            ENGINE_STOP, // engine stopped running (sent only once)
            ENGINE_RESUME, // engine resumed running
            ENGINE_PAUSE, // engine paused running
            ORIENTATION_CHANGE,
            LOW_MEMORY,
            OPEN_FILE,

            // UI events
            ENTER_NODE, // mouse or touch entered the scene node
            LEAVE_NODE, // mouse or touch left the scene node
            PRESS_NODE, // mouse or touch pressed on scene node
            RELEASE_NODE,  // mouse or touch released on scene node
            CLICK_NODE, // mouse or touch clicked on scene node
            DRAG_NODE, // mouse or touch dragged scene node
            WIDGET_CHANGE, // widget changed its value

            // animation events
            RESET, // animation has reset
            FINISH, // animation has finished

            USER // user defined event
        };

        Type type;

        KeyboardEvent keyboardEvent;
        MouseEvent mouseEvent;
        TouchEvent touchEvent;
        GamepadEvent gamepadEvent;
        WindowEvent windowEvent;
        SystemEvent systemEvent;
        UIEvent uiEvent;
        AnimationEvent animationEvent;
        UserEvent userEvent;
    };
}
