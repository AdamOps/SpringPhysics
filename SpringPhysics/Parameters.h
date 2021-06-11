#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

struct gameParameters {
	// Number of iterations to satisfy all constraints
	const int simIterations = 5;

	// RNG seed, which is kept constant for the sake of reproducibility.
	const int seed = 1;

	// Soft rectangle properties
	const int rows = 4;
	const int cols = 5;

	// Constant for Hooke's law
	const float springConstant = 0.4f;

	// Parameters affecting non-spring forces
	sf::Vector2f gravity;
	const float friction = 0.05f;
	const float frictionThreshold = 0.02f;
};