#pragma once
#include <memory>
#include <vector>
#include "Object.h"

enum GameStatus { NO_CHANGE, MENU, GAME, NEXT_LEVEL, EXIT };

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
	Text socre_text;
	Text gameover_text;
	Text debug;
	RailNode railnode;
	shared_ptr<Effect> redeploy{ nullptr };
	shared_ptr<Effect> landing{ nullptr };
	shared_ptr<Effect> level_title{ nullptr };
	AsteroidsStatus status = STARTING;

	void draw_life_image(RenderWindow&);
public:
	Asteroids(int Level);
	~Asteroids();
	GameStatus update();
	void draw(RenderWindow&);
};