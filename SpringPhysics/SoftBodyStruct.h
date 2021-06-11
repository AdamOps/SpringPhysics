#pragma once
#include <vector>

#include "Vertex.h"
#include "Spring.h"

struct softBody {
	std::vector<Vertex> vertices;
	std::vector<Spring> springs;
};