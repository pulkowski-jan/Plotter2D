#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "evaluation/function_evaluator.h"
#include "model/plot_model.h"

class Visualizer {
    FunctionEvaluator evaluator;
    PlotData* plotData;
    double zoomFactor;
    Point zoomCenter;

    // Zoom UI elements
    sf::RectangleShape zoomInButton;
    sf::RectangleShape zoomOutButton;
    sf::Text zoomInText;
    sf::Text zoomOutText;

    // Pan UI elements
    sf::RectangleShape panLeftButton;
    sf::RectangleShape panRightButton;
    sf::Text panLeftText;
    sf::Text panRightText;

    sf::Font font;
    double xMin_;
    double xMax_;
    unsigned pointsCount_;

    // Added to preserve vertical position during panning
    double yMin_;
    double yMax_;
    bool initialPlot_;

    void initializeButtons(const sf::Vector2u& windowSize);
    void updatePlotData();

    static bool isPointInButton(const sf::Vector2f& point, const sf::RectangleShape& button);

    sf::Vector2f scalePoint(const Point& p, const unsigned effectiveSize[2],
                            const unsigned offset[2]) const;

    sf::Vertex* renderGraph(const sf::Vector2u& windowSize) const;

    void drawUI(sf::RenderWindow& window) const;

    bool shouldReevaluatePlotData() const;

    // Pan functions
    void panLeft();
    void panRight();

public:
    explicit Visualizer(const ParsedFunction* function, double xMin, double xMax,
                        unsigned pointsCount);

    void render();
};

#endif //VISUALIZATION_H