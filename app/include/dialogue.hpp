#pragma once
#include <vector>
#include <string>

using namespace std;

class Dialogue
{
	public:
		Dialogue();
		string getMessage();
		void setMessage(string message);
		vector<string> getAnswers();
		string getAnswer(int number);
		void addAnswer(string answer);

	private:
		string _message;
		vector<string> _answers;
};
