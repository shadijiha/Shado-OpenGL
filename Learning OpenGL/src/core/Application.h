#pragma once
#include <string>
#include <vector>
#include "Renderer2D.h"

#include "GLFW/glfw3.h"
#include "Events/Event.h"

namespace Shado {
	
	class Scene {

	public:
		Scene(const std::string& name, int zIndex);
		Scene(const std::string& name);
		virtual ~Scene() {}

		virtual void onInit() = 0;
		virtual void onUpdate(TimeStep dt) = 0;
		virtual void onDraw() = 0;
		virtual void onDestroy() = 0;
		virtual void onEvent(Event& event) = 0;

		std::string			getName()	const { return m_Name; }
		unsigned long long	getId()		const { return m_Id; }
		int					getZIndex()	const { return zIndex; }

	private:
		std::string m_Name;
		unsigned long long m_Id;
		int zIndex;
	};

	class Application {
	private:
		using EventCallbackFn = std::function<void(Event&)>;
		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool VSync;

			EventCallbackFn eventCallback;
		};
	public:
		Application(unsigned int width, unsigned int height, const std::string& title);
		Application();
		~Application();

		static Application& get() { return *singleton; }
		static void destroy();
		static const WindowData& getWindowData() { return singleton->m_Data; }
		static float getAspectRatio() { return (float)singleton->m_Data.width / (float)singleton->m_Data.height; }

		void setWindowTitle(const std::string& s) const;
	
		void listenToEvents();
		void run();
		void submit(Scene* scene);
		void onEvent(Event& e);	

		GLFWwindow* getWindow() const { return window; }

	private:
		GLFWwindow* window;
		float m_LastFrameTime = 0.0f;	// Time took to render last frame	
		std::vector<Scene*> allScenes;
		WindowData m_Data;
	
		static Application* singleton;	
	};
}
