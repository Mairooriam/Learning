#pragma once

#include "Mir/Renderer/VertexArray.h"
#include <vector>

namespace Mir {


    class OpenGlVertexArray : public VertexArray
    {
    public:
        OpenGlVertexArray();
        virtual ~OpenGlVertexArray() {}

        virtual void Bind() const  override;
        virtual void UnBind() const override;

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& VertexBuffer) override;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& IndexBuffer) override;
       
    private:
        uint32_t m_RendererID;
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
    
}