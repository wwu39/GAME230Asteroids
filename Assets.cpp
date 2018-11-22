#include "Assets.h"

Texture Assets::menu_bg_tex;
Font Assets::font;
SoundBuffer Assets::menu_bgm;

void Assets::load()
{
	menu_bg_tex.loadFromFile("sprites/menu.png");
	font.loadFromFile("fonts/arcade.ttf");
	menu_bgm.loadFromFile("sound/menu_bg.wav");
}

Assets::Assets()
{
}


Assets::~Assets()
{
}


Effect::Effect()
{
}

Animation::Animation(Texture texture[], int length, int rate, bool loop_count)
{
}

void Animation::play()
{
}

FlashingText::FlashingText(string text, int rate, bool loop_count)
{
}

void FlashingText::play()
{
}
