#include "Ball.h"

Ball::Ball(float x, float y,int radius, sf::Color color)
{	
	this->min.x = x;
	this->min.y = y;

	this->max.x = x + (2 * radius);
	this->max.y = y + (2 * radius);

	this->position.x = x+radius;
	this->position.y = y+radius;

	this->radius = (float)radius;

	ballShape.setRadius(this->radius);
	ballShape.setPosition(x, y);
	ballShape.setFillColor(color);
}

void Ball::setVelocity(Vector2f velocity)
{
	this->velocity = velocity;
}
Ball::~Ball()
{
}
void Ball::move()
{
	position.x += velocity.x;
	position.y += velocity.y;

	min.x += velocity.x;
	min.y += velocity.y;

	max.x += velocity.x;
	max.y += velocity.y;

	this->ballShape.move(this->velocity);
}
void Ball::getMinMax()
{
	printf(" minx %f miny %f max %f maxy %f\n", min.x, min.y, max.x, max.y);
}
