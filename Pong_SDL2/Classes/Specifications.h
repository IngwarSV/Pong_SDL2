#pragma once

#include <SDL.h> // for SDL_Color

#include <string>


struct Location {
	int x = 0, y = 0;

	Location operator+(const Location& other) {
		return Location{ this->x + other.x, this->y + other.y };
	}
	void operator+=(const Location& other) {
		this->x += other.x;
		this->y += other.y;
	}
	void operator*=(const Location& other) {
		this->x *= other.x;
		this->y *= other.y;
	}
	bool operator==(const Location& other) const {
		return this->x == other.x && this->y == other.y;
	}
	bool operator!=(const Location& other) const {
		return this->x != other.x || this->y != other.y;
	}
};

enum class CollisionType {
	NONE,
	LEFT_EDGE,
	RIGHT_EDGE
};


namespace DEF_SETT {
	// Screen size	
	constexpr int SCREEN_WIDTH = 1024;
	constexpr int SCREEN_HEIGHT = 768;

	// Game characteristics
	constexpr int SPEED_RATIO = 2;
	constexpr int SCORE_TO_WIN = 3;

	// Field characteristics
	constexpr int UPPER_FIELD_EDGE = 8;
	constexpr int BOTTOM_FIELD_EDGE = 760;
	constexpr int LEFT_FIELD_EDGE = 40;
	constexpr int RIGHT_FIELD_EDGE = 980;
	constexpr Location FIELD_CENTER{ (RIGHT_FIELD_EDGE - LEFT_FIELD_EDGE) / 2, (BOTTOM_FIELD_EDGE - UPPER_FIELD_EDGE) / 2 };

	// Ball characteristics
	constexpr int BALL_SPEED = 5;
	constexpr int BALL_WIDTH = 20;
	constexpr int BALL_HEIGHT = 20;
	// Paddle characteristics
	constexpr int PADDLE_SPEED = 12;
	constexpr int AI_LOW_PADDLE_SPEED = 2;
	constexpr int PADDLE_WIDTH = 20;
	constexpr int PADDLE_HEIGHT = 80;
	constexpr int UPPER_PADDLE_EDGE = UPPER_FIELD_EDGE + PADDLE_HEIGHT / 2;
	constexpr int BOTTOM_PADDLE_EDGE = BOTTOM_FIELD_EDGE - PADDLE_HEIGHT;

	// AI characteristics
	constexpr int AI_REACTION_DIST = (RIGHT_FIELD_EDGE - LEFT_FIELD_EDGE) / 3;
	constexpr int AI_SPEED = BALL_SPEED;

	// PI
	constexpr float PI = 3.14159265f;

	// Labels stuff
	const std::string FONT_FILE{ "res\\royal-serif.ttf" };
	constexpr int FONT_SIZE = 50;
	constexpr int FPS_FONT_SIZE = 20;
	constexpr Location PL_SCORE_LABEL{ FIELD_CENTER.x / 2 - FONT_SIZE, UPPER_PADDLE_EDGE };
	constexpr Location AI_SCORE_LABEL{ FIELD_CENTER.x + FIELD_CENTER.x / 2 - FONT_SIZE, UPPER_PADDLE_EDGE };
	constexpr Location FPS_LABEL{ LEFT_FIELD_EDGE + FPS_FONT_SIZE, BOTTOM_FIELD_EDGE - FPS_FONT_SIZE };
	constexpr Location INFO_LABEL1{ FIELD_CENTER.x - FONT_SIZE * 2.3f, UPPER_PADDLE_EDGE + FONT_SIZE * 3 };
	constexpr Location INFO_LABEL2{ FIELD_CENTER.x - FONT_SIZE * 2.3f, UPPER_PADDLE_EDGE + FONT_SIZE * 5 };
	constexpr SDL_Color METALLIC_GOLD{ 212, 175, 55 };
	constexpr SDL_Color BLUE{ 65,105,225 };
	constexpr SDL_Color CHOKOLATE{ 210,105,30 };
	constexpr SDL_Color GREY{ 187, 190, 194 };
	constexpr SDL_Color DARK_GREY{ 67, 68, 69 };
}
