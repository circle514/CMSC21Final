#include "Game.h"
#include <iostream>

using namespace sf;

void Game::initVariables() {
	this->window = nullptr;

	//Clock for enemy spawning
	srand(static_cast<unsigned>(time(NULL)));

	//Time variable to determine when to spawn an enemy
	this->spawnTimer = 0;
	this->spawnTimerMax = 60;
	this->gameClock.restart();

	//Time variable to determine how fast to shoot
	this->fireTimerMax = 60.f;
	this->fireTimer = this->fireTimerMax;

	this->playerHpMax = 4;
	this->playerHp = this->playerHpMax;

	this->hpBarBack.setSize({ 10.f, 2.f });
	this->hpBarBack.setFillColor(Color(50, 50, 50, 200));

	this->hpBarFront.setSize({ 10.f, 2.f });
	this->hpBarFront.setFillColor(Color::Red);

	//background
	if (!this->backgroundTexture.loadFromFile("background.png")) {
		std::cerr << "Error loading background texture!" << std::endl;
	}
	this->initBackground();
}

void Game::initBackground() {
	this->backgroundTexture.setRepeated(true);
	this->backgroundSprite.setTexture(this->backgroundTexture);
	this->backgroundSprite.setScale({ 0.166f, 0.166f });
	this->backgroundSprite.setTextureRect({ {0, 0}, {30000, 30000} });
	this->backgroundSprite.setOrigin({ 15000.f, 15000.f });
}

//Enemy Functions
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
	enemyData enemy;
	enemy.shape.setSize({ 4.f, 4.f });
	enemy.shape.setFillColor(Color::Red);
	enemy.shape.setOrigin({ 2.f, 2.f });

	float randomAngle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.f * 3.14159f;
	float spawnRadius = 250.f;

	Vector2f playerPos = this->player.getPosition();

	float spawnX = playerPos.x + std::cos(randomAngle) * spawnRadius;
	float spawnY = playerPos.y + std::sin(randomAngle) * spawnRadius;

	enemy.shape.setPosition({ spawnX, spawnY });

	//Logic for adding healthto enemies based on how long the game has been going
	float secondsAlive = this->gameClock.getElapsedTime().asSeconds();
	enemy.hp = 2 + static_cast<int>(secondsAlive / 15.0f);

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

void Game::moveEnemy(enemyData& enemy) {
	float movementSpeed = 0.5f;
	Vector2f direction = this->player.getPosition() - enemy.shape.getPosition();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length != 0) {
		direction /= length; 
		enemy.shape.move(direction * movementSpeed); 
	}
}

void Game::resolveCollision(enemyData& currentEnemy, float minDistance) {
	for (auto& other : this->enemies) {
		if (&currentEnemy == &other) continue;

		Vector2f dir = currentEnemy.shape.getPosition() - other.shape.getPosition(); 
			float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

		if (distance < minDistance && distance > 0.f) {
			Vector2f pushDir = dir / distance;
			float pushStrength = (minDistance - distance) * 0.05f; 
			currentEnemy.shape.move(pushDir * pushStrength); 
		}
	}
}

//If I was a bit smarter I couldve foreseen that I should check all collisions (not just enemy-enemy), but yeah
void Game::updateEnemy() {
	float overlapThreshold = 10.f;
	for (size_t i = 0; i < this->enemies.size(); ) {
		this->moveEnemy(this->enemies[i]);
		this->resolveCollision(this->enemies[i], overlapThreshold);

		if (this->player.getGlobalBounds().findIntersection(this->enemies[i].shape.getGlobalBounds())) {
			this->playerHp -= 1;
			this->enemies.erase(this->enemies.begin() + i); 

			if (this->playerHp <= 0) {
				std::cout << "Game Over!" << std::endl;
				this->window->close();
			}
		}
		else {
			i++;
		}
	}
}

//Projectile functions
void Game::spawnProjectile() {
	Vector2i mousePos = Mouse::getPosition(*this->window);
	Vector2f worldPos = this->window->mapPixelToCoords(mousePos);
	this->projectiles.push_back(new Projectile(this->player.getPosition(), worldPos));
}

void Game::updateProjectiles() {
	for (size_t i = 0; i < this->projectiles.size();) {
		this->projectiles[i]->update();

		bool deleted = false;

		for (size_t j = 0; j < this->enemies.size(); j++) {
			if (this->projectiles[i]->getBounds().findIntersection(this->enemies[j].shape.getGlobalBounds())) {
				this->enemies[j].hp -= (this->projectiles[i]->getDamage());

				if (this->enemies[j].hp <= 0) {
					this->enemies.erase(this->enemies.begin() + j);
				}

				delete this->projectiles[i];
				this->projectiles.erase(this->projectiles.begin() + i);

				deleted = true;
				break;
			}
		}

		if (!deleted && this->projectiles.size() > i && this->projectiles[i]->isExpired()) {
			delete this->projectiles[i];
			this->projectiles.erase(this->projectiles.begin() + i);
			deleted = true;
		}

		if (!deleted) {
			++i;
		}
	}
}


//Actual Game Logic functions
Game::Game() : backgroundSprite(this->backgroundTexture) {
	this->initVariables();
	this->initWindow();
	this->initPlayer();
	this->initEnemies();
}

Game::~Game() {
	delete this->window;
	for (auto* p : this->projectiles) {
		delete p;
	}
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

	float movementSpeed = 1.f;
	if (Keyboard::isKeyPressed(Keyboard::Key::W))
		this->player.move({ 0.f, -movementSpeed });
	if (Keyboard::isKeyPressed(Keyboard::Key::S))
		this->player.move({ 0.f, movementSpeed });
	if (Keyboard::isKeyPressed(Keyboard::Key::A))
		this->player.move({ -movementSpeed, 0.f });
	if (Keyboard::isKeyPressed(Keyboard::Key::D))
		this->player.move({ movementSpeed, 0.f });

	if (this->fireTimer < this->fireTimerMax) {
		this->fireTimer += 1.f;
	}

	if (this->fireTimer >= this->fireTimerMax) {
		this->spawnProjectile();
		this->fireTimer = 0.f;
	}
	
	//Enemy movement
	this->updateSpawner();
	this->updateEnemy();
	this->updateProjectiles();

	Vector2f hpPos = this->player.getPosition();
	float pRadius = this->player.getRadius();

	this->hpBarBack.setPosition({hpPos.x - 1.f, hpPos.y + (pRadius * 2.f) + 1.f});
	this->hpBarFront.setPosition(this->hpBarBack.getPosition());
		
	float hpPercent = static_cast<float>(this->playerHp) / this->playerHpMax;
	this->hpBarFront.setSize({ 10.f * hpPercent, 2.f });

	this->gameView.setCenter(this->player.getPosition());
	this->window->setView(this->gameView);
}

void Game::render() {
	this->window->clear();
	this->window->draw(this->backgroundSprite);
	for (auto& e : this->enemies) {
		this->window->draw(e.shape);
	}
	for (auto* p : this->projectiles) {
		p->render(*this->window);
	}

	this->window->draw(this->hpBarBack);
	this->window->draw(this->hpBarFront);

	this->window->draw(this->player);
	this->window->display();
}