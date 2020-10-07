#include "Application.h"

#include "Debug.h"
#include "GL/glew.h"
#include "Renderer2D.h"
#include <algorithm>
#include "cameras/OrthoCamera.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

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
		
		/* Initialize the library */
		if (!glfwInit())
			SHADO_CORE_ASSERT(false, "Failed to initialize GLFW!");

		m_Data.title = title;
		m_Data.width = width;
		m_Data.height = height;

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			SHADO_CORE_ASSERT(false, "Failed to create window");
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		if (glewInit() != GLEW_OK) {		
			glfwTerminate();
			SHADO_CORE_ASSERT(false, "Failed to create GLEW context");
		}

		glfwSetWindowUserPointer(window, &m_Data);
		listenToEvents();
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

	void Application::listenToEvents() {

		m_Data.eventCallback = BIND_EVENT_FN(onEvent);
	
		// Set GLFW callbacks
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.eventCallback(event);
		});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.eventCallback(event);
		});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.eventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.eventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(keycode);
			data.eventCallback(event);
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.eventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.eventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.eventCallback(event);
		});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.eventCallback(event);
		});
	}

	void Application::run() {

		// Init Renderer if it hasn't been done
		if (!Renderer2D::hasInitialized()) {
			Renderer2D::Init();
		}

		if (allScenes.size() == 0)
			SHADO_WARN("No Scenes to draw");

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))	{

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
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}

	void Application::submit(Scene* scene) {

		// Init Renderer if it hasn't been done
		if (!Renderer2D::hasInitialized()) {
			Renderer2D::Init();
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
		//EventDispatcher dispatcher(e);
		//dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
		//dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResize));
	
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
	
		delete singleton;
	}

	void Application::setWindowTitle(const std::string& s) const {
		glfwSetWindowTitle(window, s.c_str());
	}
}
