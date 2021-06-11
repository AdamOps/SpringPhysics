#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>

#include "Init.h"
#include "Utility.h"
#include "Spring.h"
#include "Vertex.h"
#include "Parameters.h"
#include "SoftBodyStruct.h"

void createAll(sf::RenderWindow* window, int refPointCount, std::vector<Vertex>& vertices, std::vector<Spring>& springs, std::vector<Vertex>& outerBlob, sf::ConvexShape& blob, struct softBody * rectBody, gameParameters &settings) {
    int horizontal, vertical;
    getDesktopResolution(horizontal, vertical);

    int width = (*window).getSize().x;
    int height = (*window).getSize().y;

    sf::CircleShape circleReference;
    circleReference.setPointCount(refPointCount);
    circleReference.setRadius(100.f);
    circleReference.setPosition(horizontal / 4, vertical / 4);
    //circleReference.setOrigin(circleReference.getOrigin().x + circleReference.getRadius(), circleReference.getOrigin().y + circleReference.getRadius());

    sf::CircleShape refCenter;
    refCenter.setPointCount(refPointCount);
    refCenter.setRadius(4.f);
    refCenter.setPosition(horizontal / 4 + (circleReference.getRadius() - refCenter.getRadius()), vertical / 4 + (circleReference.getRadius() - refCenter.getRadius()));
    //refCenter.setOrigin(refCenter.getOrigin().x + refCenter.getRadius(), refCenter.getOrigin().y + refCenter.getRadius());

    blob.setFillColor(sf::Color::Yellow);
    blob.setPointCount(circleReference.getPointCount());

    // Set up the outer blob shape, based on a reference circle shape.
    // These vertices are used to generate the blob polygon.
    for (std::size_t i = 0; i < refPointCount; i++) {
        vertices.push_back(Vertex(circleReference.getPoint(i) + circleReference.getPosition()));
        sf::Vector2f convexPoint(vertices.at(i).position);
        blob.setPoint(i, convexPoint);
        outerBlob.push_back(Vertex(circleReference.getPoint(i) + circleReference.getPosition()));
    }

    // Set up the inner blob, based on a reference circle shape.
    // These points are only used for the physics simulation, to give the blob some more stability/strength.
    for (std::size_t i = 0; i < refPointCount; i++) {
        vertices.push_back(Vertex(refCenter.getPoint(i) + refCenter.getPosition()));
    }

    // Create springs between all vertices of the outer blob layer.
    for (int i = 0; i < circleReference.getPointCount(); i++) {
        int vertOne = i;
        int vertTwo = (i + 1) % refPointCount;
        std::cout << "Outer Blob Spring " << i << ": v1 = " << vertOne << ", v2 = " << vertTwo << std::endl;
        springs.push_back(Spring(vertOne, vertTwo, vertices.at(vertOne).position, vertices.at(vertTwo).position));
    }

    std::cout << "Created " << springs.size() << "/" << refPointCount << " springs of the outer layer." << std::endl;
    std::cout << "Size of <vertices>: " << vertices.size() << std::endl;

    // Create springs between all vertices of the inner blob layer
    for (int i = 0; i < refPointCount; i++) {
        int vertOne = i + refPointCount;
        int vertTwo = ((i + 1) % refPointCount) + refPointCount;
        std::cout << "Inner Blob Spring " << i << ": v1 = " << vertOne << ", v2 = " << vertTwo << std::endl;
        springs.push_back(Spring(vertOne, vertTwo, vertices.at(vertOne).position, vertices.at(vertTwo).position));
    }

    // Create springs between the inner and the outer blob layer
    for (int i = 0; i < refPointCount; i++) {
        int vertTwo = i + refPointCount;
        springs.push_back(Spring(i, vertTwo, vertices.at(i).position, vertices.at(vertTwo).position));
        std::cout << "Spring " << i << ": v1 = " << i << ", v2 = " << i + refPointCount << std::endl;
    }

    // Isolating the 0 vertex (which is often used as a visual reference)
    vertices.at(0).vertShape.setFillColor(sf::Color::Blue);






    sf::Vector2f rectBasePos(40, 40);
    // Creating a soft rectangle here too.
    for (int i = 0; i < (settings.rows * settings.cols); i++) {
        (*rectBody).vertices.push_back(Vertex(rectBasePos.x * (i % settings.cols + 1), rectBasePos.y * (i / settings.cols + 1)));
    } 
    
    for (int i = 0; i < (*rectBody).vertices.size(); i++) {
        //std::cout << "current row and column (i / settings.cols, i % settings.cols) = (" << i / settings.cols << ", " << i % settings.cols << ")" << std::endl;
        if (i % settings.cols != (settings.cols - 1)) {
            //std::cout << "rectBody spring made between: v1 = " << i << ", v2 = " << i + 1 << std::endl;
            (*rectBody).springs.push_back(Spring(i, i + 1, (*rectBody).vertices.at(i).position, (*rectBody).vertices.at(i + 1).position));
        }
        if ((i / settings.cols) != (settings.rows - 1) && (i % settings.cols) != 0) {
            //std::cout << "rectBody spring made between: v1 = " << i << ", v2 = " << i + settings.cols - 1 << std::endl;
            (*rectBody).springs.push_back(Spring(i, i + settings.cols - 1, (*rectBody).vertices.at(i).position, (*rectBody).vertices.at(i + settings.cols - 1).position));
        }
        if ((i / settings.cols) != (settings.rows - 1)) {
            //std::cout << "rectBody spring made between: v1 = " << i << ", v2 = " << i + settings.cols << std::endl;
            (*rectBody).springs.push_back(Spring(i, i + settings.cols, (*rectBody).vertices.at(i).position, (*rectBody).vertices.at(i + settings.cols).position));
        }
        if ((i / settings.cols) != (settings.rows - 1) && (i % settings.cols) != (settings.cols - 1)) {
            //std::cout << "rectBody spring made between: v1 = " << i << ", v2 = " << i + settings.cols + 1 << std::endl;
            (*rectBody).springs.push_back(Spring(i, i + settings.cols + 1, (*rectBody).vertices.at(i).position, (*rectBody).vertices.at(i + settings.cols + 1).position));
        }
    }

    std::cout << "Size of rectBody.vertices = " << (*rectBody).vertices.size() << std::endl;
}