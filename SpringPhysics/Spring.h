#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "Vertex.h"
#include "Utility.h"

class Spring
{
public:
	static float springConstant;

	Spring(int vertOne, int vertTwo, sf::Vector2f posOne, sf::Vector2f posTwo);

	int idOne, idTwo;
	float separation;

	sf::Vector2f distance, springForce;

	void springConstraint(std::vector<Vertex>& vertices);

};

