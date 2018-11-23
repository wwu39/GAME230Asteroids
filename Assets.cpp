#include "Assets.h"
#include <sstream>

vector<shared_ptr<Effect>> Effect::list;

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
Texture Assets::plane[2][2];
Texture Assets::muzzle[4];
Texture Assets::bullet;
SoundBuffer Assets::gunfire;

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
	plane[0][0].loadFromFile("sprites/plane_tl 0000.png");
	plane[0][1].loadFromFile("sprites/plane_tl 0001.png");
	plane[1][0].loadFromFile("sprites/plane_gs 0000.png");
	plane[1][1].loadFromFile("sprites/plane_gs 0001.png");
	for (int i = 0; i < 4; ++i) {
		ostringstream filename;
		filename << "sprites/muzzle 000" << i << ".png";
		muzzle[i].loadFromFile(filename.str());
	}
	bullet.loadFromFile("sprites/bullet.png");
	gunfire.loadFromFile("sound/gunfire.wav");
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

void Animation::play()
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
}

void Animation::draw(RenderWindow& w)
{
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

void FlashingText::play()
{
	++rate;
	if (rate / RATE) {
		flashing = !flashing;
		if (flashing && loop_count > 0) ++times_played;
		if (times_played == loop_count) dead = true;
		rate = 0;
	}
}

void FlashingText::draw(RenderWindow& w)
{
	if (flashing && !dead) w.draw(text);
}