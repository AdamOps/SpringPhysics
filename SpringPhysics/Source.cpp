#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <array>
#include <random>
#include <algorithm>

#include "Vertex.h"
#include "Spring.h"
#include "Utility.h"

const int refPointCount = 18;
const int simIterations = 2;
const int seed = 1;

int main()
{
    int horizontal, vertical;
    getDesktopResolution(horizontal, vertical);
    // sf::View view(sf::FloatRect(0, 0, horizontal / 2, vertical / 2));
    sf::RenderWindow window(sf::VideoMode(horizontal / 2, vertical / 2), "Blob test");
    window.setFramerateLimit(60);
    // window.setView(view);

    int width = window.getSize().x;
    int height = window.getSize().y;

    // "vertices" is a vector running in the background to do all physics calculations, alongside with the "springs" vector.
    std::vector<Vertex> vertices;
    std::vector<Spring> springs;

    sf::CircleShape circleReference;
    circleReference.setPointCount(refPointCount);
    circleReference.setPosition(horizontal / 4, vertical / 4);
    circleReference.setRadius(100.f);
    circleReference.setOrigin(circleReference.getOrigin().x + circleReference.getRadius(), circleReference.getOrigin().y + circleReference.getRadius());

    sf::CircleShape refCenter;
    refCenter.setPointCount(refPointCount);
    refCenter.setPosition(horizontal / 4, vertical / 4);
    refCenter.setRadius(20.f);
    refCenter.setOrigin(circleReference.getOrigin().x + circleReference.getRadius(), circleReference.getOrigin().y + circleReference.getRadius());

    sf::ConvexShape blob;
    blob.setFillColor(sf::Color::Yellow);
    blob.setPointCount(circleReference.getPointCount());

    for (std::size_t i = 0; i < circleReference.getPointCount(); i++) {
        // Set up the blob shape, based on a reference circle shape.
        vertices.push_back(Vertex(circleReference.getPoint(i) + circleReference.getPosition()));
        sf::Vector2f convexPoint(vertices.at(i).position);
        blob.setPoint(i, convexPoint);
    }

    for (int i = 0; i < vertices.size(); i++) {
        // Create springs between all vertices of the blob
        int vertOne = i;
        int vertTwo = (i + 1) % refPointCount;
        springs.push_back(Spring(vertOne, vertTwo, vertices.at(vertOne).position, vertices.at(vertTwo).position));

    }

    sf::Vector2f center(horizontal / 4 + circleReference.getOrigin().x, vertical / 4 + circleReference.getOrigin().y);
    vertices.push_back(Vertex(center));

    for (int i = 0; i < refPointCount; i++) {
       springs.push_back(Spring(i, refPointCount, vertices.at(i).position, vertices.at(refPointCount).position));
       std::cout << "Spring " << i << ": v1 = " << i << ", v2 = " << refPointCount << std::endl;
    }

    // Isolating the 0 vertex (which is often used as a visual reference)
    vertices.at(0).vertShape.setFillColor(sf::Color::Blue);

    // Calculate the initial blob area. This is used for an area constraint.
    float blobArea = getBlobArea(vertices);

    // std::cout << "blob area: " << blobArea << std::endl;

    std::vector<int> randomVert, randomSpring;
    for (int i = 0; i < vertices.size(); i++) {
        randomVert.push_back(i);
    }
    for (int i = 0; i < springs.size(); i++) {
        randomSpring.push_back(i);
    }
    std::shuffle(randomVert.begin(), randomVert.end(), std::default_random_engine(seed));
    std::shuffle(randomSpring.begin(), randomSpring.end(), std::default_random_engine(seed));

    for (int i = 0; i < randomVert.size(); i++) {
        std::cout << randomVert.at(i) << std::endl;
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Update vertex positions
        for (int i = 0; i < vertices.size(); i++) {
            vertices.at(randomVert.at(i)).updateVertexPosition();
        }

        // Loop through spring forces to adjust all vertex positions again
        for (int k = 0; k < simIterations; k++) {
            for (int i = 0; i < vertices.size(); i++) {
                vertices.at(randomVert.at(i)).constrainVertexPosition(height, width);
            }
            for (int i = 0; i < springs.size(); i++) {
                springs.at(randomSpring.at(i)).springConstraint(vertices);
            }
            // Not very stable yet. Constrains the area 
            areaConstraint(vertices, blobArea, blob.getGlobalBounds().width, blob.getGlobalBounds().height, randomVert);
        }

        for (int i = 0; i < vertices.size(); i++) {
            vertices.at(i).vertShape.setPosition(vertices.at(i).position);
        }

        // Update blob vertices after all forces/movements/etc have been calculated
        for (int i = 0; i < refPointCount; i++) {
            blob.setPoint(i, vertices.at(i).position);
        }

        window.draw(blob);

        for (int i = 0; i < vertices.size(); i++) {
            window.draw(vertices.at(i).vertShape);
        }

        window.display();
    }

    return 0;
}