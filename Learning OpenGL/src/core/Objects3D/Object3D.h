#pragma once
#include "VertexArray.h"
#include "cameras/Camera.h"

namespace Shado {
	
	class Object3D {
	public:
		Object3D(const std::string& filename);		
		virtual ~Object3D() = default;
		
		std::shared_ptr<VertexArray> getVertexArray() const { return  vao; }

	protected:
		Object3D() = default;
		
	protected:
		std::shared_ptr<VertexArray> vao;
		std::shared_ptr<IndexBuffer> indexBuffer;
		std::shared_ptr<VertexBuffer> vertexBuffer;		
	};
	
}
