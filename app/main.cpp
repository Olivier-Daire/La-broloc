#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"

using namespace glimac;

int main(int argc, char** argv) {
    GLuint screenWidth = 800, screenHeight = 600;
    float cameraSpeed = 0.05f;
    float lastMouseX = screenWidth/2, lastMouseY = screenHeight/2;

    // Initialize SDL and open a window
    SDLWindowManager windowManager(screenWidth, screenHeight, "La Broloc");

    // Initialize glew for OpenGL3+ support
    glewExperimental = GL_TRUE; 
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    Shader shader("../assets/shaders/default.vs.glsl", "../assets/shaders/default.fs.glsl");
    Model model("../assets/models/nanosuit/nanosuit.obj");

    Camera camera;
    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/


    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }
        if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_ESCAPE))){
            done = true;
        }
        if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_UP)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_z)))
        {
            camera.moveFront(cameraSpeed);
        }
        if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_DOWN)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_s)))
        {
            camera.moveFront(-cameraSpeed);
        }
        if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_LEFT)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_q)))
        {
            camera.moveLeft(cameraSpeed);
        }
        if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_RIGHT)) || windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_d)))
        {
            camera.moveLeft(-cameraSpeed);
        }
        if (windowManager.isMouseButtonPressed(SDL_BUTTON_LEFT))
        {
            glm::ivec2 mousePosition = windowManager.getMousePosition();
            camera.mouseManager(mousePosition, lastMouseX, lastMouseY);
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        // Transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        
        glm::mat4 view = camera.getViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 matModel;
        // Translate model to the center of the scene
        matModel = glm::translate(matModel, glm::vec3(0.0f, -1.75f, -5.0f));
        matModel = glm::scale(matModel, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(matModel));

        model.Draw(shader);

        // Update the display
        windowManager.swapBuffers(windowManager.Window);
    }

    return EXIT_SUCCESS;
}
