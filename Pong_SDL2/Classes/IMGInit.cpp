#include "IMGInit.h"

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <stdexcept>

IMGInit::IMGInit(int flags)
{
	//Start up SDL_image and make sure it went ok
	if ((IMG_Init(flags) & flags) != flags) {
		throw std::runtime_error("Can't IMG_Init");
	}
	std::cout << "IMGInit ctor was called" << std::endl;
}

IMGInit::~IMGInit()
{
	IMG_Quit();
	std::cout << "~IMGInit was called" << std::endl;
}
