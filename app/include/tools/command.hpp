#pragma once
#include <glimac/SDLWindowManager.hpp>
#include <glm/glm.hpp>
#include "camera.hpp"

enum direction
{
	FRONT = 0,
	BACK = 1,
	LEFT = 2,
	RIGHT = 3,
	_NULL = 4
};

class Command
{
	public:
		Command();
		static void commandHandler(glimac::SDLWindowManager& windowManager, Camera& camera, float deltaTime, bool& collision);
		static void mouseManager(Camera& camera, glm::ivec2 mousePosition, float screenWidth, float screenHeight);

	private:
		static const float mouseSensitivity;
		static direction lastDirection;
};
