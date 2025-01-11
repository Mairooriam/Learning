#include "Mirpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGlRendererAPI.h"

namespace Mir {
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}