#include "tools/camera.hpp"

Camera::Camera(){
	position = glm::vec3(0, defaultYPosition, 0);
	m_fPhi = M_PI;
	m_fTheta = 0;
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
	position.y = defaultYPosition;
}

void Camera::moveFront(float t){
	position += t*m_FrontVector;
	// Prevent the player from flying
	position.y = defaultYPosition;
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


float Camera::getSpeed(){
	return cameraSpeed;
}

float Camera::getTheta(){
	return m_fTheta;
}

void Camera::setTheta(float value){
	m_fTheta = value;
}

float Camera::getPhi(){
	return m_fPhi;
}

void Camera::setPhi(float value){
	m_fPhi = value;
}

glm::vec3 Camera::getPosition(){
	return position;
}

