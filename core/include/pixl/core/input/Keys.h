#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/core/input/KeyCode.h"

#include <unordered_map>

namespace px
{
    struct KeyEntry
    {
        bool justPressed = false; // This key was just (in this frame) pressed (resets to false after px::Window::Update() is called)
        bool justReleased = false; // This key was just (in this frame) released (resets to false after px::Window::Update() is called)
        bool pressed = false;
    };
    
    class Window;

    /**
     * @brief A simple class for quick input checking. This class only works for the first Window created! (Root Window)
     * For other Windows an EventHandler should be used.
     */
    class Keys
    {
    public:
        PX_INTERNAL Keys() = delete;
        PX_INTERNAL ~Keys() = delete;
        PX_INTERNAL static void Init(Window* rootWnd);
        PX_INTERNAL static void Update();

        PX_API bool Pressed(KeyCode code);
        PX_API bool JustPressed(KeyCode code);
        PX_API bool JustReleased(KeyCode code);

        PX_API static KeyEntry SPACE;
        PX_API static KeyEntry APOSTROPHE;
        PX_API static KeyEntry COMMA;
        PX_API static KeyEntry MINUS;
        PX_API static KeyEntry PERIOD;
        PX_API static KeyEntry SLASH;
        PX_API static KeyEntry ZERO;
        PX_API static KeyEntry ONE;
        PX_API static KeyEntry TWO;
        PX_API static KeyEntry THREE;
        PX_API static KeyEntry FOUR;
        PX_API static KeyEntry FIVE;
        PX_API static KeyEntry SIX;
        PX_API static KeyEntry SEVEN;
        PX_API static KeyEntry EIGHT;
        PX_API static KeyEntry NINE;
        PX_API static KeyEntry SEMICOLON;
        PX_API static KeyEntry EQUAL;
        PX_API static KeyEntry A;
        PX_API static KeyEntry B;
        PX_API static KeyEntry C;
        PX_API static KeyEntry D;
        PX_API static KeyEntry E;
        PX_API static KeyEntry F;
        PX_API static KeyEntry G;
        PX_API static KeyEntry H;
        PX_API static KeyEntry I;
        PX_API static KeyEntry J;
        PX_API static KeyEntry K;
        PX_API static KeyEntry L;
        PX_API static KeyEntry M;
        PX_API static KeyEntry N;
        PX_API static KeyEntry O;
        PX_API static KeyEntry P;
        PX_API static KeyEntry Q;
        PX_API static KeyEntry R;
        PX_API static KeyEntry S;
        PX_API static KeyEntry T;
        PX_API static KeyEntry U;
        PX_API static KeyEntry V;
        PX_API static KeyEntry W;
        PX_API static KeyEntry X;
        PX_API static KeyEntry Y;
        PX_API static KeyEntry Z;
        PX_API static KeyEntry LEFT_BRACKET;
        PX_API static KeyEntry BACKSLASH;
        PX_API static KeyEntry RIGHT_BRACKET;
        PX_API static KeyEntry GRAVE_ACCENT;
        PX_API static KeyEntry WORLD_1;
        PX_API static KeyEntry WORLD_2;
        PX_API static KeyEntry ESCAPE;
        PX_API static KeyEntry ENTER;
        PX_API static KeyEntry TAB;
        PX_API static KeyEntry BACKSPACE;
        PX_API static KeyEntry INSERT;
        PX_API static KeyEntry DELETE;
        PX_API static KeyEntry RIGHT;
        PX_API static KeyEntry LEFT;
        PX_API static KeyEntry DOWN;
        PX_API static KeyEntry UP;
        PX_API static KeyEntry PAGE_UP;
        PX_API static KeyEntry PAGE_DOWN;
        PX_API static KeyEntry HOME;
        PX_API static KeyEntry END;
        PX_API static KeyEntry CAPS_LOCK;
        PX_API static KeyEntry SCROLL_LOCK;
        PX_API static KeyEntry NUM_LOCK;
        PX_API static KeyEntry PRINT_SCREEN;
        PX_API static KeyEntry PAUSE;
        PX_API static KeyEntry F1;
        PX_API static KeyEntry F2;
        PX_API static KeyEntry F3;
        PX_API static KeyEntry F4;
        PX_API static KeyEntry F5;
        PX_API static KeyEntry F6;
        PX_API static KeyEntry F7;
        PX_API static KeyEntry F8;
        PX_API static KeyEntry F9;
        PX_API static KeyEntry F10;
        PX_API static KeyEntry F11;
        PX_API static KeyEntry F12;
        PX_API static KeyEntry F13;
        PX_API static KeyEntry F14;
        PX_API static KeyEntry F15;
        PX_API static KeyEntry F16;
        PX_API static KeyEntry F17;
        PX_API static KeyEntry F18;
        PX_API static KeyEntry F19;
        PX_API static KeyEntry F20;
        PX_API static KeyEntry F21;
        PX_API static KeyEntry F22;
        PX_API static KeyEntry F23;
        PX_API static KeyEntry F24;
        PX_API static KeyEntry F25;
        PX_API static KeyEntry KP_0;
        PX_API static KeyEntry KP_1;
        PX_API static KeyEntry KP_2;
        PX_API static KeyEntry KP_3;
        PX_API static KeyEntry KP_4;
        PX_API static KeyEntry KP_5;
        PX_API static KeyEntry KP_6;
        PX_API static KeyEntry KP_7;
        PX_API static KeyEntry KP_8;
        PX_API static KeyEntry KP_9;
        PX_API static KeyEntry KP_DECIMAL;
        PX_API static KeyEntry KP_DIVIDE;
        PX_API static KeyEntry KP_MULTIPLY;
        PX_API static KeyEntry KP_SUBTRACT;
        PX_API static KeyEntry KP_ADD;
        PX_API static KeyEntry KP_ENTER;
        PX_API static KeyEntry KP_EQUAL;
        PX_API static KeyEntry LEFT_SHIFT;
        PX_API static KeyEntry LEFT_CONTROL;
        PX_API static KeyEntry LEFT_ALT;
        PX_API static KeyEntry LEFT_SUPER;
        PX_API static KeyEntry RIGHT_SHIFT;
        PX_API static KeyEntry RIGHT_CONTROL;
        PX_API static KeyEntry RIGHT_ALT;
        PX_API static KeyEntry RIGHT_SUPER;
        PX_API static KeyEntry MENU;
        PX_API static KeyEntry LAST;
    };
}