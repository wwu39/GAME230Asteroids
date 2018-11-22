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
	// here is all assets we need
	static Texture menu_bg_tex;
	static Font font;
	static SoundBuffer menu_bgm;
	static void load();
	Assets();
	~Assets();
};

class Effect
{
protected:
	Effect();
public:
	virtual void play() = 0;
};

class Animation : public Effect
{
public:
	Animation(Texture texture[], int length, int rate, bool loop_count);
	void play();
};

class FlashingText : public Effect
{
public:
	FlashingText(string text, int rate, bool loop_count);
	void play();
};

