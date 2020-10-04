#define _CRT_SECURE_NO_WARNINGS
#include "Object3D.h"
#include "glm/vec4.hpp"
#include <glm/glm.hpp>

namespace Shado {

	static void vectorToArray() {
		
	}
	
	Object3D::Object3D(const std::string& filename) {

        using namespace std;
		
        ifstream in(filename, ios::in);
        if (!in)
        {
            Debug::Assert(false, "Cannot open " + filename);
        }

        string line;
        while (getline(in, line))
        {
            if (line.substr(0, 2) == "v ")
            {
                istringstream s(line.substr(2));
                glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
                vertices.push_back(v);
            } else if (line.substr(0, 2) == "f ")
            {
                istringstream s(line.substr(2));
                uint32_t a, b, c;
                s >> a; s >> b; s >> c;
                a--; b--; c--;
                elements.push_back(a); elements.push_back(b); elements.push_back(c);
            } else if (line[0] == '#')
            {
                /* ignoring this line */
            } else
            {
                /* ignoring this line */
            }
        }

        normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
        for (int i = 0; i < elements.size(); i += 3)
        {
            uint32_t ia = elements[i];
            uint32_t ib = elements[i + 1];
            uint32_t ic = elements[i + 2];
            // glm::vec3 normal = glm::normalize(glm::cross(
            //     glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
            //     glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
            // normals[ia] = normals[ib] = normals[ic] = normal;
        }

        vertexBuffer = VertexBuffer::create((float*)&vertices[0], vertices.size() * 4 * sizeof(float));
        vertexBuffer->setLayout({
            {ShaderDataType::Float4, "a_Position"}
        });
		
        indices = IndexBuffer::create(&elements[0], elements.size());
        vao = VertexArray::create();
        vao->addVertexBuffer(vertexBuffer);
        vao->setIndexBuffer(indices);
	}
	
	Object3D::~Object3D() {
	}
	
}
