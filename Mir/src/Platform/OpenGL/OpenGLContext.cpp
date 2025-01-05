#include "Mirpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Mir {
    OpenGLContext::OpenGLContext(GLFWwindow *windowHandle)
    : m_WindowHandle(windowHandle){
         MIR_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init(){
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        MIR_CORE_ASSERT(status, "Failed to initialize Glad!");
    }

    void OpenGLContext::SwapBuffers(){
        
        glfwSwapBuffers(m_WindowHandle);
    }
}
