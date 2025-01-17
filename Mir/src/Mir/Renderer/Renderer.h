#pragma once

#include "Mir/Renderer/RenderCommand.h"

#include "Mir/Renderer/OrthographicCamera.h"

#include "Mir/Renderer/Shader.h"

namespace Mir {


    class Renderer{
    public:

        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray> &vertexArray, const std::shared_ptr<Shader>& shader);
    
        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    private:
        struct SceneData{
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* m_SceneData;
    };

    

}