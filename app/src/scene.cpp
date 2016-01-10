#include "scene.hpp"

Scene::Scene(){

}

// TODO move this function to the constructor ? 
void Scene::loadSceneFromFile(const char* filename){
	XMLDocument doc;
	doc.LoadFile(filename);

    // Check for errors in the XML
    if (doc.ErrorID() != 0)
    {
        doc.PrintError();
    }

    XMLElement *music = doc.FirstChildElement("scene");
    pathMusic = music->FirstChildElement("music")->GetText();

    XMLElement *end = doc.FirstChildElement("scene");
    if(end->FirstChildElement("end")) this->end = atoi(end->FirstChildElement("end")->GetText());
    else this->end = 0;

	loadModels(doc);
	loadDialogues(doc);
	loadLights(doc);
	loadRoom(doc);
}

void Scene::loadModels(XMLDocument& doc){

	// Load all models paths
	XMLElement *models = doc.FirstChildElement("scene")->FirstChildElement("models");
    
	for (const XMLElement* model = models->FirstChildElement(); model != NULL; model = model->NextSiblingElement())
	{
        
        string path = model->FirstChildElement("path")->GetText();
        // Translate
        glm::vec3 translate =  glm::vec3(model->FirstChildElement("translate")->FindAttribute("x")->FloatValue(),
                                         model->FirstChildElement("translate")->FindAttribute("y")->FloatValue(),
                                         model->FirstChildElement("translate")->FindAttribute("z")->FloatValue());
        // Scale
        glm::vec3 scale =  glm::vec3(model->FirstChildElement("scale")->FindAttribute("x")->FloatValue(),
                                     model->FirstChildElement("scale")->FindAttribute("y")->FloatValue(),
                                     model->FirstChildElement("scale")->FindAttribute("z")->FloatValue());

        // Rotate
        glm::vec3 rotate;
        float rotateAngle;
        // Only if a rotate parameter is set in the XML
        if (model->FirstChildElement("rotate"))
        {
            rotate =  glm::vec3(model->FirstChildElement("rotate")->FindAttribute("x")->FloatValue(),
                                       model->FirstChildElement("rotate")->FindAttribute("y")->FloatValue(),
                                       model->FirstChildElement("rotate")->FindAttribute("z")->FloatValue());
            
            rotateAngle = atof(model->FirstChildElement("rotate")->GetText());
        } else {
            rotate = glm::vec3(0.0, 0.0, 0.0);
            rotateAngle = 0;
        }

        int interactionDialogue = -1; //Default state no interaction possible
        if(model->FirstChildElement("interaction")){
            interactionDialogue = atoi(model->FirstChildElement("interaction")->GetText());
        }

        _models.push_back(Model(path, translate, scale, rotate, rotateAngle, interactionDialogue));
	}
}

void Scene::loadDialogues(XMLDocument& doc){
	// Load all dialogues
	XMLElement *dialogues = doc.FirstChildElement("scene")->FirstChildElement("dialogues");

    for (const XMLElement* group = dialogues->FirstChildElement(); group != NULL; group = group->NextSiblingElement()){
        vector<Dialogue> groupContent;

        for (const XMLElement* dialogue = group->FirstChildElement(); dialogue != NULL; dialogue = dialogue->NextSiblingElement())
        {
            Dialogue dialogueContent;

            // Get message of the dialogue
            dialogueContent.setMessage(dialogue->FirstChildElement("message")->GetText());

            // Get all the possible answers
            if(dialogue->FirstChildElement("answers")) {
              const XMLElement *answers = dialogue->FirstChildElement("answers");
                for (const XMLElement* answer = answers->FirstChildElement(); answer != NULL; answer = answer->NextSiblingElement()){
                dialogueContent.addAnswer(answer->GetText());
              }
            }

            groupContent.push_back(dialogueContent);
        }
        _dialogues.push_back(groupContent);
    }
}

void Scene::loadLights(XMLDocument& doc){
	XMLElement *lights = doc.FirstChildElement("scene")->FirstChildElement("lights");
	for (const XMLElement* light = lights->FirstChildElement(); light != NULL; light = light->NextSiblingElement()){
		
		glm::vec3 position( light->FirstChildElement("position")->FindAttribute("x")->FloatValue(),
							light->FirstChildElement("position")->FindAttribute("y")->FloatValue(),
							light->FirstChildElement("position")->FindAttribute("z")->FloatValue());

		glm::vec3 ambient( light->FirstChildElement("ambient")->FindAttribute("x")->FloatValue(),
							light->FirstChildElement("ambient")->FindAttribute("y")->FloatValue(),
							light->FirstChildElement("ambient")->FindAttribute("z")->FloatValue());

		glm::vec3 diffuse( light->FirstChildElement("diffuse")->FindAttribute("x")->FloatValue(),
							light->FirstChildElement("diffuse")->FindAttribute("y")->FloatValue(),
							light->FirstChildElement("diffuse")->FindAttribute("z")->FloatValue());

		glm::vec3 specular( light->FirstChildElement("specular")->FindAttribute("x")->FloatValue(),
							light->FirstChildElement("specular")->FindAttribute("y")->FloatValue(),
							light->FirstChildElement("specular")->FindAttribute("z")->FloatValue());

		float constant = light->FirstChildElement("constant")->FindAttribute("value")->FloatValue();
		float linear = light->FirstChildElement("linear")->FindAttribute("value")->FloatValue();
		float quadratic = light->FirstChildElement("quadratic")->FindAttribute("value")->FloatValue();

		Light lightContent(position, ambient, diffuse, specular, constant, linear, quadratic);

		_lights.push_back(lightContent);
	}
}

int Scene::getModelNumber(){
    return _models.size();
}

Dialogue Scene::getDialogue(int group, int number){
	return _dialogues.at(group).at(number);
}

int Scene::getGroupNumber() {
    return _dialogues.size();
}

int Scene::getDialogueNumber(int group) {
	return _dialogues.at(group).size();
}

int Scene::getAnswerNumber(int group, int i) {
	return _dialogues.at(group).at(i).getAnswers().size();
}

Light Scene::getLight(int number){
	return _lights.at(number);
}

int Scene::getLightNumber(){
	return _lights.size();
}

glm::vec4 Scene::getWallLimits() {
    return _wallLimits;
}

const char* Scene::getPathMusic(){
    return pathMusic;
}

int Scene::getEnd(){
    return end;
}

void Scene::loadRoom(XMLDocument& doc){

	XMLElement *room = doc.FirstChildElement("scene")->FirstChildElement("room");

 	std::unique_ptr<glimac::Image> wallTexture = glimac::loadImage(room->FirstChildElement("walls")->GetText());
    std::unique_ptr<glimac::Image> floorTexture = glimac::loadImage(room->FirstChildElement("floor")->GetText());
    std::unique_ptr<glimac::Image> roofTexture = glimac::loadImage(room->FirstChildElement("roof")->GetText());

    if (wallTexture == NULL || floorTexture ==  NULL || roofTexture ==  NULL)
    {
        std::cerr << "Erreur lors du chargement de l'image" << std::endl;
    }


    _width = atof(room->FirstChildElement("width")->GetText());
    _height = atof(room->FirstChildElement("height")->GetText());
    _depth = atof(room->FirstChildElement("depth")->GetText());

    _wallLimits.x = atof(room->FirstChildElement("xmax")->GetText());
    _wallLimits.y = atof(room->FirstChildElement("xmin")->GetText());
    _wallLimits.z = atof(room->FirstChildElement("zmax")->GetText());
    _wallLimits.w = atof(room->FirstChildElement("zmin")->GetText());
    

    // Create an array of texture and bind each texture
    
    glGenTextures(3, _texturesArray);

    glBindTexture(GL_TEXTURE_2D, _texturesArray[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wallTexture->getWidth(), wallTexture->getHeight(), 0, GL_RGBA, GL_FLOAT, wallTexture->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,0);

    glBindTexture(GL_TEXTURE_2D, _texturesArray[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, floorTexture->getWidth(), floorTexture->getHeight(), 0, GL_RGBA, GL_FLOAT, floorTexture->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,0);

    glBindTexture(GL_TEXTURE_2D, _texturesArray[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, roofTexture->getWidth(), roofTexture->getHeight(), 0, GL_RGBA, GL_FLOAT, roofTexture->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,0);

    glGenBuffers (1, &_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    // Quad (wall) coordonates
    // TODO hardcoded, pass it as a parameter to the function ?
    Vertex2D vertices[] = {
        Vertex2D(glm::vec3(0, 0,0),  glm::vec2(4, 0)), // Sommet 0
        Vertex2D(glm::vec3(_width, 0,0), glm::vec2(0, 0)), // Sommet 1
        Vertex2D(glm::vec3(_width, _height,0), glm::vec2(0, 4)), // Sommet 2
        Vertex2D(glm::vec3(0, _height,0),  glm::vec2(4, 4)) // Sommet 3
    };

    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex2D), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    uint32_t indices[] = {
        0, 1, 2, 0, 2, 3
    };

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &_vao);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_COLOR = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const GLvoid*) offsetof(Vertex2D, position));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const GLvoid*) offsetof(Vertex2D, texture));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Scene::drawRoom(Shader shader){
    glDisable(GL_BLEND); // Disable blend to avoid conflicts with text being rendered on screen

    glBindTexture(GL_TEXTURE_2D, _texturesArray[0]);

    glm::mat4 matModelWall;
    glm::mat4 initialMatModelWall = matModelWall; // Initial state subsitute of push and pop
   
    glUniform1i(glGetUniformLocation(shader.Program, "uTexture"), 0);
    glBindVertexArray(_vao);
    
    // Front Wall
    matModelWall = glm::translate(matModelWall, glm::vec3(-6.5f, -3.0f, -_width));
    glm::mat4 frontMatModelWall = matModelWall;
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(matModelWall));
    glUniform1i(glGetUniformLocation(shader.Program, "wall"), 0);
   
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Left Wall
    matModelWall = glm::rotate(frontMatModelWall, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    matModelWall = glm::scale(matModelWall, glm::vec3(_depth/_width, 1.0f, 1.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(matModelWall));
    glUniform1i(glGetUniformLocation(shader.Program, "wall"), 1);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Right Wall
    matModelWall = glm::translate(matModelWall, glm::vec3(0.0f, 0.0f, -_width));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(matModelWall));
    glUniform1i(glGetUniformLocation(shader.Program, "wall"),2);

   
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

     // Back Wall
    matModelWall = glm::translate(initialMatModelWall, glm::vec3(-6.5f, -3.0f, -_width+_depth));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(matModelWall));
    glUniform1i(glGetUniformLocation(shader.Program, "wall"), 3);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Floor
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, _texturesArray[1]);

    matModelWall = glm::rotate(frontMatModelWall, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    matModelWall = glm::scale(matModelWall, glm::vec3(1.0f, _depth/_height, 1.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(matModelWall));
    glUniform1i(glGetUniformLocation(shader.Program, "wall"), 4);
   
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Roof
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, _texturesArray[2]);

    matModelWall = glm::translate(matModelWall, glm::vec3(0.0f, 0.0f, -_height));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(matModelWall));
    glUniform1i(glGetUniformLocation(shader.Program, "wall"), 5);
   
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    
    // Re enable blend to draw the text on screen
    glEnable(GL_BLEND);
}

void Scene::drawLightModel(Shader shader) {
    for (int i = 0; i < getLightNumber(); ++i)
    {
        string pointLight = "pointLights[" + to_string(i) + "]";
        Light light = getLight(i);

        glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".position").c_str()), light.getPosition().x, light.getPosition().y, light.getPosition().z);
        glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".ambient").c_str()), light.getAmbient().x, light.getAmbient().y, light.getAmbient().z);
        glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".diffuse").c_str()), light.getDiffuse().x, light.getDiffuse().y, light.getDiffuse().z);
        glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".specular").c_str()), light.getSpecular().x, light.getSpecular().y, light.getSpecular().z);
        glUniform1f(glGetUniformLocation(shader.Program, (pointLight + ".constant").c_str()), light.getConstant());
        glUniform1f(glGetUniformLocation(shader.Program, (pointLight + ".linear").c_str()), light.getLinear());
        glUniform1f(glGetUniformLocation(shader.Program, (pointLight + ".quadratic").c_str()), light.getQuadratic());
    }
}

void Scene::drawLightWall(Shader shader){
    for (int i = 0; i < getLightNumber(); ++i)
    {
        string pointLight = "pointLights[" + to_string(i) + "]";
        Light light = getLight(i);

        glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".position").c_str()), light.getPosition().x, light.getPosition().y, light.getPosition().z);
        glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".ambient").c_str()), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".diffuse").c_str()), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(shader.Program, (pointLight + ".specular").c_str()), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, (pointLight + ".constant").c_str()), 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, (pointLight + ".linear").c_str()), 0.09);
        glUniform1f(glGetUniformLocation(shader.Program, (pointLight + ".quadratic").c_str()), 0.032);
    }
}

void Scene::deleteRoom(){
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
	glDeleteTextures(3, _texturesArray);
}


