#pragma once

#include "RendererAPI.h"
#include <memory.h>
namespace Mir {
    class RenderCommand
    {
    public:
        inline static void SetClearColor(const glm::vec4& color){
            s_RendererAPI->SetClearColor(color);
        };
        inline static void Clear(){
            s_RendererAPI->Clear();
        };

        inline void static DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray){
            s_RendererAPI->DrawIndexed(vertexArray);
        };
    private:
        static RendererAPI* s_RendererAPI;
        /* data */
    };

    
}