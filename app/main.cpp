#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "shader.hpp"
#include "model/model.hpp"
#include "tools/camera.hpp"
#include "tools/command.hpp"
#include "scene.hpp"
#include "tools/text.hpp"

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
    Shader shaderText("../assets/shaders/text.vs.glsl", "../assets/shaders/text.fs.glsl");
    Shader wallShader("../assets/shaders/tex2D.vs.glsl", "../assets/shaders/tex2D.fs.glsl");
    
    Scene scene1;
    scene1.loadSceneFromFile("../assets/scenes/scene1.xml");

    // Load all the XML models using their path
    Model models[scene1.getModelNumber()];
    for (int i = 0; i < scene1.getModelNumber(); ++i)
    {
        models[i] = Model(scene1.getModel(i).Path);
    }

    // FIXME list working models, remove it afterwards
    //Model kitchen("../assets/models/bedroom/bedside_table/Skin A/Table de nuit_Final.obj");
    //Model kitchen("../assets/models/bathroom/washbasin/[.obj]/Lavabo.obj");
    //Model kitchen("../assets/models/living_room/Table/Table.obj");
    //Model kitchen("../assets/models/bedroom/heater/[.obj]/Radiateur.obj");
    //Model kitchen("../assets/models/living_room/modern-closet/[.obj]/Modern Closet.obj");
    //Model kitchen("../assets/models/living_room/modern-sideboard/[.obj]/Buffet Moderne.obj");
    //Model kitchen("../assets/models/bedroom/lit/lit.obj");
    //Model kitchen("../assets/models/bathroom/heater/radiateur.obj");
    //Model kitchen("../assets/models/bathroom/soap/com_bath2_soap.obj");
    //Model kitchen("../assets/models/bathroom/potBrossesADent/potBrossesADent.obj");

    Camera camera;

    // Text variables
    Text text;

    bool answer = 0, isAnswer = 0, isDialogue = 1;
    int cptDialogue = 0, nbAnswer = 2, chooseAnswer = 0;
    std::string dialogue;
    std::string answers[nbAnswer];

    // TODO Load text inside scene
    text.LoadText(shaderText, screenWidth, screenHeight);
    dialogue = scene1.getDialogue(0).getMessage();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // Application loop:
    float deltaTime = 0.0f;   // Time between current frame and last frame
    float lastFrame = 0.0f;  // Last frame
    bool done = false;


    cout << "x: " << model.box.x << endl;
    cout << "y: " << model.box.y << endl;
    cout << "z: " << model.box.z << endl;
    cout << "w: " << model.box.w << endl;
    cout << "h: " << model.box.h << endl;
    cout << "d: " << model.box.d << endl;
    cout << "caméra: x : " << camera.getPosition().x << " y : " << camera.getPosition().y << " z: " <<  camera.getPosition().z << endl;

    while(!done) {

        //AABB cameraBox((camera.getPosition().x-(screenWidth/2.0)),(camera.getPosition().y-(screenHeight/2.0)),camera.getPosition().z,screenWidth,screenHeight,0.0f);
        AABB cameraBox((camera.getPosition().x),(camera.getPosition().y),camera.getPosition().z,screenWidth,screenHeight,0.0f);
        // Event loop:
        SDL_Event e;
        GLfloat currentFrame = windowManager.getTime();;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

    //         cout << "x: " << model.box.x << endl;
    // cout << "y: " << model.box.y << endl;
    // cout << "z: " << model.box.z << endl;
    // cout << "w: " << model.box.w << endl;
    // cout << "h: " << model.box.h << endl;
    // cout << "d: " << model.box.d << endl;
    // cout << "caméra: " << camera.getPosition().x << "y :" << camera.getPosition().y << "z: " <<  camera.getPosition().z << endl;

         cout << "caméra: " << camera.getPosition().x << "y :" << camera.getPosition().y << "z: " <<  camera.getPosition().z << endl;
        if(model.box.collision(cameraBox))
         cout << "COLLISIOOOONS " << endl;
        else cout << "PAS COLLISION" << endl;

        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
            if( e.type == SDL_KEYDOWN ) {
                switch( e.key.keysym.sym )
                {
                    case SDLK_SPACE:
                        text.nextText(isDialogue, isAnswer, answer, cptDialogue, scene1, dialogue, answers);
                    break;
                    case SDLK_RIGHT:
                        chooseAnswer++;
                        if(chooseAnswer == nbAnswer) chooseAnswer = 0;
                    break;
                    case SDLK_LEFT:
                        if(chooseAnswer == 0) chooseAnswer = nbAnswer;
                        chooseAnswer--;
                    break;
                }
            }
        }
        if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_ESCAPE))){
            done = true;
        }

        if(!isDialogue) Command::commandHandler(windowManager, camera, deltaTime);
        Command::mouseManager(camera, windowManager.getMousePosition(), screenWidth/2.0, screenHeight/2.0);
        // Put the cursor back to the center of the scene
        SDL_WarpMouseInWindow(windowManager.Window, screenWidth/2.0, screenHeight/2.0);


        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        
        //***** ROOM *****//
        wallShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(wallShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(wallShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        scene1.drawRoom(wallShader);

        //***** MODEL *****//
        shader.Use();
        
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        

        for (int i = 0; i < scene1.getModelNumber(); ++i)
        {
            glm::mat4 matModel;
            // Translate model following the parameters set in the XML
            matModel = glm::translate(matModel, scene1.getModel(i).Translate);
            // Scale model following the parameters set in the XML
            if (scene1.getModel(i).RotateAngle != 0)
            {
                matModel = glm::rotate(matModel, glm::radians(scene1.getModel(i).RotateAngle), glm::vec3(-0.0f, -1.0f, 0.0f));
            }
            matModel = glm::scale(matModel, scene1.getModel(i).Scale);
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(matModel));
            models[i].Draw(shader);
            
        }

        //***** LIGHT *****//
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

        text.Draw(shaderText,isDialogue, isAnswer, chooseAnswer, dialogue, answers);

        // Update the display
        windowManager.swapBuffers(windowManager.Window);

    }

    scene1.deleteRoom();

    return EXIT_SUCCESS;
}
