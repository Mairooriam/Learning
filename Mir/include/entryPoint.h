#pragma once
#ifdef MIR_PLATFORM_WINDOWS
extern  Mir::Application* Mir::createApplication();
int main(int argc, char** argv) {
    printf("Mir Engine");
    auto app = Mir::createApplication();
    app->Run();
    delete app;
    return 0;
}

#endif