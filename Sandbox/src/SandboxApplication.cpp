#include <Hazel.h>

#include <imgui.h>

class ExampleLayer : public Hazel::Layer {

	public:
		ExampleLayer()
			: Layer("Example") {}

		virtual void OnImGuiRender() override {

			ImGui::Begin("Test");
			ImGui::End();
		}

		void OnEvent(Hazel::Event& event) override {

		}
};

class Sandbox : public Hazel::Application {

	public:
		Sandbox() {
		
			PushLayer(new ExampleLayer());
		}

		~Sandbox() {}
};

namespace Hazel {

	Application* CreateApplication() {

		return new Sandbox();
	}
}