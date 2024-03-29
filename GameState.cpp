#include "GameState.h"
#include <sstream>

static bool debug_on = false;

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
	if (active) {
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
				return LOADING;
			}
			if (pos.x == 450.0f) return EXIT;
		}
	}
	else {
		++wait;
		if (wait / 10) active = true;                                       
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

Asteroids::Asteroids(int level)
{
	if (debug_on) {
		debug.setCharacterSize(50);
		debug.setFont(Assets::arcade);
		debug.setFillColor(Color::Red);
		debug.setLineSpacing(0.75);
	}
	Effect::list.clear();
	Object::clear();

	bg.setSize({ 2400, 1800 });
	bg.setOrigin({ 1200, 900 });
	bg.setTexture(&Assets::sky);
	bg.setPosition({ 400, 300 });

	life_image.setSize({70, 70});
	life_image.setOrigin({ 35, 35  });
	life_image.setTexture(&Assets::plane[NOT_DASHING][0]);
	life_image.setRotation(225);

	socre_text.setCharacterSize(50);
	socre_text.setFont(Assets::arcade);
	socre_text.setFillColor(Color::Green);
	socre_text.setLineSpacing(0.75);

	gameover_text.setFont(Assets::arcade);
	gameover_text.setCharacterSize(100);
	gameover_text.setFillColor(Color::Green);
	gameover_text.setLineSpacing(0.75);
	gameover_text.setOrigin({ 215, 120 });
	gameover_text.setString("GAME OVER");

	landing_sound.setBuffer(Assets::landing);
	gameover_voice.setBuffer(Assets::gameover);
	levelclear_voice.setBuffer(Assets::levelclear);
	winning_bgm.setBuffer(Assets::winning_bgm);
	losing_bgm.setBuffer(Assets::losing_bgm);
	level_bgm.setBuffer(Assets::level_bgm);

	Plane::view.setCenter({ 400, 300 });
	Object::player_is_dead = false;
	redeploy.reset(new FlashingText("Deploying", Plane::view.getCenter() + Vector2f(-200, -80), 100));

	ostringstream str;
	str << "Level " << level;
	level_title.reset(new FlashingText(str.str(), Plane::view.getCenter() + Vector2f(-220, -100), 150));

	if (level == 1) {
		Plane::life = 3;
		Plane::rof = 5;
		Object::score = 0;
	}
	Asteroid::speed_mult = 0.5f + 0.5f*float(level);
	Object::asteroid_count = 44;
	Object::add(new Asteroid(MEDIUM, { -400, -300 }), ASTR_LAYER);
	Object::add(new Asteroid(SMALL, { -400, 300 }), ASTR_LAYER);
	Object::add(new Asteroid(MEDIUM, { -400, 900 }), ASTR_LAYER);
	Object::add(new Asteroid(SMALL, { 400, -300 }), ASTR_LAYER);
	Object::add(new Asteroid(LARGE, { 400, 900 }), ASTR_LAYER);
	Object::add(new Asteroid(LARGE, { 1200, -300 }), ASTR_LAYER);
	Object::add(new Asteroid(SMALL, { 1200, 300 }), ASTR_LAYER);
	Object::add(new Asteroid(LARGE, { 1200, 900 }), ASTR_LAYER);
	Object::add(new Asteroid(MEDIUM, { 0, 0 }), ASTR_LAYER);
	Object::add(new Asteroid(LARGE, { 800, 0 }), ASTR_LAYER);
	Object::add(new Asteroid(MEDIUM, { 0, 600 }), ASTR_LAYER);
	Object::add(new Asteroid(SMALL, { 800, 600 }), ASTR_LAYER);
	if (level % 3 == 2) {
		Object::add(new Asteroid(MEDIUM, { -200, -150 }), ASTR_LAYER);
		Object::add(new Asteroid(MEDIUM, { 200, 150 }), ASTR_LAYER);
		Object::asteroid_count = 50;
	}
	if (level % 3 == 0) {
		Object::add(new Asteroid(MEDIUM, { 600, 450 }), ASTR_LAYER);
		Object::add(new Asteroid(MEDIUM, { -600, 450 }), ASTR_LAYER);
		Object::add(new Asteroid(SMALL, { -600, -450 }), ASTR_LAYER);
		Object::add(new Asteroid(LARGE, { -600, 450 }), ASTR_LAYER);
		Object::add(new Asteroid(SMALL, { -200, -150 }), ASTR_LAYER);
		Object::add(new Asteroid(SMALL, { -200, 150 }), ASTR_LAYER);
		Object::add(new Asteroid(SMALL, { 200, 150 }), ASTR_LAYER);
		Object::add(new Asteroid(LARGE, { 200, -150 }), ASTR_LAYER);
		Object::asteroid_count = 68;
	}
}

Asteroids::~Asteroids()
{
}

GameStatus Asteroids::update()
{
	// sky color
	if (c_reverse) --c; else ++c;
	if (c == 255) c_reverse = true;
	if (c == 150) c_reverse = false;
	bg.setFillColor(Color(c, c, c));

	for (auto i = Effect::list.size() - 1; i != -1; --i) { // play all effects
		if (Effect::list[i]->isDead()) Effect::list.erase(Effect::list.begin() + i);
		else Effect::list[i]->play();
	}
	if (status == STARTING || status == RUNNING)
		if (level_bgm.getStatus() != Sound::Playing) level_bgm.play();
	if (status == STARTING) {
		if (level_title != nullptr) {
			if (level_title->isDead()) {
				level_title.reset();
				status = RUNNING;
			}
			else level_title->play();
		}
	}
	else if (status == RUNNING) {
		Object::list_update();
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			Assets::button_press.play();
			return MENU;
		}
		if (Plane::life <= 0) {
			gameover_text.setPosition(Plane::view.getCenter() + Vector2f(-20, -50));
			Effect::list.push_back(shared_ptr<Effect>(
				new FlashingText("Press Enter to Continue...", Plane::view.getCenter() + Vector2f(-300, 150), 50, -1)));
			gameover_voice.play();
			if (level_bgm.getStatus() == Sound::Playing) level_bgm.stop();
			status = GAMEOVER;
		}
		if (Object::asteroid_count == 0) {
			Effect::list.push_back(shared_ptr<Effect>(
				new FlashingText("Press Enter to Continue...", Plane::view.getCenter() + Vector2f(-300, 150), 50, -1)));
			levelclear_voice.play();
			if (level_bgm.getStatus() == Sound::Playing) level_bgm.stop();
			winning_bgm.play();
			status = ENDING;
		}
		if (Object::player_is_dead) {
			redeploy.reset(new FlashingText("Re-Deploying", Plane::view.getCenter() + Vector2f(-260, -80), 100));
			Object::player_is_dead = false;
		}
		if (redeploy != nullptr) {
			if (redeploy->isDead()) {
				redeploy.reset();
				landing.reset(new Animation(&Assets::plane_landing[0], 20, { 640, 480 }, Plane::view.getCenter()));
				landing_sound.play();
			}
			else redeploy->play();
		}
		if (landing != nullptr) {
			if (landing->isDead()) {
				landing.reset();
				Object::add(new Plane(Plane::view.getCenter()), PLANE_LAYER);
			}
			else landing->play();
		}

		if (debug_on) {
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
		}

		Vector2f score_pos = Plane::view.getCenter();
		score_pos.x -= 400.f;
		score_pos.y += 260.f;
		ostringstream score_text_content;
		score_text_content << "Score:" << Object::score;
		socre_text.setString(score_text_content.str());
		socre_text.setPosition(score_pos);

		++AlienshipInterval;
		if (AlienshipInterval == 600) {
			Object::add(new AlienShip(AlienshipLocs[AlienshipLocIdx]), ELECT_LAYER);
			AlienshipLocIdx = (AlienshipLocIdx + 1) % 4;
			AlienshipInterval = 0;
		}

	}
	else if (status == ENDING) {
		if (Keyboard::isKeyPressed(Keyboard::Enter))
			return NEXT_LEVEL;
	}
	else if (status == GAMEOVER) {
		if (losing_bgm.getStatus() != Sound::Playing) losing_bgm.play();
		if (Keyboard::isKeyPressed(Keyboard::Enter)) 
			return MENU;
	}
	return NO_CHANGE;
}

void Asteroids::draw(RenderWindow& w)
{
	w.setView(Plane::view);
	w.clear(Color(0, 0, 255));
	w.draw(bg);
	railnode.draw(w);
	Object::draw_all(w);
	for (auto& i : Effect::list) i->draw(w);
	if (status == RUNNING) {
		if (landing != nullptr) landing->draw(w);
		if (redeploy != nullptr) redeploy->draw(w);
	}
	if (level_title != nullptr) level_title->draw(w);
	if (status == GAMEOVER) w.draw(gameover_text);
	w.draw(socre_text);
	draw_life_image(w);
	if (debug_on) w.draw(debug);
	w.display();
}

void Asteroids::draw_life_image(RenderWindow &w)
{
	for (int i = 0; i < Plane::life; ++i) {
		Vector2f center = Plane::view.getCenter();
		life_image.setPosition(center + Vector2f(float(375 - 50 * i), 275));
		w.draw(life_image);
	}
}

Loading::Loading()
{
	prompt.reset(new FlashingText("Press Space to Continue...", { 100, 450 }, 50, -1));
	bg.setSize({ 800, 600 });
	bg.setTexture(&Assets::loading_bg_tex);
	ostringstream str;
	str << "Alien armada covers the sky..." << endl
		<< "Destroy all yellow targets to " << endl << "secure the area." << endl
		<< "Good luck!" << endl;
	context_content = str.str();
	context.setCharacterSize(50);
	context.setFont(Assets::arcade);
	context.setFillColor(Color::Green);
	context.setPosition({ 50, 40 });
	typing.setBuffer(Assets::typing);
}

Loading::~Loading()
{
}

GameStatus Loading::update()
{

	if (i < int(context_content.getSize()) - 1) {
		if (j == 4) {
			++i;
			j = 0;
		} else ++j;
		if (typing.getStatus() != Sound::Playing) typing.play();
	}
	context.setString(context_content.substring(0, i));
	prompt->play();
	if (stun == 10) {
		if (Keyboard::isKeyPressed(Keyboard::Space)) return GAME;
	}
	else ++stun;
	return NO_CHANGE;
}

void Loading::draw(RenderWindow &w)
{
	w.clear(Color(0, 0, 255));
	w.draw(bg);
	w.draw(context);
	prompt->draw(w);
	w.display();
}
