#include <hzpch.h>
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Hazel {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {

		switch (type) {

			case Hazel::ShaderDataType::Float:	return GL_FLOAT;
			case Hazel::ShaderDataType::Float2:	return GL_FLOAT;
			case Hazel::ShaderDataType::Float3:	return GL_FLOAT;
			case Hazel::ShaderDataType::Float4:	return GL_FLOAT;
			case Hazel::ShaderDataType::Mat3:	return GL_FLOAT;
			case Hazel::ShaderDataType::Mat4:	return GL_FLOAT;
			case Hazel::ShaderDataType::Int:	return GL_INT;
			case Hazel::ShaderDataType::Int2:	return GL_INT;
			case Hazel::ShaderDataType::Int3:	return GL_INT;
			case Hazel::ShaderDataType::Int4:	return GL_INT;
			case Hazel::ShaderDataType::Bool:	return GL_BOOL;
		}

		HZ_CORE_ASSERT(false, "ShaderDataTypeToOpenGLBaseType::Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const {

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const {

		glBindVertexArray(NULL);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {

		HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "OpenGLVertexArray::AddVertexBuffer::Vertex Buffer Has No Layout")

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();// glVertexAttribPointer
		for (const auto& element : layout) {

			glEnableVertexAttribArray(index);

			glVertexAttribPointer(index,						// glVertexAttribPointer
				element.GetComponentCount(),					// glVertexAttribPointer
				ShaderDataTypeToOpenGLBaseType(element.Type),	// glVertexAttribPointer
				element.Normalized ? GL_TRUE : GL_FALSE,		// glVertexAttribPointer
				layout.GetStride(),								// glVertexAttribPointer
				(const void*)element.Offset						// glVertexAttribPointer
				);												// glVertexAttribPointer

			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}