#pragma once
#include "Assets.h"

#define PLANE_SPEED 5.0f
#define PLANE_ACCEL 0.1f
#define PLANE_ROT 2
#define BULLET_SPEED 10.0f

class Object
{
	bool inRectTopLeft(Vector2f, float, float, float = 800.f, float = 600.f);
	bool inRectTopLeftRight(Vector2f pos, float x, float y, float w = 800.f, float h = 600.f);
	bool inRectAll(Vector2f, float, float, float = 800.f, float = 600.f);
protected:
	int rate = 5;
	int RATE = 5;
	int cur_fr = 0;
	bool dead = false;
	Object();
public:
	~Object();
	inline bool isDead() { return dead; }
	virtual void update() = 0;
	virtual void draw(RenderWindow&) = 0;
	virtual Vector2f getPosition() = 0;

	static vector<weak_ptr<Object>> render_buckets[5];
	static vector<weak_ptr<Object>> pos_buckets[9];
	static vector<shared_ptr<Object>> list;
	int getPositionBucket();
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
	Vector2f inBound(Vector2f);
	Sound move_sound;
	Sound jet_sound;
	RectangleShape rail_top;
	RectangleShape rail_left;
	Animation node{ &Assets::nod[0], 4, { 138, 138 }, {}, nullptr, -1 };
	void draw_nodes(RenderWindow&);
public:
	static View view;
	Plane();
	void update();
	void draw(RenderWindow&);
	inline Vector2f getPosition() { return shape.getPosition(); }
};

class Bullet : public Object
{
	RectangleShape shape;
	int lifetime; // in game frames
	int lifetime_remain; // in game frames
	Vector2f v;
public:
	Bullet(Vector2f, Vector2f, float rotation, int = 60);
	void update();
	void draw(RenderWindow&);
	inline Vector2f getPosition() { return shape.getPosition(); }
};
