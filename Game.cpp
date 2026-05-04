#include "Game.h"
#include <iostream>

using namespace sf;

void Game::initVariables() {
	this->window = nullptr;

	srand(static_cast<unsigned>(time(NULL)));

	this->spawnTimer = 0;
	this->spawnTimerMax = 60;
	this->gameClock.restart();
}

void Game::initWindow() {
	this->window = new RenderWindow(VideoMode({ 1920, 1080 }), "Elementalist", Style::Default);
	this->window->setFramerateLimit(60);
	this->gameView.setSize({ 320.f, 180.f });
	this->window->setView(gameView);
}

void Game::initPlayer() {
	this->player.setRadius(4.f);
	this->player.setFillColor(Color::Green);
	this->player.setPosition({ 100.f, 100.f });
}

void Game::spawnEnemy() {
	RectangleShape enemy;
	enemy.setSize({ 4.f, 4.f });
	enemy.setFillColor(Color::Red);
	enemy.setOrigin({ 2.f, 2.f });

	float randomAngle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.f * 3.14159f;
	float spawnRadius = 250.f;

	Vector2f playerPos = this->player.getPosition();

	float spawnX = playerPos.x + std::cos(randomAngle) * spawnRadius;
	float spawnY = playerPos.y + std::sin(randomAngle) * spawnRadius;

	enemy.setPosition({spawnX, spawnY});
	this->enemies.push_back(enemy);
}

void Game::updateSpawner() {
	this->spawnTimer++;

	if (this->spawnTimer >= this->spawnTimerMax) {
		this->spawnEnemy();
		this->spawnTimer = 0; 

		float secondsAlive = this->gameClock.getElapsedTime().asSeconds();
		int newSpawnRate = 60 - static_cast<int>(secondsAlive / 10.0f);
		this->spawnTimerMax = std::max(5, newSpawnRate);
	}
}

void Game::initEnemies() {
	this->enemies.clear();
}

void Game::moveEnemy(RectangleShape& enemy) {
	Vector2f direction = this->player.getPosition() - enemy.getPosition();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length != 0) {
		direction /= length; 
		enemy.move(direction * 1.f); 
	}
}

void Game::resolveCollision(RectangleShape& currentEnemy, float minDistance) {
	for (auto& other : this->enemies) {
		if (&currentEnemy == &other) continue;

		Vector2f dir = currentEnemy.getPosition() - other.getPosition(); 
			float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

		if (distance < minDistance && distance > 0.f) {
			Vector2f pushDir = dir / distance;
			float pushStrength = (minDistance - distance) * 0.05f; 
			currentEnemy.move(pushDir * pushStrength); 
		}
	}
}

void Game::updateEnemy() {
	float overlapThreshold = 10.f;
	for (auto& enemy : this->enemies) {
		this->moveEnemy(enemy);
		this->resolveCollision(enemy, overlapThreshold);
	}
}

Game::Game() {
	this->initVariables();
	this->initWindow();
	this->initPlayer();
	this->initEnemies();
}

Game::~Game() {
	delete this->window;
}

const bool Game::running() {
	return this->window->isOpen();
}

void Game::update() {
	while (const std::optional event = this->window->pollEvent()) {
		if (event->is<Event::Closed>()) {
			this->window->close();
		}
	}

	float movementSpeed = 2.f;
	if (Keyboard::isKeyPressed(Keyboard::Key::W))
		this->player.move({ 0.f, -movementSpeed });
	if (Keyboard::isKeyPressed(Keyboard::Key::S))
		this->player.move({ 0.f, movementSpeed });
	if (Keyboard::isKeyPressed(Keyboard::Key::A))
		this->player.move({ -movementSpeed, 0.f });
	if (Keyboard::isKeyPressed(Keyboard::Key::D))
		this->player.move({ movementSpeed, 0.f });
	
	//Enemy movement
	this->updateSpawner();
	this->updateEnemy();

	this->gameView.setCenter(this->player.getPosition());
	this->window->setView(this->gameView);
}

void Game::render() {
	this->window->clear();
	for (auto& e : this->enemies) {
		this->window->draw(e);
	}
	this->window->draw(this->player);
	this->window->display();
}