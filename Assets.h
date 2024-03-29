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
	static Texture loading_bg_tex;
	static SoundBuffer typing;
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
	static Texture sky;
	static SoundBuffer plane_move;
	static SoundBuffer jet_move;
	static Texture rail;
	static Texture nod[4];
	static Texture piff[4];
	static Texture plane_landing[20];
	static SoundBuffer landing;
	static Texture shield[9];
	static Texture missile;
	static Texture trailer[20];
	static Texture misl_exp[13];
	static SoundBuffer misl_exp_sound;
	static SoundBuffer misl_launch;
	static Texture arrow_red;
	static Texture arrow_yellow;
	static Texture enemy[4];
	static Texture electball[20];
	static Texture blast[27];
	static SoundBuffer laser;
	static SoundBuffer gameover;
	static SoundBuffer levelclear;
	static SoundBuffer level_bgm;
	static SoundBuffer winning_bgm;
	static SoundBuffer losing_bgm;
	static Texture powerup[20];
	static SoundBuffer firefaster;
	static SoundBuffer shieldup;
	static SoundBuffer lifeplus;
#define SMALL 0
#define MEDIUM 1
#define LARGE 2
	static Texture astr_tex[3][12];
	static Texture firepower[25];
	static Texture heal[22];
	static Texture armor[23];
	static SoundBuffer bonus;
	static Sound score_bonus;
	static SoundBuffer firefastervoice;
	static SoundBuffer shieldupvoice;
	static SoundBuffer onemorelife;

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
	virtual Vector2f getPosition() = 0;
	virtual void draw(RenderWindow&) = 0;
	static vector<shared_ptr<Effect>> list;
};

class Animation : public Effect
{
	RectangleShape sprite;
	Texture * texture {nullptr};
	int cur_fr = 0;
	int length;
	Sound sound;
public:
	Animation(Texture* , int length, Vector2f size, Vector2f pos = {0, 0}, SoundBuffer* = nullptr, int = 1, int = 5);
	void play();
	void draw(RenderWindow&);
	inline void setPosition(Vector2f pos) { sprite.setPosition(pos); }
	inline Vector2f getPosition() { return sprite.getPosition(); }
	inline void setRotation(float degree) { sprite.setRotation(degree); }
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
	inline Vector2f getPosition() { return text.getPosition(); }
};

