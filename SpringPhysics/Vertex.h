#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Parameters.h"

class Vertex
{
public:
	Vertex(sf::Vector2f passedPosition);
	Vertex(float x, float y);

	sf::Vector2f position, oldPosition;
	sf::CircleShape vertShape;

	void updateVertexPosition(gameParameters * settings);
	void constrainVertexPosition(int height, int width, gameParameters * settings);
	void addFriction(int height, gameParameters * settings);

};

