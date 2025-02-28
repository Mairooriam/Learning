#include "Mirpch.h"

#include "Application.h"
#include "Mir/Events/ApplicationEvent.h"



#include "Renderer/Renderer.h"
#include "Mir/Input.h"
#include <GLFW/glfw3.h>

#include "OpcUA/OpcuaClient.h"

#include "CPython/PythonTool.h"

namespace Mir {  

  #define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application() 
        {
		MIR_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
        
        m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

        PythonTool pythonTool;
    
        // Path to your Python script
        std::string scriptPath = "C:\\Users\\35850\\Desktop\\repositories\\learning2\\Learning\\Mir\\src\\Mir\\CPython\\excelTools.py";
        
        // Arguments for the script (excel file path and optional csv file path)
        std::vector<std::string> args = {
            "C:\\Users\\35850\\Desktop\\repositories\\learning2\\Learning\\Mir\\External\\testdata\\manual_edited.xlsx",
            "C:\\Users\\35850\\Desktop\\repositories\\learning2\\Learning\\Mir\\External\\testdata\\testdata222.csv"  // optional
        };
        
        // Run the script
        pythonTool.RunPythonScript(scriptPath, args);    

        


        m_brParser = new brParser(); // Direct initialization
        // m_brParser->initDummyData();
        //std::map<std::string, std::vector<brDataTypeNode>> plcdata = m_brParser->readPlcDataCsv("C:\\Users\\35850\\Desktop\\repositories\\learning2\\Learning\\Mir\\External\\testdata\\testdata222.csv");
        
        m_brParser->readDatafile999999("C:\\projects\\OpcUa_Sample\\Logical\\Types.typ");

        //m_brParser->m_testData.;
        //m_brParser->writeFile("C:/Users/35850/Desktop/repositories/learning2/Learning/Tools/test.typ", test9999.toString(), std::ios_base::app);


        //std::map<std::string, std::vector<brDataTypeNode>> test222 = m_brParser->readDataTypeFileOneLineAtaTime("C:\\projects\\OpcUa_Sample\\Logical\\Types.typ");
        //std::vector<std::string> datatypefile = m_brParser->readDatatypeFile("C:\\projects\\OpcUa_Sample\\Logical\\Types.typ");
        //m_brParser->mergeMaps(test222);

        //std::vector<std::string> filecontent = m_brParser->readDatatypeFile("C:\\projects\\OpcUa_Sample\\Logical\\Types.typ");
        //m_brParser->writeDummyData();

        
        
   }




    void Application::OnEvent(Event &e){        
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnwWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
    }

    Application::~Application() {
    }

    void Application::Run()
    {



        
        while (m_Running){

            float time = (float)glfwGetTime(); // TODO: abstract into Platfrom::GetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            for (Layer* layer : m_LayerStack){
                layer->OnUpdate(timestep);
            }
            
            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack){
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

			m_Window->OnUpdate();
        }
    }

   

    void Application::PushLayer(Layer *layer){
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }
    void Application::PushOverlay(Layer *layer){
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::CreateOpcUaClient(std::string_view serverAdress){
        OpcuaClient opcUaClient(serverAdress);
    }

    bool Application::OnwWindowClose(WindowCloseEvent &e)
    {
        (void)e; // Mark parameter as unused
        m_Running = false;
        return true;
    }
}