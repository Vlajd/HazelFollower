#include <hzpch.h>
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		
		switch (Renderer::GetAPI()) {

			case RendererAPI::None:  HZ_CORE_ASSERT(false, "RendererAPI::None Is Currently Not Supported!"); return nullptr;
			case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		HZ_CORE_ASSERT(false, "VertexBuffer::Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		
		switch (Renderer::GetAPI()) {

			case RendererAPI::None: HZ_CORE_ASSERT(false, "IndexBuffer::RendererAPI::None Is Currently Not Supported!"); return nullptr;
			case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, size);
		}

		HZ_CORE_ASSERT(false, "IndexBuffer::Unknown RendererAPI!");
		return nullptr;
	}
}