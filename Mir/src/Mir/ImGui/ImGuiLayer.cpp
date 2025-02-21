#include "ImGuiLayer.h"
#include "Mirpch.h"



#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "Mir/Application.h"

//TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>



namespace Mir{
    
    void SetKeyboardNavigationEnabled(bool enabled) {
        ImGuiIO& io = ImGui::GetIO();
        if (enabled) {
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        } else {
            io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
        }
    }


    // Function to display input text with auto-completion suggestions
    bool InputTextWithSuggestions(const char* label, char* buf, size_t buf_size, const std::vector<std::string_view>& suggestions) {
        bool value_changed = ImGui::InputText(label, buf, buf_size);
        static int selected_index = -1;

        if (ImGui::IsItemActive()) {
            SetKeyboardNavigationEnabled(false);
            ImGui::BeginTooltip();
            for (int i = 0; i < suggestions.size(); ++i) {
                const auto& suggestion = suggestions[i];
                if (std::string_view(buf).empty() || suggestion.find(buf) != std::string_view::npos) {
                    bool is_selected = (i == selected_index);
                    if (ImGui::Selectable(suggestion.data(), is_selected)) {
                        strncpy(buf, suggestion.data(), buf_size);
                        buf[buf_size - 1] = '\0'; // Ensure null-termination
                        value_changed = true;
                        selected_index = -1; // Reset selection after choosing
                    }
                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
            }
            ImGui::EndTooltip();

            // Handle keyboard navigation
            if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
                selected_index = (selected_index + 1) % suggestions.size();
            } else if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
                selected_index = (selected_index - 1 + suggestions.size()) % suggestions.size();
            }
        } else {
            selected_index = -1; // Reset selection when input is not active
            SetKeyboardNavigationEnabled(true);
        }

        return value_changed;
    }


 
    ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer"){

    }

    ImGuiLayer::~ImGuiLayer(){

    }

    void ImGuiLayer::OnAttach(){

        // Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
        
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    
    }


    static void HelpMarker(const char* desc){
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    static void PushStyleCompact()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, (float)(int)(style.FramePadding.y * 0.60f));
        ImGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, (float)(int)(style.ItemSpacing.y * 0.60f));
    }

    
    static void PopStyleCompact()
    {
        ImGui::PopStyleVar(2);
    }


    void ImGuiLayer::OnImGuiRender(){
        static bool show = true;
        static char str0[128] = "opc.tcp://192.168.0.55:4840";

        ImGui::ShowDemoWindow(&show);

        ImGui::SeparatorText("General");

        static int clicked = 0;
        if (ImGui::Button("Button")){
            clicked++;
            Mir::Application::Get().CreateOpcUaClient(str0);
        }

        ImGui::SameLine();

        // Example suggestions
        std::vector<std::string_view> suggestions = {
            "opc.tcp://192.168.0.55:4840",
            "opc.tcp://192.168.0.56:4840",
            "opc.tcp://192.168.0.57:4840"
        };



        
        //ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));
        ImGui::SameLine(); HelpMarker(
        "USER:\n"
        "Hold SHIFT or use mouse to select text.\n"
        "CTRL+Left/Right to word jump.\n"
        "CTRL+A or Double-Click to select all.\n"
        "CTRL+X,CTRL+C,CTRL+V clipboard.\n"
        "CTRL+Z,CTRL+Y undo/redo.\n"
        "ESCAPE to revert.\n\n");
    

        if (ImGui::Button("Save dummy data")){
            clicked++;
            Mir::Application::Get().GetBrParser().writeDummyData();
            
        }    

        if (clicked & 1)
        {
            ImGui::Text(str0);
            ImGui::Text(str0);
            ImGui::Text(str0);
            ImGui::Text(str0);
            ImGui::Text(str0);
        }

        
        if (InputTextWithSuggestions("input text", str0, IM_ARRAYSIZE(str0), suggestions)) {
            // Handle input change if needed
        }
 
        
        if (ImGui::TreeNode("Borders, background"))
        {
            // Expose a few Borders related flags interactively
            enum ContentsType { CT_Text, CT_FillButton };
            static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
            static bool display_headers = false;
            static int contents_type = CT_Text;

            PushStyleCompact();
            ImGui::CheckboxFlags("ImGuiTableFlags_RowBg", &flags, ImGuiTableFlags_RowBg);
            ImGui::CheckboxFlags("ImGuiTableFlags_Borders", &flags, ImGuiTableFlags_Borders);
            ImGui::SameLine(); HelpMarker("ImGuiTableFlags_Borders\n = ImGuiTableFlags_BordersInnerV\n | ImGuiTableFlags_BordersOuterV\n | ImGuiTableFlags_BordersInnerH\n | ImGuiTableFlags_BordersOuterH");
            ImGui::Indent();

            ImGui::CheckboxFlags("ImGuiTableFlags_BordersH", &flags, ImGuiTableFlags_BordersH);
            ImGui::Indent();
            ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuterH", &flags, ImGuiTableFlags_BordersOuterH);
            ImGui::CheckboxFlags("ImGuiTableFlags_BordersInnerH", &flags, ImGuiTableFlags_BordersInnerH);
            ImGui::Unindent();

            ImGui::CheckboxFlags("ImGuiTableFlags_BordersV", &flags, ImGuiTableFlags_BordersV);
            ImGui::Indent();
            ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuterV", &flags, ImGuiTableFlags_BordersOuterV);
            ImGui::CheckboxFlags("ImGuiTableFlags_BordersInnerV", &flags, ImGuiTableFlags_BordersInnerV);
            ImGui::Unindent();

            ImGui::CheckboxFlags("ImGuiTableFlags_BordersOuter", &flags, ImGuiTableFlags_BordersOuter);
            ImGui::CheckboxFlags("ImGuiTableFlags_BordersInner", &flags, ImGuiTableFlags_BordersInner);
            ImGui::Unindent();

            ImGui::AlignTextToFramePadding(); ImGui::Text("Cell contents:");
            ImGui::SameLine(); ImGui::RadioButton("Text", &contents_type, CT_Text);
            ImGui::SameLine(); ImGui::RadioButton("FillButton", &contents_type, CT_FillButton);
            ImGui::Checkbox("Display headers", &display_headers);
            ImGui::CheckboxFlags("ImGuiTableFlags_NoBordersInBody", &flags, ImGuiTableFlags_NoBordersInBody); ImGui::SameLine(); HelpMarker("Disable vertical borders in columns Body (borders will always appear in Headers");
            PopStyleCompact();

            if (ImGui::BeginTable("table1", 3, flags))
            {
                // Display headers so we can inspect their interaction with borders
                // (Headers are not the main purpose of this section of the demo, so we are not elaborating on them now. See other sections for details)
                if (display_headers)
                {

                    
                    ImGui::TableSetupColumn("One");
                    ImGui::TableSetupColumn("Two");
                    ImGui::TableSetupColumn("Three");
                    ImGui::TableHeadersRow();
                }

                auto& brData = Mir::Application::Get().GetBrParser().getMutable();
                std::vector<std::array<char, 1000>> buffers(brData.size()); // not efficient buffer
                
                size_t index = 0;
                for (auto& data : brData) {
                    // Initialize the buffer with the string data
                    strncpy(buffers[index].data(), data.name.c_str(), buffers[index].size());
                    buffers[index][buffers[index].size() - 1] = '\0'; // Ensure null-termination
                
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);

                    ImGui::PushItemWidth(-1);
                    if (ImGui::InputText(("##DataName" + std::to_string(index)).c_str(), buffers[index].data(), buffers[index].size())) {
                        data.name = std::string(buffers[index].data()); // Update the original string with the modified buffer
                    }
                    ImGui::PopItemWidth();

                    ImGui::TableSetColumnIndex(1);
                    ImGui::TextUnformatted(brDatatypeToString(data.type));
                    ImGui::TableSetColumnIndex(2);
                    ImGui::TextUnformatted(data.comment.c_str());
                
                    ++index;
                }
                ImGui::EndTable();



   
                
            }
            
            ImGui::TreePop();

        }
    }


    void ImGuiLayer::Begin(){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End(){
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
        
        //rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        } 
    }
} 