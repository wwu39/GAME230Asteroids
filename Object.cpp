#include <cmath>
#include "Object.h"

#define PI 3.14159265358979323846f

vector<shared_ptr<Object>> Object::list;

Object::Object()
{
}


Object::~Object()
{
}

Vector2f Plane::getFiringPosition()
{
	Vector2f firing_pos = shape.getPosition();
	float angle = shape.getRotation()*PI / 180.f;
	Vector2f dist = { 72 * cosf(angle), 72 * sinf(angle) };
	firing_pos += dist;
	return firing_pos;
}

Plane::Plane()
{
	shape.setSize({138, 138});
	shape.setOrigin({ 69, 69 });
	shape.setPosition({ 400, 300 });
	shape.setTexture(&Assets::plane[0][0]);
	shape.setRotation(270);
	firing_sound.setBuffer(Assets::gunfire);
}

void Plane::update(vector<shared_ptr<Object>>& list)
{
	// move
	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) {
		shape.rotate(-PLANE_ROT);
	}
	if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) {
		shape.rotate(PLANE_ROT);
	}
	if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) {
		if (status != DIVING) status = DIVING;
		float angle = shape.getRotation()*PI / 180.f;
		if (speed < PLANE_SPEED) speed += PLANE_ACCEL;
		Vector2f v = { speed * cosf(angle), speed * sinf(angle) };
		shape.setPosition(shape.getPosition() + v);
	}
	else {
		if (status == DIVING) status = NOT_DIVING;
		if (speed > 0) speed -= PLANE_ACCEL; else if (speed < 0) speed = 0;
		if (speed != 0) {
			float angle = shape.getRotation()*PI / 180.f;
			Vector2f v = { speed * cosf(angle), speed * sinf(angle) };
			shape.setPosition(shape.getPosition() + v);
		}
	}

	// attack
	if (fire_muzzle == nullptr && Keyboard::isKeyPressed(Keyboard::Space)) {
		Vector2f firing_pos = getFiringPosition();
		// gun muzzle
		fire_muzzle.reset(new Animation(&Assets::muzzle[0], 4, { 28, 26 }, firing_pos));
		firing_sound.play();
		// launch a bullet
		float angle = shape.getRotation()*PI / 180.f;
		Vector2f bullet_v { BULLET_SPEED * cosf(angle), BULLET_SPEED * sinf(angle) };
		Object::list.push_back(shared_ptr<Object>(new Bullet(firing_pos, bullet_v, shape.getRotation())));
	}
	if (fire_muzzle != nullptr) {
		if (fire_muzzle->isDead()) fire_muzzle.reset();
		else {
			fire_muzzle->setPosition(getFiringPosition());
			fire_muzzle->play();
		}
	}
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
	if (fire_muzzle != nullptr) if (!fire_muzzle->isDead()) fire_muzzle->draw(w);
	if (!dead) w.draw(shape);
}

Bullet::Bullet(Vector2f pos_s, Vector2f v, float rotation, int lifetime)
{
	this->v = v;
	this->lifetime = lifetime;
	lifetime_remain = lifetime;
	shape.setSize({ 13, 5 });
	shape.setOrigin({ 6.5f, 2.5f });
	shape.setTexture(&Assets::bullet);
	shape.setPosition(pos_s);
	shape.setRotation(rotation);
}

void Bullet::update(vector<shared_ptr<Object>>&)
{
	--lifetime;
	if (lifetime == 0) dead = true;
	else shape.setPosition(shape.getPosition() + v);
}

void Bullet::draw(RenderWindow &w)
{
	if (!dead) w.draw(shape);
}
