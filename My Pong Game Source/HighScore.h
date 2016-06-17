#pragma once
#include "Structs.h"
#include <fstream>
#include <Windows.h>
class HighScore
{
public:
	void static	updateHighScore(int score, std::string name);
private:
	HighScore();
	~HighScore();
	void static writeToHighScore(myList &highScore);
	void static readHighScore(myList &highScore);

};

