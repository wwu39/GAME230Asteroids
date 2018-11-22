#pragma once
#include <memory>
#include <vector>
#include "Object.h"

class GameState
{
protected:
	shared_ptr<GameState> next = { nullptr };
public:
	GameState();
	~GameState();
	inline bool hasNext() { return next != nullptr; }
	inline GameState * getNext() { return next.get(); }
	virtual void update() = 0;
	virtual void draw(RenderWindow&) = 0;
};

class Menu : public GameState
{
	Sound bgm;
	RectangleShape bg;
public:
	Menu();
	void update();
	void draw(RenderWindow&);
};

class Asteroids : public GameState
{
	vector<shared_ptr<Object>> object_list;
	vector<shared_ptr<Effect>> Effect_list;
public:
	Asteroids();
	void update();
	void draw(RenderWindow&);
};