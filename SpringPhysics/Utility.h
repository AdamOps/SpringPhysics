#pragma once
#include "Vertex.h"

void getDesktopResolution(int& horizontal, int& vertical);

float vertDistance(sf::Vector2f posOne, sf::Vector2f posTwo);

double vertAngle(sf::Vector2f posOne, sf::Vector2f posTwo);

float vertAtan2Angle(sf::Vector2f posOne, sf::Vector2f posTwo);

float vectorLength(sf::Vector2f vect);

float dotProduct(sf::Vector2f vectOne, sf::Vector2f vectTwo);

void setLength(sf::Vector2f &vectorRef, float currLength, float tarLength);

void areaConstraint(std::vector<Vertex>& vertices, float areaTarget, float width, float height, std::vector<int>& randomVert);

float getBlobArea(std::vector<Vertex>& vertices);