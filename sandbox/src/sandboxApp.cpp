#include <Mir.h>
#include "mirpch.h"
class SandBox : public Mir::Application{
    public:
    SandBox(){

    }
    ~SandBox(){
    
    }
};

Mir::Application* Mir::createApplication(){
    return new SandBox();
}