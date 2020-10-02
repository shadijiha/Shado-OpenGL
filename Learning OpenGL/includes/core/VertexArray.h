#pragma once

#include "Buffer.h"

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		virtual void bind() const;
		virtual void unBind() const;

		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& vertexBuffer);

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const { return m_VertexBuffers; };
		virtual const std::shared_ptr<IndexBuffer>& getIndexBuffers() const { return m_IndexBuffers; };

		static std::shared_ptr<VertexArray> create();

	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffers;
	};
