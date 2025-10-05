#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/core/input/KeyCode.h"
#include "pixl/core/math/Vec2.h"

namespace px
{
    struct ButtonEntry
    {
        bool justPressed = false; // This button was just (in this frame) pressed (resets to false after px::Window::Update() is called)
        bool justReleased = false; // This button was just (in this frame) released (resets to false after px::Window::Update() is called)
        bool pressed = false;
    };

    class Window;
    class Camera;
    class Sprite;

    /**
     * @brief A simple class for quick input checking. This class only works for the first Window created! (Root Window)
     * For other Windows an EventHandler should be used.
     */
    class Mouse
    {
    public:
        PX_INTERNAL Mouse() = delete;
        PX_INTERNAL ~Mouse() = delete;
        PX_INTERNAL static void Init(Window* rootWnd);
        PX_INTERNAL static void Update();

        /**
         * @brief Returns the position of the user's cursor inside the window size range. (e.g. X: 0 - 1280 | Y: 0 - 720)
         * 
         * @param cam Optional parameter for the camera to get the position in. (nullptr = Default Camera)
         * @param wnd Optional parameter for the window to get the position from. (nullptr = First window created (Root Window))
         * 
         * @returns The position of the user's cursor.
         */
        PX_API static Vec2 GetPosition(Camera* cam = nullptr, Window* wnd = nullptr);

            /**
         * @brief Checks if the user's cursor is touching a sprite.
         * 
         * @param sprite The sprite to check for.
         * @param cam Optional parameter for the camera to get the position in. (nullptr = Default Camera)
         * @param wnd Optional parameter for the window to get the position from. (nullptr = First window created (Root Window))
         * 
         * @returns Whether the user's cursor is touching the sprite.
         */
        PX_API static bool TouchingSprite(Sprite* sprite, Camera* cam = nullptr, Window* wnd = nullptr);

        PX_API static ButtonEntry BUTTON1;
        PX_API static ButtonEntry BUTTON2;
        PX_API static ButtonEntry BUTTON3;
        PX_API static ButtonEntry BUTTON4;
        PX_API static ButtonEntry BUTTON5;
        PX_API static ButtonEntry BUTTON6;
        PX_API static ButtonEntry BUTTON7;
        PX_API static ButtonEntry BUTTON8;
        PX_API static ButtonEntry BUTTON_LAST;
        PX_API static ButtonEntry BUTTON_LEFT;
        PX_API static ButtonEntry BUTTON_RIGHT;
        PX_API static ButtonEntry BUTTON_MIDDLE;
    };
}