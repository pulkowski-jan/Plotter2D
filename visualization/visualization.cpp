#include "visualization.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

Visualizer::Visualizer(PlotData* plotData) : plotData(plotData) { }

static constexpr unsigned DEFAULT_WINDOW_SIZE = 1000;
static constexpr double PADDING_SIZE[2] = {0.025, 0.01};

sf::Vector2f Visualizer::scalePoint(const Point& p, const unsigned effectiveSize[2],
                                    const unsigned offset[2]) const {
    // scale the point to the window size
    auto x = (p.x() - plotData->domain().anchor().x()) / plotData->domain().width() *
                   effectiveSize[0];
    auto y = (p.y() - plotData->domain().anchor().y()) / plotData->domain().height() *
                    effectiveSize[1];
    // Flip the y axis
    y = effectiveSize[1] - y;
    // Apply the offset
    x += offset[0];
    y += offset[1];
    return {static_cast<float>(x), static_cast<float>(y)};
}

sf::Vertex* Visualizer::renderGraph(const sf::Vector2u& windowSize) const {
    unsigned offset[2] = {
        static_cast<unsigned>(windowSize.x * PADDING_SIZE[0]),
        static_cast<unsigned>(windowSize.y * PADDING_SIZE[1])
    };
    const unsigned effectiveSize[2] = {windowSize.x - 2 * offset[0], windowSize.y - 2 * offset[1]};
    auto* line = new sf::Vertex[plotData->pointsCount()];
    for (int i = 0; i < plotData->pointsCount(); ++i) {
        const Point& p = plotData->points()[i];
        sf::Vertex v(scalePoint(p, effectiveSize, offset));
        v.color = sf::Color::Black;
        line[i] = v;
    }
    return line;
}


void Visualizer::render() const {
    sf::RenderWindow window(sf::VideoMode({DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE}), "Plotter2D");
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::White);

        auto lines = renderGraph({DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE});

        window.draw(lines, plotData->pointsCount(), sf::LineStrip);
        delete[] lines;
        window.display();
    }
}
