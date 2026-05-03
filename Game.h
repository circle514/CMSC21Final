#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>

/* Game Logic */

class Game
{
private:
	void initVariables();
	void initWindow();

public: 
	Game();
	virtual ~Game();

	//Functions
	void update();
	void render();
};

