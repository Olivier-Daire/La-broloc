#pragma once
#include <vector>
#include <string>
#include <tinyxml2/tinyxml2.h>
#include "model.hpp"
#include "dialogue.hpp"

class Scene
{
	public:
		Scene();
		void loadSceneFromFile(const char* filename);
		string getModelPath(int number);
		Dialogue getDialogues(int number);
	
	private:
		// List of all the models of the scene
		vector<string> _models;
		// List of all the dialogues of the scene
		vector<Dialogue> _dialogues;
};
