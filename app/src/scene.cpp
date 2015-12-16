#include "scene.hpp"

using namespace tinyxml2;

Scene::Scene(){

}

// TODO move this function to the constructor ? 
void Scene::loadSceneFromFile(const char* filename){
	XMLDocument doc;
	doc.LoadFile(filename);

	// Load all models paths
	XMLElement *models = doc.FirstChildElement("scene")->FirstChildElement("models");
	for (XMLElement* model = models->FirstChildElement(); model != NULL; model = model->NextSiblingElement())
	{
	    _models.push_back(model->GetText());
	}

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


string Scene::getModelPath(int number){
	// -1 so that we can call getModelPath(1)
	// for the first model
	return _models.at(number-1);
}


Dialogue Scene::getDialogues(int number){
	return _dialogues.at(number-1);
}
