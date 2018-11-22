#include "GameState.h"



GameState::GameState()
{
}


GameState::~GameState()
{
}

Menu::Menu()
{
	bgm.setBuffer(Assets::menu_bgm);
	bg.setSize({ 800, 600 });
	bg.setTexture(&Assets::menu_bg_tex);
}

void Menu::update()
{
	if (bgm.getStatus() != Sound::Playing) bgm.play();
}

void Menu::draw(RenderWindow& w)
{
	w.clear();
	w.draw(bg);
	w.display();
}

Asteroids::Asteroids()
{
}

void Asteroids::update()
{
}

void Asteroids::draw(RenderWindow& w)
{
	w.clear();
	w.display();
}
