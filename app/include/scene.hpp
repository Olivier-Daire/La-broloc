#pragma once
#include <vector>
#include <string>
#include <tinyxml2/tinyxml2.h>
#include <glimac/Image.hpp>
#include <glm/glm.hpp>
#include "model/model.hpp"
#include "dialogue.hpp"
#include "tools/light.hpp"
#include "geometry/vertex2D.hpp"
#include "modelInfos.hpp"

using namespace tinyxml2;

class Scene
{
	public:
		Scene();
		void loadSceneFromFile(const char* filename);
		ModelInfos getModel(int number);
		int getModelNumber();
		Dialogue getDialogue(int group, int number);
		int getDialogueNumber(int group);
		int getAnswerNumber(int group, int i);
		Light getLight(int number);
		int getLightNumber();

		void loadModels(XMLDocument& doc);
		void loadDialogues(XMLDocument& doc);
		void loadLights(XMLDocument& doc);

		void loadRoom(XMLDocument& doc);
		void drawRoom(Shader shader);
		void deleteRoom();

		glm::vec4 getWallLimits();
	
	private:
		// List of all the models of the scene
		vector<ModelInfos> _models;
		// List of all the dialogues of the scene
		vector<vector <Dialogue>> _dialogues;
		vector<Light> _lights;
		// Room datas
		GLuint _texturesArray[3];
    	GLuint _vao;
    	GLuint _vbo;

    	float _width;
    	float _height;
    	float _depth;

    	glm::vec4 _wallLimits;

};
