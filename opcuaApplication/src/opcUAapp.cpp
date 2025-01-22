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
    }
private:
    UA_Client* m_UaClient;
    std::string m_UaEndpointAdress; // TODO change tree view to have endpoint adress -> show namne spaces
    std::vector<std::string> m_Namespaces;
    std::vector<bool> m_SelectedNamespace;
private:

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