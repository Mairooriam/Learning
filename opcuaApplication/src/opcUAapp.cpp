#include "Mir.h"

#include "imgui.h"

#include <open62541/client.h>
#include <iostream>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>


#define GLM_ENABLE_EXPERIMENTAL



class ExampleLayer : public Mir::Layer{
public:
    ExampleLayer(){
        m_UaClient = UA_Client_new();
        UA_ClientConfig_setDefault(UA_Client_getConfig(m_UaClient));

        // Connect to the server
        UA_StatusCode status = UA_Client_connect(m_UaClient, "opc.tcp://192.168.0.55:4840");
        if (status != UA_STATUSCODE_GOOD) {
            UA_Client_delete(m_UaClient);
            MIR_TRACE("Failed to connect to server: {0}", UA_StatusCode_name(status));
        }  
         // Browse namespaces
        browseNamespaces();
    }
    
    void OnUpdate(Mir::Timestep ts) override{
        Mir::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Mir::RenderCommand::Clear();

    }   

    void OnEvent(Mir::Event& event) override{
        Mir::EventDispatcher dispatcher(event);
    }
    virtual void OnImGuiRender() override {
        ImGui::Begin("Namespace Browser");
        
        if (ImGui::TreeNode("Advanced, with Selectable nodes"))
        {
            static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
            static bool align_label_with_current_x_position = false;

            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnArrow",       &base_flags, ImGuiTreeNodeFlags_OpenOnArrow);
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_OpenOnDoubleClick", &base_flags, ImGuiTreeNodeFlags_OpenOnDoubleClick);
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAvailWidth",    &base_flags, ImGuiTreeNodeFlags_SpanAvailWidth); ImGui::SameLine(); 
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanFullWidth",     &base_flags, ImGuiTreeNodeFlags_SpanFullWidth);
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanLabelWidth",    &base_flags, ImGuiTreeNodeFlags_SpanLabelWidth); ImGui::SameLine(); 
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_SpanAllColumns",    &base_flags, ImGuiTreeNodeFlags_SpanAllColumns); ImGui::SameLine(); 
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_AllowOverlap",      &base_flags, ImGuiTreeNodeFlags_AllowOverlap);
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_Framed",            &base_flags, ImGuiTreeNodeFlags_Framed); ImGui::SameLine(); 
            ImGui::CheckboxFlags("ImGuiTreeNodeFlags_NavLeftJumpsBackHere", &base_flags, ImGuiTreeNodeFlags_NavLeftJumpsBackHere);
            ImGui::Checkbox("Align label with current X position", &align_label_with_current_x_position);
            ImGui::Text("Hello!");
            if (align_label_with_current_x_position)
                ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

            // 'selection_mask' is dumb representation of what may be user-side selection state.
            //  You may retain selection state inside or outside your objects in whatever format you see fit.
            // 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
            /// of the loop. May be a pointer to your own node type, etc.
            static int selection_mask = (1 << 2);
            int node_clicked = -1;
            for (int i = 0; i < m_Namespaces.size(); i++)
            {
                // Disable the default "open on single-click behavior" + set Selected flag according to our selection.
                // To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
                ImGuiTreeNodeFlags node_flags = base_flags;
                const bool is_selected = (selection_mask & (1 << i)) != 0;
                if (is_selected){
                    node_flags |= ImGuiTreeNodeFlags_Selected;
                }    	    
            
                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                std::string label = fmt::format("Namespace {0}: {1}", i, m_Namespaces[i]);
                ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, label.c_str());
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                    node_clicked = i;
                

            }
            if (node_clicked != -1)
            {
                selection_mask = (1 << node_clicked);           // Click to single-select
            }
            if (align_label_with_current_x_position)
                ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
            ImGui::TreePop();
        }

    ImGui::End();
    }
private:
    UA_Client* m_UaClient;
    std::string m_UaEndpointAdress; // TODO change tree view to have endpoint adress -> show namne spaces
    std::vector<std::string> m_Namespaces;
    std::vector<bool> m_SelectedNamespace;
private:
    void browseNamespaces() {
        UA_Variant value;
        UA_Variant_init(&value);

        UA_NodeId nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_NAMESPACEARRAY);
        UA_StatusCode status = UA_Client_readValueAttribute(m_UaClient, nodeId, &value);
        if (status == UA_STATUSCODE_GOOD && UA_Variant_hasArrayType(&value, &UA_TYPES[UA_TYPES_STRING])) {
            size_t arrayLength = value.arrayLength;
            UA_String* namespaceArray = (UA_String*)value.data;

            MIR_INFO("OPC UA Server has: {0} namespaces", arrayLength);
            for (size_t i = 0; i < arrayLength; ++i) {
                m_Namespaces.push_back(std::string((char*)namespaceArray[i].data, namespaceArray[i].length));
            }
        }
        m_SelectedNamespace.resize(m_Namespaces.size(), false);
        UA_Variant_clear(&value);
    }
};



class SandBox : public Mir::Application{
    public:
    SandBox(){
        PushLayer(new ExampleLayer());
        
    }
    ~SandBox(){
    
    }
};

Mir::Application* Mir::createApplication(){
    return new SandBox();
}