#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "Core.h"


int main(int, char**) {
	Core* core = Core::sharedCore();
	
	if (core)
		core->runGame();

	return 0;
}


