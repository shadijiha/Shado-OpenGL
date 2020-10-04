﻿#pragma once

#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include "../util/Util.h"
#include <memory>
#include <unordered_map>
#include "../cameras/OrthoCamera.h"
#include "../Shader.h"
#include "../VertexArray.h"
#include "../Texture2D.h"

namespace Shado::Experimantal {

	inline std::string FLAT_COLOR_SHADER_PATH = FILE_PATH + "\\src\\core\\ressources\\FlatColorShader.glsl";
	inline std::string TEXTURE2D_SHADER_PATH = FILE_PATH + "\\src\\core\\ressources\\TextureShader.glsl";
	
	class Renderer2D
	{
	public:
		static void init();
		static void shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const Camera& camera); // TODO: Remove
		static void EndScene();
		static void Flush();
		static void setClearColor(const glm::vec4& color);
		static void clear();

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static bool hasInitialized() { return s_Init; }
		
		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();
	private:
		static void FlushAndReset();
		static void CmdDrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);
		static bool s_Init;
	};
}

#endif
