#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2i.h"
#include "pixl/core/graphics/2d/Camera.h"
#include "pixl/core/event/EventManager.h"
#include "pixl/core/graphics/RenderPipeline.h"

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
        PX_API Vec2i GetViewportPos();
        PX_API Vec2i GetViewportSize();
        PX_API Vec2i GetSize();
        PX_API Vec2i GetPosition();
        PX_API void SetPosition(const Vec2i& pos);
        PX_API void SetSize(const Vec2i& size);
        PX_API void SetFullscreen(bool fullscreen);
        PX_API RENDERPIPELINE GetRenderPipeline();
        PX_API void SetRenderPipeline(RENDERPIPELINE pipeline);
        PX_API RENDERPIPELINE CreateDefaultPipeline();
        PX_API RENDERPIPELINE CreateDefault3DPipeline();
        PX_API CAMERA GetStaticCamera();
        PX_API const std::vector<CAMERA3D> GetCameras3D();
        PX_API CAMERA3D GetDefaultCamera3D();
        PX_API void AddCamera3D(CAMERA3D camera);
        PX_API void RemoveCamera3D(CAMERA3D camera);
        PX_API void Close();
        PX_API void DisableCursor();
        PX_API void EnableCursor();
        PX_API void HideCursor();
        PX_API void ShowCursor();
        PX_API void SetVSync(bool enabled);
        PX_API void SetTitle(CREFSTR title);
        PX_API std::string GetTitle();

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