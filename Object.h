#pragma once
#include "Assets.h"

#define PLANE_SPEED 5.0f
#define PLANE_ACCEL 0.1f
#define PLANE_ROT 2
#define BULLET_SPEED 10.0f

class Object
{
protected:
	int rate = 5;
	int RATE = 5;
	int cur_fr = 0;
	bool dead = false;
	Object();
public:
	~Object();
	inline bool isDead() { return dead; }
	virtual void update(vector<shared_ptr<Object>>&) = 0;
	virtual void draw(RenderWindow&) = 0;
	static vector<shared_ptr<Object>> list;
};

class Plane : public Object
{
	RectangleShape shape;
	float speed = 0;
#define NOT_DIVING 0
#define DIVING 1
	int status = NOT_DIVING;
	shared_ptr<Animation> fire_muzzle{ nullptr };
	Vector2f getFiringPosition();
	Sound firing_sound;
public:
	Plane();
	void update(vector<shared_ptr<Object>>&);
	void draw(RenderWindow&);
};

class Bullet : public Object
{
	RectangleShape shape;
	int lifetime; // in game frames
	int lifetime_remain; // in game frames
	Vector2f v;
public:
	Bullet(Vector2f, Vector2f, float rotation, int = 60);
	void update(vector<shared_ptr<Object>>&);
	void draw(RenderWindow&);
};
