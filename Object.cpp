#include <cmath>
#include "Object.h"

int Object::score;
int Object::asteroid_count;
bool Object::player_is_dead = false;
vector<shared_ptr<Object>> Object::list;
vector<shared_ptr<Object>> Object::render_buckets[LAYERS];
vector<shared_ptr<Object>> Object::pos_buckets[9];
View Plane::view { FloatRect(0, 0, 800, 600) };
int Plane::life = 3;
int Plane::rof = 5;
shared_ptr<Animation> Plane::shield = nullptr;
float Asteroid::speed_mult = 1.f;

float Object::length(Vector2f a, Vector2f b)
{
	return sqrtf((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

bool Object::is_same_sign(float a, float b)
{
	return (a > 0 && b > 0) || (a < 0 && b < 0) || (a == 0) && (b == 0);
}

Vector2f Object::getUnitVector(Vector2f v)
{
	float len = sqrtf(v.x*v.x + v.y*v.y);
	v /= len;
	return v;
}

Object::Object()
{
}


Object::~Object()
{
}

void Object::add(Object * new_obj, int render_bucket)
{
	shared_ptr<Object> obj_ptr(new_obj);
	list.push_back(obj_ptr);
	render_buckets[render_bucket].push_back(obj_ptr);
	pos_buckets[obj_ptr->getPositionBucket()].push_back(obj_ptr);
}

void Object::clear()
{
	list.clear();
	for (int i = 0; i < LAYERS; ++i) render_buckets[i].clear();
	for (int i = 0; i < 9; ++i) pos_buckets[i].clear();
}

void Object::list_update()
{
	// update all the lists
	// object list
	for (auto i = list.size() - 1; i != -1; --i) { // if dead, remove, else update
		if (list[i]->isDead()) list.erase(list.begin() + i);
		else list[i]->update();
	}
	// rendering buckets
	for (int i = 0; i < LAYERS; ++i) // if dead, remove
		for (auto j = render_buckets[i].size() - 1; j != -1; --j)
			if (render_buckets[i][j]->isDead()) render_buckets[i].erase(render_buckets[i].begin() + j);
	// position buckets
	for (int i = 0; i < 9; ++i)
		for (auto j = pos_buckets[i].size() - 1; j != -1; --j) {
			// if dead, remove
			// otherwise check if current bucket is the right bucket
			// if not, move to other buckets
			if (pos_buckets[i][j]->isDead()) pos_buckets[i].erase(pos_buckets[i].begin() + j);
			else {
				int bucket = pos_buckets[i][j]->getPositionBucket();
				if (i != bucket) {
					shared_ptr<Object> ptr = pos_buckets[i][j];
					pos_buckets[i].erase(pos_buckets[i].begin() + j);
					pos_buckets[bucket].push_back(ptr);
				}
			}
		}
}

void Object::draw_all(RenderWindow &w)
{
	for (int i = 0; i < LAYERS; ++i) for (auto &p : render_buckets[i]) {
		p->draw(w);
		if (!p->inView() && p->getType() == Type_Asteroid) {
			RectangleShape arrow;
			if (p->isAlienship()) arrow.setTexture(&Assets::arrow_red);
			else arrow.setTexture(&Assets::arrow_yellow);
			arrow.setSize({ 40, 20 });
			arrow.setOrigin({ 20, 10 });
			auto center = Plane::view.getCenter();
			Vector2f v = getUnitVector(p->getPosition() - center);
			float angle = acosf(v.x / sqrtf(v.x*v.x + v.y*v.y)) / (2.f*PI) * 360;
			if (v.y > 0) arrow.setRotation(angle);
			else if (v.y < 0) arrow.setRotation(-angle);
			else if (v.x < 0) arrow.setRotation(180);
			arrow.setPosition(center + 200.f*v);
			w.draw(arrow);
		}
	}
}

int Object::getPositionBucket()
{
	Vector2f pos = getPosition();
	if (pos.x < 0 && pos.y < 0) return 0;
	if (pos.x < 800 && pos.x >= 0 && pos.y < 0) return 1;
	if (pos.x >= 800 && pos.y < 0) return 2;
	if (pos.x < 0 && pos.y >= 0 && pos.y < 600) return 3;
	if (pos.x < 800 && pos.x >= 0 && pos.y >= 0 && pos.y < 600) return 4;
	if (pos.x >= 800 && pos.y >= 0 && pos.y < 600) return 5;
	if (pos.x < 0 && pos.y >= 600) return 6;
	if (pos.x < 800 && pos.x >= 0 && pos.y >= 600) return 7;
	if (pos.x >= 800 && pos.y >= 600) return 8;
	return -1;
}

vector<int> Object::getNearbyBuckets(int bucket)
{
	switch (bucket) {
	case 0: return vector<int> {0, 1, 3, 4};
	case 1: return vector<int> {0, 1, 2, 3, 4, 5};
	case 2: return vector<int> {1, 2, 4, 5};
	case 3: return vector<int> {0, 1, 3, 4, 6, 7};
	case 4: return vector<int> {0, 1, 2, 3, 4, 5, 6, 7, 8};
	case 5: return vector<int> {1, 2, 4, 5, 7, 8};
	case 6: return vector<int> {3, 4, 6, 7};
	case 7: return vector<int> {3, 4, 5, 6, 7, 8};
	case 8: return vector<int> {4, 5, 7, 8};
	}
	return vector<int>();
}

bool Object::inView()
{
	auto pos = getPosition();
	auto center = Plane::view.getCenter();
	return pos.x >= center.x - 400
		&& pos.x <= center.x + 400
		&& pos.y >= center.y - 300
		&& pos.y <= center.y + 300;
}

Vector2f Plane::getFiringPosition()
{
	Vector2f firing_pos = shape.getPosition();
	float angle = shape.getRotation() * PI / 180.f;
	Vector2f dist = { 72 * cosf(angle), 72 * sinf(angle) };
	firing_pos += dist;
	return firing_pos;
}

Vector2f Plane::getFiringPosition2()
{
	Vector2f firing_pos = shape.getPosition();
	float angle = shape.getRotation() * PI / 180.f;
	Vector2f dist = { 15.f*cosf(angle) + 39.f * sinf(angle), 15.f * sinf(angle) - 39.f * cosf(angle) };
	firing_pos += dist;
	return firing_pos;
}

Vector2f Plane::getFiringPosition3()
{
	Vector2f firing_pos = shape.getPosition();
	float angle = shape.getRotation() * PI / 180.f;
	Vector2f dist = { 15.f*cosf(angle) - 39.f * sinf(angle), 15.f * sinf(angle) + 39.f * cosf(angle) };
	firing_pos += dist;
	return firing_pos;
}

Vector2f Plane::inBound(Vector2f pos)
{
	if (pos.x < -764.f) pos.x = -764.f;
	if (pos.x > 1564.f) pos.x = 1564.f;
	if (pos.y < -564.f) pos.y = -564.f;
	if (pos.y > 1164.f) pos.y = 1164.f;
	return pos;
}

Plane::Plane(Vector2f pos)
{
	radius = PLANE_CR;
	shape.setSize({138, 138});
	shape.setOrigin({ 69, 69 });
	shape.setPosition(pos);
	shape.setTexture(&Assets::plane[0][0]);
	shape.setRotation(270);
	firing_sound.setBuffer(Assets::gunfire);
	firing_sound2.setBuffer(Assets::misl_launch);
	move_sound.setBuffer(Assets::plane_move);
	jet_sound.setBuffer(Assets::jet_move);
	shield.reset(new Animation(&Assets::shield[0], 9, { 130, 130 }, pos));
}

void Plane::update()
{
	if (move_sound.getStatus() != Sound::Playing) move_sound.play();

	// rotate
	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) shape.rotate(-PLANE_ROT);
	if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) shape.rotate(PLANE_ROT);
	
	// move
	Vector2f pos = shape.getPosition();
	float angle = shape.getRotation()*PI / 180.f;
	if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) {
		if (jet_sound.getStatus() != Sound::Playing) jet_sound.play();
		if (status != DASHING) status = DASHING;
		if (speed < PLANE_SPEED) speed += PLANE_ACCEL;
		Vector2f v = { speed * cosf(angle), speed * sinf(angle) };
		shape.setPosition(inBound(pos + v));
	}
	else {
		if (status == DASHING) status = NOT_DASHING;
		if (speed > 0) speed -= PLANE_DCCEL; else if (speed < 0) speed = 0;
		if (speed != 0) {
			Vector2f v = { speed * cosf(angle), speed * sinf(angle) };
			shape.setPosition(inBound(pos + v));
		}
	}

	// attack
	if (fire_muzzle == nullptr && Keyboard::isKeyPressed(Keyboard::Space)) {
		Vector2f firing_pos = getFiringPosition();
		// gun muzzle
		fire_muzzle.reset(new Animation(&Assets::muzzle[0], 4, { 28, 26 }, firing_pos, nullptr, 1, rof));
		firing_sound.play();
		// launch a bullet
		float angle = (shape.getRotation() + rand() % 11 - 5)*PI / 180.f;
		Vector2f bullet_v { BULLET_SPEED * cosf(angle), BULLET_SPEED * sinf(angle) };
		Object::add(new Bullet(firing_pos, bullet_v, shape.getRotation()), BULLET_LAYER);
		++can_fire_misl;
		if (can_fire_misl == 10) {
			shared_ptr<Object> target[2] {nullptr};
			int idx = 0;
			for (auto& i : Object::list) if (i->inView() && i->getType() == Type_Asteroid) {
				target[idx] = i;
				++idx;
				if (idx == 2) break;
			}
			if (target[1] == nullptr) target[1] = target[0];
			firing_sound2.play();
			Object::add(new Missile(getFiringPosition2(), bullet_v, shape.getRotation(), target[0]), BULLET_LAYER);
			Object::add(new Missile(getFiringPosition3(), bullet_v, shape.getRotation(), target[1]), BULLET_LAYER);
			can_fire_misl = 0;
		}
	}
	if (fire_muzzle != nullptr) {
		if (fire_muzzle->isDead()) fire_muzzle.reset();
		else {
			fire_muzzle->setPosition(getFiringPosition());
			fire_muzzle->play();
		}
	}

	if (shield != nullptr) {
		if (shield->isDead()) shield.reset();
		else {
			shield->play();
			shield->setPosition(getPosition());
		}
	}

	// check if hit by enemy bullet
	pos = getPosition();
	auto nearby = getNearbyBuckets(getPositionBucket());
	for (auto& b : nearby) {
		for (auto& i : pos_buckets[b]) {
			Vector2f ipos = i->getPosition();
			if (pos == ipos) continue; // self
			if (i->getType() == Type_Enemy_Bullet) 
				if (length(pos, ipos) <= radius + i->getRadius()) {
					i->die();
					if (!isShielded()) {
						die();
						return;
					}
			}
		}
	}

	// move view with the plane
	Vector2f view_center = pos;
	if (view_center.x < -400.f) view_center.x = -400.f;
	if (view_center.x > 1200.f) view_center.x = 1200.f;
	if (view_center.y < -300.f) view_center.y = -300.f;
	if (view_center.y > 900.f) view_center.y = 900.f;
	view.setCenter(view_center);
}

void Plane::draw(RenderWindow &w)
{
	++rate;
	if (rate / RATE) {
		shape.setTexture(&Assets::plane[status][cur_fr]);
		++cur_fr;
		if (cur_fr == 2) cur_fr = 0;
		rate = 0;
	}
	// draw the plane
	if (!dead) w.draw(shape);
	// draw fire muzzle
	if (fire_muzzle != nullptr) if (!fire_muzzle->isDead()) fire_muzzle->draw(w);
	if (shield != nullptr) shield->draw(w);
}

void Plane::die()
{
	// play the death animation
	Effect::list.push_back(
		shared_ptr<Effect>(
			new Animation(&Assets::explosion[0], 34, { 120, 134 }, getPosition(), &Assets::explosion_sound, 1, 3)));
	dead = true;
	player_is_dead = true;
	--life;
}

Bullet::Bullet(Vector2f pos_s, Vector2f v, float rotation, int lifetime)
{
	this->v = v;
	radius = BULLET_CR;
	this->lifetime = lifetime;
	lifetime_remain = lifetime;
	shape.setSize({ 13, 5 });
	shape.setOrigin({ 6.5f, 2.5f });
	shape.setTexture(&Assets::bullet);
	shape.setPosition(pos_s);
	shape.setRotation(rotation);
}

void Bullet::update()
{
	--lifetime;
	if (lifetime == 0) die();
	else shape.setPosition(getPosition() + v);
}

void Bullet::draw(RenderWindow &w)
{
	if (!dead) w.draw(shape);
}

void Bullet::die()
{
	// play the death animation
	Effect::list.push_back(shared_ptr<Effect>(new Animation(&Assets::piff[0], 4, { 38, 34 }, shape.getPosition())));
	dead = true;
}

Asteroid::Asteroid(int size, Vector2f pos)
{
	this->size = size;
	float speed = float(3 - size);
	float angle = (rand() % 360) / 360.f * 2.f * PI;
	switch (size) {
	case SMALL: 
		radius = 26;
		v = { speed_mult * speed * cosf(angle), speed_mult * speed * sinf(angle) };
		image.reset(new Animation(&Assets::astr_tex[SMALL][0], 6, { 34, 52 }, pos, nullptr, -1));
		break;
	case MEDIUM: 
		radius = 60;
		v = { speed_mult * speed * cosf(angle), speed_mult * speed * sinf(angle) };
		image.reset(new Animation(&Assets::astr_tex[MEDIUM][0], 12, { 136, 120 }, pos, nullptr, -1));
		break;
	case LARGE:
		radius = 118;
		v = { speed_mult * speed * cosf(angle), speed_mult * speed * sinf(angle) };
		shape.setSize({ 186, 212 });
		shape.setOrigin({ 93, 106 });
		shape.setPosition(pos);
		shape.setTexture(&Assets::astr_tex[LARGE][0]);
		break;
	}
}

void Asteroid::update()
{
	if (size == SMALL || size == MEDIUM) {
		image->play();
		float angle = acosf(v.x / sqrtf(v.x*v.x + v.y*v.y)) / (2.f*PI) * 360;
		if (v.y > 0) image->setRotation(angle);
		else if (v.y < 0) image->setRotation(-angle);
		else if (v.x < 0) image->setRotation(180);
	} else if (size == LARGE) shape.rotate(1);
	Vector2f pos = getPosition();

	// edge checking
    // if ((pos.x - radius <= -800 && v.x < 0) || (pos.x + radius >= 1600 && v.x > 0)) v.x = -v.x;
	// if ((pos.y - radius <= -600 && v.y < 0) || (pos.y + radius >= 1200 && v.y > 0)) v.y = -v.y;
	if ((pos.x - radius <= -800 && v.x < 0)
		|| (pos.x + radius >= 1600 && v.x > 0)
		|| (pos.y - radius <= -600 && v.y < 0)
		|| (pos.y + radius >= 1200 && v.y > 0))
		v = speed_mult * float(3 - size) * getUnitVector(Plane::view.getCenter() - pos);
	

	// collision detection
	auto nearby = getNearbyBuckets(getPositionBucket());
	for (auto& b : nearby) {
		for (auto& i : pos_buckets[b]) {
			Vector2f ipos = i->getPosition();
			if (pos == ipos) continue; // self
			if (length(pos, ipos) <= radius + i->getRadius()) { // collide
				if (i->getType() == Type_Asteroid || i->getType() == Type_Powerup) { // if collide with an asteroid, bounce off
					Vector2f diff = ipos - pos;
					if (is_same_sign(diff.x, v.x)) v.x = -v.x;
					if (is_same_sign(diff.y, v.y)) v.y = -v.y;
				}
				else if (i->getType() == Type_Bullet || i->getType() == Type_Plane) { // if collide with bullet or plane, both die
					if (!i->isShielded()) i->die();
					die();
					return;
				}
			}
		}
	}
	pos += v;
	setPosition(pos);
}

void Asteroid::draw(RenderWindow &w)
{
	if (!dead)
	{
		if (size == SMALL || size == MEDIUM) image->draw(w);
		else if (size == LARGE) w.draw(shape);
	}
}

void Asteroid::die()
{
	score += 50;
	--asteroid_count;
	Effect::list.push_back(
		shared_ptr<Effect>(
			new Animation(&Assets::explosion[0], 34, { 2.f * radius, 2.f * radius * 134.f / 120.f }, getPosition(), 
				&Assets::explosion_sound, 1, 3)));
	if (size > 0) {
		Object::add(new Asteroid(size - 1, getPosition() + Vector2f(0.5f * radius, -0.5f * radius)), ASTR_LAYER);
		Object::add(new Asteroid(size - 1, getPosition() + Vector2f(-0.5f * radius, 0.5f * radius)), ASTR_LAYER);
	} else if (rand() % 10 < 2) Object::add(new Powerup(rand() % 3, getPosition()), PLANE_LAYER);
	dead = true;
}

Vector2f Asteroid::getPosition()
{
	if (size == LARGE) return shape.getPosition();
	if (size == SMALL || size == MEDIUM) return image->getPosition();
	return Vector2f();
}

void Asteroid::setPosition(Vector2f pos)
{
	if (size == LARGE) shape.setPosition(pos);
	if (size == SMALL || size == MEDIUM) image->setPosition(pos);
}

RailNode::RailNode()
{
	rail_top.setSize({ 800, 28 });
	rail_top.setOrigin({ 400, 14 });
	rail_top.setTexture(&Assets::rail);
	rail_left.setSize({ 600, 28 });
	rail_left.setOrigin({ 300, 14 });
	rail_left.setTexture(&Assets::rail);
}

void RailNode::update()
{
}

void RailNode::draw(RenderWindow &w)
{
	Vector2f pos = Plane::view.getCenter();
	// draw rails if near edges
	if (pos.y < -250) {
		rail_top.setRotation(0);
		rail_top.setPosition({ pos.x, -564 });
		w.draw(rail_top);
	}
	else if (pos.y > 850) {
		rail_top.setRotation(180);
		rail_top.setPosition({ pos.x, 1164 });
		w.draw(rail_top);
	}
	if (pos.x < -350) {
		rail_left.setRotation(270);
		rail_left.setPosition({ -764, pos.y });
		w.draw(rail_left);
	}
	else if (pos.x > 1150) {
		rail_left.setRotation(90);
		rail_left.setPosition({ 1564, pos.y });
		w.draw(rail_left);
	}
	// draw nodes if near corners
	if (pos.x < -300 && pos.y < -200) {
		node.setPosition({ -752, -552 });
		node.setRotation(0);
		node.play();
		node.draw(w);
	}
	else if (pos.x < -300 && pos.y > 800) {
		node.setPosition({ -752, 1152 });
		node.setRotation(270);
		node.play();
		node.draw(w);
	}
	else if (pos.x > 1100 && pos.y < -200) {
		node.setPosition({ 1552, -552 });
		node.setRotation(90);
		node.play();
		node.draw(w);
	}
	else if (pos.x > 1100 && pos.y > 800) {
		node.setPosition({ 1552, 1152 });
		node.setRotation(180);
		node.play();
		node.draw(w);
	}
}

void RailNode::die()
{
	dead = true; // should be never used
}

Missile::Missile(Vector2f pos_s, Vector2f v, float rotation, shared_ptr<Object> target, int lifetime)
{
	this->v = MISSILE_SPEED * getUnitVector(v);
	this->target = target;
	radius = MISSILE_CR;
	this->lifetime = lifetime;
	lifetime_remain = lifetime;
	shape.setSize({ 13, 10 });
	shape.setOrigin({ 6.5f, 5.0f });
	shape.setTexture(&Assets::missile);
	shape.setPosition(pos_s);
	shape.setRotation(rotation);
}

void Missile::update()
{
	--lifetime;
	if (lifetime == 0) die();
	else {
		Vector2f pos = shape.getPosition();
		Effect::list.push_back(shared_ptr<Effect> (new Animation(&Assets::trailer[0], 20, {12, 12}, getPosition(), nullptr, 1, 1)));
		if (target != nullptr) {
			if (target->isDead()) target.reset();
			else v = MISSILE_SPEED * getUnitVector(v + 2.f * getUnitVector(target->getPosition() - pos));
		}
		float angle = acosf(v.x / sqrtf(v.x*v.x + v.y*v.y)) / (2.f*PI) * 360;
		if (v.y > 0) shape.setRotation(angle);
		else if (v.y < 0) shape.setRotation(-angle);
		else if (v.x < 0) shape.setRotation(180);
		shape.setPosition(pos + v);
	}
}

void Missile::draw(RenderWindow &w)
{
	if (!dead) w.draw(shape);
}

void Missile::die()
{
	Effect::list.push_back(shared_ptr<Effect>(new Animation(&Assets::misl_exp[0], 13, { 42, 50 }, getPosition())));
	dead = true;
}

AlienShip::AlienShip(Vector2f pos)
{
	float angle = (rand() % 360) / 360.f * 2.f * PI;
	v = { Asteroid::speed_mult * ALIENSHIP_SPEED * cosf(angle), Asteroid::speed_mult * sinf(angle) };
	image.setPosition(pos);
	radius = 60;
	firing_sound.setBuffer(Assets::laser);
}

void AlienShip::update()
{
	// image management
	image.play();
	float angle = acosf(v.x / sqrtf(v.x*v.x + v.y*v.y)) / (2.f*PI) * 360;
	if (v.y > 0) image.setRotation(angle);
	else if (v.y < 0) image.setRotation(-angle);
	else if (v.x < 0) image.setRotation(180);

	auto pos = image.getPosition();
	// move torwards the player
	++rot;
	if (rot == 60) {
		v = Asteroid::speed_mult * ALIENSHIP_SPEED * getUnitVector(v + 2.f*getUnitVector(Plane::view.getCenter() - pos));
		rot = 0;
	}

	// launch a bullet
	if (length(Plane::view.getCenter(), pos) <= 500.f) {
		++rof;
		if (rof == 25) {
			firing_sound.play();
			Object::add(new AlienBullet(image.getPosition()), ELECT_LAYER);
			rof = 0;
		}
	}

	// collision detection
	auto nearby = getNearbyBuckets(getPositionBucket());
	for (auto& b : nearby) {
		for (auto& i : pos_buckets[b]) {
			Vector2f ipos = i->getPosition();
			if (pos == ipos) continue; // self
			if (length(pos, ipos) <= radius + i->getRadius()) { // collide
				if (i->getType() == Type_Asteroid || i->getType() == Type_Powerup) { // if collide with an asteroid, bounce off
					Vector2f diff = ipos - pos;
					if (is_same_sign(diff.x, v.x)) v.x = -v.x;
					if (is_same_sign(diff.y, v.y)) v.y = -v.y;
				}
				else if (i->getType() == Type_Bullet || i->getType() == Type_Plane) { // if collide with bullet or plane, both die
					if (!i->isShielded()) i->die();
					die();
					return;
				}
			}
		}
	}
	pos += v;
	image.setPosition(pos);
}

void AlienShip::draw(RenderWindow &w)
{
	if (!dead) image.draw(w);
}

void AlienShip::die()
{
	Effect::list.push_back(
		shared_ptr<Effect>(
			new Animation(&Assets::explosion[0], 34, { 120, 134 }, getPosition(), &Assets::explosion_sound, 1, 3)));
	if (rand() % 10 < 3) Object::add(new Powerup(rand() % 3, getPosition()), PLANE_LAYER);
	dead = true;
}

AlienBullet::AlienBullet(Vector2f pos_s, int lifetime)
{
	image.setPosition(pos_s);
	this->lifetime = lifetime;
	lifetime_remain = lifetime;
	radius = 18;
	float angle = (rand() % 360) / 360.f * 2.f * PI;
	v = { 5.f * cosf(angle), 5.f * sinf(angle) };
}

void AlienBullet::update()
{
	--lifetime;
	if (lifetime == 0) die();
	else {
		image.play();
		image.setPosition(image.getPosition() + v);
		Effect::list.push_back(
			shared_ptr<Effect> (new Animation(nullptr, 15, {2, 2}, image.getPosition(), nullptr, 1, 1)));
	}
}

void AlienBullet::draw(RenderWindow &w)
{
	if (!dead) image.draw(w);
}

void AlienBullet::die()
{
	Effect::list.push_back(
		shared_ptr<Effect>(
			new Animation(&Assets::blast[0], 27, { 63, 58 }, getPosition(), nullptr, 1, 3)));
	dead = true;
}

void Powerup::applyEffect()
{
	switch (type) {
	case FIRE_FASTER:
		if (Plane::rof > 1) --Plane::rof; else Object::score += 50;
		break;
	case SHIELD_UP:
		Plane::shield.reset(new Animation(&Assets::shield[0], 9, { 130, 130 }, { -900, -700 }, nullptr, 8));
		break;
	case LIFE_PLUS1:
		if (Plane::life < 8) ++Plane::life; else Object::score += 50;
		break;
	}
}

Powerup::Powerup(int type, Vector2f pos_s)
{
	this->type = type;
	radius = 32.f;
	image.setPosition(pos_s);
	float angle = (rand() % 360) / 360.f * 2.f * PI;
	v = { 5.f * cosf(angle), 5.f * sinf(angle) };
}

void Powerup::update()
{
	image.play();
	auto pos = image.getPosition();

	// if hit edge, bounce off
	if ((pos.x - radius <= -800 && v.x < 0) || (pos.x + radius >= 1600 && v.x > 0)) v.x = -v.x;
	if ((pos.y - radius <= -600 && v.y < 0) || (pos.y + radius >= 1200 && v.y > 0)) v.y = -v.y;

	auto nearby = getNearbyBuckets(getPositionBucket());
	for (auto& b : nearby) {
		for (auto& i : pos_buckets[b]) {
			Vector2f ipos = i->getPosition();
			if (pos == ipos) continue; // self
			if (length(pos, ipos) <= radius + i->getRadius()) { // collide
				if (i->getType() == Type_Asteroid || i->getType() == Type_Powerup) { // if collide with an asteroid, bounce off
					Vector2f diff = ipos - pos;
					if (is_same_sign(diff.x, v.x)) v.x = -v.x;
					if (is_same_sign(diff.y, v.y)) v.y = -v.y;
				}
				else if (i->getType() == Type_Plane) { // if collide with plane, you get it
					applyEffect();
					die();
					return;
				}
			}
		}
	}
	pos += v;
	image.setPosition(pos);
}

void Powerup::draw(RenderWindow &w)
{
	if (!dead) image.draw(w);
}

void Powerup::die()
{
	if (type == LIFE_PLUS1) {
		if (Plane::life == 8) {
			if (Assets::score_bonus.getStatus() != Sound::Playing) Assets::score_bonus.play();
			Effect::list.push_back(shared_ptr<Effect>(new FlashingText("+50", getPosition(), 30)));
		}
		else Effect::list.push_back(shared_ptr<Effect>(
			new Animation(&Assets::heal[0], 22, { 26, 26 }, image.getPosition(), &Assets::lifeplus)));
	}
	else if (type == FIRE_FASTER) {
		if (Plane::rof == 1) {
			if (Assets::score_bonus.getStatus() != Sound::Playing) Assets::score_bonus.play();
			Effect::list.push_back(shared_ptr<Effect>(new FlashingText("+50", getPosition(), 30)));
		}
		else Effect::list.push_back(shared_ptr<Effect>(
			new Animation(&Assets::firepower[0], 25, { 32, 32 }, image.getPosition(), &Assets::firefaster)));
	}
	else if (type == SHIELD_UP)
		Effect::list.push_back(shared_ptr<Effect>(
			new Animation(&Assets::armor[0], 23, { 33, 37 }, image.getPosition(), &Assets::shieldup)));
	dead = true;
}
