#pragma once

#include "Mir/Renderer/Buffer.h"
#include <memory>

namespace Mir {
    class VertexArray
    {
    public:
        virtual ~VertexArray() {}

        virtual void Bind() const  = 0;
        virtual void UnBind() const = 0;

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& VertexBuffer) = 0;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& IndexBuffer) = 0;
        virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const = 0;
        static VertexArray* Create();
    private:


    };
    
}