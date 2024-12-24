#pragma once
#ifdef MIR_PLATFORM_WINDOWS
extern  Mir::Application* Mir::createApplication();
int main(int argc, char** argv) {

    Mir::Log::init();
    Mir::Log::getCoreLogger()->warn("Initialized Log!");
    Mir::Log::getClientLogger()->info("Hello!");
    printf("Mir Engine");
    auto app = Mir::createApplication();
    app->Run();
    delete app;
    return 0;
}

#endif