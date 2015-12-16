#include "light.hpp"

Light::Light(){

}

Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,	glm::vec3 specular,	float constant, float linear, float quadratic) : 
_position(position), _ambient(ambient), _diffuse(diffuse), _specular(specular), _constant(constant), _linear(linear), _quadratic(quadratic){}

glm::vec3 Light::getPosition(){
	return _position;
}

glm::vec3 Light::getAmbient(){
	return _ambient;
}

glm::vec3 Light::getDiffuse(){
	return _diffuse;
}

glm::vec3 Light::getSpecular(){
	return _specular;
}

float Light::getConstant(){
	return _constant;
}

float Light::getLinear(){
	return _linear;
}

float Light::getQuadratic(){
	return _quadratic;
}

