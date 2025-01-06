#pragma once

#include "Mir/Renderer/Buffer.h"

namespace Mir {
    class OpenGlVertexBuffer : public VertexBuffer {
    public:
        OpenGlVertexBuffer(float *verticies, uint32_t size);
        virtual ~OpenGlVertexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual const BufferLayout& GetLayout() const override { return m_Layout;}
        virtual void SetLayout(BufferLayout Layout) override { m_Layout = Layout; }
    private:
        uint32_t m_RendererID;  
        BufferLayout m_Layout;

    };

    class OpenGlIndexBuffer : public IndexBuffer {
    public:
        OpenGlIndexBuffer(uint32_t* indicies, uint32_t size);
        virtual ~OpenGlIndexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual uint32_t GetCount() const { return m_Count; }
    private:
        uint32_t m_RendererID;  
        uint32_t m_Count;  
    };
}
