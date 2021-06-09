#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
class Vertex
{
public:
	static float gravity;
	static float friction;
	static float elasticity;

	Vertex(sf::Vector2f passedPosition);

	sf::Vector2f position, oldPosition;
	sf::CircleShape vertShape;

	void updateVertexPosition();
	void constrainVertexPosition(int height, int width);
};

