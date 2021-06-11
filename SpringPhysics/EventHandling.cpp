#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "EventHandling.h"
#include "Parameters.h"
#include "Utility.h"

void eventLoop(sf::RenderWindow *window, sf::View * view, gameParameters * settings) {
    sf::Event event;
    bool revertRotation = true;

    while ((*window).pollEvent(event))
    {
        // Window can be closed by pressing Esc or clicking on the X. This breaks the loop over in Main.cpp
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
            (*window).close();
        }

        // Stops the image from being stretched when the window is resized.
        if (event.type == sf::Event::Resized)
        {
            sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
            (*window).setView(sf::View(visibleArea));
        }

        // If the left keyboard button is pressed, the world should rotate so that gravity no longer pulls downwards, but more to what used to be the left.
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left && revertRotation == true) {
            revertRotation = false;
            (*view).setRotation((*view).getRotation() + 1.0);
            std::cout << "Current rotation: " << (*view).getRotation() << " degrees." << std::endl;
            (*window).setView(*view);
            rotateVector((*settings).gravity, 1);
        }

        // If the right keyboard button is pressed, the world rotates so that gravity pulls more to the right instead. Converse of the previous event.
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && revertRotation == true) {
            revertRotation = false;
            (*view).setRotation((*view).getRotation() - 1.0);
            std::cout << "Current rotation: " << (*view).getRotation() << " degrees." << std::endl;
            (*window).setView(*view);
            rotateVector((*settings).gravity, -1);
        }

        if (event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)) {
            revertRotation = true;
        }

        if (revertRotation == true) {
            float currRotation = (*view).getRotation() - 180;
            //(*view).setRotation(currRotation - (currRotation > 0 && currRotation <= 20) + (currRotation >= 340));
            //(*window).setView(*view);
        }
    }
}