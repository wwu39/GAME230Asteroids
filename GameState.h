#pragma once
#include <memory>
#include <vector>
#include "Object.h"

enum GameStatus { NO_CHANGE, MENU, GAME, NEXT_LEVEL, EXIT };

class GameState
{
protected:
public:
	GameState();
	~GameState();
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
public:
	Menu();
	GameStatus update();
	void draw(RenderWindow&);
};

class Asteroids : public GameState
{
	RectangleShape bg;
	Sound landing_sound;
	Text socre_text;
	Text debug;
	shared_ptr<Effect> redeploy{ nullptr };
	shared_ptr<Effect> landing{ nullptr };
public:
	Asteroids(int Level);
	~Asteroids();
	GameStatus update();
	void draw(RenderWindow&);
};