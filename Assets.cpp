#include "Assets.h"
#include <sstream>

vector<shared_ptr<Effect>> Effect::list;

Texture Assets::menu_bg_tex;
Texture Assets::loading_bg_tex;
SoundBuffer Assets::typing;
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
Texture Assets::sky;
SoundBuffer Assets::plane_move;
SoundBuffer Assets::jet_move;
Texture Assets::rail;
Texture Assets::nod[4];
Texture Assets::piff[4];
Texture Assets::astr_tex[3][12];
Texture Assets::plane_landing[20];
SoundBuffer Assets::landing;
Texture Assets::shield[9];
Texture Assets::missile;
Texture Assets::trailer[20];
Texture Assets::misl_exp[13];
SoundBuffer Assets::misl_exp_sound;
SoundBuffer Assets::misl_launch;
Texture Assets::arrow_red;
Texture Assets::arrow_yellow;
Texture Assets::enemy[4];
Texture Assets::electball[20];
Texture Assets::blast[27];
SoundBuffer Assets::laser;
SoundBuffer Assets::gameover;
SoundBuffer Assets::levelclear;
SoundBuffer Assets::level_bgm;
SoundBuffer Assets::winning_bgm;
SoundBuffer Assets::losing_bgm;
Texture Assets::powerup[20];
SoundBuffer Assets::firefaster;
SoundBuffer Assets::shieldup;
SoundBuffer Assets::lifeplus;
Texture Assets::firepower[25];
Texture Assets::heal[22];
Texture Assets::armor[23];
SoundBuffer Assets::bonus;
Sound Assets::score_bonus;
SoundBuffer Assets::firefastervoice;
SoundBuffer Assets::shieldupvoice;
SoundBuffer Assets::onemorelife;

void Assets::load()
{
	menu_bg_tex.loadFromFile("sprites/menu.png");
	loading_bg_tex.loadFromFile("sprites/loading.png");
	typing.loadFromFile("sound/typing.wav");
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
	sky.loadFromFile("sprites/sky.png");
	plane_move.loadFromFile("sound/plane_move.wav");
	jet_move.loadFromFile("sound/jet_move.wav");
	rail.loadFromFile("sprites/rail.png");
	for (int i = 0; i < 4; ++i) {
		ostringstream filename;
		filename << "sprites/nod 000" << i << ".png";
		nod[i].loadFromFile(filename.str());
	}
	for (int i = 0; i < 4; ++i) {
		ostringstream filename;
		filename << "sprites/piff 000" << i << ".png";
		piff[i].loadFromFile(filename.str());
	}
	for (int i = 0; i < 6; ++i) {
		ostringstream filename;
		filename << "sprites/astr_s 000" << i << ".png";
		astr_tex[SMALL][i].loadFromFile(filename.str());
	}
	for (int i = 0; i < 12; ++i) {
		ostringstream filename;
		filename << "sprites/astr_m 00" << (i > 9 ? "" : "0") << i << ".png";
		astr_tex[MEDIUM][i].loadFromFile(filename.str());
	}
	astr_tex[LARGE][0].loadFromFile("sprites/astr_l 0000.png");
	for (int i = 0; i < 20; ++i) {
		ostringstream filename;
		filename << "sprites/landing 00" << (i > 9 ? "" : "0") << i << ".png";
		plane_landing[i].loadFromFile(filename.str());
	}
	landing.loadFromFile("sound/landing.wav");
	for (int i = 0; i < 5; ++i) {
		ostringstream filename;
		filename << "sprites/shield_white 000" << 4 - i << ".png";
		shield[i].loadFromFile(filename.str());
	}
	for (int i = 5; i < 9; ++i) {
		ostringstream filename;
		filename << "sprites/shield_white 000" << i - 4 << ".png";
		shield[i].loadFromFile(filename.str());
	}
	missile.loadFromFile("sprites/misl.png");
	for (int i = 0; i < 20; ++i) {
		ostringstream filename;
		filename << "sprites/trailer 00" << (i > 9 ? "" : "0") << i << ".png";
		trailer[i].loadFromFile(filename.str());
	}
	for (int i = 0; i < 13; ++i) {
		ostringstream filename;
		filename << "sprites/misl_exp 00" << (i > 9 ? "" : "0") << i << ".png";
		misl_exp[i].loadFromFile(filename.str());
	}
	misl_exp_sound.loadFromFile("sound/misl_exp.wav");
	misl_launch.loadFromFile("sound/misl_launch.wav");
	arrow_red.loadFromFile("sprites/arrow.png");
	arrow_yellow.loadFromFile("sprites/arrow2.png");
	for (int i = 0; i < 4; ++i) {
		ostringstream filename;
		filename << "sprites/enemy 000" << i << ".png";
		enemy[i].loadFromFile(filename.str());
	}
	for (int i = 0; i < 20; ++i) {
		ostringstream filename;
		filename << "sprites/a 00" << (i > 9 ? "" : "0") << i << ".png";
		electball[i].loadFromFile(filename.str());
	}
	for (int i = 0; i < 27; ++i) {
		ostringstream filename;
		filename << "sprites/blast 00" << (i > 9 ? "" : "0") << i << ".png";
		blast[i].loadFromFile(filename.str());
	}
	laser.loadFromFile("sound/aaa.wav");
	gameover.loadFromFile("sound/gameover.wav");
	levelclear.loadFromFile("sound/levelclear.wav");
	level_bgm.loadFromFile("sound/b05.wav");
	winning_bgm.loadFromFile("sound/win_level.wav");
	losing_bgm .loadFromFile("sound/losing_bg.wav");
	for (int i = 0; i < 20; ++i) {
		ostringstream filename;
		filename << "sprites/powerup 00" << (i > 9 ? "" : "0") << i << ".png";
		powerup[i].loadFromFile(filename.str());
	}
	firefaster.loadFromFile("sound/firefaster.wav");
	shieldup.loadFromFile("sound/shieldup.wav");
	lifeplus.loadFromFile("sound/life+1.wav");
	for (int i = 0; i < 25; ++i) {
		ostringstream filename;
		filename << "sprites/firepower 00" << (i > 9 ? "" : "0") << i << ".png";
		firepower[i].loadFromFile(filename.str());
	}
	for (int i = 0; i < 25; ++i) {
		ostringstream filename;
		filename << "sprites/heal 00" << (i > 9 ? "" : "0") << i << ".png";
		heal[i].loadFromFile(filename.str());
	}
	for (int i = 0; i < 23; ++i) {
		ostringstream filename;
		filename << "sprites/armor 00" << (i > 9 ? "" : "0") << i << ".png";
		armor[i].loadFromFile(filename.str());
	}
	bonus.loadFromFile("sound/bonus.wav");
	score_bonus.setBuffer(bonus);
	firefastervoice.loadFromFile("sound/firefastervoice.wav");
	shieldupvoice.loadFromFile("sound/shieldupvoice.wav");
	onemorelife.loadFromFile("sound/onemorelife.wav");
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
	if (texture != nullptr) {
		sprite.setTexture(texture);
		this->texture = texture;
	} else sprite.setFillColor(Color(168, 220, 255));
	if (sb) sound.setBuffer(*sb);
	sound.play();
	this->rate = rate;
	RATE = rate;
	this->loop_count = loop_count;
	this->length = length;
}

void Animation::play()
{
	++rate;
	if (rate / RATE) {
		if (texture != nullptr) sprite.setTexture(&texture[cur_fr]);
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