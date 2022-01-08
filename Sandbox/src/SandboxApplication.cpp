#include <Hazel.h>

class Sandbox : public Hazel::Application {

	public:
		Sandbox() {
		
			PushOverlay(new Hazel::ImGuiLayer());
		}

		~Sandbox() {}
};

namespace Hazel {

	Application* CreateApplication() {

		return new Sandbox();
	}
}