#include <iostream>

#include "Shado.h"
#include <stdio.h>

#include "collections/LinkedList.h"

using namespace Shado;

class TestScene : public Scene {
public:
	TestScene() :
		Scene("Test scene"), camera(Application::getAspectRatio()), obj("src/core/ressources/test.obj")
	{
	}
	virtual ~TestScene() {}

	void onInit() override {
		
	}
	
	void onUpdate(TimeStep dt) override {
		std::string  s = "FPS " + std::to_string(dt.toFPS());
		Application::get().setWindowTitle(s);

		camera.onUpdate(dt);
	}
	
	void onDraw() override {

		Renderer2D::beginScene(camera.getCamera());
		
		for (float y = -1.5f; y < 1.5f; y += 0.3f)
			for (float x = -1.5f; x < 1.5f; x += 0.3f)
				Renderer2D::drawRect({ x, y , 0 }, { 0.25, 0.25}, {0, 0, 0}, *riven2);
		
		Renderer2D::drawRect({ -0.5, -0.5 , -2 }, { 6, 4}, {0, 0, 0}, *riven2);

		Renderer2D::drawModel(obj, { 0, 0, 0 });
		
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				Color color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Renderer2D::drawRect({ x, y , -5 }, { 0.45f, 0.45f}, {0, 0, 0}, color);
			}
		}		
		
	}
	
	void onDestroy() override{}
	
	void onEvent(Event& e) override {
		camera.onEvent(e);
	}

private:
	std::shared_ptr<Texture2D> riven1 = std::make_shared<Texture2D>("src/core/ressources/riven.png");;
	//Texture2D riven2 = { "src/core/ressources/riven2.jpg" };
	OrbitCameraController camera;
	std::shared_ptr<Texture2D> riven2 = std::make_shared<Texture2D>("src/core/ressources/riven2.jpg");
	Object3D obj;

	float angle;
};

int main(int argc, const char** argv)
{
	auto& application = Application::get();
	application.submit(new TestScene());
	application.run();
	
	Application::destroy();

	std::cin.get();
}
