#pragma once

#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include "util/Util.h"
#include <memory>


#include "Camera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture2D.h"

class Renderer2D {
public:
	Renderer2D();
	Renderer2D(const Renderer2D& o) = delete;
	~Renderer2D();

public:

	static void clear();
	static void init();
	static void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	static OrthoCameraController* setCamera(OrthoCameraController* camera);	// Returns old camera so you can delete it
	
	static void setClearColor(const Color& color);
	
	static void drawRect(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec3& rotation, Color c);
	static void drawRect(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec3& rotation, const Texture2D& texture);
		
	static void draw(const VertexArray& va, const Shader& shader);
	static void draw(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<Shader>& shader);

	static unsigned int drawCallsPerFrame();
	static unsigned int totalDrawCalls();

	static Renderer2D& getInstance() { return *singleton; }
	static OrthoCameraController& getCamera() { return *singleton->m_Camera; }

private:
	void collectStats();
	void checkForInit() const;
	void initializeCache();

private:
	unsigned int m_DrawCallsPerFrame = 0;
	unsigned int m_TotalDrawCalls = 0;

	bool hasInitialized = false;

	Color m_ClearColor;
	Shader* flatColorShader;
	Shader* textureShader;
	OrthoCameraController* m_Camera;

	static Renderer2D* singleton;

	// CACHE
	struct Renderer2DDataCache {

		~Renderer2DDataCache() {
			delete quadFlatColorVAO;
			delete quadTextureVAO;
		}
		
		VertexArray* quadFlatColorVAO;
		VertexArray* quadTextureVAO;
	};

	Renderer2DDataCache* m_Cache;
};

#endif

