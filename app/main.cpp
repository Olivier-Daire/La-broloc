#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "shader.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "command.hpp"
#include "scene.hpp"
#include "text.hpp"

using namespace glimac;

int main(int argc, char** argv) {
    GLuint screenWidth = 800, screenHeight = 600;

    // Initialize SDL and open a window
    SDLWindowManager windowManager(screenWidth, screenHeight, "La Broloc");

    Text text;

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
    Shader shaderText("../assets/shaders/text.vs.glsl", "../assets/shaders/text.fs.glsl");

    Scene scene1;
    scene1.loadSceneFromFile("../assets/scenes/scene1.xml");

    // FIXME Test remove this afterwards
    // cout << scene1.getModelPath(1) << endl;
    // cout << scene1.getDialogue(0).getMessage() << endl;
    // cout << scene1.getDialogue(0).getAnswer(1) << endl;

    Model model("../assets/models/nanosuit/nanosuit.obj");

    Camera camera;

    bool answer = 0;
    bool isAnswer = 0;
    int cptDialogue = 0;
    int nbAnswer = 2;

    std::string dialogue;
    std::string answers[nbAnswer];

    text.LoadText(shaderText,screenWidth, screenHeight);
    dialogue = scene1.getDialogue(0).getMessage();


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
            if( e.type == SDL_KEYDOWN ) {
                switch( e.key.keysym.sym )
                {
                    case SDLK_SPACE:
                        text.nextText(isAnswer, answer, cptDialogue, scene1, dialogue, answers);
                    break;
                }
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

        // Lights
        for (int i = 0; i < scene1.getLightNumber(); ++i)
        {
            string pointLight = "pointLights[" + to_string(i) + "]";
            Light light = scene1.getLight(i);

            glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".position").c_str()), light.getPosition().x, light.getPosition().y, light.getPosition().z);
            glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".ambient").c_str()), light.getAmbient().x, light.getAmbient().y, light.getAmbient().z);
            glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".diffuse").c_str()), light.getDiffuse().x, light.getDiffuse().y, light.getDiffuse().z);
            glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".specular").c_str()), light.getSpecular().x, light.getSpecular().y, light.getSpecular().z);
            glUniform1f(glGetUniformLocation(shader.Program, (pointLight + ".constant").c_str()), light.getConstant());
            glUniform1f(glGetUniformLocation(shader.Program, (pointLight + ".linear").c_str()), light.getLinear());
            glUniform1f(glGetUniformLocation(shader.Program, (pointLight + ".quadratic").c_str()), light.getQuadratic());
        }

        model.Draw(shader);
       
        // Draw texts
        if(dialogue != "") {
            if(!isAnswer)
                text.RenderText(shaderText, dialogue, 25.0f, 100.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.4f));
            else {
                text.RenderText(shaderText, answers[0], 100.0f, 100.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.4f));
                text.RenderText(shaderText, answers[1], 300.0f, 100.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.4f));
            }
        }

        // Update the display
        windowManager.swapBuffers(windowManager.Window);
    }

    return EXIT_SUCCESS;
}
