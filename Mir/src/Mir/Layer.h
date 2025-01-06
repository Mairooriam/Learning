#pragma once

#include "Mir/Core.h"
#include "Mir/Events/Event.h"

struct ImGuiData{
    float* vertices[3];
};

namespace Mir{

    class MIR_API Layer{
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) { (void)event; }

        inline const std::string& GetName() const {return m_DebugName;}
    protected:
        std::string m_DebugName;
    };
}