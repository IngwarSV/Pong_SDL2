#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "Core.h"


int main(int, char**) {
	try {
		Core& core = Core::sharedCore();
		core.runGame();
	}
	catch (const std::runtime_error& e){
		std::cout << "Exception: " << e.what() << std::endl;
		return -1;
	}


	

	return 0;
}


