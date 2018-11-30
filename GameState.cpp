#include "GameState.h"
#include <sstream>


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

Asteroids::Asteroids(int Level)
{
	// debug
	debug.setCharacterSize(50);
	debug.setFont(Assets::arcade);
	debug.setFillColor(Color::Red);
	debug.setLineSpacing(0.75);
	//debug

	Effect::list.clear();
	Object::clear();
	bg.setSize({ 2400, 1800 });
	bg.setOrigin({ 1200, 900 });
	bg.setTexture(&Assets::sky);
	bg.setPosition({ 400, 300 });
	socre_text.setCharacterSize(50);
	socre_text.setFont(Assets::arcade);
	socre_text.setFillColor(Color::Green);
	socre_text.setLineSpacing(0.75);
	landing_sound.setBuffer(Assets::landing);
	Plane::view.setCenter({ 400, 300 });
	Object::player_is_dead = false;
	redeploy.reset(new FlashingText("Deploying", Plane::view.getCenter() + Vector2f(-200, -80), 100));
	Object::score = 0;

	Object::add(new Asteroid(LARGE, { -400, -300 }, { cosf((rand() % 360) / 360.f * 2 * PI), sinf((rand() % 360) / 360.f * 2 * PI) }), ASTR_LAYER);
	Object::add(new Asteroid(LARGE, { -400, 300 }, { cosf((rand() % 360) / 360.f * 2 * PI), sinf((rand() % 360) / 360.f * 2 * PI) }), ASTR_LAYER);
	Object::add(new Asteroid(LARGE, { -400, 900 }, { cosf((rand() % 360) / 360.f * 2 * PI), sinf((rand() % 360) / 360.f * 2 * PI) }), ASTR_LAYER);
	Object::add(new Asteroid(LARGE, { 400, -300 }, { cosf((rand() % 360) / 360.f * 2 * PI), sinf((rand() % 360) / 360.f * 2 * PI) }), ASTR_LAYER);
	Object::add(new Asteroid(LARGE, { 400, 900 }, { cosf((rand() % 360) / 360.f * 2 * PI), sinf((rand() % 360) / 360.f * 2 * PI) }), ASTR_LAYER);
	Object::add(new Asteroid(LARGE, { 1200, -300 }, { cosf((rand() % 360) / 360.f * 2 * PI), sinf((rand() % 360) / 360.f * 2 * PI) }), ASTR_LAYER);
	Object::add(new Asteroid(LARGE, { 1200, 300 }, { cosf((rand() % 360) / 360.f * 2 * PI), sinf((rand() % 360) / 360.f * 2 * PI) }), ASTR_LAYER);
	Object::add(new Asteroid(LARGE, { 1200, 900 }, { cosf((rand() % 360) / 360.f * 2 * PI), sinf((rand() % 360) / 360.f * 2 * PI) }), ASTR_LAYER);
}

Asteroids::~Asteroids()
{
}

GameStatus Asteroids::update()
{
	Object::list_update();
	for (auto i = Effect::list.size() - 1; i != -1; --i) { // play all effects
		if (Effect::list[i]->isDead()) Effect::list.erase(Effect::list.begin() + i);
		else Effect::list[i]->play();
	}
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		Assets::button_press.play();
		return MENU;
	}
	if (Object::score == 2800) return NEXT_LEVEL;

	if (Object::player_is_dead) {
		redeploy.reset(new FlashingText("Re-Deploying", Plane::view.getCenter() + Vector2f(-260, -80), 100));
		Object::player_is_dead = false;
	}
	if (redeploy != nullptr)
		if (redeploy->isDead()) {
			redeploy.reset();
			landing.reset(new Animation(&Assets::plane_landing[0], 20, { 640, 480 }, Plane::view.getCenter()));
			landing_sound.play();
		} else redeploy->play();
	if (landing != nullptr)
		if (landing->isDead()) {
			landing.reset();
			Object::add(new Plane(Plane::view.getCenter()), PLANE_LAYER);
		} else landing->play();

	// debug
	ostringstream str;
	str << "RBucket Count: ";
	for (int i = 0; i < LAYERS; ++i) str << Object::render_buckets[i].size() << " ";
	str << endl;
	str << "PBucket Count: ";
	for (int i = 0; i < 9; ++i) str << Object::pos_buckets[i].size() << " ";
	debug.setString(str.str());
	Vector2f debug_pos = Plane::view.getCenter();
	debug_pos.x -= 385.f;
	debug_pos.y -= 285.f;
	debug.setPosition(debug_pos);
	// debug

	Vector2f score_pos = Plane::view.getCenter();
	score_pos.x -= 400.f;
	score_pos.y += 260.f;
	ostringstream score_text_content;
	score_text_content << "Score:" << Object::score;
	socre_text.setString(score_text_content.str());
	socre_text.setPosition(score_pos);

	return NO_CHANGE;
}

void Asteroids::draw(RenderWindow& w)
{
	w.setView(Plane::view);
	w.clear(Color(0, 0, 255));
	w.draw(bg);
	Object::draw_all(w);
	for (auto& i : Effect::list) i->draw(w);
	if (landing != nullptr) landing->draw(w);
	if (redeploy != nullptr) redeploy->draw(w);
	w.draw(socre_text);

	// debug
	w.draw(debug);

	w.display();
}
