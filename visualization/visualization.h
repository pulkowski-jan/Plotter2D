#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <interface/plotter2d.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "evaluation/function_evaluator.h"
#include "model/plot_model.h"

class Visualizer {
    plotter2d::Options config;
    FunctionEvaluator evaluator;
    PlotData* plotData;
    double zoomFactor;
    Point zoomCenter;

    sf::RectangleShape zoomInButton{};
    sf::RectangleShape zoomOutButton{};
    sf::Text zoomInText{};
    sf::Text zoomOutText{};

    sf::RectangleShape panLeftButton{};
    sf::RectangleShape panRightButton{};
    sf::Text panLeftText{};
    sf::Text panRightText{};

    sf::RectangleShape rescaleButton{};
    sf::Text rescaleText{};

    sf::Font font{};
    double xMin_;
    double xMax_;
    unsigned pointsCount_;

    double yMin_;
    double yMax_;
    bool rescaleY_;
    bool useCustomPlotRange_;
    mutable int validPointCount_{};
    std::pair<double, double> plotRange_;

    void initializeButtons(const sf::Vector2u& windowSize);

    void updatePlotData();

    void drawGraph(sf::RenderWindow& window, const sf::Vertex* lines) const;

    static bool isMouseInButton(const sf::Vector2f& mousePosition,
                                const sf::RectangleShape& button);

    sf::Vector2f scalePoint(const Point& p, const unsigned effectiveSize[2],
                            const unsigned offset[2]) const;

    sf::Vertex* renderGraph(const sf::Vector2u& windowSize) const;

    static double calculateAxisPosition(double min, double max);

    std::vector<sf::Vertex> renderGrid(const sf::Vector2u& windowSize) const;

    std::vector<sf::Vertex> renderAxes(const sf::Vector2u& windowSize) const;

    void drawUI(sf::RenderWindow& window) const;

    static bool doublesSignificantlyDiffer(double a, double b);

    bool shouldReevaluatePlotData() const;

    void panLeft();

    void panRight();

    public:
        explicit Visualizer(std::vector<const ParsedFunction*> functions, double xMin, double xMax,
                            const plotter2d::Options& options);

        ~Visualizer();

        Visualizer& operator=(const Visualizer&) = delete;

        Visualizer(const Visualizer&) = delete;

        void render();

        static void drawVertices(sf::RenderWindow& window, const std::vector<sf::Vertex>& axes);

        void zoomIn();

        void zoomOut();

};

#endif //VISUALIZATION_H
