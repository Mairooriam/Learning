#pragma once

#include "Mir/Renderer/VertexArray.h"
#include <vector>

namespace Mir {


    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray() {}

        virtual void Bind() const  override;
        virtual void UnBind() const override;

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& VertexBuffer) override;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& IndexBuffer) override;
        virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }
    
    private:
        uint32_t m_RendererID;
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
    
}