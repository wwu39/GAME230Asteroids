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
	options.setFont(Assets::arcade);
	options.setCharacterSize(75);
	options.setPosition({ 175, 425 });
	options.setFillColor(Color::Green);
	options.setString("Start     Exit");
	cursor.setRadius(45);
	cursor.setTexture(&Assets::bomb[14]);
	cursor.setPosition({ 90.0f, 425 });
	cursor.setOutlineColor(Color::Yellow);
	option.setBuffer(Assets::option);
	title.setBuffer(Assets::title);
	title.play();
}

GameStatus Menu::update()
{
	if (bgm.getStatus() != Sound::Playing) bgm.play();
	Vector2f pos = cursor.getPosition();
	if (pos.x == 90.0f && (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))) {
		cursor.setPosition({ 450.0f, 425 });
		option.play();
	}
	if (pos.x == 450.0f && (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))) {
		cursor.setPosition({ 90.0f, 425 });
		option.play();
	}
	if (Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::Enter)) {
		if (pos.x == 90.0f) {
			Assets::button_press.play();
			return GAME;
		}
		if (pos.x == 450.0f) return EXIT;
	}
	return NO_CHANGE;
}

void Menu::draw(RenderWindow& w)
{
	w.clear(Color(0, 0, 255));
	w.draw(bg);
	w.draw(options);
	w.draw(cursor);
	w.display();
}

Asteroids::Asteroids()
{
	
}

GameStatus Asteroids::update()
{
	for (auto i = object_list.size() - 1; i != -1; --i) {
		if (object_list[i]->isDead()) object_list.erase(object_list.begin() + i);
		else object_list[i]->update();
	}
	return NO_CHANGE;
}

void Asteroids::draw(RenderWindow& w)
{
	w.clear(Color(0, 0, 255));
	for (auto i = effect_list.size() - 1; i != -1; --i) {
		if (effect_list[i]->isDead()) effect_list.erase(effect_list.begin() + i);
		else effect_list[i]->play(w);
	}
	for (auto& i : object_list) i->draw(w);
	w.display();
}
