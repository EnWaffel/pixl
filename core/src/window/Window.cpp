#include "pixl/core/window/Window.h"
#include "pixl/core/pixl.h"
#include "pixl/core/graphics/RenderPipeline.h"
#include "pixl/core/graphics/CameraRenderer.h"
#include "pixl/core/graphics/WindowRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <math.h>

extern px::InitOptions __pixl_opts;

using namespace px;

extern WINDOW __pixl_rootwnd;
extern bool __pixl_singlewnd;
extern void __pixl_timer_update(float delta);

static std::string GetDebugSource(GLenum source)
{
    switch (source)
    {
    case GL_DEBUG_SOURCE_API: return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window System";
    case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
    case GL_DEBUG_SOURCE_THIRD_PARTY: return "Third Party";
    case GL_DEBUG_SOURCE_APPLICATION: return "Application";
    case GL_DEBUG_SOURCE_OTHER: return "Other";
    default: return "unknown source";
    }
}

static std::string GetDebugType(GLenum type)
{
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR: return "Error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behavior";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined Behavior";
    case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
    case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
    case GL_DEBUG_TYPE_OTHER: return "Other";
    case GL_DEBUG_TYPE_MARKER: return "Marker";
    case GL_DEBUG_TYPE_PUSH_GROUP: return "Push Group";
    case GL_DEBUG_TYPE_POP_GROUP: return "Pop Group";
    default: return "unknown type";
    }
}

static std::string GetDebugSeverity(GLenum severity)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH: return "High";
    case GL_DEBUG_SEVERITY_MEDIUM: return "Medium";
    case GL_DEBUG_SEVERITY_LOW: return "Low";
    case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
    default: return "unknown severitry";
    }
}

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::cout << "[pixl::OpenGL] [" << GetDebugSource(source) << ":" << GetDebugType(type) << "/" << GetDebugSeverity(severity) << "] (" << id << "): " << message << std::endl;
}

static void _glfw_framebuffer_resize(GLFWwindow* window, int width, int height);
static void _glfw_keycallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void _glfw_mousebuttoncallback(GLFWwindow* window, int button, int action, int mods);
static void _glfw_cursorposcallback(GLFWwindow* window, double xpos, double ypos);

namespace px
{
    class WindowImpl
    {
    private:
        WINDOW m_Parent;
        GLFWwindow* m_Handle;
        RENDERPIPELINE m_Pipeline;
        std::vector<CAMERA> m_Cameras;
        DRAWINGCTX m_DrawingContext;
        Vec2i m_InitialSize;
        EventManager* m_EventManager;
    public:
        WindowImpl(WINDOW parent, CREFSTR title, const Vec2i& size, bool startVisible, bool decorated) : m_Parent(parent), m_InitialSize(size)
        {
            glfwDefaultWindowHints();
			glfwWindowHint(GLFW_VISIBLE, startVisible);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_SAMPLES, 4);
			glfwWindowHint(GLFW_DECORATED, decorated);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

            GLFWmonitor* primaryMonitor;
#ifdef PX_WIN
            primaryMonitor = glfwGetPrimaryMonitor();
#elif defined(PX_UNIX)
            GLFWmonitor** monitors;
            int count;
            monitors = glfwGetMonitors(&count);
            primaryMonitor = monitors[0];
#endif

            PX_DEBUG_LOG("WindowImpl::WindowImpl()", "Creating window: (parent: 0x%x) title: %s | width: %d | height: %d | startVisible: %s | decorated: %s", parent, title.c_str(), size.x, size.y, startVisible ? "true" : "false", decorated ? "true" : "false");
            PX_DEBUG_LOG("WindowImpl::WindowImpl()", "Is root window?: %s", !__pixl_rootwnd ? "Yes" : "No");

            if (__pixl_rootwnd)
            {
                __pixl_singlewnd = false;
            }

            m_Handle = glfwCreateWindow(size.x, size.y, title.c_str(), NULL, __pixl_rootwnd ? (GLFWwindow*)__pixl_rootwnd->GetHandle() : nullptr);
            if (!m_Handle)
            {
                Error::Throw(PX_ERROR_WINDOW_CREATE, "Failed to create window");
                return;
            }

			glfwMakeContextCurrent(m_Handle);
			glfwSwapInterval(0);

            glfwSetWindowUserPointer(m_Handle, this);
            glfwSetFramebufferSizeCallback(m_Handle, _glfw_framebuffer_resize);
            glfwSetKeyCallback(m_Handle, _glfw_keycallback);
            glfwSetMouseButtonCallback(m_Handle, _glfw_mousebuttoncallback);
            glfwSetCursorPosCallback(m_Handle, _glfw_cursorposcallback);

            const GLFWvidmode* vidmode = glfwGetVideoMode(primaryMonitor);

#ifdef PX_WIN
            glfwSetWindowPos(m_Handle, vidmode->width / 2 - size.x / 2, vidmode->height / 2 - size.y / 2);
#elif defined(PX_UNIX)
            int monitorX, monitorY;
            glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);
            int windowX = monitorX + (vidmode->width - size.x) / 2;
            int windowY = monitorY + (vidmode->height - size.y) / 2;

            glfwSetWindowPos(m_Handle, windowX, windowY);
#endif

            if (!__pixl_rootwnd)
            {
                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                {
                    Error::Throw(PX_ERROR_OPENGL_LOAD, "Failed to load opengl");
                    return;
                }
            }

            if (!__pixl_rootwnd)
            {
                __pixl_rootwnd = parent;
            }

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(DebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        ~WindowImpl()
        {
            for (CAMERA camera : m_Cameras)
            {
                delete camera;
            }

            delete m_DrawingContext;
            delete m_Pipeline;
            delete m_EventManager;

            glfwDestroyWindow(m_Handle);
        }

        void PostInit()
        {
            m_EventManager = new EventManager;

            m_DrawingContext = new DrawingContext;

            m_Pipeline = new RenderPipeline(m_Parent);
            m_Pipeline->AddElement("cam", new CameraRenderer);
            m_Pipeline->AddElement("wnd", new WindowRenderer);

            AddCamera(new Camera(m_Parent)); // default camera

            WindowChangedData data{};
            data.viewportPos = { 0, 0 };
            data.viewportSize = m_InitialSize;
        
            m_EventManager->CallEvent(PX_EVENT(WINDOW_CHANGED), data);
        }

        void* GetHandle()
        {
            return m_Handle;
        }

        void SetVisible(bool visible)
        {
            if (visible)
            {
                glfwShowWindow(m_Handle);
            }
            else
            {
                glfwHideWindow(m_Handle);
            }
        }

        bool ShouldClose()
        {
            return glfwWindowShouldClose(m_Handle);
        }

        void Update(float delta)
        {
            if (!__pixl_singlewnd) glfwMakeContextCurrent(m_Handle);

            if (__pixl_rootwnd == m_Parent)
            {
                __pixl_timer_update(delta);
            }

            for (CAMERA camera : m_Cameras)
            {
                camera->Update(delta);
            }
        }

        void UpdateEvents(float delta)
        {
            glfwMakeContextCurrent(m_Handle);
            glfwPollEvents();
        }

        void Draw()
        {
            glfwMakeContextCurrent(m_Handle);

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Pipeline->Flush();

            glfwSwapBuffers(m_Handle);
        }

        const std::vector<CAMERA> GetCameras()
        {
            return m_Cameras;
        }

        void AddCamera(CAMERA camera)
        {
            m_Cameras.push_back(camera);
        }

        void RemoveCamera(CAMERA camera)
        {
            auto it = std::find(m_Cameras.begin(), m_Cameras.end(), camera);
            if (it == m_Cameras.end()) return;
            m_Cameras.erase(it);
        }

        CAMERA GetDefaultCamera()
        {
            if (m_Cameras.empty()) return nullptr;
            return m_Cameras.front();
        }

        DRAWINGCTX GetDrawingContext()
        {
            return m_DrawingContext;
        }

        Vec2i GetFixedSize()
        {
            return m_InitialSize;
        }

        EVTMGR GetEventManager()
        {
            return m_EventManager;
        }
    };
};

static void _glfw_framebuffer_resize(GLFWwindow* window, int width, int height)
{
    if (width < 1 || height < 1) return;

	WindowImpl* wnd = (WindowImpl*)glfwGetWindowUserPointer(window);
	Vec2i size = wnd->GetFixedSize();

	if (width < 1 || height < 1) return;

	float ratioX = width / (float)size.x;
	float ratioY = height / (float)size.y;
	float ratio = fminf(ratioX, ratioY);

	int viewWidth = (int)(size.x * ratio);
	int viewHeight = (int)(size.y * ratio);

	int viewX = (int)(width - size.x * ratio) / 2;
	int viewY = (int)(height - size.y * ratio) / 2;

    glViewport(viewX, viewY, viewWidth, viewHeight);

    WindowChangedData data{};
    data.viewportPos = { viewX, viewY };
    data.viewportSize = { viewWidth, viewHeight };

    wnd->GetEventManager()->CallEvent(PX_EVENT(WINDOW_CHANGED), data);
}

static void _glfw_keycallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    WindowImpl* wnd = (WindowImpl*)glfwGetWindowUserPointer(window);

    KeyEvent data{};
    data.code = (KeyCode)key;

    if (action == GLFW_PRESS)
    {
        wnd->GetEventManager()->CallEvent(PX_EVENT(KEY_PRESS), data);
    }
    else if (action == GLFW_RELEASE)
    {
        wnd->GetEventManager()->CallEvent(PX_EVENT(KEY_RELEASE), data);
    }
}

static void _glfw_mousebuttoncallback(GLFWwindow* window, int button, int action, int mods)
{
    WindowImpl* wnd = (WindowImpl*)glfwGetWindowUserPointer(window);

    MouseEvent data{};
    data.button = (MouseButton)button;

    if (action == GLFW_PRESS)
    {
        wnd->GetEventManager()->CallEvent(PX_EVENT(MOUSE_PRESS), data);
    }
    else if (action == GLFW_RELEASE)
    {
        wnd->GetEventManager()->CallEvent(PX_EVENT(MOUSE_RELEASE), data);
    }
}

static void _glfw_cursorposcallback(GLFWwindow* window, double xpos, double ypos)
{
    WindowImpl* wnd = (WindowImpl*)glfwGetWindowUserPointer(window);

    MouseMoveEvent data{};
    data.x = xpos;
    data.y = ypos;

    wnd->GetEventManager()->CallEvent(PX_EVENT(MOUSE_MOVE), data);
}

px::Window::Window(CREFSTR title, const Vec2i& size, bool startVisible, bool decorated)
{
    m_Impl = new WindowImpl(this, title, size, startVisible, decorated);
}

px::Window::~Window()
{
    delete m_Impl;
}

void px::Window::SetVisible(bool visible)
{
    m_Impl->SetVisible(visible);
}

bool px::Window::ShouldClose()
{
    return m_Impl->ShouldClose();
}

const std::vector<CAMERA> px::Window::GetCameras()
{
    return m_Impl->GetCameras();
}

void px::Window::AddCamera(CAMERA camera)
{
    m_Impl->AddCamera(camera);
}

void px::Window::RemoveCamera(CAMERA camera)
{
    m_Impl->RemoveCamera(camera);
}

CAMERA px::Window::GetDefaultCamera()
{
    return m_Impl->GetDefaultCamera();
}

DRAWINGCTX px::Window::GetDrawingContext()
{
    return m_Impl->GetDrawingContext();
}

Vec2i px::Window::GetFixedSize()
{
    return m_Impl->GetFixedSize();
}

EVTMGR px::Window::GetEventManager()
{
    return m_Impl->GetEventManager();
}

void px::Window::Update(float delta)
{
    m_Impl->Update(delta);
}

void px::Window::UpdateEvents(float delta)
{
    m_Impl->UpdateEvents(delta);
}

void px::Window::Draw()
{
    m_Impl->Draw();
}

void* px::Window::GetHandle()
{
    return m_Impl->GetHandle();
}

void px::Window::PostInit()
{
    m_Impl->PostInit();
}
