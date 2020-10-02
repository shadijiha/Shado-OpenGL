#pragma once
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

class OrthoCamera {
public:
	OrthoCamera(float left, float right, float bottom, float top);

	const glm::vec3& getPosition() const { return m_Position; }
	float getRotation() const { return m_Rotation; }

	void setPosition(const glm::vec3& position) { m_Position = position; reCalculateViewMatrix(); }
	void setRotation(float rotation) {
		m_Rotation = rotation;
		reCalculateViewMatrix();
	}

	void setProjection(float left, float right, float bottom, float top);

	const glm::mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
	const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
	const glm::mat4& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

private:
	void reCalculateViewMatrix();

private:
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_viewProjectionMatrix;

	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	float m_Rotation = 0.0;
};

class OrthoCameraController {
public:
	OrthoCameraController(float aspectRatio, bool rotation = false);		// Zoom level: 1.0, X: aspect ratio * units of space in Y (default = 2 units in Y)

	void onUpdate(float dt);
	void onEvent(Event& e);

	OrthoCamera& getCamera() { return m_Camera; }
	const OrthoCamera& getCamera() const { return m_Camera; }

	// Setters
	void setRotationSpeed(float degrees_per_second) { m_CameraRotationSpeed = degrees_per_second; }
	void setTranslationSpeed(float camera_speed) { m_CameraTranslationSpeed = camera_speed; }

private:
	bool onMouseScrolled(MouseScrolledEvent& e);
	bool onWindowResized(WindowResizeEvent& e);

private:
	float m_aspectRatio;
	float m_ZoomLevel = 1.0f;
	OrthoCamera m_Camera;

	float m_Rotation = false;

	float m_CameraRotation = 0.0f;
	glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };

	float m_CameraTranslationSpeed = 1.0f;
	float m_CameraRotationSpeed = 180.0f;
};