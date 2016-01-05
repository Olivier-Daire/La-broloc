#include "application.hpp"

using namespace glimac;

Application::Application() {}

std::string Application::launch(std::string currentScene) {

    GLuint screenWidth = 800, screenHeight = 600;

    // Initialize SDL and open a window
    SDLWindowManager windowManager(screenWidth, screenHeight, "La Broloc");

    // Initialize glew for OpenGL3+ support
    glewExperimental = GL_TRUE; 
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return "fin";
    }

    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********** END INIT ************/
    Camera camera;
    // Text related data
    Text text;
    bool answer = 0, isAnswer = 0, isDialogue = 1;
    int cptDialogue = 0, nbAnswer = 2, chooseAnswer = 0;
    std::string dialogue;
    std::string answers[nbAnswer];
    int group = 0;
    // Collision related data 
    bool isCollision = false;
    int modelCollision = 0;

    std::string nextScene;
    Scene scene;

    Shader shader("../assets/shaders/pointlight.vs.glsl", "../assets/shaders/pointlight.fs.glsl");
    Shader shaderText("../assets/shaders/text.vs.glsl", "../assets/shaders/text.fs.glsl");
    Shader wallShader("../assets/shaders/tex2D.vs.glsl", "../assets/shaders/tex2D.fs.glsl");
    
    // Load models infos, room, dialogues, lights
    scene.loadSceneFromFile(("../assets/scenes/" + currentScene  + ".xml").c_str());

    // Load all the XML models using their path
    Model models[scene.getModelNumber()];
    for (int i = 0; i < scene.getModelNumber(); ++i)
    {
        ModelInfos model = scene.getModel(i);
        models[i] = Model(model.Path);

        // Update model aabbox given its translate and scale
        // FIXME doesn't work well and need to be moved to a function
        models[i].box.x =  model.Translate.x;
        models[i].box.y =  model.Translate.y;
        models[i].box.z =  model.Translate.z;
        models[i].box.w *= model.Scale.x;
        models[i].box.h *= model.Scale.y;
        models[i].box.d *= model.Scale.z;
    }

    text.LoadText(shaderText, screenWidth, screenHeight);

    dialogue = scene.getDialogue(group, 0).getMessage();
    

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    // Application loop:
    float deltaTime = 0.0f;   // Time between current frame and last frame
    float lastFrame = 0.0f;  // Last frame
    bool done = false;
    while(!done) {

        /* Create a smal box for the camera (player)
         * the last three numbers are for the distance
         * 0.2 from object on X axis --> collide
         * 5.0 (and maybe more ?) on Y axis because we need to collide on the whole height as if we were a person
         * 0.2 from object on Z axis --> collide */
        AABB cameraBox(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z, 0.5f, 500.0f, 0.5f);
        
        // Event loop:
        SDL_Event e;
        GLfloat currentFrame = windowManager.getTime();;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                nextScene = "fin";
                done = true; // Leave the loop after this iteration
            }
            if( e.type == SDL_KEYDOWN ) {
                switch( e.key.keysym.sym )
                {
                    case SDLK_e:

                        if(!isDialogue && isCollision) {
                            // If we can interact with the model in collision 
                            if (scene.getModel(modelCollision).interactionDialogue != -1)
                            {
                                cptDialogue = 1; // FIXME Why do I need to do that ?!
                                isDialogue = 1;
                                group = scene.getModel(modelCollision).interactionDialogue;
                            }
                           
                            dialogue = scene.getDialogue(group, 0).getMessage();
                        }
                    break;

                    case SDLK_SPACE:
                        if(isAnswer) {
                            istringstream iss(answers[chooseAnswer]);
                            std::string word;
                            iss >> word;
                            nextScene = word;
                            done = true;
                        }
                        if(isDialogue) text.nextText(isDialogue, isAnswer, answer, cptDialogue, scene, dialogue, answers, group);
            
                    break;
                    case SDLK_RIGHT:
                       if(isAnswer) {
                            chooseAnswer++;
                            if(chooseAnswer == nbAnswer) chooseAnswer = 0;
                        }
                    break;
                    case SDLK_LEFT:
                        if(isAnswer) {
                            if(chooseAnswer == 0) chooseAnswer = nbAnswer;
                            chooseAnswer--;
                        }
                    break;
                }
            }
        }
        // ESCAPE
        if (windowManager.isKeyPressed(SDL_GetScancodeFromKey(SDLK_ESCAPE))){
            nextScene = "fin";
            done = true;
        }

        // Loop through each model and check for a collision
        int i;
        for (i = 0; i < scene.getModelNumber(); ++i)
        {
            if(models[i].box.collision(cameraBox)) {
                modelCollision = i;
                isCollision = true;
                break;
            }
        }
        if(i == scene.getModelNumber()) isCollision = false;
        
        // Handle all mouse related inputs
        if(!isDialogue) Command::commandHandler(windowManager, camera, deltaTime, scene.getWallLimits());
        Command::mouseManager(camera, windowManager.getMousePosition(), screenWidth/2.0, screenHeight/2.0);
        // Put the cursor back to the center of the scene
        SDL_WarpMouseInWindow(windowManager.Window, screenWidth/2.0, screenHeight/2.0);


        /*********************************
         *         RENDERING CODE        *
         *********************************/

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        
        //***** ROOM *****//
        wallShader.Use();
        glUniform1i(glGetUniformLocation(wallShader.Program, "material.diffuse"),  0);
        glUniform1i(glGetUniformLocation(wallShader.Program, "material.specular"), 1);
        glUniform1f(glGetUniformLocation(wallShader.Program, "material.shininess"), 32.0f);
        glUniformMatrix4fv(glGetUniformLocation(wallShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(wallShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        scene.drawRoom(wallShader);

        //***** MODEL *****//
        shader.Use();
        
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        for (int i = 0; i < scene.getModelNumber(); ++i)
        {
            glm::mat4 matModel;
            // Translate model following the parameters set in the XML
            matModel = glm::translate(matModel, scene.getModel(i).Translate);
            // Rotate model following the parameters set in the XML, if any
            if (scene.getModel(i).RotateAngle != 0) matModel = glm::rotate(matModel, glm::radians(scene.getModel(i).RotateAngle), glm::vec3(-0.0f, -1.0f, 0.0f));
            // Scale model following the parameters set in the XML
            matModel = glm::scale(matModel, scene.getModel(i).Scale);

            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(matModel));

            models[i].Draw(shader);
        }

        //***** LIGHT *****//
        // Set the lighting uniforms
        glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

        // Lights
        for (int i = 0; i < scene.getLightNumber(); ++i)
        {
            string pointLight = "pointLights[" + to_string(i) + "]";
            Light light = scene.getLight(i);

            glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".position").c_str()), light.getPosition().x, light.getPosition().y, light.getPosition().z);
            glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".ambient").c_str()), light.getAmbient().x, light.getAmbient().y, light.getAmbient().z);
            glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".diffuse").c_str()), light.getDiffuse().x, light.getDiffuse().y, light.getDiffuse().z);
            glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".specular").c_str()), light.getSpecular().x, light.getSpecular().y, light.getSpecular().z);
            glUniform1f(glGetUniformLocation(shader.Program, (pointLight + ".constant").c_str()), light.getConstant());
            glUniform1f(glGetUniformLocation(shader.Program, (pointLight + ".linear").c_str()), light.getLinear());
            glUniform1f(glGetUniformLocation(shader.Program, (pointLight + ".quadratic").c_str()), light.getQuadratic());
        }

        wallShader.Use();
        glUniform3f(glGetUniformLocation(wallShader.Program, "viewPos"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

        for (int i = 0; i < scene.getLightNumber(); ++i)
        {
            string pointLight = "pointLights[" + to_string(i) + "]";
            Light light = scene.getLight(i);

            glUniform3f(glGetUniformLocation(wallShader.Program, (pointLight + ".position").c_str()), light.getPosition().x, light.getPosition().y, light.getPosition().z);
            glUniform3f(glGetUniformLocation(wallShader.Program, (pointLight + ".ambient").c_str()), 0.5f, 0.5f, 0.5f);
            glUniform3f(glGetUniformLocation(wallShader.Program, (pointLight + ".diffuse").c_str()), 0.5f, 0.5f, 0.5f);
            glUniform3f(glGetUniformLocation(wallShader.Program, (pointLight + ".specular").c_str()), 1.0f, 1.0f, 1.0f);
            glUniform1f(glGetUniformLocation(wallShader.Program, (pointLight + ".constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(wallShader.Program, (pointLight + ".linear").c_str()), 0.09);
            glUniform1f(glGetUniformLocation(wallShader.Program, (pointLight + ".quadratic").c_str()), 0.032);
        }

        text.Draw(shaderText,isDialogue, isAnswer, chooseAnswer, dialogue, answers);

        // Update the display
        windowManager.swapBuffers(windowManager.Window);
    }
    scene.deleteRoom();

    return nextScene;
}
