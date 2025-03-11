//
// Created by dzikimlecz on 11.03.25.
//

#include "visualization.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

Visualizer::Visualizer(PlotData* plotData) : plotData(plotData) { }

void Visualizer::render() const {
    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "Plotter2D");
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        window.clear(sf::Color::White);

        sf::Vertex line[plotData->pointsCount()];
        for (int i = 0; i < plotData->pointsCount(); ++i) {
            const Point& p = plotData->points()[i];
            sf::Vertex v(sf::Vector2f(10 * p.x(), window.getSize().y / 2 - 10 * p.y()));
            v.color = sf::Color::Black;
            line[i] = v;
        }

        window.draw(line, plotData->pointsCount(), sf::LineStrip);
        window.display();
    }
}
