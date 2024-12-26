#pragma once
#ifdef MIR_PLATFORM_WINDOWS
extern  Mir::Application* Mir::createApplication();
int main(int argc, char** argv) {
    Mir::Log::Init();
    MIR_CORE_WARN("MIR initialized");
    int a = 5;
    MIR_CLIENT_INFO("Hello: Var1:{0}",a);
    printf("Mir Engine");
    auto app = Mir::createApplication();
    app->Run();
    delete app;
    return 0;
}

#endif