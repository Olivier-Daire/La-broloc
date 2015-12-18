#include "scene.hpp"

Scene::Scene(){

}

// TODO move this function to the constructor ? 
void Scene::loadSceneFromFile(const char* filename){
	XMLDocument doc;
	doc.LoadFile(filename);

	loadModelsPaths(doc);
	loadDialogues(doc);
	loadLights(doc);

}

void Scene::loadModelsPaths(XMLDocument& doc){
	// Load all models paths
	XMLElement *models = doc.FirstChildElement("scene")->FirstChildElement("models");
	for (XMLElement* model = models->FirstChildElement(); model != NULL; model = model->NextSiblingElement())
	{
	    _models.push_back(model->GetText());
	}
}

void Scene::loadDialogues(XMLDocument& doc){
	// Load all dialogues
	XMLElement *dialogues = doc.FirstChildElement("scene")->FirstChildElement("dialogues");
	for (XMLElement* dialogue = dialogues->FirstChildElement(); dialogue != NULL; dialogue = dialogue->NextSiblingElement())
	{
		Dialogue dialogueContent;
		// Get message of the dialogue
		dialogueContent.setMessage(dialogue->FirstChildElement("message")->GetText());

		// Get all the possible answers
		XMLElement *answers = dialogue->FirstChildElement("answers");
		for (XMLElement* answer = answers->FirstChildElement(); answer != NULL; answer = answer->NextSiblingElement()){
			dialogueContent.addAnswer(answer->GetText());
		}

	    _dialogues.push_back(dialogueContent);
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


string Scene::getModelPath(int number){
	return _models.at(number);
}

Dialogue Scene::getDialogue(int number){
	return _dialogues.at(number);
}

int Scene::getDialogueNumber() {
	return _dialogues.size();
}

int Scene::getAnswerNumber(int i) {
	return _dialogues[i].getAnswers().size();
}

Light Scene::getLight(int number){
	return _lights.at(number);
}

int Scene::getLightNumber(){
	return _lights.size();
}
