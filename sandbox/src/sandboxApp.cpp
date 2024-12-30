#include <Mir.h>

class ExampleLayer : public Mir::Layer{
public:
    ExampleLayer()
        : Layer("Example"){

        }
    
    void OnUpdate() override{
        MIR_INFO("ExampleLayer::Update");
    }

    void OnEvent(Mir::Event& event) override{
        MIR_TRACE("{0}", event);
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