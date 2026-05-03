#include "Game.h"

using namespace sf;

int main() {

	RenderWindow window(VideoMode({ 1920, 1080 }), "Elementalist", Style::Default);
	window.setFramerateLimit(60);
	View gameView(FloatRect({ 0, 0 }, { 320, 180 }));
	window.setView(gameView);

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<Event::Closed>()) {
				window.close();
			}
		}
		window.clear(Color::Blue);
		window.display();
	}
}
