#pragma once
#include <vector>
#include <string>
#include "model.hpp"
#include <tinyxml2/tinyxml2.h>

class Scene
{
	public:
		Scene();
		void loadSceneFromFile(const char* filename);
		string getModelPath(int number);
	
	private:
		// List of all the models of the scene
		vector<string> _models;
		// List of all the dialogues of the scene
		vector<string> _dialogues;
};
