#include "Mirpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
namespace Mir {
    void VertexArray::Bind() const{

    }

    void VertexArray::UnBind() const{
    
    }
    
    void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &VertexBuffer){
    
    }

    void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &IndexBuffer){
    
    }

    VertexArray *VertexArray::Create(){        
        switch (Renderer::GetAPI()){
        case RendererAPI::API::None: MIR_CORE_ASSERT(false, "Render::API::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
        };

        MIR_CORE_ASSERT(false, "Unkown RendererAPI!");
        return nullptr;
    }
}
