#include <cmath>
#include "Object.h"

#define PI 3.14159265358979323846f

vector<shared_ptr<Object>> Object::list;
vector<weak_ptr<Object>> Object::render_buckets[5];
vector<weak_ptr<Object>> Object::pos_buckets[9];
View Plane::view { FloatRect(0, 0, 800, 600) };

bool Object::inRectTopLeft(Vector2f pos, float x, float y, float w, float h)
{
	if (pos.x >= x && pos.x < x + w && pos.y >= y && pos.y < y + h)
		return true;
	return false;
}

bool Object::inRectTopLeftRight(Vector2f pos, float x, float y, float w, float h)
{
	if (pos.x >= x && pos.x <= x + w && pos.y >= y && pos.y < y + h)
		return true;
	return false;
}

bool Object::inRectAll(Vector2f pos, float x, float y, float w, float h)
{
	if (pos.x >= x && pos.x <= x + w && pos.y >= y && pos.y <= y + h)
		return true;
	return false;
}

Object::Object()
{
}


Object::~Object()
{
}

int Object::getPositionBucket()
{
	Vector2f pos = getPosition();
	if (inRectTopLeft(pos, -800, -600)) return 0;
	else if (inRectTopLeft(pos, 0, -600)) return 1;
	else if (inRectTopLeftRight(pos, 800, -600)) return 2;
	else if (inRectTopLeft(pos, -800, 0)) return 3;
	else if (inRectTopLeft(pos, 0, 0)) return 4;
	else if (inRectTopLeftRight(pos, 800, 0)) return 5;
	else if (inRectTopLeft(pos, -800, 600)) return 6;
	else if (inRectTopLeft(pos, 0, 600)) return 7;
	else if (inRectAll(pos, 800, 600)) return 8;
	return 0;
}

Vector2f Plane::getFiringPosition()
{
	Vector2f firing_pos = shape.getPosition();
	float angle = shape.getRotation()*PI / 180.f;
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

void Plane::draw_nodes(RenderWindow &)
{
}

Plane::Plane()
{
	shape.setSize({138, 138});
	shape.setOrigin({ 69, 69 });
	shape.setPosition({ 400, 300 });
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
	if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) {
		shape.rotate(-PLANE_ROT);
	}
	if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) {
		shape.rotate(PLANE_ROT);
	}
	// move
	Vector2f pos = shape.getPosition();
	float angle = shape.getRotation()*PI / 180.f;
	if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) {
		if (jet_sound.getStatus() != Sound::Playing) jet_sound.play();
		if (status != DIVING) status = DIVING;
		if (speed < PLANE_SPEED) speed += PLANE_ACCEL;
		Vector2f v = { speed * cosf(angle), speed * sinf(angle) };
		shape.setPosition(inBound(pos + v));
	}
	else {
		if (status == DIVING) status = NOT_DIVING;
		if (speed > 0) speed -= PLANE_ACCEL; else if (speed < 0) speed = 0;
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
		Object::list.push_back(shared_ptr<Object>(new Bullet(firing_pos, bullet_v, shape.getRotation())));
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

void Bullet::update()
{
	--lifetime;
	if (lifetime == 0) {
		dead = true;
		Effect::list.push_back(shared_ptr<Effect>(new Animation(&Assets::piff[0], 4, { 38, 34 }, shape.getPosition())));
	}
	else shape.setPosition(shape.getPosition() + v);
}

void Bullet::draw(RenderWindow &w)
{
	if (!dead) w.draw(shape);
}
