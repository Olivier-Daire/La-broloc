#include "camera.hpp"

Camera::Camera(){
	position = glm::vec3(0, 0, 0);
	m_fPhi = M_PI;
	m_fTheta = 0;
	mouseSensitivity = 0.002f;
	cameraSpeed = 1.2f;

	computeDirectionVectors();
}

void Camera::computeDirectionVectors(){
	m_FrontVector = glm::vec3(cos(m_fTheta)*sin(m_fPhi), sin(m_fTheta), cos(m_fTheta)*cos(m_fPhi));
	m_LeftVector = glm::vec3(sin(m_fPhi + M_PI_2), 0, cos(m_fPhi + M_PI_2));
	m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
}

void Camera::moveLeft(float t){
	position += t*m_LeftVector;
	// Prevent the player from flying
	position.y = 0.0f;
}

void Camera::moveFront(float t){
	position += t*m_FrontVector;
	// Prevent the player from flying
	position.y = 0.0f;
}

void Camera::rotateLeft(float degrees){
	m_fPhi += glm::radians(degrees);
}

void Camera::rotateUp(float degrees){
	m_fTheta += glm::radians(degrees);
}

glm::mat4 Camera::getViewMatrix() const{
	return glm::lookAt(position, position + m_FrontVector, m_UpVector);
}

void Camera::mouseManager(glm::ivec2 mousePosition, float screenWidth, float screenHeight){
    float xoffset = mousePosition.x - screenWidth;
    float yoffset = mousePosition.y - screenHeight;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    m_fPhi += xoffset;
    m_fTheta += yoffset;

    // Prevent weird movements by restraining the angles
    if(m_fTheta > glm::radians(89.0f)){
      m_fTheta =  glm::radians(89.0f);
    }
    if(m_fTheta < glm::radians(-89.0f)){
      m_fTheta = glm::radians(-89.0f);
    }

	computeDirectionVectors();

}
