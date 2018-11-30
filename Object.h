#pragma once
#include "Assets.h"

#define PLANE_SPEED 10.0f
#define PLANE_ACCEL 0.2f
#define PLANE_DCCEL 0.1f
#define PLANE_ROT 4.0f
#define PLANE_CR 69.0f
#define BULLET_SPEED 20.0f
#define BULLET_CR 3.0f

// layering
#define LAYERS 3
#define PLANE_LAYER 0
#define BULLET_LAYER 1
#define ASTR_LAYER 2

#define PI 3.14159265358979323846f

enum ObjectType { Type_Plane, Type_Bullet, Type_Asteroid };

class Object
{
protected:
	int rate = 5;
	int RATE = 5;
	int cur_fr = 0;
	float radius;
	bool dead = false;
	// handy math functions
	float length(Vector2f, Vector2f);
	bool is_same_sign(float, float);
	Object();
public:
	virtual ~Object();
	inline bool isDead() { return dead; }
	inline float getRadius() { return radius; }
	virtual void update() = 0;
	virtual void draw(RenderWindow&) = 0;
	virtual void die() = 0;
	virtual Vector2f getPosition() = 0;
	virtual ObjectType getType() = 0;

	static int score;
	static bool player_is_dead;
	static vector<shared_ptr<Object>> render_buckets[LAYERS];
	static vector<shared_ptr<Object>> pos_buckets[9];
	static vector<shared_ptr<Object>> list;
	static void add(Object*, int);
	static void clear();
	static void list_update();
	static void draw_all(RenderWindow&);
	int getPositionBucket();
};

class Plane : public Object
{
	RectangleShape shape;
	float speed = 0;
#define NOT_DASHING 0
#define DASHING 1
	int status = NOT_DASHING;
	shared_ptr<Animation> fire_muzzle{ nullptr };
	Vector2f getFiringPosition();
	Sound firing_sound;
	Vector2f inBound(Vector2f);
	Sound move_sound;
	Sound jet_sound;
	RectangleShape rail_top;
	RectangleShape rail_left;
	Animation node{ &Assets::nod[0], 4, { 138, 138 }, {}, nullptr, -1 };
public:
	static View view;
	Plane(Vector2f pos = {0, 0});
	void update();
	void draw(RenderWindow&);
	void die();
	inline Vector2f getPosition() { return shape.getPosition(); }
	inline ObjectType getType() { return Type_Plane; }
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
	void die();
	inline Vector2f getPosition() { return shape.getPosition(); }
	inline ObjectType getType() { return Type_Bullet; }
};

class Asteroid : public Object
{
	shared_ptr<Animation> image {nullptr};
	Vector2f v;
	int size;
public:
	Asteroid(int size, Vector2f pos, Vector2f v);
	void update();
	void draw(RenderWindow&);
	void die();
	inline Vector2f getPosition() { return image->getPosition(); }
	inline ObjectType getType() { return Type_Asteroid; }
};
