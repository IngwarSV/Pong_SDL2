#include "MixAudio.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include <iostream>
#include <stdexcept>

MixAudio::MixAudio(int frequency, Uint16 format, int channels, int chunksize)
{
	// Initializing SDL_mixer
	if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0)
	{
		throw std::runtime_error("Can't initialize SDL_mixer");
	}
	std::cout << "Mix_OpenAudio ctor was called" << std::endl;
}

MixAudio::~MixAudio()
{
	Mix_CloseAudio();
	std::cout << "~MixAudio was called" << std::endl;
}
