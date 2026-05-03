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
	CircleShape player;

	void initVariables();
	void initWindow();
	void initPlayer();

public: 
	Game();
	virtual ~Game();

	//Accessors
	const bool running();

	//Functions
	void update();
	void render();
};

