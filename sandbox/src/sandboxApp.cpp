#include "Mir.h"

#include "imgui.h"




#define GLM_ENABLE_EXPERIMENTAL

class ExampleLayer : public Mir::Layer{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f){

        m_VertexArray.reset(Mir::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Mir::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Mir::VertexBuffer::Create(vertices, sizeof(vertices)));
		Mir::BufferLayout layout = {
			{ Mir::ShaderDataType::Float3, "a_Position" },
			{ Mir::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Mir::IndexBuffer> indexBuffer;
		indexBuffer.reset(Mir::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Mir::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Mir::VertexBuffer> squareVB;
		squareVB.reset(Mir::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Mir::ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Mir::IndexBuffer> squareIB;
		squareIB.reset(Mir::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
                
			}
		)";

		m_Shader.reset(new Mir::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new Mir::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
        
    }
    
    void OnUpdate() override{

        if (Mir::Input::IsKeyPressed(MIR_KEY_LEFT)){
           m_CameraPosition.x -= m_CameraSpeed; 
        }
        else if (Mir::Input::IsKeyPressed(MIR_KEY_RIGHT)){
           m_CameraPosition.x += m_CameraSpeed; 
        }

        if (Mir::Input::IsKeyPressed(MIR_KEY_UP)){
           m_CameraPosition.y += m_CameraSpeed; 
        }
        else if (Mir::Input::IsKeyPressed(MIR_KEY_DOWN)){
           m_CameraPosition.y -= m_CameraSpeed; 
        }

        if (Mir::Input::IsKeyPressed(MIR_KEY_E)){
           m_CameraRotation += m_CameraRotationSpeed; 
        }
        else if (Mir::Input::IsKeyPressed(MIR_KEY_Q)){
           m_CameraRotation -= m_CameraRotationSpeed; 
        }
        Mir::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Mir::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Mir::Renderer::BeginScene(m_Camera);
 
        Mir::Renderer::Submit(m_SquareVA, m_BlueShader);
        Mir::Renderer::Submit(m_VertexArray, m_Shader);

        Mir::Renderer::EndScene();   
    }   

    void OnEvent(Mir::Event& event) override{
        Mir::EventDispatcher dispatcher(event);
    }

    virtual void OnImGuiRender() override{

    }
private:
    std::shared_ptr<Mir::VertexArray> m_VertexArray;
    std::shared_ptr<Mir::VertexArray> m_SquareVA;
    std::shared_ptr<Mir::Shader> m_Shader;
    std::shared_ptr<Mir::Shader> m_BlueShader;

    Mir::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraRotation;
    float m_CameraSpeed = 0.1f;
    float m_CameraRotationSpeed = 1.0f;
};



class SandBox : public Mir::Application{
    public:
    SandBox(){
        PushLayer(new ExampleLayer());
        
    }
    ~SandBox(){
    
    }
};

Mir::Application* Mir::createApplication(){
    return new SandBox();
}