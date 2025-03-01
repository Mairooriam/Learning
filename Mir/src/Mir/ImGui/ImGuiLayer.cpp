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

    // Store the original GLFW error callback
    GLFWerrorfun g_PreviousErrorCallback = nullptr;

    // Custom error callback that filters clipboard errors
    void CustomGLFWErrorCallback(int error, const char* description) 
    {
        // Filter out clipboard conversion errors (code 65545)
        if (error == 65545 && strstr(description, "Failed to convert clipboard to string") != nullptr) {
            // Silently ignore this specific error
            return;
        }
        
        // Forward all other errors to the previous callback if it exists
        if (g_PreviousErrorCallback) {
            g_PreviousErrorCallback(error, description);
        }
    }



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

        // disables clipboard error msg GLFW ERROR 65545
        g_PreviousErrorCallback = glfwSetErrorCallback(CustomGLFWErrorCallback);


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

        // Display clipboard content
        ImGui::SeparatorText("Clipboard Content");
        MirUI::multilineTextClipboard();         
        ImGui::Separator();


        brTyp& data = Mir::Application::Get().GetBrParser().getData();
                
        // 1. Header with dirty indicator
        ImGui::SeparatorText(data.m_isDirty ? "Type Editor *" : "Type Editor");
        // Add button to show cached string in a window
        static bool showTypeData = false;
        if (ImGui::Button("Show Type Data")) {
            showTypeData = true;
        }

        // Create a separate window for the cached string
        if (showTypeData) {
            ImGui::Begin("Type Data Viewer", &showTypeData);
            ImGui::TextWrapped("%s", data.getCachedString().c_str());
            ImGui::End();
        }

        
        // Collection Tree Loop
        for (size_t i = 0; i < data.size(); i++)
        {

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 1.0f));
            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed;
            std::string label = "Collection" + std::to_string(i) + "\t\t"  + data.collections[i].comment + "###Collection" + std::to_string(i) + data.collections[i].comment;
            bool treeOpen = ImGui::TreeNodeEx(label.c_str(), node_flags, "Collection %d\t\t %s",i, data.collections[i].comment.c_str());
            ImGui::PopStyleVar();

            bool renameOpen = false;
                // Context Menu for each Tree
                if (ImGui::BeginPopupContextItem(("collection_context_menu_" + std::to_string(i)).c_str())) {
                    if (ImGui::MenuItem(("Delete##collection" + std::to_string(i)).c_str())) {
                        data.deleteCollectionAt(i);
                        ImGui::EndPopup();
                        if (treeOpen) {
                            ImGui::TreePop();
                        }
                        continue;  // Skip to next iteration instead of returning
                    }
                                
                    if (ImGui::MenuItem(("Duplicate##collection" + std::to_string(i)).c_str())) {
                        data.duplicateCollectionAt(i);
                        
                    }
                    
                    if (ImGui::MenuItem(("Export##collection" + std::to_string(i)).c_str())) {
                        // Handle export operation
                        // For example: data.exportType(i);
                    }
                    
                    ImGui::Separator();
                    
                    if (ImGui::MenuItem(("Rename##collection" + std::to_string(i)).c_str())) {
                        renameOpen = true;
                    }

                    ImGui::EndPopup();

                  
                }
                // Render the rename popup
                if (renameOpen) {
                    ImGui::OpenPopup(("Rename Collection##" + std::to_string(i)).c_str());
                    ImVec2 mousePos = ImGui::GetMousePos();
                    ImGui::SetNextWindowPos(ImVec2(mousePos.x - 100, mousePos.y - 50)); // Offset 100 pixels left and 50 up
                    renameOpen = false; // Reset flag after opening popup
                }
           
                if (ImGui::BeginPopupModal(("Rename Collection##" + std::to_string(i)).c_str(), nullptr, 
                        ImGuiWindowFlags_AlwaysAutoResize | 
                        ImGuiWindowFlags_NoSavedSettings)) {
                    //static char tempName[256];
                    // if (ImGui::IsWindowAppearing()) {
                    //     strcpy(tempName, data.collections[i].comment.c_str());
                    // }

                    ImGui::Text("Enter new name:");
                    if (ImGui::IsWindowAppearing()) {
                        ImGui::SetKeyboardFocusHere();
                    }
                    if (ImGui::InputText("##rename_input", &data.collections[i].comment, 
                        ImGuiInputTextFlags_EnterReturnsTrue | 
                        ImGuiInputTextFlags_AutoSelectAll)) {
                        data.m_isDirty = true; 
                        ImGui::CloseCurrentPopup();
                    }

                    if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
                        data.m_isDirty = true;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel")) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

            if(treeOpen){
                // Sturcts tree node creation
                for (size_t j = 0; j < data.collections[i].structs.size(); j++) {

                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 1.0f));
                    std::string structLabel = "Struct" + std::to_string(j) + "\t\t"  + data.collections[i].structs[j].name + "###Struct" + std::to_string(j) + data.collections[i].structs[j].name;
                    bool structOpen = ImGui::TreeNodeEx(structLabel.c_str(), node_flags, "Struct %d\t\t %s", j, data.collections[i].structs[j].name.c_str());
                    ImGui::PopStyleVar();

                    // Context menu for Structs tree
                    if (ImGui::BeginPopupContextItem(("struct_context_menu_" + std::to_string(i) + "_" + std::to_string(j)).c_str())) {
                        if (ImGui::MenuItem(("Delete##struct_" + std::to_string(i) + "_" + std::to_string(j)).c_str())) {
                            if (structOpen) {
                                ImGui::TreePop();
                            }
                            data.deleteStructAt(i,j);
                            ImGui::EndPopup();
                            continue;  
                        }         
                        if (ImGui::MenuItem(("Duplicate##struct_" + std::to_string(i) + "_" + std::to_string(j)).c_str())) {
                            data.duplicateStructAt(i,j);
                            
                        }
                        ImGui::Separator();
                        if (ImGui::MenuItem(("Rename##struct_" + std::to_string(i) + "_" + std::to_string(j)).c_str())) {
                            // Add rename stufff later
                        }
                        ImGui::EndPopup();
                    }


                    static float nameWidth = 400.0f;
                    static float typeWidth = 130.0f;
                    static float valueWidth = 70.0f;
                    // if struct tree open ->> make tables
                    if (structOpen)
                    {
                        // Display current width info
                        float availWidth = ImGui::GetContentRegionAvail().x;
                        ImGui::Text("Available width: %.1f", availWidth);

                        static ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_NoBordersInBodyUntilResize | ImGuiTableFlags_Resizable;
                        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0.0f, 2.0f));
                        if (ImGui::BeginTable("Node", 4, tableFlags)) {
                            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, nameWidth);
                            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, typeWidth);
                            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, valueWidth);
                            ImGui::TableSetupColumn("Comment", ImGuiTableColumnFlags_WidthStretch);
                            ImGui::TableHeadersRow();


                            ImGui::TableNextRow();
                            for (int column = 0; column < 4; column++) {
                                ImGui::TableSetColumnIndex(column);
                                ImGui::Text("(w: %5.1f)", ImGui::GetColumnWidth(column));
                            }
                            

                            // Values table Creation
                            for (size_t k = 0; k < data.collections[i].structs[j].values.size(); k++) {
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn(); 
                                std::string nameLbl = "##name" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k);
                                ImGui::PushItemWidth(-FLT_MIN);
                                ImGui::InputText(nameLbl.c_str(), &data.collections[i].structs[j].values[k].name);
                                ImGui::PopItemWidth();
                                ImGui::TableNextColumn();

                                
                                
                                
                                static char searchBuffers[1000][64] = {};  // Support up to 1000 total cells
                                static bool comboFocused[1000] = {};
                                static int selectedIndices[1000] = {};
                                static bool hasAutoCompleted[1000] = {};
                                std::string typeLbl = "##type" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k);
                                auto& currentType = data.collections[i].structs[j].values[k].type;

                                int stateIndex = (i * 100 * 100) + (j * 100) + k;  // Create unique index for i,j,k combination
                                char* searchBuffer = searchBuffers[stateIndex % 1000]; 
                                bool& focused = comboFocused[stateIndex % 1000];
                                int& selectedIndex = selectedIndices[stateIndex % 1000];
                                bool& autoCompleted = hasAutoCompleted[stateIndex % 1000];
                                ImGui::PushItemWidth(-FLT_MIN); 
                                if (ImGui::BeginCombo(typeLbl.c_str(), currentType.c_str(), ImGuiComboFlags_HeightLarge)) {
                                    // Add search filter at top of combo
                                    ImGui::PushItemWidth(-1);
                                    if (!focused) {
                                        ImGui::SetKeyboardFocusHere();
                                        focused = true;
                                        strncpy(searchBuffer, currentType.c_str(), sizeof(searchBuffers[0]) - 1);
                                        searchBuffer[sizeof(searchBuffers[0]) - 1] = '\0';

                                        selectedIndex = -1;
                                        autoCompleted = false;
                                    }
                                    
                                    

                                    // Handle keyboard input for auto-completion
                                    if (ImGui::InputText("##search", searchBuffer, sizeof(searchBuffers[0]), 
                                                        ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
                                        // Apply search buffer text directly on Enter
                                        currentType = searchBuffer;
                                        focused = false; // Reset focus state
                                        ImGui::CloseCurrentPopup();
                                    }

                                    bool keyPressed = false;

                                    // TAB KEY HANDLING - FIXED
                                    if (ImGui::IsKeyPressed(ImGuiKey_Tab) && !autoCompleted) {
                                        std::string_view search(searchBuffer);
                                        // First, create a list of visible/filtered suggestions
                                        std::vector<size_t> visibleSuggestions;
                                        for (size_t idx = 0; idx < brDataTypesSuggestions.size(); idx++) {
                                            if (search.empty() || 
                                                brDataTypesSuggestions[idx].find(search) != std::string_view::npos) {
                                                visibleSuggestions.push_back(idx);
                                            }
                                        }
                                        
                                        // Then select the first visible suggestion
                                        if (!visibleSuggestions.empty()) {
                                            selectedIndex = visibleSuggestions[0];
                                            strncpy(searchBuffer, std::string(brDataTypesSuggestions[selectedIndex]).c_str(), 
                                                    sizeof(searchBuffers[0]) - 1);
                                            searchBuffer[sizeof(searchBuffers[0]) - 1] = '\0';
                                            autoCompleted = true;
                                            keyPressed = true;
                                        }
                                    }

                                    ImGui::PopItemWidth();
                                    ImGui::Separator();
                                    
                                    // Show filtered suggestions
                                    std::string_view search(searchBuffer);
                                    bool hasVisibleItems = false;
                                    
                                    for (size_t idx = 0; idx < brDataTypesSuggestions.size(); idx++) {
                                        const auto& type = brDataTypesSuggestions[idx];
                                        
                                        bool match = search.empty();
                                        if (!match) {
                                            std::string lowerSearch(search);
                                            std::string lowerType(type);
                                            
                                            // Convert both strings to lowercase for comparison
                                            std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(),
                                                [](unsigned char c){ return std::tolower(c); });
                                            std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(),
                                                [](unsigned char c){ return std::tolower(c); });
                                            
                                            match = lowerType.find(lowerSearch) != std::string::npos;
                                        }
                                        
                                        if (match) {
                                            const bool is_selected = (selectedIndex == static_cast<int>(idx));
                                            
                                            if (ImGui::Selectable(std::string(type).c_str(), is_selected)) {
                                                currentType = std::string(type);
                                                data.m_isDirty = true;
                                                data.updateCachedString();
                                                ImGui::CloseCurrentPopup();
                                            }
                                            
                                            // Set focus when an item is selected
                                            if (is_selected) {
                                                ImGui::SetItemDefaultFocus();
                                            }
                                            
                                            hasVisibleItems = true;
                                        }
                                    }
                                    ImGui::EndCombo();
                                } 
                                else if (focused) {
                                    // Reset focus state when combo is closed
                                    focused = false;
                                }
                                ImGui::PopItemWidth();
                                
                                ImGui::TableNextColumn();
                                std::string valueLbl = "##value" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k);
                                ImGui::PushItemWidth(-FLT_MIN);  // Make input fill cell width
                                ImGui::InputText(valueLbl.c_str(), &data.collections[i].structs[j].values[k].value);
                                ImGui::PopItemWidth();
                                
                                ImGui::TableNextColumn(); 
                                std::string commentLbl = "##comment" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k);
                                ImGui::PushItemWidth(-FLT_MIN);  // Make input fill cell width
                                ImGui::InputText(commentLbl.c_str(), &data.collections[i].structs[j].values[k].comment);
                                ImGui::PopItemWidth();
                            }
                            ImGui::EndTable();
                            
                        }
                    
                    ImGui::PopStyleVar();
                    ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        }
    
        data.updateCachedString();



        // int node_clicked = -1;
        // for (size_t i = 0; i < data.size(); i++) {
        //     ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed;
        //     std::string label = "Type" + std::to_string(i) + "\t\t"  + data.typ[i].comment + "###Type" + std::to_string(i) + std::to_string(data.id);
        //     bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node\t\t %s", data.typ[i].nodes.data()->name.c_str());

        //     // Handle left-click (original functionality)
        //     if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        //         node_clicked = i;
                
        //     // Add right-click context menu
        //     if (ImGui::BeginPopupContextItem(("context_menu_" + std::to_string(i)).c_str())) {
        //         if (ImGui::MenuItem("Delete")) {
        //             data.deleteIndex(i);
        //         }
                
        //         if (ImGui::MenuItem("Duplicate")) {
        //             brStructCollection copy = data.getIndexData(i);
        //             copy.nodes.data()->name = copy.nodes.data()->name + copy.nodes.data()->name;
        //             data.push_back(copy);
        //         }
                
        //         if (ImGui::MenuItem("Export")) {
        //             // Handle export operation
        //             // For example: data.exportType(i);
        //         }
                
        //         ImGui::Separator();
                
        //         if (ImGui::MenuItem("Rename")) {
        //             // Handle rename operation or open a rename dialog
        //         }
                
        //         ImGui::EndPopup();
        //     }
        //     if (node_open)
        //     {
        //         if (ImGui::BeginTable("Node", 4, ImGuiTableFlags_ContextMenuInBody))
        //         {
        //             ImGui::TableSetupColumn("Name");
        //             ImGui::TableSetupColumn("Type");
        //             ImGui::TableSetupColumn("Value");
        //             ImGui::TableSetupColumn("Comment");
        //             ImGui::TableHeadersRow();


        //         //printing for nodes
        //         for (size_t j = 0; j < data.typ[i].nodes.data()->data.size(); j++) {
        //             ImGui::TableNextRow();
        //             ImGui::TableNextColumn();
        //             std::string nameLbl = "##name" + std::to_string(j) + std::to_string(i) + data.typ[i].nodes.data()->name;
        //             if(ImGui::InputText(nameLbl.c_str(), &data.typ[i].nodes.data()->data[j].name)){
        //             }
                    
                    
                    
                    
        //             ImGui::TableNextColumn();

                

        //             static char searchBuffers[1000][64] = {};  // Support up to 1000 total cells
        //             static bool comboFocused[1000] = {};
        //             static int selectedIndices[1000] = {};
        //             static bool hasAutoCompleted[1000] = {};
        //             std::string typeLbl = "##type" + std::to_string(j) + std::to_string(i) + data.typ[i].nodes.data()->name;
        //             auto& currentType = data.typ[i].nodes.data()->data[j].type;

        //             int stateIndex = i * 100 + j;  // Assuming max 100 rows per table
        //             char* searchBuffer = searchBuffers[stateIndex % 100]; 
        //             bool& focused = comboFocused[stateIndex % 100];
        //             int& selectedIndex = selectedIndices[stateIndex % 100];
        //             bool& autoCompleted = hasAutoCompleted[stateIndex % 100];

        //             if (ImGui::BeginCombo(typeLbl.c_str(), currentType.c_str(), ImGuiComboFlags_HeightLarge)) {
        //                 // Add search filter at top of combo
        //                 ImGui::PushItemWidth(-1);
        //                 if (!focused) {
        //                     ImGui::SetKeyboardFocusHere();
        //                     focused = true;
        //                     strncpy(searchBuffer, currentType.c_str(), sizeof(searchBuffers[0]) - 1);
        //                     searchBuffer[sizeof(searchBuffers[0]) - 1] = '\0';

        //                     selectedIndex = -1;
        //                     autoCompleted = false;
        //                 }
                        
                        

        //                 // Handle keyboard input for auto-completion
        //                 if (ImGui::InputText("##search", searchBuffer, sizeof(searchBuffers[0]), 
        //                                     ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
        //                     // Apply search buffer text directly on Enter
        //                     currentType = searchBuffer;
        //                     data.m_isDirty = true;
        //                     data.updateCachedString();
        //                     focused = false; // Reset focus state
        //                     ImGui::CloseCurrentPopup();
        //                 }

        //                 bool keyPressed = false;

        //                 // TAB KEY HANDLING - FIXED
        //                 if (ImGui::IsKeyPressed(ImGuiKey_Tab) && !autoCompleted) {
        //                     std::string_view search(searchBuffer);
        //                     // First, create a list of visible/filtered suggestions
        //                     std::vector<size_t> visibleSuggestions;
        //                     for (size_t idx = 0; idx < brDataTypesSuggestions.size(); idx++) {
        //                         if (search.empty() || 
        //                             brDataTypesSuggestions[idx].find(search) != std::string_view::npos) {
        //                             visibleSuggestions.push_back(idx);
        //                         }
        //                     }
                            
        //                     // Then select the first visible suggestion
        //                     if (!visibleSuggestions.empty()) {
        //                         selectedIndex = visibleSuggestions[0];
        //                         strncpy(searchBuffer, std::string(brDataTypesSuggestions[selectedIndex]).c_str(), 
        //                                 sizeof(searchBuffers[0]) - 1);
        //                         searchBuffer[sizeof(searchBuffers[0]) - 1] = '\0';
        //                         autoCompleted = true;
        //                         keyPressed = true;
        //                     }
        //                 }

        //                 ImGui::PopItemWidth();
        //                 ImGui::Separator();
                        
        //                 // Show filtered suggestions
        //                 std::string_view search(searchBuffer);
        //                 bool hasVisibleItems = false;
                        
        //                 for (size_t idx = 0; idx < brDataTypesSuggestions.size(); idx++) {
        //                     const auto& type = brDataTypesSuggestions[idx];
                            
        //                     bool match = search.empty();
        //                     if (!match) {
        //                         std::string lowerSearch(search);
        //                         std::string lowerType(type);
                                
        //                         // Convert both strings to lowercase for comparison
        //                         std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(),
        //                             [](unsigned char c){ return std::tolower(c); });
        //                         std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(),
        //                             [](unsigned char c){ return std::tolower(c); });
                                
        //                         match = lowerType.find(lowerSearch) != std::string::npos;
        //                     }
                            
        //                     if (match) {
        //                         const bool is_selected = (selectedIndex == static_cast<int>(idx));
                                
        //                         if (ImGui::Selectable(std::string(type).c_str(), is_selected)) {
        //                             currentType = std::string(type);
        //                             data.m_isDirty = true;
        //                             data.updateCachedString();
        //                             ImGui::CloseCurrentPopup();
        //                         }
                                
        //                         // Set focus when an item is selected
        //                         if (is_selected) {
        //                             ImGui::SetItemDefaultFocus();
        //                         }
                                
        //                         hasVisibleItems = true;
        //                     }
        //                 }
        //                 ImGui::EndCombo();
        //             } 
        //             else if (focused) {
        //                 // Reset focus state when combo is closed
        //                 focused = false;
        //             }

        //             ImGui::TableNextColumn();
        //             std::string valueLbl = "##value" + std::to_string(j) + std::to_string(i) + data.typ[i].nodes.data()->data[j].value;
                    
        //             bool valueModified = ImGui::InputText(valueLbl.c_str(), &data.typ[i].nodes.data()->data[j].value);
        //             ImGui::TableNextColumn();   
        //             std::string commentLbl = "##comment" + std::to_string(j) + std::to_string(i) + data.typ[i].nodes.data()->data[j].name;
        //             bool commentModified = ImGui::InputText(commentLbl.c_str(), &data.typ[i].nodes.data()->data[j].comment);
        //             ImGui::TableNextColumn();
                 
        //             data.updateCachedString();
        //         }

        //             ImGui::EndTable();
        //         }
                
        //         ImGui::TreePop();
        //     }
        // }

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