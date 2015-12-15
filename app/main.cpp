#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "command.hpp"

using namespace glimac;

int main(int argc, char** argv) {
    GLuint screenWidth = 800, screenHeight = 600;

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

    Shader shader("../assets/shaders/pointlight.vs.glsl", "../assets/shaders/pointlight.fs.glsl");
    
    Model model("../assets/models/nanosuit/nanosuit.obj");
    
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.5f, 0.7f, -4.0f),
        glm::vec3(-0.7f, 0.9f, -4.0f)
    };

    Camera camera;
    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/


    // Application loop:
    float deltaTime = 0.0f;   // Time between current frame and last frame
    float lastFrame = 0.0f;  // Last frame
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        GLfloat currentFrame = windowManager.getTime();;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }
        if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_ESCAPE))){
            done = true;
        }

        Command::commandHandler(windowManager, camera, deltaTime);
        Command::mouseManager(camera, windowManager.getMousePosition(), screenWidth/2.0, screenHeight/2.0);
        // Put the cursor back to the center of the scene
        SDL_WarpMouseInWindow(windowManager.Window, screenWidth/2.0, screenHeight/2.0);


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

        // Set the lighting uniforms
        glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
        // Point light 1
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);     
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);       
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].diffuse"), 1.0f, 1.0f, 1.0f); 
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].linear"), 0.009);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[0].quadratic"), 0.0032);      
        // Point light 2
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);     
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);       
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 1.0f); 
        glUniform3f(glGetUniformLocation(shader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].linear"), 0.009);
        glUniform1f(glGetUniformLocation(shader.Program, "pointLights[1].quadratic"), 0.0032); 

        model.Draw(shader);

        // Update the display
        windowManager.swapBuffers(windowManager.Window);
    }

    return EXIT_SUCCESS;
}
