#include "Mirpch.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace Mir {

    // --------------------------------------------------------------------------------------- //
    // VERTEX BUFFER ------------------------------------------------------------------------- //
    // --------------------------------------------------------------------------------------- //
    OpenGLVertexBuffer::OpenGLVertexBuffer(float *verticies, uint32_t size)
        : m_Size(size)  {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer(){
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const{
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }
    
    void OpenGLVertexBuffer::UnBind() const{
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }




    // -------------------------------------------------------------------------------------- //
    // INDEX BUFFER ------------------------------------------------------------------------- //
    // -------------------------------------------------------------------------------------- //
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indicies, uint32_t count)
    :m_Count(count) {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indicies, GL_STATIC_DRAW);
    }
    
    OpenGLIndexBuffer::~OpenGLIndexBuffer(){
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const{
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }
    
    void OpenGLIndexBuffer::UnBind() const{
        glBindBuffer(GL_ARRAY_BUFFER, 0);  
    }
}