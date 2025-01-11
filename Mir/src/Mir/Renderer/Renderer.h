#pragma once

#include "Renderer/RenderCommand.h"

namespace Mir {


    class Renderer{
    public:

        static void BeginScene();
        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
    
        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };

    

}