#pragma once
#include "Assets.h"

#define PLANE_SPEED 10.0f
#define PLANE_ACCEL 0.2f
#define PLANE_DCCEL 0.1f
#define PLANE_ROT 4.0f
#define PLANE_CR 69.0f
#define BULLET_SPEED 20.0f
#define BULLET_CR 3.0f
#define ASTR_L_SPEED 1.0f
#define ASTR_M_SPEED 2.0f
#define ASTR_S_SPEED 3.0f
#define MISSILE_CR 10.0f
#define MISSILE_SPEED 12.0f
#define ALIENSHIP_SPEED 2.5f

// layering
#define LAYERS 4
#define PLANE_LAYER 0
#define BULLET_LAYER 1
#define ASTR_LAYER 2
#define ELECT_LAYER 3

#define PI 3.14159265358979323846f

enum ObjectType { Type_Plane, Type_Bullet, Type_Asteroid, Type_Enemy_Bullet, Type_Powerup, Type_Others };

class Object
{
protected:
	int rate = 5;
	int RATE = 5;
	int cur_fr = 0;
	float radius;
	bool dead = false;
	// handy math functions
	static float length(Vector2f, Vector2f);
	static bool is_same_sign(float, float);
	static Vector2f getUnitVector(Vector2f);
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
	virtual bool isShielded() = 0;
	virtual bool isAlienship() = 0;

	static int score;
	static int asteroid_count;
	static bool player_is_dead;
	static vector<shared_ptr<Object>> render_buckets[LAYERS];
	static vector<shared_ptr<Object>> pos_buckets[9];
	static vector<shared_ptr<Object>> list;
	static void add(Object*, int);
	static void clear();
	static void list_update();
	static void draw_all(RenderWindow&);
	int getPositionBucket();
	vector<int> getNearbyBuckets(int bucket);
	bool inView();
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
	Vector2f getFiringPosition2();
	Vector2f getFiringPosition3();
	Sound firing_sound;
	Sound firing_sound2;
	Vector2f inBound(Vector2f);
	Sound move_sound;
	Sound jet_sound;
	int can_fire_misl = 5;
public:
	static View view;
	static shared_ptr<Animation> shield;
	static int life;
	static int rof;
	Plane(Vector2f pos = {0, 0});
	void update();
	void draw(RenderWindow&);
	void die();
	inline Vector2f getPosition() { return shape.getPosition(); }
	inline ObjectType getType() { return Type_Plane; }
	inline bool isShielded() { return shield != nullptr; }
	inline bool isAlienship() { return false; }
};

class Bullet : public Object
{
	RectangleShape shape;
	int lifetime; // in game frames
	int lifetime_remain; // in game frames
	Vector2f v;
public:
	Bullet(Vector2f pos_s, Vector2f v, float rotation, int = 30);
	void update();
	void draw(RenderWindow&);
	void die();
	inline Vector2f getPosition() { return shape.getPosition(); }
	inline ObjectType getType() { return Type_Bullet; }
	inline bool isShielded() { return false; }
	inline bool isAlienship() { return false; }
};

class Asteroid : public Object
{
	shared_ptr<Animation> image{ nullptr };
	RectangleShape shape;

	Vector2f v;
	int size;
public:
	static float speed_mult;
	Asteroid(int size, Vector2f pos);
	void update();
	void draw(RenderWindow&);
	void die();
	Vector2f getPosition();
	void setPosition(Vector2f pos);
	inline ObjectType getType() { return Type_Asteroid; }
	inline bool isShielded() { return false; }
	inline bool isAlienship() { return false; }
};

class RailNode : public Object
{
	RectangleShape rail_top;
	RectangleShape rail_left;
	Animation node{ &Assets::nod[0], 4, { 138, 138 }, {}, nullptr, -1 };
public:
	RailNode();
	void update();
	void draw(RenderWindow&);
	void die();
	inline Vector2f getPosition() { return {}; } // has no position
	inline ObjectType getType() { return Type_Others; }
	inline bool isShielded() { return false; }
	inline bool isAlienship() { return false; }
};

class Missile : public Object
{
	RectangleShape shape;
	int lifetime; // in game frames
	int lifetime_remain; // in game frames
	int trailer_delay = 0;
	Vector2f v;
	shared_ptr<Object> target;
public:
	Missile(Vector2f pos_s, Vector2f v, float rotation, shared_ptr <Object> = nullptr, int = 60);
	void update();
	void draw(RenderWindow&);
	void die();
	inline Vector2f getPosition() { return shape.getPosition(); }
	inline ObjectType getType() { return Type_Bullet; }
	inline bool isShielded() { return false; }
	inline bool isAlienship() { return false; }
};

class AlienShip : public Object
{
	Animation image{ &Assets::enemy[0], 4, {150, 112}, {}, nullptr, -1 };
	Vector2f v;
	Sound firing_sound;
	int rot = 0;
	int rof = 0;;
public:
	AlienShip(Vector2f);
	void update();
	void draw(RenderWindow&);
	void die();
	inline Vector2f getPosition() { return image.getPosition(); }
	inline ObjectType getType() { return Type_Asteroid; }
	inline bool isShielded() { return false; }
	inline bool isAlienship() { return true; }
};

class AlienBullet : public Object
{
	Animation image{ &Assets::electball[0], 20, {35, 36}, {}, nullptr, -1 };
	int lifetime; // in game frames
	int lifetime_remain; // in game frames
	Vector2f v;
public:
	AlienBullet(Vector2f pos_s, int lifetime = 120);
	void update();
	void draw(RenderWindow&);
	void die();
	inline Vector2f getPosition() { return image.getPosition(); }
	inline ObjectType getType() { return Type_Enemy_Bullet; }
	inline bool isShielded() { return false; }
	inline bool isAlienship() { return true; }
};

class Powerup : public Object
{
#define FIRE_FASTER 0
#define SHIELD_UP 1
#define LIFE_PLUS1 2
	Animation image { &Assets::powerup[0], 20, {64, 60}, {}, nullptr, -1 };
	Vector2f v;
	int type;
	void applyEffect();
public:
	Powerup(int type, Vector2f pos_s);
	void update();
	void draw(RenderWindow&);
	void die();
	inline Vector2f getPosition() { return image.getPosition(); }
	inline ObjectType getType() { return Type_Powerup; }
	inline bool isShielded() { return false; }
	inline bool isAlienship() { return true; }
};