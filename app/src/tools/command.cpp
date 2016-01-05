#include "tools/command.hpp"
#include <iostream>

const float Command::mouseSensitivity = 0.002f;
direction Command::lastDirection = _NULL;

Command::Command(){}

void Command::commandHandler(glimac::SDLWindowManager& windowManager, Camera& camera, float deltaTime, glm::vec4 wallLimits){
	Camera cameraMove(camera);

    if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_UP)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_z)))
    {
        cameraMove.moveFront(camera.getSpeed() * deltaTime);
        if(!(cameraMove.getPosition().x > wallLimits.x || cameraMove.getPosition().x < wallLimits.y || cameraMove.getPosition().z > wallLimits.z || cameraMove.getPosition().z < wallLimits.w)){
            camera.moveFront(camera.getSpeed() * deltaTime);
            lastDirection = FRONT;
        }      
    }
    if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_DOWN)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_s)))
    {
        cameraMove.moveFront(-camera.getSpeed() * deltaTime);
        if(!(cameraMove.getPosition().x > wallLimits.x || cameraMove.getPosition().x < wallLimits.y || cameraMove.getPosition().z > wallLimits.z || cameraMove.getPosition().z < wallLimits.w)){
            camera.moveFront(-camera.getSpeed() * deltaTime);
            lastDirection = BACK;
        }      
    }
    if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_LEFT)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_q)))
    {        
        cameraMove.moveLeft(camera.getSpeed() * deltaTime);
        if(!(cameraMove.getPosition().x > wallLimits.x || cameraMove.getPosition().x < wallLimits.y || cameraMove.getPosition().z > wallLimits.z || cameraMove.getPosition().z < wallLimits.w)){
            camera.moveLeft(camera.getSpeed() * deltaTime);
            lastDirection = LEFT;
        }       
    }
    if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_RIGHT)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_d)))
    {
        cameraMove.moveLeft(-camera.getSpeed() * deltaTime);
        if(!(cameraMove.getPosition().x > wallLimits.x || cameraMove.getPosition().x < wallLimits.y || cameraMove.getPosition().z > wallLimits.z || cameraMove.getPosition().z < wallLimits.w)){
            camera.moveLeft(-camera.getSpeed() * deltaTime);
            lastDirection = RIGHT;
        }
    }
}

void Command::mouseManager(Camera& camera, glm::ivec2 mousePosition, float screenWidth, float screenHeight){
    float xoffset = mousePosition.x - screenWidth;
    float yoffset = mousePosition.y - screenHeight;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    // Player make a full tour --> change direction
    if (glm::cos(camera.getPhi() + xoffset) < 0)
    {
        if (lastDirection == BACK)
        {
            lastDirection = FRONT;
        }
    } else {
        if (lastDirection == FRONT)
        {
            lastDirection = BACK;    
        }
    }

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
