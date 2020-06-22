#include "Paddle.h"

Paddle::Paddle(Location location) : _currLocation{ location }
{
}

void Paddle::movePaddleUp(int speed)
{
	_currLocation.y -= speed;
}

void Paddle::movePaddleDown(int speed)
{
	_currLocation.y += speed;
}

void Paddle::setLocation(int coordY)
{
	_currLocation.y = coordY;
}

Location Paddle::getLocation() const
{
	return _currLocation;
}