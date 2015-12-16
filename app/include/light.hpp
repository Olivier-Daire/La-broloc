#pragma once 
#include <glm/glm.hpp>

class Light{
	public:
		Light();
		Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,	glm::vec3 specular,	float constant, float linear, float quadratic);
		glm::vec3 getPosition();
		glm::vec3 getAmbient();
		glm::vec3 getDiffuse();
		glm::vec3 getSpecular();
		float getConstant();
		float getLinear();
		float getQuadratic();

	private:
		glm::vec3 _position;
		glm::vec3 _ambient;
		glm::vec3 _diffuse;
		glm::vec3 _specular;
		float _constant;
		float _linear;
		float _quadratic;
};
