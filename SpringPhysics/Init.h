#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

#include "Vertex.h"
#include "Spring.h"
#include "Parameters.h"

void createAll(sf::RenderWindow* window, int refPointCount, std::vector<Vertex>& vertices, std::vector<Spring>& springs, std::vector<Vertex>& outerBlob, sf::ConvexShape& blob, struct softBody * rectBody, gameParameters &settings);