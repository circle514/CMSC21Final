#pragma once
#include "projectile.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

//I understand having namespace in a header is bad practice, 
// but for the sake of simplicity in this project, I'll use it here. In a larger project, 
// it's better to avoid this and use fully qualified names or wrap the entire class in a namespace.
using namespace sf;

//I wont make another class for enemies because the only difference between them is health and I don't plan to have different types
struct enemyData {
	RectangleShape shape;
	int hp;
};

class Game {
private:
	//Variables
	//Window
	RenderWindow* window;
	View gameView;
	std::vector<enemyData> enemies;

	//Spawning variables
	int spawnTimer;
	int spawnTimerMax;
	Clock gameClock;

	//Entities
	CircleShape player;

	//Projectiles
	std::vector<Projectile*> projectiles;

	//Background
	Texture backgroundTexture;
	Sprite backgroundSprite;

	//Attack timer
	float fireTimer;
	float fireTimerMax;
	
	//HP stuff
	int playerHp;
	int playerHpMax;
	RectangleShape hpBarBack;
	RectangleShape hpBarFront;
	void initUI();

	//init Functions for Game
	void initPlayer();
	void initEnemies();
	void initVariables();
	void initBackground();
	void initWindow();

	//Logic functions for enemies
	void spawnEnemy();
	void moveEnemy(enemyData& enemy);
	void resolveCollision(enemyData& currentEnemy, float minDistance);

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

