#pragma once
#include <glimac/SDLWindowManager.hpp>
#include <glm/glm.hpp>
#include "camera.hpp"

class Command
{
	public:
		Command();
		static void commandHandler(glimac::SDLWindowManager& windowManager, Camera& camera, float deltaTime);
		static void mouseManager(Camera& camera, glm::ivec2 mousePosition, float screenWidth, float screenHeight);

	private:
		static const float mouseSensitivity;
};
