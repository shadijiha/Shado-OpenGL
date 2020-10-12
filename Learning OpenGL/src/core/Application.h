#pragma once
#include <string>
#include <vector>
#include "Renderer2D.h"

#include "opengl.h"
#include "Events/Event.h"
#include "Window.h"

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
	public:
		Application(unsigned int width, unsigned int height, const std::string& title);
		Application();
		~Application();

		static Application& get() { return *singleton; }
		static void destroy();		

		static void close();
		
		void run();
		void submit(Scene* scene);
		void onEvent(Event& e);	

		Window& getWindow() const { return *window; }

	private:
		std::unique_ptr<Window> window;
		float m_LastFrameTime = 0.0f;	// Time took to render last frame	
		std::vector<Scene*> allScenes;
		bool m_Running = true;
	
		static Application* singleton;	
	};
}
