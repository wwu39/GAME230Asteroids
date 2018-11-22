#pragma once
#include "Assets.h"
class Object
{
protected:
	Object();
public:
	~Object();
	virtual void update() = 0;
	virtual void draw(RenderWindow&) = 0;
};

