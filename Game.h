#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/* Game Logic */
using namespace sf;

class Game
{
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
	
	//init Functions for Game
	void initPlayer();
	void initEnemies();
	void initVariables();
	void initWindow();

	//Logic functions
	void spawnEnemy();
	void moveEnemy(RectangleShape& enemy);
	void resolveCollision(RectangleShape& currentEnemy, float minDistance);

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

