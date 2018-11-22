#pragma once
#include "Assets.h"
class Object
{
protected:
	bool dead = false;
	Object();
public:
	~Object();
	inline bool isDead() { return dead; }
	virtual void update() = 0;
	virtual void draw(RenderWindow&) = 0;
};

