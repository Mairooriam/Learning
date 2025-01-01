#pragma once

#include "Mir/Layer.h"

#include "Mir/Events/Event.h"
#include "Mir/Events/KeyEvent.h"
#include "Mir/Events/MouseEvent.h"
#include "Mir/Events/ApplicationEvent.h"
namespace Mir {

    class MIR_API ImGuiLayer : public Layer{
    public:
        ImGuiLayer();
        ~ImGuiLayer();
        void OnAttach();
        void OnDetach();
        void OnUpdate();
        void OnEvent(Event& event);
    private:
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnMouseMovedEvent(MouseMovedEvent& e);
        bool OnMouseScrolledEvent(MouseScrolledEvent& e);
        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnKeyTypedEvent(KeyTypedEvent& e);
        bool OnWindowResizeEvent(WindowResizeEvent& e);

    private:
        float m_Time = 0.0f;

    };



}
