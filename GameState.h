#pragma once
#include <memory>
#include <vector>
#include "Object.h"

enum GameStatus { NO_CHANGE, MENU, GAME, EXIT };

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
	vector<shared_ptr<Object>> object_list;
	vector<shared_ptr<Effect>> effect_list;
public:
	Asteroids();
	GameStatus update();
	void draw(RenderWindow&);
};