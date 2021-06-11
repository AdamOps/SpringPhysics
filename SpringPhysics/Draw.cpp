#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>

#include "Vertex.h"
#include "Spring.h"
#include "Parameters.h"
#include "SoftBodyStruct.h"

void drawAll(sf::RenderWindow* window, std::vector<int>& randomVert, std::vector<int>& randomSpring, std::vector<Vertex>& vertices, std::vector<Spring>& springs, sf::ConvexShape& blob, gameParameters settings, struct softBody * rectBody) {
    (*window).clear();

    int width = (*window).getSize().x;
    int height = (*window).getSize().y;
    int refPointCount = blob.getPointCount();

    int seed = settings.seed;
    int simIterations = settings.simIterations;

    // This line shouldn't be necessary, but atm it is.
    gameParameters setCopy = settings;

    // Shouldn't be necessary.
    std::vector<Vertex>& vertAddress = vertices;

    // Shuffle the order in which vertices and springs are updated, to avoid generating patterns due to a sequential update order.
    std::shuffle(randomVert.begin(), randomVert.end(), std::default_random_engine(seed));
    std::shuffle(randomSpring.begin(), randomSpring.end(), std::default_random_engine(seed));

    // Update vertex positions
    for (int i = 0; i < vertices.size(); i++) {
        vertices.at(randomVert.at(i)).updateVertexPosition(&setCopy);
    }

    // Loop through spring forces to adjust all vertex positions again
    for (int k = 0; k < simIterations; k++) {
        for (int i = 0; i < vertices.size(); i++) {
            vertices.at(randomVert.at(i)).constrainVertexPosition(height, width, &setCopy);
        }
        for (int i = 0; i < springs.size(); i++) {
            springs.at(randomSpring.at(i)).springConstraint(vertAddress, &setCopy);
        }
        // Not very stable yet. Constrains the area 
        // areaConstraint(vertices, blobArea, blob.getGlobalBounds().width, blob.getGlobalBounds().height, randomVert);
    }

    for (int i = 0; i < refPointCount; i++) {
        vertices.at(randomVert.at(i)).addFriction(height, &setCopy);
    }

    for (int i = 0; i < vertices.size(); i++) {
        vertices.at(i).vertShape.setPosition(vertices.at(i).position);
    }

    // Update blob vertices after all forces/movements/etc have been calculated
    for (int i = 0; i < refPointCount; i++) {
        blob.setPoint(i, vertices.at(i).position);
    }

    sf::View charMove;
    charMove.setSize(static_cast<float>(width), static_cast<float>(height));
    charMove.setCenter(width/2, vertices.at(0).oldPosition.y);
    // (*window).setView(charMove);

    //(*window).draw(blob);
    //(*window).draw(refCenter);

    for (int i = 0; i < vertices.size(); i++) {
        (*window).draw(vertices.at(i).vertShape);
    }

    for (int i = 0; i < springs.size(); i++) {
        springs.at(i).drawSpring(vertices.at(springs.at(i).idOne).position, vertices.at(springs.at(i).idTwo).position);
        (*window).draw(springs.at(i).springShape);
    }




    // Draw rectBody
    std::vector<int> rectRandomVert, rectRandomSpring;
    for (int i = 0; i < (*rectBody).vertices.size(); i++) {
        rectRandomVert.push_back(i);
    }
    for (int i = 0; i < (*rectBody).springs.size(); i++) {
        rectRandomSpring.push_back(i);
    }

    std::shuffle(rectRandomVert.begin(), rectRandomVert.end(), std::default_random_engine(seed));
    std::shuffle(rectRandomSpring.begin(), rectRandomSpring.end(), std::default_random_engine(seed));

    
    for (int i = 0; i < rectRandomVert.size(); i++) {
        (*rectBody).vertices.at(rectRandomVert.at(i)).updateVertexPosition(&setCopy);
    }
    
    // Loop through spring forces to adjust all vertex positions again
    for (int k = 0; k < simIterations; k++) {
        for (int i = 0; i < rectRandomVert.size(); i++) {
            (*rectBody).vertices.at(rectRandomVert.at(i)).constrainVertexPosition(height, width, &setCopy);
        }
        for (int i = 0; i < rectRandomSpring.size(); i++) {
            (*rectBody).springs.at(rectRandomSpring.at(i)).springConstraint((*rectBody).vertices, &setCopy);
        }
    }
    
    for (int i = 0; i < rectRandomVert.size(); i++) {
        (*rectBody).vertices.at(rectRandomVert.at(i)).addFriction(height, &setCopy);
    }

    for (int i = 0; i < rectRandomVert.size(); i++) {
        (*rectBody).vertices.at(i).vertShape.setPosition((*rectBody).vertices.at(i).position);
    }
    
    for (int i = 0; i < rectRandomVert.size(); i++) {
        (*window).draw((*rectBody).vertices.at(i).vertShape);
    }
    
    for (int i = 0; i < rectRandomSpring.size(); i++) {
        (*rectBody).springs.at(i).drawSpring((*rectBody).vertices.at((*rectBody).springs.at(i).idOne).position, (*rectBody).vertices.at((*rectBody).springs.at(i).idTwo).position);
        (*window).draw((*rectBody).springs.at(i).springShape);
    }

    
    (*window).display();
}