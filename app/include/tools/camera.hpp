#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

class Camera
{
	public:
		Camera();
		void moveLeft(float t);
		void moveFront(float t);
		void rotateLeft(float degrees);
		void rotateUp(float degrees);
		glm::mat4 getViewMatrix() const;
		void computeDirectionVectors();

		// Getter & setter
		float getSpeed();
		float getTheta();
		void setTheta(float value);
		float getPhi();
		void setPhi(float value);
		glm::vec3 getPosition();


	private: 
		const float defaultYPosition = 0.8f;

		glm::vec3 position;
		float cameraSpeed;
		float m_fPhi;
		float m_fTheta;

		glm::vec3 m_FrontVector;
		glm::vec3 m_LeftVector;
		glm::vec3 m_UpVector;
};
