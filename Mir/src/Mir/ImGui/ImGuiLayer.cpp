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

#include "imgui_internal.h"

#include "styles.h"
#include "IconsFontAwesome6.h"

#include <tinyxml2.h>

#include "ImGuiTableWidget.h"
#include "b&rParser/classTypes.h"
// TODO:
/*
- CTRL + C -> CTRL + V WHEN NAVIGATING WITH ARROWS
- ADD NEW MEMBER TO STRUCT -> RIGHT CLICK ON STRCUT? BUTTON?
-
- FORMAT VALIDATION WHEN TYPING NAMES -> NO SPACES 
- FORMAT VALIDATION WHEN TYPING KNOWN TYPES -> BOOL 1 = TRUE 0 = FALSE ETC.
- HOTKEY DISPLAY ON RIGHT CLICK MENU?
- FIX IOM STYLING TO MATCH TYP
- Redo icon based buttons as image buttons
- Undo/redo for maybe 2 steps
- When using trashcan delete have it take next available selectable item of same kind
- Move filebrowser into widgets. make it reusable. also refine implementation of it
- add Table that takes xml table recusivly? or some other way.
- Test read xml
- Read available IO cards. -> dropdown menu on which to add struct to.
- Search for B&R cards directory -> ability to add cards to config.
- iom display red if duplicate IO adress / names
- When opening specific Struct "AF101 " -> opens corresponding IOM.ABC
- If struct contains multiple of same IO dont make .iom on read -> error popup -> show mismatch -> have "generate .iom"

- CLEAN TCODE
- STYLING
- WINDOWS FILE EXPLORER FILE TYPE INTEGRATION
- COPY FOR VAR CONFIG SINGLE STRINGS? MAYBE NOT?
- OPENGL WINDOW TITLE BAR TO DARK MODE??
- STYLES FROM STRUCT TABLE TO VAR CONFIG
- CTRL + C COPY FOR KEYBOARD NAVIGATION?
- AS STUDIO PROJECT OPEN IN THE LEFT?

*/



namespace Mir{


    bool shouldIgnoreModule(const char* namePtr) {
        if (!namePtr) return false;
        
        std::string_view name(namePtr);
        static const std::array<std::string_view, 6> ignorePrefixes = {
            "X20TB12", "X20BM11", "X20cBM11", "X20BB67", "X20CP", "X20PS"
        };
        
        for (const auto& prefix : ignorePrefixes) {
            if (name.size() >= prefix.size() && name.substr(0, prefix.size()) == prefix) {
                return true;
            }
        }
        return false;
    }


    
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
    static ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg;
    float temptime = 0; 
    bool hightlight = false;
    selectableElement m_LastSelectableElement;
 
    ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer"){

    }

    ImGuiLayer::~ImGuiLayer(){

    }

    void ImGuiLayer::OnAttach(){

        MirSettings m_Settings;

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        // disables clipboard error msg GLFW ERROR 65545
        g_PreviousErrorCallback = glfwSetErrorCallback(CustomGLFWErrorCallback);


        Mir::SetStyle(m_currentStyle);
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        /////////////// FONTS ///////////////
        // Font selection at compile time with a define
        #define USE_ROBOTO_FONT 0  // Set to 0 to use ImGui default font instead
        
        ImFont* baseFont = nullptr;
        
        #if USE_ROBOTO_FONT
            // 1. Try to load Roboto font
            std::string robotoPath = "C:/Users/35850/Desktop/repositories/learning2/Learning/Mir/Assets/Fonts/Roboto-Regular.ttf";
            baseFont = io.Fonts->AddFontFromFileTTF(robotoPath.c_str(), 16.0f);
            
            // Fall back to default if Roboto failed to load
            if (!baseFont) {
            MIR_WARN("Failed to load Roboto font, falling back to default");
            baseFont = io.Fonts->AddFontDefault();
            }
        #else
            // Use ImGui default font
            baseFont = io.Fonts->AddFontDefault();
        #endif
        
        // 2. Setup icon font size and config
        float baseFontSize = 16.0f; // Base font size
        float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome icons scale
        
        // 3. Merge Font Awesome icons into the base font
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
        ImFontConfig icons_config; 
        icons_config.MergeMode = true; // Important: This merges with the previous font
        icons_config.PixelSnapH = true; 
        icons_config.GlyphMinAdvanceX = iconFontSize;
        std::string fontPath = "C:/Users/35850/Desktop/repositories/learning2/Learning/Mir/Assets/Fonts/";
        fontPath += FONT_ICON_FILE_NAME_FAS;
        
        io.Fonts->AddFontFromFileTTF(fontPath.c_str(), iconFontSize, &icons_config, icons_ranges);
        /////////////// FONTS ///////////////
        
    

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
        static bool dooockkk = true;
        MirUI::ShowExampleAppDockSpace(&dooockkk);
        

        ////////////////////////////////////////
        //////// READ XML TEST//////////////////
        ////////////////////////////////////////
        ImGui::Begin("Read XML");
        if (ImGui::Button("Read"))
        {
            tinyxml2::XMLDocument doc;
            doc.LoadFile("C:\\projects\\OpcUa_Sample\\Physical\\ArSim\\Hardware.hw");
            
            // Get the Hardware root element
            tinyxml2::XMLElement* hardwareElement = doc.FirstChildElement("Hardware");
            if (hardwareElement)
            {
                // Loop through all Module elements
                tinyxml2::XMLElement* moduleElement = hardwareElement->FirstChildElement("Module");
                while (moduleElement)
                {
                    // Get module attributes
                    const char* name = moduleElement->Attribute("Name");
 
                    
                    
                    const char* type = moduleElement->Attribute("Type");
                    if (shouldIgnoreModule(type)) {  
                        moduleElement = moduleElement->NextSiblingElement("Module");
                        continue;
                    }   
                    const char* version = moduleElement->Attribute("Version");
                    
                    printf("Module: Name=%s, Type=%s, Version=%s\n", 
                           name ? name : "Unknown",
                           type ? type : "Unknown", 
                           version ? version : "Unknown");
                    
                    moduleElement = moduleElement->NextSiblingElement("Module");
                }
            }
            else
            {
                printf("Error: Could not find Hardware element\n");
            }


            

        }

        std::vector<std::string> dropdownOptions = {
            "Option 1", 
            "Option 2", 
            "Option 3",
            "Hello World",
            "Test String 1",
            "Test String 2"
        };
        static StringValue testStringvalue = StringValue();
        static StringValue testStringvalue2 = StringValue();
        
        
        // Create the settings
        BaseTableSettings settings("My Table23222");
        settings.WithColumn("Name")
                .WithColumn("Age")
                .WithDropdownColumn("Gender", {"Male", "Female", "Other"})
                .WithColumn("Notes");

        // Create the table
        static BaseTable myTable(settings);
        static StructTable structTable("Variables");

        static int hihi = 0;
        if (hihi == 0)
        {
            myTable.AddRow({"Test1", "test2", "test3", "test4"});
            structTable.AddRow({"speed", "float", "0.0", "Speed in m/s"});
            structTable.AddRow({"isActive", "bool", "true", "Activation status"});
            structTable.AddRow({"counter", "int", "42", "Loop counter"});
            structTable.AddRow({"name", "string", "\"Motor\"", "Device identifier"});
            hihi++;
        }
       
        structTable.Render();
        myTable.Render();
    //     static brStruct testStruct = brStruct("test sturct 1");
    //     static brStruct testStruct2 = brStruct("test sturct 2");
    //     static int hihi = 0;
    //     if (hihi < 10)
    //     {
    //         testStruct.push_back("Test1", "test2", "test3", "test4");
    //         hihi++;
    //     }
        
    //     if (ImGui::Button("Delete value at index"))
    //     {
    //         testStruct.deleteAt(4);
    //     }
    //     static std::vector<StringValue> testbuf;
    //     if (ImGui::Button("copy at index 0"))
    //     {
    //         testbuf = testStruct.copyAtIndex(0);
    //     }
    //     if (ImGui::Button("Paste to index last"))
    //     {
    //         testStruct.pasteToIndex(1000,testbuf);
    //     }
    //     testStruct.SetTypeSuggestions(dropdownOptions);
    //     testStruct2.SetTypeSuggestions(dropdownOptions);
    //     MIR_INFO("testStringvalue ID: {0}", testStringvalue.GetID());
    //     MIR_INFO("testStringvalue2 ID: {0}", testStringvalue2.GetID());
    //     MIR_INFO("testStruct ID: {0}", testStruct.GetID());
    //     MIR_INFO("testStruct IDs: {0}", testStruct.GetAllIDsAsString());
    //     testStruct.Render();
    //     testStruct2.Render();
    //     // Add dropdown to select render type
    //     static int currentRenderType = 0;
    //     const char* renderTypes[] = { "Default", "Input", "MultiLine", "Dropdown"};
        
    //     ImGui::SetNextItemWidth(150.0f);
    //     if (ImGui::Combo("Render Type", &currentRenderType, renderTypes, IM_ARRAYSIZE(renderTypes))) {
    //         // Set render type based on selection
    //         switch (currentRenderType) {
    //         case 0: 
    //             testStringvalue.SetRenderType(StringValue::RenderType::Default); 
    //             testStringvalue2.SetRenderType(StringValue::RenderType::Default); 
    //         break;
    //         case 1: 
    //             testStringvalue.SetRenderType(StringValue::RenderType::Input); 
    //             testStringvalue2.SetRenderType(StringValue::RenderType::Input); 
    //             break;
    //         case 2: 
    //             testStringvalue.SetRenderType(StringValue::RenderType::MultiLine);
    //             testStringvalue2.SetRenderType(StringValue::RenderType::MultiLine);  
    //             break;
    //         case 3: 
    //             testStringvalue.SetRenderType(StringValue::RenderType::Dropdown);
    //             testStringvalue2.SetRenderType(StringValue::RenderType::Dropdown);  

    //             testStringvalue.SetSuggestions(dropdownOptions);
    //             testStringvalue2.SetSuggestions(dropdownOptions);
    //         break;
    //         }
    //     }
        
    // testStringvalue.Render();
    // testStringvalue2.Render();



        brTyp myTypData;
        brStructData testdata = brStructData("TestName", "TestType", "testValue", "TestComment");
        std::vector<brStructData> testDataVector;
        testDataVector.push_back(testdata);
        brStructNode testNode = brStructNode("SturctTestname", testDataVector);
        std::vector<brStructNode> testSturctVector;
        testSturctVector.push_back(testNode);
        myTypData.collections.push_back(brStructCollection("CollectionTestName",testSturctVector));
        static brVarConfigCollection myConfigData;
        static int test2 = 2;
        // Create and configure typed table widgets
        static Mir::Widgets::TableWidget<brTyp> typTable("BR Type Table", 3);
        static Mir::Widgets::TableWidget<brVarConfigCollection> configTable("Var Config Table", 3);
        static Mir::Widgets::TableWidget<int> intTable("Var Config Table", 3);

        // Set data pointers
        typTable.SetData(&myTypData);
        configTable.SetData(&myConfigData);
        intTable.SetData(&test2);
        
        // Optional: Configure table settings
        typTable.GetSettings()
            .ShowHeaderWidth(false)
            .ShowLineNumbers(true);

        configTable.GetSettings()
            .ShowHeaderWidth(true)
            .ShowLineNumbers(false);
        
        if (ImGui::CollapsingHeader("BR Type Data")) {
            typTable.Render();
        }
        
        ImGui::Separator();
        
        if (ImGui::CollapsingHeader("Var Config Data")) {
            configTable.Render();
        }

        if (ImGui::CollapsingHeader("int test")) {
            intTable.Render();
        }

        ImGui::End();
        ////////////////////////////////////////
        //////// READ XML TEST//////////////////
        ////////////////////////////////////////



        ////////////////////////////////////////
        //////// ICONS EXAMPLE  ////////////////
        ////////////////////////////////////////
        if (ImGui::Begin("Icons Example")) {
            ImGui::Text("%s File Operations", ICON_FA_FOLDER);
            
            if (ImGui::Button(ICON_FA_SACK_DOLLAR " Save File")) {
                // Save file logic
            }
            
            if (ImGui::Button(ICON_FA_FOLDER_OPEN " Open")) {
                // Open file logic
            }
            
            if (ImGui::Button(ICON_FA_TRASH " Delete")) {
                // Delete file logic
            }
            
            // Display a grid of icons
            ImGui::Separator();
            ImGui::Text("Icon Gallery:");
            
            const char* icons[] = {
                ICON_FA_ADDRESS_BOOK, ICON_FA_ARROW_RIGHT, ICON_FA_BELL, ICON_FA_BOOKMARK,
                ICON_FA_CALENDAR, ICON_FA_CAMERA, ICON_FA_CHART_LINE, ICON_FA_CIRCLE_INFO, 
                ICON_FA_DATABASE, ICON_FA_DOWNLOAD, ICON_FA_ENVELOPE, ICON_FA_FILE
            };
            
            for (int i = 0; i < IM_ARRAYSIZE(icons); i++) {
                ImGui::PushID(i);
                if (ImGui::Button(icons[i], ImVec2(40, 40))) {
                    // Copy to clipboard or perform action
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Icon %d", i);
                ImGui::PopID();
                
                if ((i + 1) % 4 != 0)
                    ImGui::SameLine();
            }
            
            ImGui::End();
        }
        ////////////////////////////////////////
        //////// ICONS EXAMPLE  ////////////////
        ////////////////////////////////////////



        // Create a style selector in your UI
        if (ImGui::Begin("Settings")) {
            // Display dropdown for style selection
            const char* currentStyleName = Mir::GetStyleName(m_currentStyle);
            if (ImGui::BeginCombo("Theme", currentStyleName)) {
                for (int i = 0; i < Mir::GetStyleCount(); i++) {
                    bool isSelected = (m_currentStyle == i);
                    if (ImGui::Selectable(Mir::GetStyleName(i), isSelected)) {
                        m_currentStyle = i;
                        Mir::SetStyle(m_currentStyle);
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::Checkbox("Display Header Width", &m_Settings.showHeaderWidth);
            ImGui::CheckboxFlags("Table Row Highlight", &tableFlags, ImGuiTableFlags_RowBg);
            ImGui::End();
        }


        if (hightlight)
        {
            ImGui::PushStyleColor(ImGuiCol_TabActive, IM_COL32(0xff, 0x00, 0xff, 0xff));

        }



        brParser& brparser = Mir::Application::Get().GetBrParser();
        brTyp& data = Mir::Application::Get().GetBrParser().getData();
         
        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        


        ImGui::Begin("Fileeeee");

        static int currentItem = 0;
        const char* items[] = { ".csv", ".typ", ".iom"};
        static COMDLG_FILTERSPEC filter = {L"br DataType", L"*.csv*"};
        static FileSelection selectedFile;  // Default initialization





        ImGui::SeparatorText("Browse path");
        if (ImGui::InputText("##browseField", &selectedFile.filePath)) {}
        ImGui::SameLine();
        if (ImGui::Button("Browse")) {
            std::vector<COMDLG_FILTERSPEC> filterVec = { filter };
            m_fileDialog.OpenFile(false, filterVec);
            selectedFile = m_fileDialog.GetFileSelection();
        }
        ImGui::Separator();

        ImGui::SetNextItemWidth(100.0f); 
        if (ImGui::Combo("File type", &currentItem, items, IM_ARRAYSIZE(items))) {
            if (currentItem == 0)
            {
            filter = {L"br DataType", L"*.csv*"};
            selectedFile.clear();
            }
            if (currentItem == 1)
            {
            filter ={L"br DataType", L"*.typ*"};
            selectedFile.clear();
            } 
            if (currentItem == 2)
            {
            filter ={L"br Mapping", L"*.iom*"};
            selectedFile.clear();
            } 
        }

        if (ImGui::Button("Read file")){ 
            if (selectedFile.fileName != "") {
                if (currentItem == 0) {
                    auto csvData = brparser.readPlcDataCsv(selectedFile.filePath, "Location,Type,BR Name,Card,Eplan name");
                    brStructCollection col =brparser.parseCsvIntoBrCollection(csvData);
                    brparser.addCollection(col);
                    
                    std::vector<brVarConfigNode> parsedConfig = brparser.parseCollectionToConfigMap(col);
                    for (auto &&config : parsedConfig)
                    {
                        brparser.addVarConfigNode(config);
                    }
                    
                } 
                else if (currentItem == 1) {
                    brparser.clear(); //BUG might cause bug or unwanted behavior not usre
                    brTyp data = brparser.readDatafile999999(selectedFile.filePath);
                
                    brparser.setData(data);
                    brparser.getData().markDirty();
                } 
                else if (currentItem == 2) {
                    brparser.clear(); 
                    auto varConfig = brparser.readBrVarConfig(selectedFile.filePath);
                    brparser.setVarConfig(varConfig);
                    brparser.getVarConfig().markDirty();
                }
            }

        }

        ImGui::SameLine();

        if (ImGui::Button("Clear")){
            Mir::Application::Get().GetBrParser().clear();
    
        }

        
        




        std::vector<COMDLG_FILTERSPEC> filters = {
            { L"Image Files", L"*.png;*.jpg;*.jpeg;*.bmp" },
            { L"Text Files", L"*.txt;*.doc;*.docx" },
            { L"All Files", L"*.*" },
            { L"br DataType", L"*.typ*" }
        };

        if(ImGui::Button("Readconfig")){
            brVarConfigCollection resultt = brparser.readBrVarConfig(R"(C:\projects\OpcUa_Sample\Physical\ArSim\X20CP0482\IoMap.iom)");
            ImGui::SetClipboardText(resultt.toString().c_str());
        }
        ImGui::End();


        ////////////////////////////////////////
        //////// CLIPBOARD    //////////////////
        ////////////////////////////////////////
        ImGui::Begin("Clipboard");
        MirUI::multilineTextClipboard();         
        ImGui::End();



       

        ImGui::SetNextWindowSize(ImVec2(200, 200));
        ////////////////////////////////////////
        //////// PARSER DATA TABLE//////////////
        ////////////////////////////////////////
        ImGui::Begin(".typ Editor");
    

        // Make button bigger with custom size
        ImVec2 bigButtonSize(40, 40);
        float oldScale = ImGui::GetIO().FontGlobalScale;
        ImGui::GetIO().FontGlobalScale = 2.0f;

        ImGui::GetIO().FontGlobalScale = oldScale;


        static std::vector<int> selectedStruct(2);
        if (ImGui::Begin("Debug Info")) {
            ImGuiID activeID = ImGui::GetActiveID();
            ImGuiID hoveredID = ImGui::GetHoveredID();
            ImGuiID focusedID = ImGui::GetFocusID();
            
            ImGui::Text("Active ID: %u", activeID);
            ImGui::Text("Hovered ID: %u", hoveredID);
            ImGui::Text("Focus ID: %u", focusedID);
            ImGui::Text("Previous Focus ID: %u", m_PreviousFocus);
            ImGui::Text("Focus Changed: %s", (focusedID != m_PreviousFocus) ? "YES" : "NO");
            ImGui::Text("Selected col: %u, struct: %u", selectedStruct[0], selectedStruct[1]);
            
            if (m_LastSelectableElement.isType(typTypes::Collection))
            {
            CollectionInfo info = std::get<CollectionInfo>(m_LastSelectableElement.data);
            ImGui::Text("[LAST SELECTABLE]: Collection selected i: %u [%s]", info.i, info.name.c_str());
               // std::cout << "printing from if i: " << info.i << "\n";
            } else if (m_LastSelectableElement.isType(typTypes::Struct))
            {
            StructInfo info = std::get<StructInfo>(m_LastSelectableElement.data);
            ImGui::Text("[LAST SELECTABLE]: Struct selected i: %u j: %u [%s]", info.i, info.j, info.name.c_str());
            } else if (m_LastSelectableElement.isType(typTypes::Value))
            {
            ValueInfo info = std::get<ValueInfo>(m_LastSelectableElement.data);
            ImGui::Text("[LAST SELECTABLE]: Value selected i: %u j: %u k: %u [%s]", info.i, info.j, info.k, info.name.c_str());
            }
            ImGui::End();
            if (m_PreviousFocus != focusedID){
                    m_PreviousFocus = focusedID;  // Single history item
                    if (brparser.m_IdMap[focusedID].type != typTypes::None)
                    {
                        m_LastSelectableElement = brparser.m_IdMap[focusedID];
                    }
                    
                    

                        
                  
            }
        }
        
       
        

        selectableElement focusedElement = brparser.getId(ImGui::GetFocusID());
        selectableElement previousfocusedElement = brparser.getId(m_PreviousFocus);
        ImGui::BeginDisabled(!focusedElement.isType(typTypes::None));
        if (ImGui::Button(ICON_FA_EXPLOSION, bigButtonSize)){ 
                brStructCollection tempCol("Added with button");
                brparser.addCollection(tempCol);
        }    
        
        ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::BeginDisabled(!m_LastSelectableElement.isType(typTypes::Collection));
        if (ImGui::Button("2", bigButtonSize)){ 
            CollectionInfo colInfo = *m_LastSelectableElement.GetData<CollectionInfo>();
            brStructNode tempStruct("Added with button");
            brparser.addStructToCol(colInfo.i,tempStruct);

        } ImGui::SameLine();
        ImGui::EndDisabled();

        ImGui::BeginDisabled(!m_LastSelectableElement.isType(typTypes::Struct));
        if (ImGui::Button("3", bigButtonSize)){ 
            brStructData tempData("added with button");
            StructInfo structinfo = *m_LastSelectableElement.GetData<StructInfo>();
            brparser.addDataToStruct(tempData, structinfo.i,structinfo.j);
        } ImGui::SameLine();
        ImGui::EndDisabled();

        if (ImGui::Button("4", bigButtonSize)){ } ImGui::SameLine();
        if (ImGui::Button("5", bigButtonSize)){ } ImGui::SameLine();
        if (ImGui::Button(ICON_FA_TRASH_CAN, bigButtonSize)){ 
            switch (m_LastSelectableElement.type)
            {
            case typTypes::Collection:
                brparser.getData().deleteCollectionAt(m_LastSelectableElement.GetData<CollectionInfo>()->i);
                break;
            case typTypes::Struct: {
                StructInfo structInfo = *m_LastSelectableElement.GetData<StructInfo>();
                brparser.getData().deleteStructAt(structInfo.i, structInfo.j); 
                break;
            }
            case typTypes::Value: {
                ValueInfo valueInfo = *m_LastSelectableElement.GetData<ValueInfo>();
                brparser.getData().deleteValueAt(valueInfo.i, valueInfo.j, valueInfo.k);
                break;
            }
            case typTypes::None:
                break;
            
            default:
                break;
            }
         } 

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 1.0f));
        // Sturcts tree node creation
        for (size_t i = 0; i < data.size(); i++){

            
            std::string label = "Collection" + std::to_string(i) + "\t\t"  + data.collections[i].comment + "###Collection" + std::to_string(i) + data.collections[i].comment;
            bool treeOpen = ImGui::CollapsingHeader(label.c_str());
            
            // ID MAPPING COLLECTIONS
            brparser.m_IdMap[ImGui::GetItemID()] = selectableElement(typTypes::Collection, CollectionInfo{i, data.collections[i].comment});


            ////////////////////////////////////////
            /////////  CONTEXT MENU ////////////////
            ////////////////////////////////////////
            bool renameOpen = false;
            bool exportOpen = false;
            if (ImGui::BeginPopupContextItem(("collection_context_menu_" + std::to_string(i)).c_str())) {
                if (ImGui::MenuItem(("Delete##collection" + std::to_string(i)).c_str())) {
                    data.deleteCollectionAt(i);
                    ImGui::EndPopup();
                    continue;  
                }  
                if (ImGui::MenuItem(("Copy##collection" + std::to_string(i)).c_str())) {
                    brparser.clipboard.copyCollection(data.collections[i], i);
                    ImGui::SetClipboardText(brparser.clipboard.toString().c_str());
                }
                if (ImGui::MenuItem(("Duplicate##collection" + std::to_string(i)).c_str())) { data.duplicateCollectionAt(i); }

                if (ImGui::MenuItem(("Export##collection" + std::to_string(i)).c_str())) { exportOpen = true; }
                if (ImGui::MenuItem(("Generate iom ##collection" + std::to_string(i)).c_str())) { 
                    if (!data.empty())
                    {
                        std::vector<brVarConfigNode> parsedCollection = brparser.parseCollectionToConfigMap(data.collections[i]);
                        for (auto &node : parsedCollection)
                        {
                            brparser.addVarConfigNode(node);
                        }
                        
                        brparser.getVarConfig().markDirty();
                    }
                    

                 }
                
                if (ImGui::MenuItem(("Copy as [.iom]##collection" + std::to_string(i)).c_str())) { 
                    std::string buffer;
                    for (auto &brstruct : data.collections[i])
                    {
                        buffer += brparser.parseSturctToConfig(brstruct).toStringPlain();
                    }
                    ImGui::SetClipboardText(buffer.c_str());
                }
                bool clipboardHasContent = brparser.clipboard.isCollection() || brparser.clipboard.isStruct();
                if (ImGui::MenuItem(("Paste##collection" + std::to_string(i)).c_str(), nullptr, false, clipboardHasContent)) 
                {
                    if (brparser.clipboard.isCollection()) {
                        brparser.clipboard.pasteToCollection(data);
                    } else if (brparser.clipboard.isStruct()) {
                        brparser.clipboard.pasteStructToCollection(data, i); // i is your collection index
                    }
                }
                ImGui::Separator();
                if (ImGui::MenuItem(("Rename##collection" + std::to_string(i)).c_str())) {renameOpen = true;}
                ImGui::EndPopup();
            }
            ////////////////////////////////////////
            ///////// END OF CONTEXT MENU //////////
            ////////////////////////////////////////



            ////////////////////////////////////////
            //////// START OF RENAME POP UP///////////
            ////////////////////////////////////////
            if (renameOpen) {
                ImGui::OpenPopup(("Rename Collection##" + std::to_string(i)).c_str());
                ImVec2 mousePos = ImGui::GetMousePos();
                ImGui::SetNextWindowPos(ImVec2(mousePos.x - 100, mousePos.y - 50)); 
                renameOpen = false; 
            }
            if (ImGui::BeginPopupModal(("Rename Collection##" + std::to_string(i)).c_str(), nullptr, 
                    ImGuiWindowFlags_AlwaysAutoResize)) {

                ImGui::Text("Enter new name:");
                if (ImGui::IsWindowAppearing()) {ImGui::SetKeyboardFocusHere();}
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
            ////////////////////////////////////////
            //////// END OF RENAME POP UP///////////
            ////////////////////////////////////////



            ////////////////////////////////////////
            //////// START OF EXPORT POP UP/////////
            ////////////////////////////////////////
            if (exportOpen) {
                ImGui::OpenPopup(("Export Collection##" + std::to_string(i)).c_str());
                ImVec2 mousePos = ImGui::GetMousePos();
                ImGui::SetNextWindowPos(ImVec2(mousePos.x - 100, mousePos.y - 50)); 
                exportOpen = false; 
            }
            if (ImGui::BeginPopupModal(("Export Collection##" + std::to_string(i)).c_str(), nullptr, 
                    ImGuiWindowFlags_AlwaysAutoResize)) {

                static FileSelection exportFile;

                ImGui::Text("Export collection to file:");
                
                if (ImGui::InputText("##export_path", &exportFile.filePath, 
                    ImGuiInputTextFlags_EnterReturnsTrue)) {
                    // Handle Enter key in path input
                    if (!exportFile.filePath.empty()) {
                        exportOpen = false;
                        ImGui::CloseCurrentPopup();
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Browse")) {
                    std::vector<COMDLG_FILTERSPEC> filterVec = { {L"B&R Type Files", L"*.typ"} };
                    m_fileDialog.SetInitialFileName("myexport.typ");
                    if (m_fileDialog.SaveFile(filterVec, L"typ")) {
                        exportFile = m_fileDialog.GetFileSelection();
                    }
                }
                
                // Add status text or preview if needed
                if (!exportFile.filePath.empty()) {
                    ImGui::TextWrapped("Will export to: %s", exportFile.filePath.c_str());
                }
                
                // Buttons row
                if (ImGui::Button("Export", ImVec2(120, 0)) || 
                    (ImGui::IsKeyPressed(ImGuiKey_Enter) && !exportFile.filePath.empty())) {
                    brparser.writeFile(exportFile.filePath, data.collections[i].toString(), std::ios::out | std::ios::trunc);
                    exportOpen = false;
                    ImGui::CloseCurrentPopup();
                }
                
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0)) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
                    exportOpen = false;
                    ImGui::CloseCurrentPopup();
                }
                
                ImGui::EndPopup();
            }
            ////////////////////////////////////////
            //////// END OF EXPORT POP UP///////////
            ////////////////////////////////////////




            if (treeOpen){
                for (size_t j = 0; j < data.collections[i].structs.size(); j++) {

                    /////////////// STYLE SET STRUCT COLLAPSING HEADER ///////////////
                    float structHeaderIndent = ImGui::GetStyle().IndentSpacing * 0.25f;
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 1.0f));
                    ImGui::Indent(structHeaderIndent); 
                    /////////////// STYLE SET STRUCT COLLAPSING HEADER ///////////////

                    std::string structLabel = "Struct" +  std::to_string(j) + "\t\t"  + data.collections[i].structs[j].name + "###Struct" + "_"+ std::to_string(i) +  "_" + std::to_string(j) + data.collections[i].structs[j].name + data.collections[i].comment;
                    bool structOpen = ImGui::CollapsingHeader(structLabel.c_str());

                    // ID MAPPING STRUCTS
                    brparser.m_IdMap[ImGui::GetItemID()] = selectableElement(typTypes::Struct, StructInfo{i,j, data.collections[i].structs[j].name});



                    /////////////// STYLE POP STRUCT COLLAPSING HEADER ///////////////
                    ImGui::PopStyleVar();
                    ImGui::Unindent(structHeaderIndent);
                    /////////////// STYLE POP STRUCT COLLAPSING HEADER ///////////////

                    // Context menu for Structs tree
                    if (ImGui::BeginPopupContextItem(("struct_context_menu_" + std::to_string(i) + "_" + std::to_string(j)).c_str())) {
                        if (ImGui::MenuItem(("Delete##struct_" + std::to_string(i) + "_" + std::to_string(j)).c_str())) {
                            if (structOpen) {
                                
                            }
                            data.deleteStructAt(i,j);
                            ImGui::EndPopup();
                            continue;  
                        } 
                        if (ImGui::MenuItem(("Copy##struct_" + std::to_string(i) + "_" + std::to_string(j)).c_str())) {
                            brparser.clipboard.copyStruct(data.collections[i].structs[j], i, j);
                            ImGui::SetClipboardText(brparser.clipboard.toString().c_str());
                            MIR_INFO("TEST");
                        }       
                        if (ImGui::MenuItem(("Duplicate##struct_" + std::to_string(i) + "_" + std::to_string(j)).c_str())) {
                            brStructNode copy = data.duplicateStructAt(i,j);
                            ImGui::SetClipboardText(copy.toString().c_str());
                            data.insertStructAt(i,j,copy);
                        }
                        ImGui::Separator();
                        if (ImGui::MenuItem(("Copy as [.iom]##struct_" + std::to_string(i) + "_" + std::to_string(j)).c_str())) {
                            brVarConfigNode node = brparser.parseSturctToConfig(data.collections[i].structs[j]);
                            ImGui::SetClipboardText(node.toString().c_str());
                        }
                        ImGui::EndPopup();
                    }



                    if (structOpen)
                    {
                        static float availWidth = ImGui::GetContentRegionAvail().x;
                        static float percentages[4] = {0.35f, 0.20f, 0.15f, 0.30f};

                        static bool pendingWidthChange = false;
                        static int columnToResize = -1;
                        static float newColumnWidth = 0.0f;
                        static bool resizeAllColumns = false;
                        static bool resetColumnWidth = false;
                                                
                        /////////////// .TYP TABLE STYLES SET ///////////////
                        float tableIndent = ImGui::GetStyle().IndentSpacing * 0.75f;
                        ImGui::Indent(tableIndent);
                        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0.0f, 0.0f));
                        /////////////// .TYP TABLE STYLES SET ///////////////

                        if (ImGui::BeginTable("Node", 4, tableFlags)) {
                            // Setup columns
                            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, percentages[0]);
                            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, percentages[1]);
                            ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, percentages[2]);
                            ImGui::TableSetupColumn("Comment", ImGuiTableColumnFlags_WidthStretch, percentages[3]);
                            ImGui::TableHeadersRow();

                            ImGui::TableNextRow();
                            if (m_Settings.showHeaderWidth){
                                for (int column = 0; column < 4; column++) {
                                    ImGui::TableSetColumnIndex(column);
                                    ImGui::Text("(w: %5.1f)", ImGui::GetColumnWidth(column));
                                }
                                
                            }
                            
                            
                            
                            // Values table Creation
                            for (size_t k = 0; k < data.collections[i].structs[j].values.size(); k++) {
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn(); 
                                std::string nameLbl = "##name" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k);
                                ImGui::PushItemWidth(-FLT_MIN);
                                if (ImGui::InputText(nameLbl.c_str(), &data.collections[i].structs[j].values[k].name)) { data.m_isDirty = true; }
                                ImGui::PopItemWidth();
                                ImGui::TableNextColumn();

                                brparser.m_IdMap[ImGui::GetItemID()] = selectableElement(typTypes::Value, ValueInfo{i,j,k, data.collections[i].structs[j].values[k].name});

                                
                                
                                
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
                                std::string valueLbl = "##value" + std::to_string(k) + "_" + std::to_string(j) + "_" + std::to_string(k);
                                ImGui::PushItemWidth(-FLT_MIN);  // Make input fill cell width
                                if (ImGui::InputText(valueLbl.c_str(), &data.collections[i].structs[j].values[k].value)) { data.m_isDirty = true; }
                                ImGui::PopItemWidth();
                                
                                ImGui::TableNextColumn(); 
                                std::string commentLbl = "##comment" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k);
                                ImGui::PushItemWidth(-FLT_MIN);  // Make input fill cell width
                                if (ImGui::InputText(commentLbl.c_str(), &data.collections[i].structs[j].values[k].comment)) { data.m_isDirty = true; }
                                ImGui::PopItemWidth();
                            }
                            ImGui::EndTable();
                        }
    
                        /////////////// STYLES SET .TYP TABLE POP ///////////////
                        ImGui::Unindent(tableIndent); 
                        ImGui::PopStyleVar(); 
                        /////////////// STYLES SET .TYP TABLE POP ///////////////

 
                 
                    }
                    
                }
            }
        }

        ImGui::PopStyleVar(); 
        
        data.updateCachedString();
        brparser.updateVarConfigString();

        ImGui::End();

        ////////////////////////////////////////
        //////// PARSER DATA TABLE END//////////
        ////////////////////////////////////////


        ////////////////////////////////////////
        //////// PARSER DATA STRING VIEW ///////
        ////////////////////////////////////////
        ImGui::Begin("Type Data Viewer");
        ImGui::TextWrapped("%s", data.getCachedString().c_str());
        ImGui::End();
        ////////////////////////////////////////
        //////// PARSER DATA STRING VIEW ///////
        ////////////////////////////////////////

        ////////////////////////////////////////
        //////// VAR CONFIG  STRING VIEW ///////
        ////////////////////////////////////////
        ImGui::Begin("Var Config Viewer");
        ImGui::TextWrapped("%s", brparser.getVarConfigString().c_str());
        ImGui::End();
        ////////////////////////////////////////
        //////// VAR CONFIG  STRING VIEW ///////
        ////////////////////////////////////////


        ImGui::Begin(".iom editor");
        brVarConfigCollection VarConfigCollection = brparser.getVarConfig();
        
        static bool treeOpen = false;
        if (!VarConfigCollection.empty())
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 1.0f));
            std::string label = "IO Data Table###IO Data Table";
            treeOpen = ImGui::CollapsingHeader(label.c_str());
            ImGui::PopStyleVar();
        }

        // Context menu for Structs tree
        if (ImGui::BeginPopupContextItem("varConfig_context_menu")) { 
        if (ImGui::MenuItem("Delete##varConfig")) { brparser.clearVarConfig(); } 
        if (ImGui::MenuItem("Copy##varConfig")) { 
            brparser.clipboard.copyVarConfigCollection(VarConfigCollection);
            ImGui::SetClipboardText(brparser.getVarConfigString().c_str());
        }       
        ImGui::Separator();
        if (ImGui::MenuItem("Copy as [.iom]##varConfig")) { }
        ImGui::EndPopup();
        }
        if (treeOpen)
        {
            size_t i = 0;
            for (auto &varConfig : VarConfigCollection){

                /////////////// STYLE SET IO DATA COLLAPSING HEADER ///////////////
                float ioDataHeaderIndent = ImGui::GetStyle().IndentSpacing * 0.25f;
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 1.0f));
                ImGui::Indent(ioDataHeaderIndent); 
                /////////////// STYLE IO DATA COLLAPSING HEADER ///////////////

                std::string label = "IO Data \t\t"  + varConfig.comment + "###IO Data Table" + std::to_string(i);
                bool innerTreeOpen = ImGui::CollapsingHeader(label.c_str());

                // ID MAPPING STRUCTS
                //brparser.m_IdMap[ImGui::GetItemID()] = selectableElement(typTypes::Struct, StructInfo{i,j, data.collections[i].structs[j].name});



                /////////////// STYLE POP IO DATA COLLAPSING HEADER ///////////////
                ImGui::PopStyleVar();
                ImGui::Unindent(ioDataHeaderIndent);
                /////////////// STYLE POP IO DATA COLLAPSING HEADER ///////////////

    
                if (innerTreeOpen)
                {
                    static float nameWidth = 400.0f;
                    static float typeWidth = 78.0f;
                    static float valueWidth = 60.0f;
                    
                
                    // if (displayCellSize)
                    // {
                    //     float availWidth = ImGui::GetContentRegionAvail().x;
                    //     ImGui::Text("Available width: %.1f", availWidth);
                    // }


                    /////////////// TABLE STYLES SET ///////////////
                    float ioTableIndent = ImGui::GetStyle().IndentSpacing * 0.75f;
                    ImGui::Indent(ioTableIndent);
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1.0f, 1.0f));
                    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0.0f, 0.0f));
                    /////////////// TABLE STYLES SET ///////////////

                    if (ImGui::BeginTable("Node", 4, tableFlags)) 
                    {
                        ImGui::TableSetupColumn("IO Adress", ImGuiTableColumnFlags_WidthFixed, nameWidth);
                        ImGui::TableSetupColumn("Process Variable", ImGuiTableColumnFlags_WidthFixed, typeWidth);
                        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, valueWidth);
                        ImGui::TableSetupColumn("Comment", ImGuiTableColumnFlags_WidthStretch, valueWidth);
                        ImGui::TableHeadersRow();
    
                        ImGui::TableNextRow();

                        if (m_Settings.showHeaderWidth)
                        {
                            for (int column = 0; column < 4; column++) {
                                ImGui::TableSetColumnIndex(column);
                                ImGui::Text("(w: %5.1f)", ImGui::GetColumnWidth(column));
                            }     
                        }
                        
                        
                        size_t j = 0;
                        for (auto &values : varConfig)
                        {
                            ImGui::TableNextRow();
                            
                            ImGui::TableNextColumn(); 
                            std::string nameLbl = "##ioAdress" + std::to_string(i) + "_" + std::to_string(j);
                            ImGui::PushItemWidth(-FLT_MIN);
                            std::string text1 = "text1";
                            if (ImGui::InputText(nameLbl.c_str(), &values.ioAdress)) {}
                            ImGui::PopItemWidth();
                            
                            ImGui::TableNextColumn();
                            std::string typeLbl = "##processVariable" + std::to_string(i) + "_" + std::to_string(j);
                            ImGui::PushItemWidth(-FLT_MIN);
                            std::string text2 = "text2";
                            if (ImGui::InputText(typeLbl.c_str(), &values.processVariable)) {}
                            ImGui::PopItemWidth();
                            
                            ImGui::TableNextColumn();
                            std::string valueLbl = "##type" + std::to_string(i) + "_" + std::to_string(j);
                            ImGui::PushItemWidth(-FLT_MIN);  
                            std::string text3 = "text3";
                            if (ImGui::InputText(valueLbl.c_str(), &values.type)) {}
                            ImGui::PopItemWidth();
    
                            ImGui::TableNextColumn();
                            std::string commentLbl = "##comment" + std::to_string(i) + "_" + std::to_string(j);
                            ImGui::PushItemWidth(-FLT_MIN);  
                            std::string text4 = "text3";
                            if (ImGui::InputText(commentLbl.c_str(), &values.comment)) {}
                            ImGui::PopItemWidth();
                            
                            j++;
                        }
                        
                        ImGui::EndTable();
                    }
                    
                    /////////////// STYLES SET IO TABLE POP ///////////////
                    ImGui::Unindent(ioTableIndent); 
                    ImGui::PopStyleVar(2); 
                    /////////////// STYLES SET IO TABLE POP ///////////////
                }
                i++;
            }
            ImGui::Unindent();
        }
        ImGui::End();
        ////////////////////////////////////////
        //////// PARSER DATA STRING VIEW ///////
        ////////////////////////////////////////

        if (hightlight)
        {
            ImGui::PopStyleColor();
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