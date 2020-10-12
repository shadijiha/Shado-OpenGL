#include "Application.h"

#include "Debug.h"
#include "GL/glew.h"
#include "Renderer2D.h"
#include <algorithm>
#include "cameras/OrthoCamera.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Renderer3D.h"
#include "util/Random.h"

namespace Shado {
	
	Scene::Scene(const std::string& name, int zIndex)
		: m_Name(name), m_Id(rand() % UINT64_MAX), zIndex(zIndex)
	{
	}

	Scene::Scene(const std::string& name)
		: Scene(name, 0)
	{
	}

	// =========================== APPLICATION CLASS ===========================

	Application* Application::singleton = new Application();

	Application::Application(unsigned width, unsigned height, const std::string& title)
	{
		Log::init();
		Random::init();
		
		/* Initialize the library */
		window = std::make_unique<Window>(width, height, title);
	}

	Application::Application()
		: Application(1280, 720, "Shado OpenGL simple Rendering engine")
	{
	}

	Application::~Application() {
		
		for (Scene* scene : allScenes) {
			if (scene == nullptr)
				continue;
		
			scene->onDestroy();
			delete scene;
		}		

		glfwTerminate();
	}

	void Application::run() {

		// Init Renderer if it hasn't been done
		if (!Renderer2D::hasInitialized()) {
			Renderer2D::Init();
			Renderer3D::Init();
		}

		if (allScenes.size() == 0)
			SHADO_WARN("No Scenes to draw");

		/* Loop until the user closes the window */
		while (m_Running)	{

			float time = (float)glfwGetTime();	// TODO: put it in platform specific
			float timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			
			/* Render here */
			Renderer2D::Clear();

			// Draw scenes here
			for (Scene* scene : allScenes) {
				if (scene == nullptr) {
					continue;
				}
			
				scene->onUpdate(timestep);
				scene->onDraw();
			}

			/* Swap front and back buffers */
			/* Poll for and process events */
			window->onUpdate();		
		}
	}

	void Application::submit(Scene* scene) {

		// Init Renderer if it hasn't been done
		if (!Renderer2D::hasInitialized()) {
			Renderer2D::Init();
			Renderer3D::Init();
		}
	
		scene->onInit();
		allScenes.push_back(scene);

		// Reverse sorting
		std::sort(allScenes.begin(), allScenes.end(), [](Scene* a, Scene* b) {
			return a->getZIndex() < b->getZIndex();
		});
	}

	void Application::onEvent(Event& e) {
		// Distaptch the event and excute the required code
	
		// Pass Events to layer
		for (Scene* scene : allScenes) {
			scene->onEvent(e);
			if (e.isHandled())
				break;
		}
	}

	void Application::destroy() {

		for (Scene*& scene : singleton->allScenes) {
			scene->onDestroy();
			delete scene;
			scene = nullptr;
		}

		singleton->m_Running = false;
	
		delete singleton;
	}

	void Application::close() {
		singleton->m_Running = false;
	}

}
