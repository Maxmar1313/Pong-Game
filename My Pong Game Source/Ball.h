#pragma once

#include <SFML\Graphics.hpp>
using namespace sf;
class Ball
{
public:
	CircleShape ballShape;

	Vector2f velocity;
	Vector2f position;
	Vector2f min;
	Vector2f max;

	float radius;

	Ball(float x, float y,int radius, sf::Color);
	void setVelocity(Vector2f velocity);
	void move();
	void getMinMax();
	~Ball();
};

