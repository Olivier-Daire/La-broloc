#include "command.hpp"

const float Command::mouseSensitivity = 0.002f;

Command::Command(){}

void Command::commandHandler(glimac::SDLWindowManager& windowManager, Camera& camera, float deltaTime){
	if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_UP)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_z)))
    {
        camera.moveFront(camera.cameraSpeed * deltaTime);
    }
    if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_DOWN)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_s)))
    {
        camera.moveFront(-camera.cameraSpeed * deltaTime);
    }
    if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_LEFT)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_q)))
    {
        camera.moveLeft(camera.cameraSpeed * deltaTime);
    }
    if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_RIGHT)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_d)))
    {
        camera.moveLeft(-camera.cameraSpeed * deltaTime);
    }
}


void Command::mouseManager(Camera& camera, glm::ivec2 mousePosition, float screenWidth, float screenHeight){
    float xoffset = mousePosition.x - screenWidth;
    float yoffset = mousePosition.y - screenHeight;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    camera.m_fPhi += xoffset;
    camera.m_fTheta += yoffset;

    // Prevent weird movements by restraining the angles
    if(camera.m_fTheta > glm::radians(89.0f)){
      camera.m_fTheta =  glm::radians(89.0f);
    }
    if(camera.m_fTheta < glm::radians(-89.0f)){
      camera.m_fTheta = glm::radians(-89.0f);
    }

	camera.computeDirectionVectors();
}
