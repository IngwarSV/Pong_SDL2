#include "Core.h"

template<typename Creator, typename... Arguments>
auto make_resource(Creator c, Arguments&&... args)
{
	auto r = c(std::forward<Arguments>(args)...);
	if (!r) { 
		throw std::system_error(errno, std::generic_category()); 
		//throw std::runtime_error("Can't create something");
	}
	std::cout << typeid(r).name() << " ctor was called" << std::endl;
	return std::unique_ptr<std::decay_t<decltype(*r)>>(r);
}


Core::Core() {
	std::cout << "Core ctor began its work" << std::endl;

	// SDL library and its sublibraries initialization
	sdlInit = std::make_unique<SDLInit>(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	imgInit = std::make_unique<IMGInit>(IMG_INIT_PNG);
	ttfInit = std::make_unique<TTFInit>();
	audioInit = std::make_unique<MixAudio>(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	
	// window and renderer
	_window = make_resource(SDL_CreateWindow, "Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		DEF_SETT::SCREEN_WIDTH, DEF_SETT::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	_renderer = make_resource(SDL_CreateRenderer, _window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	// Loading fonts
	_font = make_resource(TTF_OpenFont, FONT_FILE.c_str(), FONT_SIZE);
	_fontFPS = make_resource(TTF_OpenFont, FONT_FILE.c_str(), FPS_FONT_SIZE);

	// Loading textures
	_ballTexture = make_resource(IMG_LoadTexture, _renderer.get(), "res\\ball.png");
	_paddlePlTexture = make_resource(IMG_LoadTexture, _renderer.get(), "res\\paddlePlayer.png");
	_paddleAITexture = make_resource(IMG_LoadTexture, _renderer.get(), "res\\paddleAI.png");
	
	_scorePlTexture = make_resource(SDL_CreateTextureFromSurface, _renderer.get(), 
		(make_resource(TTF_RenderText_Blended, _font.get(), ("PL : " + std::to_string(_playerScore)).c_str(), BLUE)).get());
	_scoreAITexture = make_resource(SDL_CreateTextureFromSurface, _renderer.get(),
		(make_resource(TTF_RenderText_Blended, _font.get(), ("AI : " + std::to_string(_AIScore)).c_str(), CHOKOLATE)).get());
	_FPSTexture = make_resource(SDL_CreateTextureFromSurface, _renderer.get(),
		(make_resource(TTF_RenderText_Blended, _fontFPS.get(), ("FPS: " + std::to_string(_FPS)).c_str(), GREY)).get());
	_infoLabelTexture1 = make_resource(SDL_CreateTextureFromSurface, _renderer.get(),
		(make_resource(TTF_RenderText_Blended, _font.get(), "<<PONG!!!>>", BLUE)).get());
	_infoLabelTexture2 = make_resource(SDL_CreateTextureFromSurface, _renderer.get(),
		(make_resource(TTF_RenderText_Blended, _font.get(), "PRESS SPC", CHOKOLATE)).get());
	
	// Loading sounds
	_paddleHitSound = make_resource(Mix_LoadWAV_RW, SDL_RWFromFile("res\\paddleHitSound.wav", "rb"), 1);
	_wallHitSound = make_resource(Mix_LoadWAV_RW, SDL_RWFromFile("res\\wallHitSound.wav", "rb"), 1);
	_goalSound = make_resource(Mix_LoadWAV_RW, SDL_RWFromFile("res\\goalSound.wav", "rb"), 1);
		
		
	// Initializing other gameData
	_randomGenerator.seed(time(NULL));
	_ball = std::make_unique<Ball>();
	_paddlePl = std::make_unique<Paddle>(Location{ LEFT_FIELD_EDGE - PADDLE_WIDTH, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2 });
	_paddleAI = std::make_unique<Paddle>(Location{ RIGHT_FIELD_EDGE, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2 });

	std::cout << "Core ctor finished its work" << std::endl;
}


Core::~Core()
{
	std::cout << "~Core was called" << std::endl;
}

Core& Core::sharedCore()
{
	static Core s_sharedCore;

	return s_sharedCore;
}

void Core::runGame()
{
	Timer timer;
	int sum = 0;
	int deltaTime = 0;

	while (!_gameOver)
	{
		timer.startTimer(); // starting time counting per frame

		input();

		updateF(deltaTime);

		Render();
		
		while (1)
		{
			
			deltaTime = timer.timeElapsed(); // time from frame beginning
			// waiting at least DELAY_UPDATE millisecond
			if (deltaTime > DELAY_UPDATE)
				break;
		}

		sum += deltaTime;
		_FPS += 1;
		// updating FPS label every ~ 1 second
		if (sum >= 1000) 
		{
			_FPSTexture = make_resource(SDL_CreateTextureFromSurface, _renderer.get(),
				(make_resource(TTF_RenderText_Blended, _fontFPS.get(), ("FPS: " + std::to_string(_FPS)).c_str(), GREY)).get());

			_FPS = 0;
			sum = 0;
		}
	}
}

void Core::input()
{
	SDL_Event event;    // stores next event to be processed

	// Handling events
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym) {
			// Moving Player_paddle up
			case SDLK_UP:
			case SDLK_w:
				_paddlePl->movePaddleUp();
				if (_paddlePl->getLocation().y < UPPER_PADDLE_EDGE) {
					_paddlePl->setLocation(UPPER_PADDLE_EDGE);
				}
				break;
			// Moving Player_paddle down
			case SDLK_DOWN:
			case SDLK_s:
				_paddlePl->movePaddleDown();
				if (_paddlePl->getLocation().y + PADDLE_HEIGHT > BOTTOM_FIELD_EDGE) {
					_paddlePl->setLocation(BOTTOM_PADDLE_EDGE);
				}
				break;

			// Pressing ESC exits from the game
			case SDLK_SPACE:
				// Launching ball at the beginning of the game
				if (!_ballLaunched) {
					_ballLaunched = true;
					_ball->resetBallLocation(FIELD_CENTER.y - BALL_HEIGHT / 2, getRandom(120, 240));
					// Erasing score in case of restarted game
					_scorePlTexture = make_resource(SDL_CreateTextureFromSurface, _renderer.get(),
						(make_resource(TTF_RenderText_Blended, _font.get(), ("PL : " + std::to_string(_playerScore)).c_str(), BLUE)).get());
					_scoreAITexture = make_resource(SDL_CreateTextureFromSurface, _renderer.get(),
						(make_resource(TTF_RenderText_Blended, _font.get(), ("AI : " + std::to_string(_AIScore)).c_str(), CHOKOLATE)).get());
					
					// Erasing infoLabels
					_infoLabelTexture1 = make_resource(SDL_CreateTextureFromSurface, _renderer.get(),
						(make_resource(TTF_RenderText_Blended, _font.get(), "<<PONG!!!>>", BLUE)).get());
					_infoLabelTexture2 = make_resource(SDL_CreateTextureFromSurface, _renderer.get(),
						(make_resource(TTF_RenderText_Blended, _font.get(), "PRESS SPC", CHOKOLATE)).get());
				}
				break;
			case SDLK_ESCAPE:
				_gameOver = true;
				break;
			}
		}
		//Clicking 'x'
		if (event.type == SDL_QUIT)
		{
			_gameOver = true;
		}
	}
}

void Core::updateF(int deltaTime) {
	// for speed increasing
	static int s_ballHits = 0;

	// managing AI_Paddle
	AI_Movement();

	// cheking for paddles collisions
	CollisionType type = isPaddleCollision();

	if (type != CollisionType::NONE) {
		// handling paddles collisions
		handlePaddleCollision(type);
		// cheking and handling walls collisions
		handleWallCollision();

		// calculating meeting point for AI
		if (type == CollisionType::LEFT_EDGE) {
			_meetingPoint = getMeetingPoint();
		}

		// counting ball's hits, increasing speed
		s_ballHits += 1;
		if (!(s_ballHits % SPEED_RATIO)) {
			_ball->increaseSpeed();
		}
	}
	else {
		// cheking for walls collisions. changing direction if needed
		handleWallCollision();
		// moving ball in case of no paddle collision
		_ball->moveBall();
	}

	// checking for Goal 
	int ballCoordX = _ball->getLocation().x;
	if (ballCoordX > SCREEN_WIDTH || ballCoordX < 0) {

		if (ballCoordX > SCREEN_WIDTH) {
			// updating player's score
			_playerScore += 1;
			_scorePlTexture = make_resource(SDL_CreateTextureFromSurface, _renderer.get(),
				(make_resource(TTF_RenderText_Blended, _font.get(), ("PL : " + std::to_string(_playerScore)).c_str(), BLUE)).get());
			Mix_PlayChannel(-1, _goalSound.get(), 0);
		}
		else if (ballCoordX < 0) {
			// updating AI's score
			_AIScore += 1;
			_scoreAITexture = make_resource(SDL_CreateTextureFromSurface, _renderer.get(),
				(make_resource(TTF_RenderText_Blended, _font.get(), ("AI : " + std::to_string(_AIScore)).c_str(), CHOKOLATE)).get());
			Mix_PlayChannel(-1, _goalSound.get(), 0);
		}
		// reseting paddles hits and reducing speed to initial state
		s_ballHits = 0;
		_ball->resetSpeed();

		// Chekin for end of the game
		if (_playerScore == SCORE_TO_WIN || _AIScore == SCORE_TO_WIN) {
			_ball->setSpeedVector(Location{ 0,0 });
			_ball->setLocation(Location{ FIELD_CENTER.x - BALL_WIDTH / 2, FIELD_CENTER.y - BALL_HEIGHT / 2 });
			_ballLaunched = false;
			_infoLabelTexture1 = (_playerScore == SCORE_TO_WIN) ?
				make_resource(SDL_CreateTextureFromSurface, _renderer.get(),
					(make_resource(TTF_RenderText_Blended, _font.get(), "<PL WINS!!!>", BLUE)).get()) :
				make_resource(SDL_CreateTextureFromSurface, _renderer.get(),
					(make_resource(TTF_RenderText_Blended, _font.get(), "<AI WINS!!!>", CHOKOLATE)).get());
			_infoLabelTexture2 = make_resource(SDL_CreateTextureFromSurface, _renderer.get(),
				(make_resource(TTF_RenderText_Blended, _font.get(), "PRESS SPC", CHOKOLATE)).get());
			// reseting score
			_playerScore = 0;
			_AIScore = 0;
		}
		else {
			// Placing ball in initial position, direction angle is in 120-240* frame (towards Player_Paddle)
			_ball->resetSpeed();
			_ball->resetBallLocation(getRandom(UPPER_PADDLE_EDGE + PADDLE_HEIGHT, BOTTOM_PADDLE_EDGE - PADDLE_HEIGHT), getRandom(120, 240));
		}
	}
}

CollisionType Core::isPaddleCollision()
{
	int nextBallCoordX = _ball->getLocation().x + _ball->getSpeedVector().x;
	int currBallCoordX = _ball->getLocation().x;
	int currBallCoordY = _ball->getLocation().y;

	// checking ball's collision with right paddle
	if (nextBallCoordX + BALL_WIDTH >= RIGHT_FIELD_EDGE && currBallCoordX < RIGHT_FIELD_EDGE) {
		// checking if ball is in paddle's frame
		if (currBallCoordY + BALL_HEIGHT >= _paddleAI->getLocation().y) {
			if (currBallCoordY <= _paddleAI->getLocation().y + PADDLE_HEIGHT) {
				return CollisionType::RIGHT_EDGE;
			}
		}
	}

	// checking ball's collision with left paddle
	if (nextBallCoordX <= LEFT_FIELD_EDGE && currBallCoordX > LEFT_FIELD_EDGE) {
		// checking if ball is in paddle's frame
		if (currBallCoordY + BALL_HEIGHT >= _paddlePl->getLocation().y) {
			if (currBallCoordY <= _paddlePl->getLocation().y + PADDLE_HEIGHT) {
				return CollisionType::LEFT_EDGE;
			}
		}
	}

	return CollisionType::NONE;
}

void Core::handlePaddleCollision(CollisionType type)
{
	int ballCenter = _ball->getLocation().y + BALL_HEIGHT / 2;
	/*
	Paddle is divided into 100 parts, if center of the ball hits paddle between
	45 and 55 part - ball moves perpendicularly paddle's surface (0 or 180*), else -
	ball changes angle from 120* to 240* (from 300* to 420* (60*) for Player_Paddle),
	considering hitted part of the paddle
	*/
	int hittedPoint;
	
	switch (type) {
	case CollisionType::RIGHT_EDGE:
		// setting ball on paddle's surface
		_ball->setLocation(Location{ RIGHT_FIELD_EDGE - PADDLE_WIDTH, _ball->getLocation().y });

		// setting bounce angle
		//// case ball hits upper edge of paddle - setting angle for 240*
		if (ballCenter <= _paddleAI->getLocation().y) {
			_ball->changeSpeedVector(240);
			break;
		}
		//// case ball hits bottom edge of paddle - setting angle for 120*
		if (ballCenter >= _paddleAI->getLocation().y + PADDLE_HEIGHT) {
			_ball->changeSpeedVector(120);
			break;
		}
		//// checking hitted part of the paddle
		hittedPoint = (ballCenter - _paddleAI->getLocation().y) * 100 / PADDLE_HEIGHT;
		//// ball hitted center part of the paddle - setting angle for 180*
		if (hittedPoint > 45 && hittedPoint < 55) {
			_ball->changeSpeedVector(180);
			break;
		}
		//// ball hitted upper / bottom part of the paddle - setting angle from 120* to 240*
		_ball->changeSpeedVector((100 - hittedPoint) * 120 / 100 + 120);
		break;

	case CollisionType::LEFT_EDGE:
		// setting ball on paddle's surface
		_ball->setLocation(Location{ LEFT_FIELD_EDGE, _ball->getLocation().y });

		// setting bounce angle
		//// case ball hits upper edge of paddle - setting angle for 300*
		if (ballCenter <= _paddlePl->getLocation().y) {
			_ball->changeSpeedVector(300);
			break;
		}
		//// case ball hits bottom edge of paddle - setting angle for 60*
		if (ballCenter >= _paddlePl->getLocation().y + PADDLE_HEIGHT) {
			_ball->changeSpeedVector(60);
			break;
		}
		//// checking hitted part of the paddle
		hittedPoint = (ballCenter - _paddlePl->getLocation().y) * 100 / PADDLE_HEIGHT;
		//// ball hitted center part of the paddle - setting angle for 0*
		if (hittedPoint > 45 && hittedPoint < 55) {
			_ball->changeSpeedVector(0);
			break;
		}
		//// ball hitted upper / bottom part of the paddle - setting angle from 300* to 420* (60*)
		_ball->changeSpeedVector((120 * hittedPoint / 100 + 300));
		break;

	default:
		break;
	}

	Mix_PlayChannel(-1, _paddleHitSound.get(), 0);
}

void Core::handleWallCollision()
{
	int nextBallCoordY = _ball->getLocation().y + _ball->getSpeedVector().y;

	if (nextBallCoordY < UPPER_FIELD_EDGE || nextBallCoordY + BALL_HEIGHT > BOTTOM_FIELD_EDGE) {
		// Changing direction for 90*
		_ball->changeSpeedVector(Location{ 1, -1 });
		Mix_PlayChannel(-1, _wallHitSound.get(), 0);
	}
}

int Core::getMeetingPoint()
{
	// Calculating finalY without wallCollisions
	//// Calculating tangent
	float tangent = static_cast<float>(_ball->getSpeedVector().y) / _ball->getSpeedVector().x;
	//// Calculating playingField width 
	int distance = RIGHT_FIELD_EDGE - LEFT_FIELD_EDGE - BALL_WIDTH;

	int finalY = static_cast<int>(abs(tangent * distance + _ball->getLocation().y));

	// number of collisions
	int fieldHeight = BOTTOM_FIELD_EDGE - UPPER_FIELD_EDGE;
	int collisionNumbers = finalY / fieldHeight;

	// getting random place of paddle to hit the ball
	int hittingPlace = getRandom(0, PADDLE_HEIGHT);

	if (collisionNumbers % 2) {
		finalY = fieldHeight - finalY % fieldHeight;
	}
	else {
		finalY = finalY % fieldHeight;
	}
	finalY += BALL_HEIGHT / 2;
	finalY -= hittingPlace;

	return finalY;
}

void Core::AI_Movement()
{
	int ballSpeedX = _ball->getSpeedVector().x;
	int paddleCoordY = _paddleAI->getLocation().y;

	// ball moving towards AIPaddle, distance is < AI_REACTION_DIST
	if (ballSpeedX > 0 && _ball->getLocation().x > AI_REACTION_DIST) {
		// AIPaddle moving towards meetingPoint
		if (paddleCoordY < _meetingPoint)
			_paddleAI->movePaddleDown(AI_SPEED);
		else if (abs(paddleCoordY - _meetingPoint) < AI_SPEED)
			_paddleAI->setLocation(_meetingPoint);
		else _paddleAI->movePaddleUp(AI_SPEED);
	}
	// ball moving towards AIPaddle, distance is > AI_REACTION_DIST,
	else if (ballSpeedX > 0) {
		// paddle moving towards center position
		if (paddleCoordY < FIELD_CENTER.y) {
			_paddleAI->movePaddleDown(AI_LOW_PADDLE_SPEED);
		}  
		else if (abs(paddleCoordY - FIELD_CENTER.y) <= AI_LOW_PADDLE_SPEED)
			_paddleAI->setLocation(FIELD_CENTER.y);
		else {
			_paddleAI->movePaddleUp(AI_LOW_PADDLE_SPEED);
		}
	}

	// adjusting AI_Paddle position in gameFrames
	paddleCoordY = _paddleAI->getLocation().y;
	if (paddleCoordY < UPPER_PADDLE_EDGE) {
		_paddleAI->setLocation(UPPER_PADDLE_EDGE);
	}
	if (paddleCoordY + PADDLE_HEIGHT > BOTTOM_FIELD_EDGE) {
		_paddleAI->setLocation(BOTTOM_PADDLE_EDGE);
	}
}

void Core::Render()
{
	// Clearing current rendering target with the drawing color
	SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
	SDL_RenderClear(_renderer.get());

	// Rendering left field in grey
	SDL_Rect leftField = { LEFT_FIELD_EDGE, UPPER_FIELD_EDGE, FIELD_CENTER.x - LEFT_FIELD_EDGE, BOTTOM_FIELD_EDGE - UPPER_FIELD_EDGE };
	SDL_SetRenderDrawColor(_renderer.get(), 105, 105, 105, 255);
	SDL_RenderFillRect(_renderer.get(), &leftField);

	// Rendering right field in dark grey
	SDL_Rect rightField = { FIELD_CENTER.x, UPPER_FIELD_EDGE, RIGHT_FIELD_EDGE - FIELD_CENTER.x, BOTTOM_FIELD_EDGE - UPPER_FIELD_EDGE };
	SDL_SetRenderDrawColor(_renderer.get(), 67, 68, 69, 255);
	SDL_RenderFillRect(_renderer.get(), &rightField);

	renderTexture(_scorePlTexture.get(), _renderer.get(), PL_SCORE_LABEL.x, PL_SCORE_LABEL.y);
	renderTexture(_scoreAITexture.get(), _renderer.get(), AI_SCORE_LABEL.x, AI_SCORE_LABEL.y);

	renderTexture(_ballTexture.get(), _renderer.get(), _ball->getLocation().x, _ball->getLocation().y);
	renderTexture(_paddlePlTexture.get(), _renderer.get(), _paddlePl->getLocation().x, _paddlePl->getLocation().y);
	renderTexture(_paddleAITexture.get(), _renderer.get(), _paddleAI->getLocation().x, _paddleAI->getLocation().y);
	renderTexture(_FPSTexture.get(), _renderer.get(), FPS_LABEL.x, FPS_LABEL.y);
	renderTexture(_infoLabelTexture1.get(), _renderer.get(), INFO_LABEL1.x, INFO_LABEL1.y);
	renderTexture(_infoLabelTexture2.get(), _renderer.get(), INFO_LABEL2.x, INFO_LABEL2.y);

	SDL_RenderPresent(_renderer.get());
}


void Core::renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

void Core::renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y)
{
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}



int Core::getRandom(int min, int max)
{
	assert(max >= min);

	std::uniform_int_distribution<int> uid(min, max);

	return uid(_randomGenerator);
}
