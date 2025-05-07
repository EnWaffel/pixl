#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2i.h"
#include "pixl/core/graphics/Camera.h"
#include "pixl/core/event/EventManager.h"

#include <vector>

#define PX_ERROR_OPENGL_LOAD 0x50
#define PX_ERROR_WINDOW_CREATE 0x51

namespace px
{
    class WindowImpl;

    class Window
    {
    public:
        PX_INTERNAL Window(CREFSTR title, const Vec2i& size, bool startVisible, bool decorated);
        PX_INTERNAL ~Window();

        PX_API void SetVisible(bool visible = true);
        PX_API bool ShouldClose();
        PX_API const std::vector<CAMERA> GetCameras();
        PX_API void AddCamera(CAMERA camera);
        PX_API void RemoveCamera(CAMERA camera);
        PX_API CAMERA GetDefaultCamera();
        PX_API DRAWINGCTX GetDrawingContext();
        PX_API Vec2i GetFixedSize();
        PX_API EVTMGR GetEventManager();

        PX_API void Update(float delta);
        PX_API void UpdateEvents(float delta);
        PX_API void Draw();

        PX_API void* GetHandle();
        PX_INTERNAL void PostInit();
    private:
        WindowImpl* m_Impl;
    };

    typedef Window* WINDOW;
}