#include "TTFInit.h"

#include <SDL.h>
#include <SDL_ttf.h> 

#include <iostream>
#include <stdexcept>

TTFInit::TTFInit()
{
	//Start up SDL_ttf and make sure it went ok
	if (TTF_Init()) {
		throw std::runtime_error("Can't TTF_Init");
	}
	std::cout << "TTFInit ctor was called" << std::endl;
}

TTFInit::~TTFInit()
{
	TTF_Quit();
	std::cout << "~TTFInit was called" << std::endl;
}
