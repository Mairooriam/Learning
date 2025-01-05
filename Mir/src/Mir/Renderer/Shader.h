#pragma once
#include <string>
#include <cstdint>


namespace Mir {
    class Shader{
    public:
        Shader(const std::string& vertexSrc, const std::string& fargmentSrc);
        ~Shader();
        
        void Bind() const;
        void UnBind() const;
    private:
        uint32_t m_RendererID;
    };

    
}