#include "dialogue.hpp"

Dialogue::Dialogue(){

}

string Dialogue::getMessage(){
	return _message;
}

void Dialogue::setMessage(string message){
	_message = message;
}

vector<string> Dialogue::getAnswers(){
	return _answers;
}

string Dialogue::getAnswer(int number){
	return _answers.at(number-1);
}

void Dialogue::addAnswer(string answer){
	_answers.push_back(answer);
}
