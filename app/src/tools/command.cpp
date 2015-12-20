#include "tools/command.hpp"

const float Command::mouseSensitivity = 0.002f;

Command::Command(){}

void Command::commandHandler(glimac::SDLWindowManager& windowManager, Camera& camera, float deltaTime){
	if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_UP)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_z)))
    {
        camera.moveFront(camera.getSpeed() * deltaTime);
    }
    if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_DOWN)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_s)))
    {
        camera.moveFront(-camera.getSpeed() * deltaTime);
    }
    if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_LEFT)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_q)))
    {
        camera.moveLeft(camera.getSpeed() * deltaTime);
    }
    if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_RIGHT)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_d)))
    {
        camera.moveLeft(-camera.getSpeed() * deltaTime);
    }
}


void Command::mouseManager(Camera& camera, glm::ivec2 mousePosition, float screenWidth, float screenHeight){
    float xoffset = mousePosition.x - screenWidth;
    float yoffset = mousePosition.y - screenHeight;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    //m_fPhi += xoffset;
    camera.setPhi(camera.getPhi() + xoffset);
    //m_fTheta += yoffset;
    camera.setTheta(camera.getTheta() + yoffset);

    // Prevent weird movements by restraining the angles
    if(camera.getTheta() > glm::radians(89.0f)){
      camera.setTheta(glm::radians(89.0f));
    }
    if(camera.getTheta() < glm::radians(-89.0f)){
      camera.setTheta(glm::radians(-89.0f));
    }

	camera.computeDirectionVectors();
}
