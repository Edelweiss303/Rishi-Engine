#pragma once

#include "REngine/Core/Layer.h"
#include "REngine/Events/ApplicationEvent.h"
#include "REngine/Events/KeyEvent.h"
#include "REngine/Events/MouseEvent.h"

namespace REngine
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void Begin();
        void End();

    private:
        float m_time = 0.0f;
    };
}

