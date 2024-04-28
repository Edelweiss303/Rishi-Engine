#include "REpch.h"
#include "RenderCommand.h"

namespace REngine
{
    Scope<RendererAPI> RenderCommand::s_rendererAPI = RendererAPI::Create();
}