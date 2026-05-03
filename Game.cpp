#include "Game.h"

using namespace sf;

void Game::initVariables() {
	this->window = nullptr;
	this->initPlayer();
}

void Game::initWindow() {
	this->window = new RenderWindow(VideoMode({ 1920, 1080 }), "Elementalist", Style::Default);
	this->window->setFramerateLimit(60);
	View gameView(FloatRect({ 0, 0 }, { 320, 180 }));
	this->window->setView(gameView);
}

void Game::initPlayer() {
	this->player.setRadius(5.f);
	this->player.setFillColor(Color::Green);
	this->player.setPosition({ 100.f, 100.f });
}

Game::Game() {
	this->initVariables();
	this->initWindow();
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
}

void Game::render() {
	this->window->clear();
	this->window->draw(this->player);
	this->window->display();
}