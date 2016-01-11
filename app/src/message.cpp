#include "message.hpp"

Message::Message(string t) {
	textdialogue = t;
    isAnswer = false;
}

string Message::getText(int i) {
    return textdialogue;
}

void Message::draw(Shader shaderText, int chooseAnswer, Text text) {
    text.RenderText(shaderText, textdialogue, 110.0f, 100.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
}

bool Message::getAnswer() {
    return isAnswer;
}

Message::~Message() {
    delete this;
}