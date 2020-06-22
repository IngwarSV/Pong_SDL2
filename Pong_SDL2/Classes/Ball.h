#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Specifications.h"

using namespace DEF_SETT;

class Ball {
private:
// attributes
	Location _currLocation;
	Location _speedVector;
	int _ballSpeed;

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
