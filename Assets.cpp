#include "Assets.h"
#include <sstream>

Texture Assets::menu_bg_tex;
Font Assets::arcade;
SoundBuffer Assets::menu_bgm;
Texture Assets::bomb[16];
SoundBuffer Assets::option;
SoundBuffer Assets::button;
SoundBuffer Assets::title;
Sound Assets::button_press;
Texture Assets::explosion[34];
SoundBuffer Assets::explosion_sound;

void Assets::load()
{
	menu_bg_tex.loadFromFile("sprites/menu.png");
	arcade.loadFromFile("fonts/arcade.ttf");
	menu_bgm.loadFromFile("sound/menu_bg.wav");
	for (int i = 0; i < 16; ++i) {
		ostringstream filename;
		filename << "sprites/bomb 00" << (i > 9 ? "" : "0") << i << ".png";
		bomb[i].loadFromFile(filename.str());
	}
	option.loadFromFile("sound/option.wav");
	button.loadFromFile("sound/button.wav");
	button_press.setBuffer(button);
	title.loadFromFile("sound/title.wav");
	for (int i = 0; i < 34; ++i) { // explosion
		ostringstream filename;
		filename << "sprites/explode 00" << (i > 9 ? "" : "0") << i << ".png";
		explosion[i].loadFromFile(filename.str());
	}
	explosion_sound.loadFromFile("sound/explosion.wav");
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



Animation::Animation(Texture * texture, int length, Vector2f size, Vector2f pos, SoundBuffer * sb, int loop_count, int rate)
{
	if (loop_count == 0) {
		dead = true;
		return;
	}
	sprite.setSize(size);
	sprite.setPosition(pos);
	sprite.setOrigin(size / 2.0f);
	sprite.setTexture(texture);
	if (sb) sound.setBuffer(*sb);
	sound.play();
	this->texture = texture;
	this->rate = rate;
	RATE = rate;
	this->loop_count = loop_count;
	this->length = length;
}

void Animation::play(RenderWindow& w)
{
	++rate;
	if (rate / RATE) {
		sprite.setTexture(&texture[cur_fr]);
		++cur_fr;
		if (cur_fr == length) {
			cur_fr = 0;
			if (loop_count > 0) ++times_played;
			if (times_played == loop_count) dead = true;
			if (!dead) sound.play();
		}
		rate = 0;
	}
	if (!dead) w.draw(sprite);
}


FlashingText::FlashingText(string text, Vector2f pos, int char_size, int loop_count, int rate)
{
	if (loop_count == 0) {
		dead = true;
		return;
	}
	this->text.setFont(Assets::arcade);
	this->text.setCharacterSize(char_size);
	this->text.setPosition(pos);
	this->text.setString(text);
	this->text.setFillColor(Color::Green);
	this->rate = rate;
	RATE = rate;
	this->loop_count = loop_count;

}

void FlashingText::play(RenderWindow& w)
{
	++rate;
	if (rate / RATE) {
		flashing = !flashing;
		if (flashing && loop_count > 0) ++times_played;
		if (times_played == loop_count) dead = true;
		rate = 0;
	}
	if (flashing && !dead) w.draw(text);
}
