#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

using namespace sf;

class Projectile {
private:
	CircleShape shape;
	Vector2f velocity;
	float movementSpeed;

	//Time variables for deleting after a set amount of time
	float timer;
	float timerMax;

public:
	Projectile(Vector2f playerPos, Vector2f targetPos);
	virtual ~Projectile();

	//Accessors
	const FloatRect getBounds() const;
	const bool isExpired() const;

	void update();
	void render(RenderTarget& target);
};

