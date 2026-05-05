#include "projectile.h"
#include <cmath>

using namespace sf;

Projectile::Projectile(Vector2f playerPos, Vector2f targetPos) {
	this->shape.setRadius(2.f);
	this->shape.setFillColor(Color::Yellow);
	this->shape.setPosition(playerPos);
	this->movementSpeed = 2.f;
	this->damage = 1;
	
	this->timer = 0.f;
	this->timerMax = 600.f;

	Vector2f direction = targetPos - playerPos;
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length != 0) {
		this->velocity = (direction / length) * this->movementSpeed;
	}
}

Projectile::~Projectile() {}

const FloatRect Projectile::getBounds() const {
	return this->shape.getGlobalBounds();
}

const bool Projectile::isExpired() const {
	return this->timer >= this->timerMax;
}

void Projectile::update() {
	this->shape.move(this->velocity);
	this->timer += 1.f;
}

const int Projectile::getDamage() const {
	return this->damage;
}

void Projectile::render(RenderTarget& target) {
	target.draw(this->shape);
}