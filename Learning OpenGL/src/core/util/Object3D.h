#pragma once
#include <memory>
#include <string>
#include <vector>
#include "../Buffer.h"
#include "../VertexArray.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace Shado {
	
	class Object3D {
	public:
		Object3D(const std::string& filename);
		virtual ~Object3D();

		const std::shared_ptr<VertexArray>& getVertexArray() const { return vao; }
		const std::shared_ptr<IndexBuffer>& getIndexBuffer() const { return indices; }
	
	private:
		std::vector<glm::vec4> vertices;
		std::vector<glm::vec3> normals;
		std::vector<uint32_t> elements;

		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<IndexBuffer> indices;
		std::shared_ptr<VertexArray> vao;
	};
}
