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

#include "SDLInit.h"
#include "IMGInit.h"
#include "TTFInit.h"
#include "MixAudio.h"

#include "Specifications.h"
#include "Timer.h"
#include "Ball.h"
#include "Paddle.h"
#include "cleanUp.h"

using namespace DEF_SETT;
/*
Main game class (Meyers Singleton), composites:
- Ball object, Player_Paddle object, AI_Paddle object (wrapped in unique_ptr);
- four objects, which were created for SDL library and its sublibraries initialization following RAII idiom.
- all other SDL objects (wrapped in unique_ptr + std::default_delete).
*/

template<typename Creator, typename... Arguments>
auto make_resource(Creator c, Arguments&&... args);

// Specialising default deleters for SDL types
namespace std {
	template<>
	struct std::default_delete<SDL_Window> {
		void operator()(SDL_Window* var) {
			SDL_DestroyWindow(var);
			std::cout << "~Window was called" << std::endl;
		}
	};

	template<>
	struct std::default_delete<SDL_Renderer> {
		void operator()(SDL_Renderer* var) {
			SDL_DestroyRenderer(var);
			std::cout << "~Renderer was called" << std::endl;
		}
	};

	template<>
	struct std::default_delete<SDL_Texture> {
		void operator()(SDL_Texture* var) {
			SDL_DestroyTexture(var);
			std::cout << "~Texture was called" << std::endl;
		}
	};

	template<>
	struct std::default_delete<SDL_Surface> {
		void operator()(SDL_Surface* var) {
			SDL_FreeSurface(var);
			std::cout << "~Surface was called" << std::endl;
		}
	};

	template<>
	struct std::default_delete<TTF_Font> {
		void operator()(TTF_Font* var) {
			TTF_CloseFont(var);
			std::cout << "~Font was called" << std::endl;
		}
	};

	template<>
	struct std::default_delete<Mix_Chunk> {
		void operator()(Mix_Chunk* var) {
			Mix_FreeChunk(var);
			std::cout << "~Mix_Chunk was called" << std::endl;
		}
	};
}

class Core
{
private:
	Core();
	Core(const Core& copy) = delete;
	Core& operator=(const Core& copy) = delete;
	~Core();

// attributes
	// SDL libraries
	std::unique_ptr<SDLInit> sdlInit;
	std::unique_ptr<IMGInit> imgInit;
	std::unique_ptr<TTFInit> ttfInit;
	std::unique_ptr<MixAudio> audioInit;
	
	// window and renderer
	std::unique_ptr<SDL_Window> _window;
	std::unique_ptr<SDL_Renderer> _renderer;
	
	// game objects
	std::unique_ptr<Ball> _ball;
	std::unique_ptr<Paddle> _paddlePl;
	std::unique_ptr<Paddle> _paddleAI;
	
	// textures
	std::unique_ptr<SDL_Texture> _ballTexture;
	std::unique_ptr<SDL_Texture> _paddlePlTexture;
	std::unique_ptr<SDL_Texture> _paddleAITexture;
	std::unique_ptr<SDL_Texture> _scorePlTexture;
	std::unique_ptr<SDL_Texture> _scoreAITexture;
	std::unique_ptr<SDL_Texture> _FPSTexture;
	std::unique_ptr<SDL_Texture> _infoLabelTexture1;
	std::unique_ptr<SDL_Texture> _infoLabelTexture2;

	// sounds
	std::unique_ptr<Mix_Chunk> _paddleHitSound;
	std::unique_ptr<Mix_Chunk> _wallHitSound;
	std::unique_ptr<Mix_Chunk> _goalSound;
	
	// fonts for labels
	std::unique_ptr<TTF_Font> _font;
	std::unique_ptr<TTF_Font> _fontFPS;
	
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
	// handling keys input
	virtual void input();

	/* main game block: */
	// updating gameData every frame 
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

	// rendering gameScene every frame
	void Render();

	// service methods
	void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h);

	void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y);
	
	int getRandom(int min, int max);

public:
// methods
	// getting Meyers Singleton for Core
	static Core& sharedCore();

	// launching gameLoop
	void runGame();
};
