#pragma once
#include <vector>
#include <string>
#include "tools/text.hpp"
#include "shader.hpp"
#include <glm/glm.hpp>
#include <iostream>


using namespace std;

class Dialogue
{
	public:
		Dialogue();
		virtual void draw(Shader shaderText, int chooseAnswer,Text text);
		virtual string getText(int i);
		virtual bool getAnswer();
		virtual ~Dialogue();


};