#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h> 
#include <SDL_mixer.h>

#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <assert.h>
#include <memory>

#include "Specifications.h"
#include "Timer.h"
#include "Ball.h"
#include "Paddle.h"
#include "cleanUp.h"


using namespace DEF_SETT;
/*
Main game class (Meyers Singleton), composites:
- Ball object, Player_Paddle object, AI_Paddle object (wrapped in unique_ptr);
- all SDL stuff, including SDL_mixer and SDL_ttf objects.

Future improvements:
- class is too big, needs to be divided into smaller parts (through composition)

*/
class Core
{
private:
	Core() {};
	Core(const Core& copy) = delete;
	Core& operator=(const Core& copy) = delete;
	~Core();

// attributes
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;

	// game objects
	std::unique_ptr<Ball> _ball;
	std::unique_ptr<Paddle> _paddlePl;
	std::unique_ptr<Paddle> _paddleAI;
	
	// textures
	SDL_Texture* _ballTexture = nullptr;
	SDL_Texture* _paddlePlTexture = nullptr;
	SDL_Texture* _paddleAITexture = nullptr;
	SDL_Texture* _scorePlTexture = nullptr;     
	SDL_Texture* _scoreAITexture = nullptr;
	SDL_Texture* _FPSTexture = nullptr;
	SDL_Texture* _infoLabelTexture1 = nullptr;
	SDL_Texture* _infoLabelTexture2 = nullptr;

	// sounds
	Mix_Chunk* _paddleHitSound = nullptr;
	Mix_Chunk* _wallHitSound = nullptr;
	Mix_Chunk* _goalSound = nullptr;

	// fonts for labels
	TTF_Font* _font = nullptr;
	TTF_Font* _fontFPS = nullptr;

	// random generator for randomizing ball starting angles etc.
	std::mt19937 _randomGenerator;

	// gameData
	bool _gameOver = false;
	int _playerScore = 0;
	int _AIScore = 0;
	int _meetingPoint = (BOTTOM_FIELD_EDGE - UPPER_FIELD_EDGE) / 2;
	int _FPS = 0;
	bool _ballLaunched = false;

// methods
	bool init();

	// handling keys input
	virtual void input();

	/* main game block: */
	// updating gameData every frame (~48 FPS)
	virtual void updateF(int deltaTime);

	//// checking for paddles collisions
	CollisionType isPaddleCollision();

	//// handling paddles collisions
	void handlePaddleCollision(CollisionType type);

	//// handling walls collisions
	void handleWallCollision();

	//// handling AI movement
	void AI_Movement();

	//// calculating MeetingPoint (yCoord) with ball for AI_paddle
	int getMeetingPoint();
	/**/

	// rendering gameScene every frame (~48 FPS)
	void Render();


	// service methods
	void logSDLError(std::ostream& os, const std::string& msg);

	SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* ren);

	void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h);

	void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y);

	SDL_Texture* renderText(const std::string& message, TTF_Font* font,
		SDL_Color color, int fontSize, SDL_Renderer* renderer);
	
	int getRandom(int min, int max);

public:
// methods
	// getting Meyers Singleton for Core
	static Core* sharedCore();

	// launching gameLoop
	void runGame();
};
