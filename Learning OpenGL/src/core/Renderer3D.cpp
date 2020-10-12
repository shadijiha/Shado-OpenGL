#include "Renderer3D.h"

#include "GL/glew.h"
#include "Renderer2D.h"
#include "Shader.h"

namespace Shado {

	struct Renderer3DData {
		Shader* flatColorShader;

		glm::mat4 viewProj;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init() {
		s_Data.flatColorShader = new Shader(OBJECT3D_DEFAULT_SHADER_PATH);
	}

	void Renderer3D::Clear() {
	}

	void Renderer3D::BeginScene(const Camera& camera) {
		s_Data.viewProj = camera.getViewProjectionMatrix();
	}

	void Renderer3D::EndScene() {
	}

	void Renderer3D::DrawModel(const std::shared_ptr<Object3D>& mesh, const glm::vec3& position, const glm::vec3& scale,
		const glm::vec4& modelColor, const DiffuseLight& light, bool fill) {

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), scale);
		
		s_Data.flatColorShader->bind();
		s_Data.flatColorShader->setMat4("u_ViewProjection", s_Data.viewProj);
		s_Data.flatColorShader->setMat4("u_Transform", transform);
		s_Data.flatColorShader->setFloat4("u_Color", modelColor);

		s_Data.flatColorShader->setFloat3("u_LightPos", light.getPosition());
		s_Data.flatColorShader->setFloat3("u_LightColor", light.getColor());
		
		const auto& vao = mesh->getVertexArray();
		vao->bind();

		{
			auto mode = GL_TRIANGLES;
			if (!fill)
				mode = GL_LINES;
			
			glDrawElements(mode, vao->getIndexBuffers()->getCount(), GL_UNSIGNED_INT, nullptr);
		}		
	}
}
