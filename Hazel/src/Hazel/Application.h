#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Events/ApplicationEvent.h"

// [temp]
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"

namespace Hazel {

	class HAZEL_API Application {

		public:

			Application();
			virtual ~Application();
			void Run();

			void OnEvent(Event& e);

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* layer);

			inline static Application& Get() { return *s_Instance; }
			inline Window& GetWindow() { return *m_Window; }

		private:

			bool OnWindowClose(WindowCloseEvent& e);

			std::unique_ptr<Window> m_Window;
			ImGuiLayer* m_ImGuiLayer;
			bool m_Running = true;
			LayerStack m_LayerStack;

			unsigned int m_VertexArray;
			std::unique_ptr<Shader> m_Shader;
			std::unique_ptr<VertexBuffer> m_VertexBuffer;
			std::unique_ptr<IndexBuffer> m_IndexBuffer;

			static Application* s_Instance;
	};

	// defined in client
	Application* CreateApplication();
}

