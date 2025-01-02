#include <Mir.h>

class ExampleLayer : public Mir::Layer{
public:
    ExampleLayer()
        : Layer("Example"){

        }
    
    void OnUpdate() override{
        //MIR_INFO("ExampleLayer::Update");
        if(Mir::Input::IsKeyPressed(MIR_KEY_TAB))
            MIR_TRACE("Tab key is pressed!");
    }

    void OnEvent(Mir::Event& event) override{
        //MIR_TRACE("{0}", event);
    }
};



class SandBox : public Mir::Application{
    public:
    SandBox(){
        PushLayer(new ExampleLayer());
        PushOverlay(new Mir::ImGuiLayer());
    }
    ~SandBox(){
    
    }
};

Mir::Application* Mir::createApplication(){
    return new SandBox();
}