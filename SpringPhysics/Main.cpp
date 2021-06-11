#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <array>
#include <random>
#include <algorithm>

#include "Vertex.h"
#include "Spring.h"
#include "Utility.h"
#include "EventHandling.h"
#include "Init.h"
#include "Draw.h"
#include "Parameters.h"
#include "SoftBodyStruct.h"

// Number of vertices per circle
const int refPointCount = 20;

   int main()
{
    gameParameters settings;
    settings.gravity.x = 0.f;
    settings.gravity.y = 0.2f;

        // "vertices" is a vector running in the background to do all physics calculations, alongside with the "springs" vector.
    std::vector<Vertex> vertices;
    std::vector<Spring> springs;
    std::vector<Vertex> outerBlob;

    sf::ConvexShape blob;

    //
    softBody rectBody;

    int horizontal, vertical;
    getDesktopResolution(horizontal, vertical);
    sf::View view(sf::FloatRect(0, 0, horizontal / 2, vertical / 2));
    sf::RenderWindow window(sf::VideoMode(horizontal / 2, vertical / 2), "Blob test");
    window.setFramerateLimit(60);
    window.setView(view);

    int width = window.getSize().x;
    int height = window.getSize().y;

    createAll(&window, refPointCount, vertices, springs, outerBlob, blob, &rectBody, settings);

    //// Calculate the initial blob area. This is used for an area constraint.
    //float blobArea = getBlobArea(shapes.outerBlob);

    std::vector<int> randomVert, randomSpring;
    for (int i = 0; i < vertices.size(); i++) {
        randomVert.push_back(i);
    }
    for (int i = 0; i < springs.size(); i++) {
        randomSpring.push_back(i);
    }

    while (window.isOpen())
    {
        eventLoop(&window, &view, &settings);
        drawAll(&window, randomVert, randomSpring, vertices, springs, blob, settings, &rectBody);
    }

    return 0;
}