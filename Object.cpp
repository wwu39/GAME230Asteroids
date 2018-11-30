#include <cmath>
#include "Object.h"

int Object::score;
bool Object::player_is_dead = false;
vector<shared_ptr<Object>> Object::list;
vector<shared_ptr<Object>> Object::render_buckets[LAYERS];
vector<shared_ptr<Object>> Object::pos_buckets[9];
View Plane::view { FloatRect(0, 0, 800, 600) };

float Object::length(Vector2f a, Vector2f b)
{
	return sqrtf((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

bool Object::is_same_sign(float a, float b)
{
	return (a > 0 && b > 0) || (a < 0 && b < 0) || (a == 0) && (b == 0);
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
	for (int i = 0; i < LAYERS; ++i) for (auto &p : render_buckets[i]) p->draw(w);
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

Vector2f Plane::getFiringPosition()
{
	Vector2f firing_pos = shape.getPosition();
	float angle = shape.getRotation() * PI / 180.f;
	Vector2f dist = { 72 * cosf(angle), 72 * sinf(angle) };
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
	move_sound.setBuffer(Assets::plane_move);
	jet_sound.setBuffer(Assets::jet_move);
	rail_top.setSize({ 800, 28 });
	rail_top.setOrigin({ 400, 14 });
	rail_top.setTexture(&Assets::rail);
	rail_left.setSize({ 600, 28 });
	rail_left.setOrigin({ 300, 14 });
	rail_left.setTexture(&Assets::rail);
}

void Plane::update()
{
	if (move_sound.getStatus() != Sound::Playing) move_sound.play();
	// rotate
	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
		shape.rotate(-PLANE_ROT);
	if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
		shape.rotate(PLANE_ROT);
	
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
		fire_muzzle.reset(new Animation(&Assets::muzzle[0], 4, { 28, 26 }, firing_pos));
		firing_sound.play();
		// launch a bullet
		float angle = (shape.getRotation() + rand() % 11 - 5)*PI / 180.f;
		Vector2f bullet_v { BULLET_SPEED * cosf(angle), BULLET_SPEED * sinf(angle) };
		Object::add(new Bullet(firing_pos, bullet_v, shape.getRotation()), BULLET_LAYER);
	}
	if (fire_muzzle != nullptr) {
		if (fire_muzzle->isDead()) fire_muzzle.reset();
		else {
			fire_muzzle->setPosition(getFiringPosition());
			fire_muzzle->play();
		}
	}

	// move view with the plane
	Vector2f view_center = pos;
	if (view_center.x < -400.f) view_center.x = -400.f;
	if (view_center.x > 1200.f) view_center.x = 1200.f;
	if (view_center.y < -300.f) view_center.y = -300.f;
	if (view_center.y > 900.f) view_center.y = 900.f;
	view.setCenter(view_center);

	// collision detection
	for (auto& i : pos_buckets[getPositionBucket()])
		if (i->getType() == Type_Asteroid) // only collide with asteroids
			if (length(getPosition(), i->getPosition()) <= radius + i->getRadius()) die();
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
	Vector2f pos = view.getCenter();
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
	// draw the plane
	if (!dead) w.draw(shape);
	// draw fire muzzle
	if (fire_muzzle != nullptr) if (!fire_muzzle->isDead()) fire_muzzle->draw(w);
}

void Plane::die()
{
	// play the death animation
	Effect::list.push_back(
		shared_ptr<Effect>(
			new Animation(&Assets::explosion[0], 34, { 120, 134 }, getPosition(), &Assets::explosion_sound, 1, 3)));
	dead = true;
	player_is_dead = true;
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

Asteroid::Asteroid(int size, Vector2f pos, Vector2f v)
{
	this->size = size;
	Vector2f canvas_size;
	switch (size) {
	case SMALL: 
		canvas_size = { 72, 76 }; 
		radius = 36;
		break;
	case MEDIUM: 
		canvas_size = { 136, 120 }; 
		radius = 60;
		break;
	case LARGE: 
		canvas_size = { 236, 264 }; 
		radius = 118;
		break;
	}
	image.reset(new Animation(&Assets::astr_tex[size][0], 10, canvas_size, pos, nullptr, -1, 5 * (size + 1)));
	this->v = v;
}

void Asteroid::update()
{
	image->play();
	Vector2f pos = image->getPosition();

	// edge checking
	if ((pos.x - radius <= -800 && v.x < 0) || (pos.x + radius >= 1600 && v.x > 0)) v.x = -v.x;
	if ((pos.y - radius <= -600 && v.y < 0) || (pos.y + radius >= 1200 && v.y > 0)) v.y = -v.y;

	// collision detection
	for (auto& i : pos_buckets[getPositionBucket()]) {
		Vector2f ipos = i->getPosition();
		if (pos == ipos) continue; // self
		if (length(pos, ipos) <= radius + i->getRadius()) { // collide
			if (i->getType() != Type_Bullet) { // if not collide with bullet, bounce off
				Vector2f diff = ipos - pos;
				if (is_same_sign(diff.x, v.x)) v.x = -v.x;
				if (is_same_sign(diff.y, v.y)) v.y = -v.y;
			}
			else { // if collide with bullet, both die
				i->die();
				die();
				return;
			}
		}
	}
	pos += v;
	image->setPosition(pos);
}

void Asteroid::draw(RenderWindow &w)
{
	image->draw(w);
}

void Asteroid::die()
{
	score += 50;
	Effect::list.push_back(
		shared_ptr<Effect>(
			new Animation(&Assets::explosion[0], 34, { 120, 134 }, getPosition(), &Assets::explosion_sound, 1, 3)));
	if (size > 0) {
		Vector2f v1 = v, v2 = v;
		v1.x = -2 * v1.x;
		v1.y = 2 * v1.y;
		v2.x = 2 * v2.y;
		v2.y = -2 * v2.y;
		Object::add(new Asteroid(size - 1, getPosition() + v1, v1), ASTR_LAYER);
		Object::add(new Asteroid(size - 1, getPosition() + v2, v2), ASTR_LAYER);
	}
	dead = true;
}

