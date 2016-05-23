#pragma once

#include <SFML\Graphics.hpp>
using namespace sf;
class Platform
{
public:


	RectangleShape platformShape;

	Vector2f velocity;
	Vector2f position;

	Vector2f min;
	Vector2f max;

	Vector2f leftNormal;
	Vector2f rightNormal;
	Vector2f backNormal;
	Vector2f faceNormal;

	Platform();
	Platform(float x, float y, float platformWidth, float platformHeight, sf::Color color, Vector2f leftNormal, Vector2f rightNormal, Vector2f backNormal, Vector2f faceNormal);
	~Platform();

	void setVelocityX(float x);
	void setVelocityY(float y);
	void move();
	void getMinMax();

private:
	float width;
	float height;

	

};

