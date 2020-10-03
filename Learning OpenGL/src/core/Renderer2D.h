#pragma once

#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include "util/Util.h"
#include <memory>
#include <unordered_map>
#include "cameras/OrthoCamera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture2D.h"
#include "util/Object3D.h"

namespace Shado {

	inline std::string FLAT_COLOR_SHADER_PATH = FILE_PATH + "\\src\\core\\ressources\\FlatColorShader.glsl";
	inline std::string TEXTURE2D_SHADER_PATH = FILE_PATH + "\\src\\core\\ressources\\TextureShader.glsl";
	
	class Renderer2D {

		friend class Application;

	public:
		Renderer2D();
		Renderer2D(const Renderer2D& o) = delete;
		~Renderer2D();

	public:

		static void clear();
		static void init();
		static void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

		static void setClearColor(const Color& color);
		static void beginScene(const Camera& camera);

		static void drawRect(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec3& rotation = { 0, 0, 0 }, const Color& c = { 1, 1, 1 });
		static void drawRect(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec3& rotation, Shader& shader);
		static void drawRect(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec3& rotation, const Texture2D& texture);

		static void drawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& rotation = { 0, 0, 0 }, const Color& c = { 1, 1, 1 });
		static void drawEllipse(const glm::vec3& position, const glm::vec2& radiuses, const glm::vec3& rotation, const Color& c);
		static void drawLine(const glm::vec3& p0, const glm::vec3& p1, Color color = Color(1.0f, 1.0f, 1.0f));

		static void drawModel(const Object3D& object, const glm::vec3& position);
		
		static void draw(const VertexArray& va, const Shader& shader);
		static void draw(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<Shader>& shader);

		static unsigned int drawCallsPerFrame();
		static unsigned int totalDrawCalls();

		static Renderer2D& getInstance() { return *singleton; }
		static bool hasInitialized() { return singleton->m_hasInitialized; }

	private:
		void collectStats();
		void checkForInit() const;
		void initializeCache();

	private:
		unsigned int m_DrawCallsPerFrame = 0;
		unsigned int m_TotalDrawCalls = 0;

		bool m_hasInitialized = false;

		Color m_ClearColor;
		Shader* flatColorShader;
		Shader* textureShader;

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
}

#endif

