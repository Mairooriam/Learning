#pragma once

#include "Mir/Layer.h"

#include "Mir/Events/Event.h"
#include "Mir/Events/KeyEvent.h"
#include "Mir/Events/MouseEvent.h"
#include "Mir/Events/ApplicationEvent.h"

#include "imgui.h"
#include "WindowsFilebrowser.h"
namespace Mir {

       struct ImGuiData;

    class MIR_API ImGuiLayer : public Layer{
    public:
        ImGuiLayer();
        ~ImGuiLayer();
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();
    private:
        float m_Time = 0.0f;
        int m_currentStyle = 0;
        WindowsFileBrowser m_fileDialog;

    };

}
