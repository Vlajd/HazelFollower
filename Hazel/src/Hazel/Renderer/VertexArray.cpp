#include <hzpch.h>
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel {

	VertexArray* VertexArray::Create() {

		switch (Renderer::GetAPI()) {

			case RendererAPI::None:  HZ_CORE_ASSERT(false, "RendererAPI::None Is Currently Not Supported!"); return nullptr;
			case RendererAPI::OpenGL: return new OpenGLVertexArray();
		}

		HZ_CORE_ASSERT(false, "VertexArray::Unknown RendererAPI!");
		return nullptr;
	}
}