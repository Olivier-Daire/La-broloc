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
#include "model/model.hpp"

using namespace tinyxml2;

class Scene
{
	public:
		Scene();
		void loadSceneFromFile(const char* filename);
		int getModelNumber();
		Dialogue getDialogue(int group, int number);
		int getGroupNumber();
		int getDialogueNumber(int group);
		int getAnswerNumber(int group, int i);
		Light getLight(int number);
		int getLightNumber();
		const char* getPathMusic();
		int getEnd();

		void loadModels(XMLDocument& doc);
		void loadDialogues(XMLDocument& doc);
		void loadLights(XMLDocument& doc);

		void loadRoom(XMLDocument& doc);
		void drawRoom(Shader shader);
		void drawLightModel(Shader shader);
		void drawLightWall(Shader shader);
		void deleteRoom();

		glm::vec4 getWallLimits();

		// All the models of the scene
		vector<Model> _models;
	
	private:
		const char* pathMusic;
		// List of all the dialogues of the scene
		vector<vector <Dialogue>> _dialogues;
		vector<Light> _lights;
		// Room datas
		GLuint _texturesArray[3];
		int end;
    	GLuint _vao;
    	GLuint _vbo;

    	float _width;
    	float _height;
    	float _depth;

    	glm::vec4 _wallLimits;
};
