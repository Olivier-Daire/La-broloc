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
	XMLElement *dialogues = doc.FirstChildElement("scene")->FirstChildElement("models");
	for (XMLElement* dialogue = dialogues->FirstChildElement(); dialogue != NULL; dialogue = dialogue->NextSiblingElement())
	{
	    _dialogues.push_back(dialogue->GetText());
	}

}


string Scene::getModelPath(int number){
	// -1 so that we can call getModelPath(1)
	// for the first model
	return _models.at(number-1);
}
