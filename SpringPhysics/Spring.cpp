#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>

#include "Spring.h"
#include "Utility.h"
#include "Parameters.h"

Spring::Spring(int vertOne, int vertTwo, sf::Vector2f posOne, sf::Vector2f posTwo) {
	idOne = vertOne;
	idTwo = vertTwo;

	separation = vertDistance(posOne, posTwo);
}

// Check current length of spring. Use this to calculate the resulting force, via Hooke's law
// F = -k*L
// Where L = (current Length - equilibrium length)
// Direction of F is based on the angle of the vertex to the spring.
void Spring::springConstraint(std::vector<Vertex>& vertices, gameParameters * settings){
	distance = vertices.at(idTwo).position - vertices.at(idOne).position;
	float currLength = sqrt(distance.x * distance.x + distance.y * distance.y);
	float excessLength = currLength - separation;
	springForce = (excessLength / currLength) * distance * (*settings).springConstant;

	vertices.at(idOne).position += springForce;
	vertices.at(idTwo).position -= springForce;
}

void Spring::drawSpring(sf::Vector2f vertOne, sf::Vector2f vertTwo) {
	float distVertices = vectorLength(vertOne - vertTwo);
	springShape.setSize(sf::Vector2f(distVertices, 1));

	if (vertOne.x <= vertTwo.x) {
		springShape.setPosition(vertOne.x, vertOne.y);
	}
	else {
		springShape.setPosition(vertTwo.x, vertTwo.y);
	}

	springShape.setRotation(vertAngle(vertOne, vertTwo) * 180 / 3.14159);
}