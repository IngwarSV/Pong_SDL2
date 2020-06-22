#include "Ball.h"

Ball::Ball()
{
	_currLocation.x = FIELD_CENTER.x - BALL_WIDTH / 2;
	_currLocation.y = FIELD_CENTER.y - BALL_HEIGHT / 2;
	_ballSpeed = BALL_SPEED;
}


void Ball::resetBallLocation(int CoordY, int angle)
{
	_currLocation.x = FIELD_CENTER.x - BALL_WIDTH / 2;
	_currLocation.y = CoordY;

	changeSpeedVector(angle);
}

void Ball::moveBall()
{
	_currLocation += _speedVector;
}

void Ball::setLocation(Location newLocation)
{
	_currLocation = newLocation;
}

void Ball::setSpeedVector(Location location)
{
	_speedVector = location;
}

void Ball::changeSpeedVector(Location direction)
{
	_speedVector *= direction;
}

void Ball::changeSpeedVector(int angle)
{
	float radians = static_cast<float>(angle) * PI / 180.0f;

	_speedVector.x = static_cast<int>(roundf(cosf(radians) * _ballSpeed));
	_speedVector.y = static_cast<int>(roundf(sinf(radians) * _ballSpeed));
}

void Ball::increaseSpeed()
{
	_ballSpeed += 1;
}

void Ball::resetSpeed()
{
	_ballSpeed = BALL_SPEED;
}

Location Ball::getLocation() const
{
	return _currLocation;
}

Location Ball::getSpeedVector() const
{
	return _speedVector;
}
