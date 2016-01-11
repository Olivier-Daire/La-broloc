#include "application.hpp"

using namespace glimac;

Application::Application() {}

std::string Application::launch(std::string currentScene) {

    GLuint screenWidth = 1366, screenHeight = 768;
    // Initialize SDL and open a window
    SDLWindowManager windowManager(screenWidth, screenHeight, "The Broloc");
    // Initialize glew for OpenGL3+ support
    glewExperimental = GL_TRUE; 
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_SUCCESS;
    }

    glViewport(0, 0, screenWidth, screenHeight);
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;
    glEnable(GL_DEPTH_TEST);

    /*********** END INIT ************/
    Camera camera;
    // Text related data
    bool isDialogue = true;
    int cptDialogue = 0, nbAnswer = 2, chooseAnswer = 0;
    int group = 0;
    bool interaction = false;

    std::string nextScene;
    Scene scene;
    Text text;


    Shader shaderText("../assets/shaders/text.vs.glsl", "../assets/shaders/text.fs.glsl");
    Shader shader("../assets/shaders/pointlight.vs.glsl", "../assets/shaders/pointlight.fs.glsl");
    Shader wallShader("../assets/shaders/tex2D.vs.glsl", "../assets/shaders/tex2D.fs.glsl");
    
    // Load models infos, room, dialogues, lights
    if(currentScene != "Home" && currentScene != "Command" && currentScene != "End1" && currentScene != "End2") {
        scene.loadSceneFromFile(("../assets/scenes/" + currentScene  + ".xml").c_str());
        if(scene.getPathMusic() != NULL) Sound::loadSound(scene.getPathMusic());
    }
    if(currentScene == "Home") {
        Sound::loadSound("../assets/musics/Blind.wav");
    }
    text.LoadText(shaderText, screenWidth, screenHeight);


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
                nextScene = "fin";
                done = true; // Leave the loop after this iteration
            }
            if( e.type == SDL_KEYDOWN ) {
                switch( e.key.keysym.sym )
                {
                    case SDLK_e:
                        if (interaction)
                        {
                            isDialogue = true;  
                            interaction = false;
                            cptDialogue = 0;
                        }
                        
                    break;

                    case SDLK_SPACE:
                        if(currentScene == "Home") {
                            nextScene = "Command";
                            done = true;
                        }
                        else if(currentScene == "Command") {
                            nextScene = "Hospital";
                            done = true;
                        }
                        else if((currentScene == "End1") || (currentScene == "End2")){
                            nextScene = "Hospital";
                            done = true;
                        }
                        else if(scene.getDialogue(group, cptDialogue)->getAnswer()) {
                            istringstream iss(scene.getDialogue(group, cptDialogue)->getText(chooseAnswer));
                            std::string word;
                            iss >> word;
                            nextScene = word;
                            done = true;
                        }
                        else if(scene.getEnd() != 0 && group == scene.getGroupNumber()-1 && cptDialogue == scene.getDialogueNumber(group)-1) {
                            if(scene.getEnd() == 1) nextScene = "End1";
                            if(scene.getEnd() == 2) nextScene = "End2";
                            if(scene.getEnd() != 0) done = true;
                        }
                        else if(isDialogue) {
                            if(cptDialogue < scene.getDialogueNumber(group)-1) {
                                cptDialogue++;
                            }
                            else {
                                isDialogue = false;
                                cptDialogue = 0; 
                            }
                        }
                    break;
                    case SDLK_RIGHT:
                       if(isDialogue && scene.getDialogue(group, cptDialogue)->getAnswer()) {
                            chooseAnswer++;
                            if(chooseAnswer == nbAnswer) chooseAnswer = 0;
                        }
                    break;
                    case SDLK_LEFT:
                        if(isDialogue && scene.getDialogue(group, cptDialogue)->getAnswer()) {
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
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(currentScene == "Home") text.drawHome(shaderText, screenWidth, screenHeight);
        else if(currentScene == "Command") text.drawCommand(shaderText);
        else if(currentScene == "End1") text.drawEnd1(shaderText, screenWidth, screenHeight);  
        else if(currentScene == "End2") text.drawEnd2(shaderText, screenWidth, screenHeight);  

        else {
            // Handle all mouse related inputs
            if(!isDialogue) Command::commandHandler(windowManager, camera, deltaTime, scene.getWallLimits());
            Command::mouseManager(camera, windowManager.getMousePosition(), screenWidth/2.0, screenHeight/2.0);
            // Put the cursor back to the center of the scene
            SDL_WarpMouseInWindow(windowManager.Window, screenWidth/2.0, screenHeight/2.0);


            /*********************************
             *         RENDERING CODE        *
             *********************************/

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

            bool isInteraction = false;
            for (int i = 0; i < scene.getModelNumber(); ++i)
            {
                // Update model's position givent its translate
                Model currentModel = scene._models[i];
                currentModel.updatePosition();
                // If we can interact with the model
                if (currentModel.getInteractionDialogue() != -1)
                {
                    if(abs(currentModel.getPosition().x - camera.getPosition().x) < 1 && abs(currentModel.getPosition().z - camera.getPosition().z) < 1){
                        if(!isDialogue) interaction = true;
                        group = currentModel.getInteractionDialogue();
                        isInteraction = true;
                    }
                } 
                glm::mat4 matModel;
                // Translate model following the parameters set in the XML
                matModel = glm::translate(matModel, currentModel.getTranslate());
                // Rotate model following the parameters set in the XML, if any
                if (currentModel.getRotateAngle() != 0) matModel = glm::rotate(matModel, glm::radians(currentModel.getRotateAngle()), currentModel.getRotate());
                // Scale model following the parameters set in the XML
                matModel = glm::scale(matModel, currentModel.getScale());

                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(matModel));

                currentModel.Draw(shader);
            }
            if(!isInteraction) interaction = false;

            //***** LIGHT *****//
            shader.Use();
            glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
            scene.drawLightModel(shader);

            wallShader.Use();
            glUniform3f(glGetUniformLocation(wallShader.Program, "viewPos"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
            scene.drawLightWall(wallShader);

            //***** TEXT *****//
            if(interaction) text.DrawHint(shaderText);
            if(isDialogue) scene.getDialogue(group, cptDialogue)->draw(shaderText, chooseAnswer,text);
        }

        // Update the display
        windowManager.swapBuffers(windowManager.Window);
    }
    scene.deleteRoom();
    return nextScene;
}
