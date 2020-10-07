#include "Shado.h"

using namespace Shado;

class TestScene : public Scene {
public:
	TestScene() :
		Scene("Test scene"), camera(Application::getAspectRatio())
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

		Renderer2D::BeginScene(camera.getCamera());
		
		for (float y = -1.5f; y < 1.5f; y += 0.3f)
			for (float x = -1.5f; x < 1.5f; x += 0.3f)
				Renderer2D::DrawQuad({ x, y , -2 }, { 0.25, 0.25}, riven1);
		
		Renderer2D::DrawQuad({ -0.5, -0.5 , -5 }, { 6, 4}, riven2);
		
		
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Renderer2D::DrawQuad({ x, y , -10}, { 0.45f, 0.45f }, color);
			}
		}		
		
		Renderer2D::EndScene();
	}
	
	void onDestroy() override{}
	
	void onEvent(Event& e) override {
		camera.onEvent(e);
	}

private:
	std::shared_ptr<Texture2D> riven1 = std::make_shared<Texture2D>("src/core/ressources/test.jpg");
	//Texture2D riven2 = { "src/core/ressources/riven2.jpg" };
	OrbitCameraController camera;
	std::shared_ptr<Texture2D> riven2 = std::make_shared<Texture2D>("src/core/ressources/riven2.jpg");

	float angle;
};

int main(int argc, const char** argv)
{

	auto& application = Application::get();
	application.submit(new TestScene());
	application.run();

	Application::destroy();
}
