#pragma once
#include "projectile.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/* Game Logic */
//I understand having namespace in a header is bad practice, but for the sake of simplicity in this project, I'll use it here. In a larger project, it's better to avoid this and use fully qualified names or wrap the entire class in a namespace.
using namespace sf;

class Game {
private:
	//Variables
	//Window
	RenderWindow* window;
	View gameView;
	std::vector<RectangleShape> enemies;

	//Spawning variables
	int spawnTimer;
	int spawnTimerMax;
	Clock gameClock;

	//Entities
	CircleShape player;
	RectangleShape enemy;

	//Projectiles
	std::vector<Projectile*> projectiles;

	float fireTimer;
	float fireTimerMax;
	
	//init Functions for Game
	void initPlayer();
	void initEnemies();
	void initVariables();
	void initWindow();

	//Logic functions for enemies
	void spawnEnemy();
	void moveEnemy(RectangleShape& enemy);
	void resolveCollision(RectangleShape& currentEnemy, float minDistance);

	//Logic functions for projectiles
	void updateProjectiles();
	void spawnProjectile();

public: 
	Game();
	virtual ~Game();

	//Accessors
	const bool running();

	//Functions
	void update();
	void updateEnemy();
	void updateSpawner();
	void render();
};

