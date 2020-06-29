#include "SDLInit.h"



#include <iostream>
#include <stdexcept>

SDLInit::SDLInit(Uint32 flags)
{
	//Start up SDL and make sure it went ok // 
	if (SDL_Init(flags) != 0) {
		throw std::runtime_error("Can't SDL_Init");
	}
	std::cout << "SDLInit ctor was called" << std::endl;
}

SDLInit::~SDLInit()
{
	SDL_Quit();
	std::cout << "~SDLInit was called" << std::endl;
}
