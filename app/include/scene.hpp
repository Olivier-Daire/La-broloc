#pragma once
#include <vector>
#include <string>
#include <tinyxml2/tinyxml2.h>
#include "model.hpp"
#include "dialogue.hpp"
#include "light.hpp"

using namespace tinyxml2;

class Scene
{
	public:
		Scene();
		void loadSceneFromFile(const char* filename);
		string getModelPath(int number);
		Dialogue getDialogue(int number);
		int getDialogueNumber();
		int getAnswerNumber(int i);
		Light getLight(int number);
		int getLightNumber();

		void loadModelsPaths(XMLDocument& doc);
		void loadDialogues(XMLDocument& doc);
		void loadLights(XMLDocument& doc);
	
	private:
		// List of all the models of the scene
		vector<string> _models;
		// List of all the dialogues of the scene
		vector<Dialogue> _dialogues;
		vector<Light> _lights;
};
