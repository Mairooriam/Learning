#pragma once

#include "Mir/Core.h"
#include "Mir/Events/Event.h"
#include "Mir/Events/ApplicationEvent.h"
#include "Mir/Window.h"

#include <GLFW/glfw3.h>


namespace Mir {

    class WindowsWindow : public Window {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        inline virtual void* GetNativeWindow() const { return m_Window; };
    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();
    private:
        GLFWwindow* m_Window;

    public:
        struct WindowData {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };
    private:
        WindowData m_Data;


        
    };

};


namespace fmt {
    template <>
    struct formatter<Mir::WindowsWindow::WindowData> {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }
        
        template <typename FormatContext>
        auto format(const Mir::WindowsWindow::WindowData& wd, FormatContext& ctx) const {
            return format_to(
                ctx.out(),
                "WindowData(Title: {}, Width: {}, Height: {}, VSync: {})",
                wd.Title, wd.Width, wd.Height, wd.VSync
            );
        }
    };
}
