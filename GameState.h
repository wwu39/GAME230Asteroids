#pragma once
#include <memory>
#include <vector>
#include "Object.h"

enum GameStatus { NO_CHANGE, MENU, LOADING, GAME, NEXT_LEVEL, EXIT };

class GameState
{
protected:
	GameState();
public:
	virtual ~GameState();
	virtual GameStatus update() = 0;
	virtual void draw(RenderWindow&) = 0;
};

class Menu : public GameState
{
	Sound bgm;
	RectangleShape bg;
	Text options;
	CircleShape cursor;
	Sound option;
	Sound title;
	int wait = 0;
	bool active = false;
public:
	Menu();
	GameStatus update();
	void draw(RenderWindow&);
};

enum AsteroidsStatus {STARTING, RUNNING, ENDING, GAMEOVER};

class Asteroids : public GameState
{
	RectangleShape bg;
	RectangleShape life_image;
	Sound landing_sound;
	Sound gameover_voice;
	Sound levelclear_voice;
	Sound winning_bgm;
	Sound losing_bgm;
	Sound level_bgm;
	Text socre_text;
	Text gameover_text;
	Text debug;
	RailNode railnode;
	shared_ptr<Effect> redeploy{ nullptr };
	shared_ptr<Effect> landing{ nullptr };
	shared_ptr<Effect> level_title{ nullptr };
	AsteroidsStatus status = STARTING;
	int AlienshipInterval = 300;
	Vector2f AlienshipLocs[4]{ {-900, -700},{1700, -700},{-900, 1300},{1700, 1300} };
	int AlienshipLocIdx = 0;
	void draw_life_image(RenderWindow&);
	int c = 255;
	bool c_reverse = true;
public:
	Asteroids(int Level);
	~Asteroids();
	GameStatus update();
	void draw(RenderWindow&);
};

class Loading : public GameState
{
	Text context;
	String context_content;
	RectangleShape bg;
	shared_ptr<FlashingText> prompt{nullptr};
	Sound typing;
	int stun = 0;
	int i = 1, j = 0;
public:
	Loading();
	~Loading();
	GameStatus update();
	void draw(RenderWindow&);
};