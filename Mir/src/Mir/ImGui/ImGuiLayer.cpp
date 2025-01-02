#include "ImGuiLayer.h"
#include "Mirpch.h"



#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"


#include "Mir/Application.h"

//TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Mir{

        // Define the function
    ImGuiKey MapKeyCodeToImGuiKey(int keyCode) {
    switch (keyCode) {
        case GLFW_KEY_TAB: return ImGuiKey_Tab;
        case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
        case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
        case GLFW_KEY_UP: return ImGuiKey_UpArrow;
        case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
        case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
        case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
        case GLFW_KEY_HOME: return ImGuiKey_Home;
        case GLFW_KEY_END: return ImGuiKey_End;
        case GLFW_KEY_INSERT: return ImGuiKey_Insert;
        case GLFW_KEY_DELETE: return ImGuiKey_Delete;
        case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
        case GLFW_KEY_SPACE: return ImGuiKey_Space;
        case GLFW_KEY_ENTER: return ImGuiKey_Enter;
        case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
        case GLFW_KEY_A: return ImGuiKey_A;
        case GLFW_KEY_C: return ImGuiKey_C;
        case GLFW_KEY_V: return ImGuiKey_V;
        case GLFW_KEY_X: return ImGuiKey_X;
        case GLFW_KEY_Y: return ImGuiKey_Y;
        case GLFW_KEY_Z: return ImGuiKey_Z;
        case GLFW_KEY_F1: return ImGuiKey_F1;
        case GLFW_KEY_F2: return ImGuiKey_F2;
        case GLFW_KEY_F3: return ImGuiKey_F3;
        case GLFW_KEY_F4: return ImGuiKey_F4;
        case GLFW_KEY_F5: return ImGuiKey_F5;
        case GLFW_KEY_F6: return ImGuiKey_F6;
        case GLFW_KEY_F7: return ImGuiKey_F7;
        case GLFW_KEY_F8: return ImGuiKey_F8;
        case GLFW_KEY_F9: return ImGuiKey_F9;
        case GLFW_KEY_F10: return ImGuiKey_F10;
        case GLFW_KEY_F11: return ImGuiKey_F11;
        case GLFW_KEY_F12: return ImGuiKey_F12;
        case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
        case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
        case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
        case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
        case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
        case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
        // Add other key mappings as needed
        default: return ImGuiKey_None;
        }
    }
    
    ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer")
    {

    }

    ImGuiLayer::~ImGuiLayer(){

    }

    void ImGuiLayer::OnAttach(){
        MIR_CORE_INFO("ImGuiLayer::OnAttach: done!");
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 

        // TEMPORARY: should eventually use Mir key codes
		// io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		// io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		// io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		// io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		// io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		// io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		// io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		// io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		// io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		// io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		// io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		// io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		// io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		// io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		// io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		// io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		// io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		// io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		// io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		// io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		// io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;


		ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach(){

    }

    void ImGuiLayer::OnUpdate(){
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

   

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        
        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }           

    void ImGuiLayer::OnEvent(Event &event){
        //MIR_CORE_INFO("ImGuiLayer::OnEvent {0}!",event);
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(MIR_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(MIR_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(MIR_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(MIR_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
        dispatcher.Dispatch<KeyPressedEvent>(MIR_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(MIR_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(MIR_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
        
        dispatcher.Dispatch<WindowResizeEvent>(MIR_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseButtonEvent(e.GetMouseButton(), true);
        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseButtonEvent(e.GetMouseButton(), false);
        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMousePosEvent(e.GetX(), e.GetY());
        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseWheelEvent(e.GetXOffset(), e.GetYOffset());
        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddKeyEvent(MapKeyCodeToImGuiKey(e.GetKeyCode()), true);

        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.AddKeyEvent(MapKeyCodeToImGuiKey(e.GetKeyCode()), false);

        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent &e){
        ImGuiIO& io = ImGui::GetIO();
        int keycode = e.GetKeyCode();
        io.AddInputCharacter(keycode);
        return false;
    }

    bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent &e) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f,1.0f);
        glViewport(0,0,e.GetWidth(),e.GetHeight());
       
        return false;
    }



}