#include "HighScore.h"
using namespace std;


HighScore::HighScore()
{
}


HighScore::~HighScore()
{
}
void HighScore::updateHighScore(int score, std::string name)
{
	myList highScore;
	readHighScore(highScore);
	//highScore.printHighScore();
	Node *temp = highScore.tail;
	Node *nodeToBeSwapped = NULL;

	while (temp != NULL && score > temp->value)
	{
		printf("tutaj\n");
		nodeToBeSwapped = temp;
		temp = temp->previous;
	}
	if (nodeToBeSwapped != NULL)
	{

		temp = new Node();
		temp->value = score;
		temp->name = name;

		highScore.insert(temp, nodeToBeSwapped);
	}
//	highScore.printHighScore();

	writeToHighScore(highScore);

}
void HighScore::readHighScore(myList &highScore)
{
	printf("tutaj1\n");
	string mystring;
	fstream mystream("highscore.dat", ios::in);
	//printf("tutaj1\n");
	if (mystream && !mystream.eof())
	{
		printf("tutaj2\n");
		for (int i = 0; i < 10; ++i)
		{
			int value;
			char name[4];
			mystream >> name >> value;
			highScore.add(value, name);
		}
	}
	//if file is empty, then fill it with 0s (reseting highScore)
	else
	{
	//	printf("tutaj3\n");
		MessageBox(NULL, "Reseting High Score", NULL, NULL);
		highScore.fill(10, 0, "XXX");
		writeToHighScore(highScore);
	}
	
}
void HighScore::writeToHighScore(myList &highScore)
{
	ofstream mystream("highscore.dat");
	Node *node = highScore.head;
	while (node != NULL)
	{
		mystream << node->name.c_str() << " "<< node->value << endl;
		node = node->next;
	}
	mystream.close();
}