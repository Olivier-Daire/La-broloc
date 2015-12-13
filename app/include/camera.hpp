#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

class Camera
{
	public:
		glm::vec3 position;
		float m_fPhi;
		float m_fTheta;

		glm::vec3 m_FrontVector;
		glm::vec3 m_LeftVector;
		glm::vec3 m_UpVector;
		Camera();
		void moveLeft(float t);
		void moveFront(float t);
		void rotateLeft(float degrees);
		void rotateUp(float degrees);
		glm::mat4 getViewMatrix() const;

	private:
		void computeDirectionVectors();
};
