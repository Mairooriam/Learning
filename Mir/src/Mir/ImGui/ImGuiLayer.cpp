#include "ImGuiLayer.h"
#include "Mirpch.h"

#include <filesystem>




#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
//#include "IconsFontAwesome5/IconsFontAwesome5.h"

#include "Mir/Application.h"

//TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include "ImguiWidgets.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Mir{


    
    std::string test1 = "HELLO WORDL MIRO";
    FileSelection fileselection1;
    FileSelection fileselection2;
    std::string outputFileStr; 
 
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

        // init path to deaulft bad needs rework
        fileselection1.filePath = "C:\\Users\\35850\\Desktop\\repositories\\learning2\\Learning\\Mir\\External\\testdata\\manual_edited.xlsx";

        fileselection2.filePath = "C:\\Users\\35850\\Desktop\\repositories\\learning2\\Learning\\Mir\\External\\testdata\\Luotu.csv";
       

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    
    }



    std::vector<COMDLG_FILTERSPEC> filters = {
        { L"Image Files", L"*.png;*.jpg;*.jpeg;*.bmp" },
        { L"Text Files", L"*.txt;*.doc;*.docx" },
        { L"All Files", L"*.*" },
        { L"br DataType", L"*.typ*" }
    };


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

        if (ImGui::Button("Save dummy data")){
            clicked++;
            //Mir::Application::Get().GetBrParser().writeDummyData();
        }    

        if (clicked & 1)
        {
            ImGui::Text(str0);
            ImGui::Text(str0);
            ImGui::Text(str0);
            ImGui::Text(str0);
            ImGui::Text(str0);
        }
        
        if (MirUI::InputTextWithSuggestions("input text", str0, IM_ARRAYSIZE(str0), suggestions)) {
            // Handle input change if needed
        }

        if(ImGui::Button("Browse###BrowseInput")){
            m_fileDialog.OpenFile(false);
            fileselection1 = m_fileDialog.GetFileSelection();
        }
        ImGui::SameLine();
        ImGui::InputText("Input Path", &fileselection1.filePath);

        if(ImGui::Button("Browse###BrowseOutput")){
            m_fileDialog.OpenFile(false, filters);
            fileselection2 = m_fileDialog.GetFileSelection();
        }
        ImGui::SameLine();
        ImGui::InputText("Output Path", &fileselection2.filePath);

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

        brTyp& data = Mir::Application::Get().GetBrParser().getData();
                
        // 1. Header with dirty indicator
        ImGui::SeparatorText(data.m_isDirty ? "Type Editor *" : "Type Editor");

        // 2. Content
        for (size_t i = 0; i < data.size(); i++) {
            auto& collection = data.typ[i];
            if (ImGui::InputText(("Comment##collection" + std::to_string(i)).c_str(), &collection.comment)) {
            data.m_isDirty = true;
            data.updateCachedString();
            }
        }

        for (size_t i = 0; i < data.typ.data()->nodes.size(); i++) {
            auto& nodes = data.typ.data()->nodes;
            if (ImGui::InputText(("name##node" + std::to_string(i)).c_str(), &nodes[0].name)) {
            data.m_isDirty = true;
            data.updateCachedString();
            }
        }

        for (size_t i = 0; i < data.typ.data()->nodes.size(); i++) {
            auto& nodes = data.typ.data()->nodes;
            if (ImGui::InputText(("data##data" + std::to_string(i)).c_str(), &nodes[0].data[0].name)) {
            data.m_isDirty = true;
            data.updateCachedString();
            }
        }

        ImGui::Text("%s", data.getCachedString().c_str());
        

        for (size_t i = 0; i < data.size(); i++) {
            std::string label = "Type" + std::to_string(i) + "\t\t"  + data.typ[i].comment + "###Type" + std::to_string(i) + data.typ[i].comment;

            

            if (ImGui::TreeNode(label.c_str()))
            {
                if (ImGui::BeginTable("Node", 4)){
                    ImGui::TableSetupColumn("Name");
                    ImGui::TableSetupColumn("Type");
                    ImGui::TableSetupColumn("Value");
                    ImGui::TableSetupColumn("Comment");
                    ImGui::TableHeadersRow();


                //printing for nodes
                for (size_t i = 0; i < data.typ.data()->nodes.data()->data.size(); i++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    std::string nameLbl = "##name" + std::to_string(i);
                    bool nameModified = ImGui::InputText(nameLbl.c_str(), &data.typ.data()->nodes.data()->data[i].name);
                    ImGui::TableNextColumn();
                    std::string typeLbl = "##type" + std::to_string(i);
                    bool typeModified = ImGui::InputText(typeLbl.c_str(), &data.typ.data()->nodes.data()->data[i].type);
                    ImGui::TableNextColumn();
                    std::string valueLbl = "##value" + std::to_string(i);
                    bool valueModified = ImGui::InputText(valueLbl.c_str(), &data.typ.data()->nodes.data()->data[i].value);
                    ImGui::TableNextColumn();
                    std::string commentLbl = "##comment" + std::to_string(i);
                    bool commentModified = ImGui::InputText(commentLbl.c_str(), &data.typ.data()->nodes.data()->data[i].comment);
                    ImGui::TableNextColumn();

                    if (nameModified || typeModified || valueModified || commentModified) {
                        data.updateCachedString();
                    }
                }

                    ImGui::EndTable();
                }
                
                ImGui::TreePop();
            }
        }

        // 2. Content

        
        
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