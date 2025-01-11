#include "Mirpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGlBuffer.h"

namespace Mir {
    
    VertexBuffer *VertexBuffer::Create(float *verticies, uint32_t size){
        switch (Renderer::GetAPI()){

        case RendererAPI::API::None: MIR_CORE_ASSERT(false, "Render::API::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(verticies, size);
        }

        MIR_CORE_ASSERT(false, "Unkown RendererAPI!");
        return nullptr;
    }

    IndexBuffer *IndexBuffer::Create(uint32_t *indicies, uint32_t size){
        switch (Renderer::GetAPI()){
            
        case RendererAPI::API::None: MIR_CORE_ASSERT(false, "Render::API::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indicies, size);
        }

        MIR_CORE_ASSERT(false, "Unkown RendererAPI!");
        return nullptr;
    }


}
