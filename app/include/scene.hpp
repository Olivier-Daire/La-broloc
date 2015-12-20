#pragma once
#include <vector>
#include <string>
#include <tinyxml2/tinyxml2.h>
#include <glimac/Image.hpp>
#include "model/model.hpp"
#include "dialogue.hpp"
#include "tools/light.hpp"
#include "geometry/vertex2D.hpp"

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

		void loadRoom(XMLDocument& doc);
		void drawRoom(Shader shader);
		void deleteRoom();
	
	private:
		// List of all the models of the scene
		vector<string> _models;
		// List of all the dialogues of the scene
		vector<Dialogue> _dialogues;
		vector<Light> _lights;
		// Room datas
		GLuint _texturesArray[3];
    	GLuint _vao;

};
