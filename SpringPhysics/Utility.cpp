#include <SFML/Graphics.hpp>
#include <cmath>
#include <wtypes.h>
#include <vector>
#include <iostream>

#include "Utility.h"
#include "Vertex.h"

// Obtains the desktop resolution and stores the width and height to the arguments.
void getDesktopResolution(int& horizontal, int& vertical)
{
	// Basically a 4D vector, containing longs defining the corners of a rectangle.
	RECT desktop;

	// Window handle identifying the desktop.
	const HWND hDesktop = GetDesktopWindow();

	// Get the hDesktop features and store them under the address of the desktop RECT.
	GetWindowRect(hDesktop, &desktop);

	// Adjust the existing variables from the main function with the width and the height of the desktop rectangle (in pixels).
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

// Calculates and returns the Euclidean distance between two vectors
float vertDistance(sf::Vector2f posOne, sf::Vector2f posTwo) {
	return sqrt((posOne.x - posTwo.x) * (posOne.x - posTwo.x) + (posOne.y - posTwo.y) * (posOne.y - posTwo.y));
}

// Calculates and returns the angle between two vectors.
// Useful for e.g. drawing lines between vertices (a RectShape that needs to be rotated all the time)
double vertAngle(sf::Vector2f posOne, sf::Vector2f posTwo) {
	float height = posTwo.y - posOne.y;
	float base = posTwo.x - posOne.x;
	return atan(height / base);
}

// Calculates and returns the angle between two vectors using atan2
float vertAtan2Angle(sf::Vector2f posOne, sf::Vector2f posTwo) {
	float dot = dotProduct(posOne, posTwo);
	float det = posOne.x * posTwo.y - posOne.y * posTwo.x;
	float angle = atan2(det, dot);
	return angle;
}

// Calculates and returns the length of a vector from the origin (0, 0)
float vectorLength(sf::Vector2f vect) {
	return sqrt(vect.x * vect.x + vect.y * vect.y);
}

// Calculates and returns the 
float dotProduct(sf::Vector2f vectOne, sf::Vector2f vectTwo) {
	return vectOne.x * vectTwo.x + vectOne.y * vectTwo.y;
}

void setLength(sf::Vector2f &vectorRef, float currLength, float tarLength) {
	float scaling = tarLength / currLength;
	vectorRef *= scaling;
}

// Compares the area of the convex shape to a target area. If it is smaller, the shape is expanded. If it is bigger, it is shrunk.
// 
// The adjustment process is based on approximating the convex shape with a circle.
// T = target area, A = current area. Then T/A = 1 + n, where n is the percentage by which the two areas are off.
// If the current area A has radius r0, then this radius needs to be expanded to r1 = r0 * sqrt(1+n)
// This way, the area will be exactly equal to the correct size.
void areaConstraint(std::vector<Vertex>& vertices, float areaTarget, float width, float height, std::vector<int>& randomVert) {
	float area = 0;
	area += getBlobArea(vertices);
	float areaAdjustStrength = 0;

	if (area > 1.1 * areaTarget || area < 0.9 * areaTarget) {
		sf::Vector2f force, distance, normal;
		areaAdjustStrength = (width / 2) * (sqrt(1 + areaTarget / area) - 1);

		// Move vertices along the normal of the edge they're connected to clockwise.

		for (int i = 0; i < vertices.size(); i++) {
			normal = vertices.at(randomVert.at(i)).position - vertices.at((randomVert.at(i) + 1) % vertices.size()).position;
			float normalLength = vectorLength(normal);
			float inBetween = normal.x;
			normal.x = normal.y / normalLength;
			normal.y = -normal.x / normalLength;
			if (i == 0) {
				// std::cout << "Normal Length: " << normalLength << std::endl; // << "normal vector X: " << normal.x << ", Y: " << normal.y << std::endl;
			}
			if (area > 1.01 * areaTarget) {
				vertices.at(randomVert.at(i)).position += normal * (areaAdjustStrength / 10);
			}
			else {
				vertices.at(randomVert.at(i)).position -= normal * (areaAdjustStrength / 10);
			}
			// vertices.at(i).oldPosition += normal * (areaAdjustStrength / 10);
		}
	}

	//std::cout << "strength needed " << areaAdjustStrength << std::endl;
	std::cout << "Area: " << area << ", target: " << areaTarget << std::endl;
}

// This function creates splits the convex shape into triangles.
// These triangles are used to calculate the area of the blob.
// Here, the code uses Heron's Method: 
// 1) Calculate the circumference of a triangle with sides a, b and c. The semi-circumference is then s = (a + b + c)/2
// 2) The area of the triangle is sqrt(s * (s - a) * (s - b) * (s - c))
// 3) Add up all the triangle areas to get the full area.
float getBlobArea(std::vector<Vertex>& vertices) {
	int numTriangles = vertices.size() - 2;
	float area = 0;
	float A, B, C, S;
	for (int i = 0; i < numTriangles; i++) {
		A = vertDistance(vertices.at(0).position, vertices.at((i + 1)).position);
		B = vertDistance(vertices.at((i + 1)).position, vertices.at((i + 2)).position);
		C = vertDistance(vertices.at(0).position, vertices.at((i + 2)).position);
		S = (A + B + C) / 2;
		area += sqrt(S * (S - A) * (S - B) * (S - C));
	}

	return area;
}

void rotateVector(sf::Vector2f& vect, float angle) {
	float radAngle = angle * (3.1415926 / 180);
	sf::Vector2f newVect;
	newVect.x = vect.x * cos(radAngle) - vect.y * sin(radAngle);
	newVect.y = vect.x * sin(radAngle) + vect.y * cos(radAngle);
	vect.x = newVect.x;
	vect.y = newVect.y;
}