#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "model/plot_model.h"

class Visualizer {
    PlotData* plotData;
    double zoomFactor;
    Point zoomCenter;

    sf::RectangleShape zoomInButton;
    sf::RectangleShape zoomOutButton;
    sf::Font font;
    sf::Text zoomInText;
    sf::Text zoomOutText;

    void initializeButtons(const sf::Vector2u& windowSize);

    bool isPointInButton(const sf::Vector2f& point, const sf::RectangleShape& button) const;

    sf::Vector2f scalePoint(const Point& p, const unsigned effectiveSize[2],
                          const unsigned offset[2]);

    sf::Vertex* renderGraph(const sf::Vector2u& windowSize);

    void drawUI(sf::RenderWindow& window) const;

public:
    explicit Visualizer(PlotData*);

    void render();
};

#endif //VISUALIZATION_H