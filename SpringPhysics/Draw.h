#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>

#include "Vertex.h"
#include "Spring.h"
#include "Parameters.h"
#include "SoftBodyStruct.h"

void drawAll(sf::RenderWindow* window, std::vector<int>& randomVert, std::vector<int>& randomSpring, std::vector<Vertex>& vertices, std::vector<Spring>& springs, sf::ConvexShape& blob, struct gameParameters settings, struct softBody * rectBody);