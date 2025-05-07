#include "pixl/core/graphics/RenderPipeline.h"
#include "pixl/core/window/Window.h"

#include <vector>
#include <algorithm>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace px;

namespace px
{
    class RenderPipelineImpl
    {
    private:
        WINDOW m_Wnd;
        std::vector<std::pair<std::string, PIPEELEMENT>> m_Elements;
    public:
        RenderPipelineImpl(WINDOW window) : m_Wnd(window)
        {
        }

        ~RenderPipelineImpl()
        {
            for (const auto& v : m_Elements)
            {
                delete v.second;
            }
        }

        void AddElement(CREFSTR name, PIPEELEMENT element)
        {
            element->m_Wnd = m_Wnd;
            element->Construct();
            m_Elements.push_back({ name, element });
        }

        void AddElementBefore(CREFSTR other, CREFSTR name, PIPEELEMENT element)
        {
            auto it = std::find_if(m_Elements.begin(), m_Elements.end(), [&](const std::pair<std::string, PIPEELEMENT>& p) {
                return p.first == other;
            });
        
            if (it == m_Elements.begin())
            {
                Error::Throw(PX_ERROR_INVALID_OPERATION, "Elements cannot be added before the first element");
                return;
            }
        
            if (it != m_Elements.end()) {
                element->m_Wnd = m_Wnd;
                element->Construct();
                m_Elements.insert(it, { name, element });
            }
        }

        void AddElementAfter(CREFSTR other, CREFSTR name, PIPEELEMENT element)
        {
            auto it = std::find_if(m_Elements.begin(), m_Elements.end(), [&](const std::pair<std::string, PIPEELEMENT>& p) {
                return p.first == other;
            }) + 1;
        
            if (it >= m_Elements.end())
            {
                Error::Throw(PX_ERROR_INVALID_OPERATION, "Elements cannot be after the last element");
                return;
            }
        
            if (it != m_Elements.end()) {
                element->m_Wnd = m_Wnd;
                element->Construct();
                m_Elements.insert(it, { name, element });
            }
        }

        void RemoveElement(CREFSTR name)
        {
            int i = 0;
            int erase = -1;
            for (auto& v : m_Elements)
            {
                if (v.first == name)
                {
                    erase = i;
                    break;
                }
                i++;
            }
        
            if (erase == -1) return;
            m_Elements.erase(m_Elements.begin() + erase);
        }

        PIPEELEMENT GetElement(CREFSTR name)
        {
            for (auto& v : m_Elements)
            {
                if (v.first == name) return v.second;
            }
            return nullptr;
        }

        void Flush()
        {
            for (CAMERA camera : m_Wnd->GetCameras())
            {
                PipelineData data{};
                data.camera = camera;
                data.ctx = m_Wnd->GetDrawingContext();
                
                for (auto& pair : m_Elements)
                {
                    PIPEELEMENT e = pair.second;
                    data.final = pair == m_Elements.back();

                    data = e->Downstream(data);
                }
            }

        }
    };
};

px::RenderPipeline::RenderPipeline(WINDOW window)
{
    m_Impl = new RenderPipelineImpl(window);
}

px::RenderPipeline::~RenderPipeline()
{
    delete m_Impl;
}

void px::RenderPipeline::AddElement(CREFSTR name, PIPEELEMENT element)
{
	m_Impl->AddElement(name, element);
}

void px::RenderPipeline::AddElementBefore(CREFSTR other, CREFSTR name, PIPEELEMENT element)
{
    m_Impl->AddElementBefore(other, name, element);
}

void px::RenderPipeline::AddElementAfter(CREFSTR other, CREFSTR name, PIPEELEMENT element)
{
    m_Impl->AddElementAfter(other, name, element);
}

void px::RenderPipeline::RemoveElement(CREFSTR name)
{
    m_Impl->RemoveElement(name);
}

PIPEELEMENT px::RenderPipeline::GetElement(CREFSTR name)
{
    return m_Impl->GetElement(name);
}

void px::RenderPipeline::Flush()
{
    m_Impl->Flush();
}
