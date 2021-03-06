#include <Hazel.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Hazel::Layer {

	public:
		ExampleLayer()
			: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquarePosition(0.0f) {

			m_VertexArray.reset(Hazel::VertexArray::Create());

			float vertices[3 * 7] = {

				-0.5f, -0.5f, 0.0f, 0.1f, 0.15f, 0.2f, 0.0f,
				 0.0f,  0.5f, 0.0f, 0.1f, 0.1f, 0.7f, 0.25f,
				 0.5f, -0.5f, 0.0f, 0.15f, 0.1f, 0.2f, 0.0f
			};

			Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
			vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

			Hazel::BufferLayout layout = {

				{ Hazel::ShaderDataType::Float3, "a_Position" },
				{ Hazel::ShaderDataType::Float4, "a_Color" }
			};

			vertexBuffer->SetLayout(layout);
			m_VertexArray->AddVertexBuffer(vertexBuffer);

			uint32_t indices[3] = { 0, 1, 2 };

			Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
			indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
			m_VertexArray->SetIndexBuffer(indexBuffer);

			m_SquareVA.reset(Hazel::VertexArray::Create());

			float squareVertices[5 * 4] = {

				-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
				-0.5f,  0.5f,  0.0f, 0.0f, 1.0f
			};

			Hazel::Ref<Hazel::VertexBuffer> squareVB;
			squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

			squareVB->SetLayout({
				{ Hazel::ShaderDataType::Float3, "a_Position" },
				{ Hazel::ShaderDataType::Float2, "a_TexCoord"}
			});
			m_SquareVA->AddVertexBuffer(squareVB);

			uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
			Hazel::Ref<Hazel::IndexBuffer> squareIB;
			squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
			m_SquareVA->SetIndexBuffer(squareIB);

			// [[Shader1]]
			std::string vertexSrc = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;

				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				out vec3 v_Position;
				out vec4 v_Color;

				void main() {
					v_Position = a_Position;
					v_Color = a_Color;
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
				}
			)";

			std::string fragmentSrc = R"(
				#version 330 core

				layout(location = 0) out vec4 color;

				in vec3 v_Position;
				in vec4 v_Color;	

				void main() {
					//color = vec4(v_Position * 0.5f + 0.5f, 1.0f);
					color = v_Color;
				}
			)";

			m_Shader = Hazel::Shader::Create("VertexColorTriangle", vertexSrc, fragmentSrc);

			// [[Shader2]]

			std::string flatColorShaderVertexSrc = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;

				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				out vec3 v_Position;

				void main() {

					v_Position = a_Position;
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
				}
			)";

			std::string flatColorShaderFragmentSrc = R"(
				#version 330 core

				layout(location = 0) out vec4 color;

				in vec3 v_Position;

				uniform vec3 u_Color;

				void main() {

					color = vec4(u_Color, 1.0f);
				}
			)";

			m_FlatColorShader = Hazel::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

			// [[Shader2]]


			// [[Shader3]]
			auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

			m_Texture = (Hazel::Texture2D::Create("assets/textures/Checkerboard.png"));
			m_CodeChickenTexture = (Hazel::Texture2D::Create("assets/textures/chicken.png"));

			std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
			std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
			// [[Shader3]]
		}

		virtual void OnUpdate(Hazel::Timestep ts) override {

			if (Hazel::Input::IsKeyPressed(HZ_KEY_A)) m_CameraPosition.x -= m_CameraMoveSpeed * ts;
			if (Hazel::Input::IsKeyPressed(HZ_KEY_D)) m_CameraPosition.x += m_CameraMoveSpeed * ts;
			if (Hazel::Input::IsKeyPressed(HZ_KEY_S)) m_CameraPosition.y -= m_CameraMoveSpeed * ts;
			if (Hazel::Input::IsKeyPressed(HZ_KEY_W)) m_CameraPosition.y += m_CameraMoveSpeed * ts;
			if (Hazel::Input::IsKeyPressed(HZ_KEY_E)) m_CameraRotation -= m_CameraRotationSpeed * ts;
			if (Hazel::Input::IsKeyPressed(HZ_KEY_Q)) m_CameraRotation += m_CameraRotationSpeed * ts;

			Hazel::RenderCommand::SetClearColor(glm::vec4(0.14f, 0.15f, 0.16f, 1.0f));
			Hazel::RenderCommand::Clear();

			m_Camera.SetPosition(m_CameraPosition);
			m_Camera.SetRotation(m_CameraRotation);

			// Begin Scene
			Hazel::Renderer::BeginScene(m_Camera);

			static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

			std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind(); 
			std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

			for (int x = 0; x < 25; x++) {
				for (int y = 0; y < 25; y++) {

					glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
					Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
				}
			}

			auto textureShader = m_ShaderLibrary.Get("Texture");

			m_Texture->Bind();
			Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
			
			m_CodeChickenTexture->Bind();
			Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			// Triangle
			//Hazel::Renderer::Submit(m_Shader, m_VertexArray);

			Hazel::Renderer::EndScene();
			// End Scene
		}

		virtual void OnImGuiRender() override {

			// ImGui Code Here
			ImGui::Begin("Settings");

			ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

			ImGui::End();
		}

		void OnEvent(Hazel::Event& event) override {

			// OnEvent Actions Here
		}

	private:

		Hazel::ShaderLibrary m_ShaderLibrary;

		Hazel::Ref<Hazel::Shader> m_Shader;
		Hazel::Ref<Hazel::VertexArray> m_VertexArray;

		Hazel::Ref<Hazel::Shader> m_FlatColorShader;
		Hazel::Ref<Hazel::VertexArray> m_SquareVA;

		// texture
		Hazel::Ref<Hazel::Texture2D> m_Texture, m_CodeChickenTexture;

		Hazel::OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition;
		float m_CameraRotation = 0.0f;
		float m_CameraMoveSpeed = 5.0f;
		float m_CameraRotationSpeed = 190.0f;

		glm::vec3 m_SquarePosition;
		float m_SquareMoveSpeed = 1.0f;

		glm::vec3 m_SquareColor = glm::vec3(0.2f, 0.3f, 0.8f);
};

class Sandbox : public Hazel::Application {

	public:
		Sandbox() {
		
			PushLayer(new ExampleLayer());
		}

		~Sandbox() {}
};

namespace Hazel {

	Application* CreateApplication() { return new Sandbox(); }
}