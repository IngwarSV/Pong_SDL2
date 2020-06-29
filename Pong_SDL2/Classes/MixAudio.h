#pragma once

#include <SDL.h>

class MixAudio {
public:
	MixAudio(int frequency, Uint16 format, int channels, int chunksize);
	~MixAudio();
};