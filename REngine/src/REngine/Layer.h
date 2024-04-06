#pragma once
#include "REngine/Core.h"
#include "REngine/Events/Event.h"
#include "REngine/Core/TimeStep.h"

namespace REngine
{
    class RE_API Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(TimeStep ts) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const { return m_debugName; }

    protected:
        std::string m_debugName;
    };
}

