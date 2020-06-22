#pragma once

#include "Specifications.h"

using namespace DEF_SETT;

class Paddle {
private:
// attributes
	Location _currLocation;

public:
	Paddle(Location location);
	~Paddle() {}

// methods
	void movePaddleUp(int speed = PADDLE_SPEED);
	void movePaddleDown(int speed = PADDLE_SPEED);

	// setters
	void setLocation(int coordY);

	//getters
	Location getLocation() const;
};
