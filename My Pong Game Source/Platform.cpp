#include "Platform.h"

Platform::Platform() {};

Platform::Platform(float x, float y, float platformWidth, float platformHeight, sf::Color color, Vector2f faceNormal,Vector2f backNormal,Vector2f leftNormal,Vector2f rightNormal)
{	
	Vector2f size{ platformWidth,platformHeight };

	position.x = x;
	position.y = y;

	min.x = x;
	min.y = y;

	max.x = x + platformWidth;
	max.y = y + platformHeight;

	width = platformWidth;
	height = platformHeight;

	platformShape.setFillColor(color);
	platformShape.setPosition(x, y);
	platformShape.setSize(size);

	this->faceNormal = faceNormal;
	this->backNormal = backNormal;
	this->leftNormal = leftNormal;
	this->rightNormal = rightNormal;

}
void Platform::getMinMax()
{
	printf(" minx %f miny %f max %f maxy %f\n", min.x, min.y, max.x, max.y);
}

Platform::~Platform()
{
}

void Platform::setVelocityX(float x)
{
	velocity.x = x;

}
void Platform::setVelocityY(float y)
{
	velocity.y = y;

}
void Platform::move()
{
	this->position.x += velocity.x;
	this->position.y += velocity.y;

	this->min.x += velocity.x;
	this->min.y += velocity.y;

	this->max.x += velocity.x;
	this->max.y += velocity.y;

	platformShape.move(velocity);
}