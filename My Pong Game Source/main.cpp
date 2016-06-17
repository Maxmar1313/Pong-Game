#include <iostream>
#include <ctime>
#include <random>
#include "structs.h"
#include <Windows.h>
#include <fstream>
#include "Ball.h"
#include "Platform.h"
//#include <string>
#include "HighScore.h"


using namespace sf;
using namespace std;

const int MAX_RAND_SIZE = 37535;
unsigned char speed = 1;
bool isGameStarted = false;
bool onCollision = false;
unsigned int score=0;

Vector2f position;

//sfml related objects
RenderWindow window(VideoMode(1024, 800), "the Best Pong Game");
Event myEvent;
Font font;
Text scoreText;
Text initialText;
Ball ball(502, 380, 20, Color::White);
Platform platform[4];

//Frontal declarations of functions
Vector2f operator+(Vector2f vector, Vector2f vector2);
Vector2f operator*(Vector2f vector, RotationMatrix rotationMatrix);
Vector2f operator/(Vector2f vector, double divisor);

Vector2f rotate2D(double angle, Vector2f point);

double vectorLength(Vector2f vector);
unsigned short getRandomNumber();
short distance(Vector2f position, Vector2f position2);
void printPosition(Vector2f vector);
void update();
void render();
void handleEvent();
void broadPhase();
void whenGameIsOver();
void setText(Text &ObjText, Font &font, Color color, int size, string text, Vector2f position);

int main()
{
	
	//{ -1,0 }left GlobalNormal
	//{1,0}right GlobalNormal
	//{0,1}down GlobalNormal
	//{0,-1}up GlobalNormal	
	
				//position x, position y, width, height, Color      ,faceNormal ,backNormal ,leftNormal , rightNormal
	platform[0] = { 512 - 40 ,  750 - 8 ,  150, 20,Color::White     ,{0,-1 }   ,{0 ,1 }   ,{-1,0 }   ,{ 1,0 }		}; //kinda tryhard ;p but it works, so its not stupid
	platform[1] = { 20, 380, 20, 150, Color::White					,{ 1,0 }   ,{-1,0 }   ,{ 0,-1}	 ,{ 0,1 }		};
	platform[2] = { 512 - 40, 42, 150, 20, Color::White				,{ 0,1 }   ,{ 0,1 }   ,{ 1,0 }   ,{-1,0	}		};
	platform[3] = { 984, 380, 20, 150, Color::White					,{-1,0 }   ,{ 1,0 }   ,{ 0,1 }   ,{ 0,-1}		};

	position.x = ball.position.x;
	position.y = ball.position.y;

	if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) 
	{
		MessageBox(NULL,"ERROR:Couldn't load font",NULL,NULL);
		window.close();
	}
	
	setText(scoreText, font, Color::Red, 24, "Score: 0", {0,0});
	setText(initialText, font, Color::White, 20, "Press space to start", { 420,320 });

	//game loop
	clock_t previous = clock();
	clock_t lag = 0;
	while (window.isOpen())
	{	
		clock_t current = clock();
		clock_t elapsed = current-previous;

		lag += elapsed;

		previous = current;
		handleEvent();
		
		while (lag > 16)
		{
			update();
			lag -=16;
		}
		render();
	}
	return 0;
}

void handleEvent()
{


	while (window.pollEvent(myEvent))
	{
		switch (myEvent.type)
		{
		case Event::Closed:
			window.close();
			break;
		case Event::KeyPressed:
			if (myEvent.key.code == sf::Keyboard::Space)
			{
				if (isGameStarted == false) 
				{
					ball.setVelocity(		rotate2D(getRandomNumber(), {0,-1})		);
					isGameStarted = true;
				}
				break;
			}

		case Event::MouseButtonPressed:
			break;
		case Event::MouseButtonReleased:
			break;
		}
		

	}

}

unsigned short getRandomNumber()
{

	std::random_device device;
	std::mt19937 mersenne(device());
	double fraction = 1.0/(4294967295.0+1);
	unsigned short rand = static_cast<int>(mersenne() * fraction * (360 - 0 + 1) + 0);
	return rand;

}

void updateScore()
{
	int dist = distance(position, ball.position);
	if (dist >= 10)
	{
		score=score+((dist+1)%10);
		char text[256];
		sprintf_s(text, "Score: %u", score);

		scoreText.setString(text);
		position.x = ball.position.x;
		position.y = ball.position.y;
	}
}

void update()
{
	
	platform[0].velocity.x = sf::Mouse::getPosition(window).x - platform[0].position.x;
	platform[1].velocity.y = sf::Mouse::getPosition(window).y - platform[1].position.y;
	platform[2].velocity.x = sf::Mouse::getPosition(window).x - platform[2].position.x;
	platform[3].velocity.y = sf::Mouse::getPosition(window).y - platform[3].position.y;

	for (int i = 0; i < 4; ++i) 
	{
		platform[i].move();
	}

	whenGameIsOver();
	broadPhase();

	ball.move();
	updateScore();
}

void render()
{
		
		window.clear(Color::Black);
		if (isGameStarted == false) 
		{
			window.draw(initialText);
		}

		window.draw(ball.ballShape);
		window.draw(scoreText);

		for (int i = 0; i < 4; ++i) {
			window.draw(platform[i].platformShape);
		}
		window.display();
}

//function not used  at this moment, may be usefull in the future
short distance(Vector2f position, Vector2f position2) 
{
	float x = position2.x - position.x;
	float y = position2.y - position.y;
	float f= sqrt(pow(x, 2) + pow(y, 2));
	return f;
}

//Function that returns dot product of two vectors
float dot(Vector2f velocity, Vector2f normal)
{

	return velocity.x * normal.x + velocity.y * normal.y;
}

//Function that resolves collision
Vector2f collision(Platform platform)
{

	Vector2f temp;
	//collision from face
	if (platform.min.y > ball.min.y && platform.min.x <ball.max.x && platform.max.x> ball.min.x && platform.max.y>ball.max.y)
	{	
		temp = (-(2 * dot(ball.velocity, platform.faceNormal)) * platform.faceNormal) + ball.velocity;
	}
	//collision from back
	else if (platform.min.y < ball.min.y && ball.max.x > platform.min.x &&ball.min.x < platform.max.x)
	{
		temp = (-(2 * dot(ball.velocity, platform.backNormal)) * platform.backNormal) + ball.velocity;
	}//collision from left
	else if (platform.min.x < ball.min.x && platform.max.x > ball.min.x)
	{
		temp = (-(2 * dot(ball.velocity, platform.leftNormal)) * platform.leftNormal) + ball.velocity;

	}//collision from right
	else if (platform.min.x < ball.min.x && platform.min.y > ball.min.y&& platform.min.y < ball.max.y)
	{
	
		temp = (-(2 * dot(ball.velocity, platform.rightNormal)) * platform.rightNormal) + ball.velocity;
	}

	return temp;
}

//return magnitude of provided vector
double vectorLength(Vector2f vector)
{
	return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

//testing "possible" collision
void narrowPhase(Platform platform)
{

	if (platform.min.x - ball.max.x < 0.0f && ball.min.x - platform.max.x < 0.0f)
	{	
		onCollision = true;

		ball.velocity = collision(platform);

		if (vectorLength(ball.velocity) < 25) 
		{
			//increasing velocity by 1 unit
			ball.setVelocity(
					(	
						ball.velocity / vectorLength(ball.velocity)	)	+ ball.velocity
					);
		}
	}
	else onCollision = false;
}

//testing for any "possible" collision
void broadPhase()
{
	for (int i = 0; i < 4; i++) {
		float j = (platform[i].min.y - ball.max.y);
		float  k = (ball.min.y - platform[i].max.y);

		if (j <= 0.0f && k <= 0.0f)
		{
			narrowPhase(platform[i]);
		}
	}
}

//overloading operator to add two vectors. Much prettier than separate function
Vector2f operator+(Vector2f vector, Vector2f vector2)
{
	Vector2f tempVector;
	tempVector.x = vector.x + vector2.x;
	tempVector.y = vector.y + vector2.y;
	return tempVector;
}

//this function serves only for testing purposes
void printPosition(Vector2f vector)
{
	printf("Position x is: %f, Position y is : %f\n", vector.x,vector.y);

}

//rotation about angle from orgin of object
Vector2f rotate2D(double angle,Vector2f point) 
{
	RotationMatrix rotateMatrix(angle);
	return point*rotateMatrix;
}

// this function returns Vector multiplied by 2x2 matrice
Vector2f operator*(Vector2f vector, RotationMatrix rotationMatrix)
{
	Vector2f temp;
	temp.x = vector.x*rotationMatrix.matrix[0][0]+vector.y*rotationMatrix.matrix[1][0];
	temp.y = vector.x*rotationMatrix.matrix[0][1] + vector.y*rotationMatrix.matrix[1][1];
	return temp;
}

//this function returns Vector/Point devided by scalar
Vector2f operator/(Vector2f vector, double divisor)
{
	vector.x = vector.x / divisor;
	vector.y = vector.y / divisor;
	return vector;
}

void drawTextes(const Text &gameOverText , const Text &enterNameText, const Text &providedNameText, const Text &scoreText,const Text &resultText)
{
	window.draw(gameOverText);
	window.draw(enterNameText);
	window.draw(providedNameText);
	window.draw(scoreText);
	window.draw(resultText);
}

void gameOverInput(Event &myEvent, string &userName,char &chosenLetter,pair<unsigned char,unsigned char>&myPair,int &i)
{
	switch (myEvent.type)
		//Return represents 'Enter' key, it is named as Return, because of historical reasons(in typewriters key, that was in place of nowadays enter, was name as Return)
	{

	case Event::KeyPressed:
		if (myEvent.key.code == Keyboard::Return)
		{
			userName[i] = chosenLetter;
			++i;
			cout << userName << endl;
			break;
		}
		if (myEvent.key.code == Keyboard::BackSpace)
		{
			if (i > 0)
			{
				--i;
				userName[i] = '_';
			}
			break;
		}

		if (myEvent.key.code == Keyboard::Up)
		{
			if (myPair.first == 0)
			{
				chosenLetter += 13;
				myPair.first += 1;
			}
			else
			{
				myPair.first -= 1;
				chosenLetter -= 13;
			}
			break;
		}
		if (myEvent.key.code == Keyboard::Down)
		{
			if (myPair.first == 1)
			{
				myPair.first -= 1;
				chosenLetter -= 13;
			}
			else
			{
				chosenLetter += 13;
				myPair.first += 1;
			}
			break;
		}
		if (myEvent.key.code == Keyboard::Left)
		{
			if (myPair.second == 0)
			{
				chosenLetter += 12;
				myPair.second += 12;
			}
			else
			{
				myPair.second -= 1;
				chosenLetter -= 1;
			}
			break;
		}
		if (myEvent.key.code == Keyboard::Right)
		{
			if (myPair.second == 12)
			{
				chosenLetter -= 12;
				myPair.second -= 12;
			}
			else
			{
				chosenLetter += 1;
				myPair.second += 1;
			}
			break;
		}
	}
}

void setMarker(RectangleShape &marker,Vector2f position)
{
	marker.setFillColor(Color::White);
	marker.setPosition(position);
	marker.setSize({ 20,20 });
}
//a little bit ugly
string fetchNameFromUser()
{	
	string resultString= "You have ended the game with score:" + std::to_string(score);
	
	Text alphabet [26];

	Text resultText;
	setText(resultText, font, Color::White, 35, resultString, { 200,220 });

	Text enterNameText;
	setText(enterNameText, font, Color::White, 30, "Please Enter Your Name", { 340,300 });

	Text gameOverText;
	setText(gameOverText, font, Color::Red, 40, "GAME OVER", { 370,150 });

	pair <unsigned char, unsigned char> myPair = { 0,0 };
	char chosenLetter = 'A';
	string userName = "___";

	Text providedNameText;

	RectangleShape marker;
	while (window.waitEvent(myEvent))
	{
		window.clear(Color::Black);
		setText(providedNameText, font, Color::White, 30, userName, { 460,360 });
		drawTextes(gameOverText, enterNameText, providedNameText, scoreText,resultText);

		int j = 1;
		int k = 0;

		for (unsigned char i = 0; i<26; ++i)
		{	
			if (0 == i % 13) 
			{
					++j; k=0;
			}
			++k;
			char letter[2] = "A";
			letter[0] = letter[0] + i;
	
			setText(alphabet[i], font, Color::White, 20 , letter , {350.0f+20*k,380.0f+20*j});
			window.draw(alphabet[i]);
		}

		setMarker(marker, alphabet[chosenLetter - 65].getPosition());
		window.draw(marker);
	
		static int i = 0;
		gameOverInput(myEvent, userName, chosenLetter, myPair, i);

		window.display();

		if (3 == i)
			return userName;
	}
}

void whenGameIsOver()
{
	if (ball.min.x <= 0 || ball.min.y <= 0 || ball.max.x >= 1024 || ball.max.y >= 800)
	{
		char text[60];
		sprintf_s(text,"You have ended the game with score : %i",score);
		MessageBox(NULL,text , NULL, NULL);
		string name;
		name=fetchNameFromUser();
		
		HighScore::updateHighScore(score,name);
		getchar();
		window.close();
	}
}

void setText(Text &ObjText,Font &font ,Color color, int size, string text, Vector2f position)
{
	ObjText.setFont(font);
	ObjText.setColor(color);
	ObjText.setCharacterSize(size);
	ObjText.setString(text);
	ObjText.setPosition(position);
}
