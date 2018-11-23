#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace sf;
using namespace std;

class Assets
{
public:
	// here are all assets we need
	static Texture menu_bg_tex;
	static Font arcade;
	static SoundBuffer menu_bgm;
	static Texture bomb[16];
	static SoundBuffer option;
	static SoundBuffer button;
	static SoundBuffer title;
	static Sound button_press;
	static Texture explosion[34];
	static SoundBuffer explosion_sound;
	static Texture plane[2][2];
	static Texture muzzle[4];
	static Texture bullet;
	static SoundBuffer gunfire;

	// load all files needed
	static void load();
	Assets();
	~Assets();
};

class Effect
{
protected:
	Effect();
	bool dead = false;
	int rate;
	int RATE;
	int loop_count;
	int times_played { 0 };
public:
	inline bool isDead() { return dead; }
	virtual void play() = 0;
	virtual void setPosition(Vector2f) = 0;
	virtual void draw(RenderWindow&) = 0;
	static vector<shared_ptr<Effect>> list;
};

class Animation : public Effect
{
	RectangleShape sprite;
	Texture * texture;
	int cur_fr = 0;
	int length;
	Sound sound;
public:
	Animation(Texture* , int, Vector2f, Vector2f = {0, 0}, SoundBuffer* = nullptr, int = 1, int = 5);
	void play();
	void draw(RenderWindow&);
	inline void setPosition(Vector2f pos) { sprite.setPosition(pos); }
};

class FlashingText : public Effect
{
	Text text;
	bool flashing = true;
public:
	FlashingText(string text, Vector2f pos, int char_size = 50, int loop_count = 3, int rate = 15);
	void play();
	void draw(RenderWindow&);
	inline void setPosition(Vector2f pos) { text.setPosition(pos); }
};

