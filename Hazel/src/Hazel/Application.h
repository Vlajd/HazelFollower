#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Events/ApplicationEvent.h"
#include "Core/Timestep.h"

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

			Scope<Window> m_Window;
			ImGuiLayer* m_ImGuiLayer;
			bool m_Running = true;
			LayerStack m_LayerStack;
			float m_LastFrameTime = 0.0f;

			static Application* s_Instance;
	};

	// defined in client
	Application* CreateApplication();
}
