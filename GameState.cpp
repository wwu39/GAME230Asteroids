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
	else if (pos.x == 450.0f && (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))) {
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
	Effect::list.clear();
	Object::list.clear();
	bg.setSize({ 2400, 1800 });
	bg.setOrigin({ 1200, 900 });
	bg.setTexture(&Assets::sky);
	bg.setPosition({ 400, 300 });
	Object::list.push_back(shared_ptr<Object> (new Plane()));
}

Asteroids::~Asteroids()
{
	Effect::list.clear();
	Object::list.clear();
}

GameStatus Asteroids::update()
{
	for (auto i = Object::list.size() - 1; i != -1; --i) { // update all objects
		if (Object::list[i]->isDead()) Object::list.erase(Object::list.begin() + i);
		else Object::list[i]->update();
	}
	for (auto i = Effect::list.size() - 1; i != -1; --i) { // play all effects
		if (Effect::list[i]->isDead()) Effect::list.erase(Effect::list.begin() + i);
		else Effect::list[i]->play();
	}
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Assets::button_press.play();
		return MENU;
	}
	return NO_CHANGE;
}

void Asteroids::draw(RenderWindow& w)
{
	w.setView(Plane::view);
	w.clear(Color(0, 0, 255));
	w.draw(bg);
	for (auto& i : Object::list) i->draw(w);
	for (auto& i : Effect::list) i->draw(w);
	w.display();
}
