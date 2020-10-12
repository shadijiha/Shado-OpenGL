﻿#pragma once
#include "VertexArray.h"
#include "cameras/Camera.h"

namespace Shado {
	
	class Object3D {
	public:
		Object3D(const std::string& filename);
		~Object3D() = default;
		
		std::shared_ptr<VertexArray> getVertexArray() const { return  vao; }

	private:
		std::shared_ptr<VertexArray> vao;
		std::shared_ptr<IndexBuffer> indexBuffer;
		std::shared_ptr<VertexBuffer> vertexBuffer;		
	};
	
}
