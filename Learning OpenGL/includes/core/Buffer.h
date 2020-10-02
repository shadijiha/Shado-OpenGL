#pragma once

#include <cstdint>
#include <vector>
#include "Debug.h"

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
		}

		Debug::Assert(false, "Unknown Shader datatype!");
		return 0;
	}

	struct BufferElement {
		std::string name;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;
		bool normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool Normalized = false)
			: name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(Normalized){
		
		}

		uint32_t getComponentCount() const {
			switch (type)
			{
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3 * 3;
				case ShaderDataType::Mat4:    return 4 * 4;
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;
			}

			Debug::Assert(false, "Unknown ShaderDataType!");
			return 0;
		}

	};

	class BufferLayout {
	public:

		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			calculateOffsetsAndStride();
		}

		inline const std::vector<BufferElement>& getElements() const { return m_Elements; }
		inline uint32_t getStride() const { return m_Stride; }


		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

	private: 
		void calculateOffsetsAndStride() {
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.offset = offset;
				offset += element.size;
				m_Stride += element.size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer {
	public:
		VertexBuffer(float* vertices, uint32_t size);
		virtual ~VertexBuffer();

		virtual void bind() const;
		virtual void unBind() const;

		virtual void setLayout(const BufferLayout& layout) { m_Layout = layout; };
		virtual const BufferLayout& getLayout() const { return m_Layout; };
		
		static std::shared_ptr<VertexBuffer> create(float* vertices, uint32_t size);

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class IndexBuffer {
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~IndexBuffer();

		virtual void bind() const;
		virtual void unBind() const;

		virtual uint32_t getCount() const {
			return m_Count;
		}

		static std::shared_ptr<IndexBuffer> create(uint32_t* indices, uint32_t size);

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
