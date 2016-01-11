#include "answer.hpp"

Answer::Answer(string t[]) {
	isAnswer = true;
	textdialogue[0] = t[0];
	textdialogue[1] = t[1];

}

string Answer::getText(int i) {
	return textdialogue[i];
}

bool Answer::getAnswer() {
	return isAnswer;
}


void Answer::draw(Shader shaderText, int chooseAnswer,Text text) {
	glm::vec3 answerColor1;
    glm::vec3 answerColor2;

 	if(chooseAnswer == 0)  {
            answerColor1 = glm::vec3(0.8f, 0.0f, 0.0f);
            answerColor2 = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    else {
        answerColor1 = glm::vec3(0.0f, 0.0f, 0.0f);
        answerColor2 = glm::vec3(0.8f, 0.0f, 0.0f);
    }
    text.RenderText(shaderText, textdialogue[0], 110.0f, 100.0f, 0.5f, answerColor1);
    text.RenderText(shaderText, textdialogue[1], 600.0f, 100.0f, 0.5f, answerColor2);
}

Answer::~Answer() {
	delete this;
}