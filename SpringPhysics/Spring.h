#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "Vertex.h"

class Spring
{
public:
	Spring(int vertOne, int vertTwo, sf::Vector2f posOne, sf::Vector2f posTwo);

	int idOne, idTwo;
	float separation;

	sf::RectangleShape springShape;

	sf::Vector2f distance, springForce;

	void springConstraint(std::vector<Vertex>& vertices, gameParameters * settings);
	void drawSpring(sf::Vector2f vertOne, sf::Vector2f vertTwo);

};

