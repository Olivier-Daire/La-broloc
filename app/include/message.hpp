#pragma once
#include "dialogue.hpp"

class Message : public Dialogue
{
	public:
		Message(string t);
		string getText(int i);
		void draw(Shader shaderText, int chooseAnswer,Text text);
		bool getAnswer();
		~Message();

	private:
		bool isAnswer = false;
		string textdialogue;
};