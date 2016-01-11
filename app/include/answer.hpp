#pragma once
#include "dialogue.hpp"

class Answer : public Dialogue
{
	public:
		Answer(string t[]);
		void draw(Shader shaderText, int chooseAnswer,Text text);
		string getText(int i);
		bool getAnswer();
		~Answer();

	private:
		bool isAnswer = false;
		string textdialogue[2];
};