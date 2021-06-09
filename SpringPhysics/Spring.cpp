#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>

#include "Spring.h"
#include "Utility.h"

float Spring::springConstant = 0.05f;

Spring::Spring(int vertOne, int vertTwo, sf::Vector2f posOne, sf::Vector2f posTwo) {
	idOne = vertOne;
	idTwo = vertTwo;

	separation = vertDistance(posOne, posTwo);

}

// Check current length of spring. Use this to calculate the resulting force, via Hooke's law
// F = -k*L
// Where L = (current Length - equilibrium length)
// Direction of F is based on the angle of the vertex to the spring.
void Spring::springConstraint(std::vector<Vertex>& vertices){
	distance = vertices.at(idTwo).position - vertices.at(idOne).position;
	float currLength = sqrt(distance.x * distance.x + distance.y * distance.y);
	float excessLength = currLength - separation;

	springForce = (excessLength / currLength) * distance * springConstant;

	vertices.at(idOne).position += springForce;
	vertices.at(idTwo).position -= springForce;
}