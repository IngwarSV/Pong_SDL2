#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Specifications.h"

using namespace DEF_SETT;

class Ball {
private:
// attributes
	Location _currLocation{ FIELD_CENTER.x - BALL_WIDTH / 2, FIELD_CENTER.y - BALL_HEIGHT / 2 };
	Location _speedVector;
	int _ballSpeed = BALL_SPEED;

public:
	Ball();
	~Ball() {}
	
// methods
	void resetBallLocation(int CoordY, int angle);
	void moveBall();

	// setters
	void setLocation(Location newLocation);
	void setSpeedVector(Location location);
	void changeSpeedVector(Location direction);
	void changeSpeedVector(int angle);
	void increaseSpeed();
	void resetSpeed();

	// getters
	Location getLocation() const;
	Location getSpeedVector() const;
};
