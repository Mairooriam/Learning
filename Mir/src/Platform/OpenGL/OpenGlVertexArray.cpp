
#include "Mirpch.h"
#include "OpenGlVertexArray.h"
#include "glad/glad.h"
namespace Mir {

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type){
        switch (type){
            case ShaderDataType::Float:     return GL_FLOAT;
            case ShaderDataType::Float2:    return GL_FLOAT;
            case ShaderDataType::Float3:    return GL_FLOAT;
            case ShaderDataType::Float4:    return GL_FLOAT;
            case ShaderDataType::Mat3:      return GL_FLOAT;
            case ShaderDataType::Mat4:      return GL_FLOAT;
            case ShaderDataType::Int:       return GL_INT;
            case ShaderDataType::Int2:      return GL_INT;
            case ShaderDataType::Int3:      return GL_INT;
            case ShaderDataType::Int4:      return GL_INT;
            case ShaderDataType::Bool:      return GL_BOOL;
    }

    MIR_CORE_ASSERT(false, "Unkown ShaderDataType!");
    return 0;
    }  

    OpenGlVertexArray::OpenGlVertexArray(){
        glCreateVertexArrays(1, &m_RendererID);
    }

    void OpenGlVertexArray::Bind() const{
        glBindVertexArray(m_RendererID);
    }

    void OpenGlVertexArray::UnBind() const{
        glBindVertexArray(0);
    }

    void OpenGlVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &VertexBuffer){
        MIR_CORE_ASSERT(VertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout");


        glBindVertexArray(m_RendererID);
        VertexBuffer->Bind();

        uint32_t index = 0;
        const auto& layout =  VertexBuffer->GetLayout();
        for (const auto& element :layout){
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
            element.GetComponentCount(element.Type),
            ShaderDataTypeToOpenGLBaseType(element.Type), 
            element.Normalized ? GL_TRUE : GL_FALSE,
            layout.GetStride(),
            (const void*)(uintptr_t)element.Offset); // ADDED (uintptr_t) to remove error

            index++;
        }
        m_VertexBuffers.push_back(VertexBuffer);
    }
    
    void OpenGlVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &IndexBuffer){
        glBindVertexArray(m_RendererID);
        IndexBuffer->Bind();

        m_IndexBuffer = IndexBuffer;
    }

}