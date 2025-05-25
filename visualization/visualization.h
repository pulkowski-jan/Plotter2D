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
    /*
     * BUTTONS
     */
    enum ButtonType {
        ZOOM_IN,
        ZOOM_OUT,
        PAN_LEFT,
        PAN_RIGHT,
        RESCALE,
        DERIVATIVE
    };

    class Button {
        sf::RectangleShape rectangle_;
        sf::Text text_;
        const std::function<void()>* action_ = nullptr;

        public:
            ~Button();

            Button();

            Button(const sf::RectangleShape& rectangle, const sf::Text& text);

            sf::RectangleShape& rectangle();

            const sf::RectangleShape& rectangle() const;

            sf::Text& text();

            void setAction(const std::function<void()>& action);

            void trigger() const;
    };

    std::map<ButtonType, Button> buttons{};

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

    void drawUI(sf::RenderWindow& window);

    bool doublesSignificantlyDiffer(double a, double b) const;

    bool shouldReevaluatePlotData() const;

    void panLeft();

    void panRight();

    public:
        explicit Visualizer(const std::vector<const ParsedFunction*>& functions, double xMin, double xMax,
                            const plotter2d::Options& options);

        ~Visualizer();

        Visualizer& operator=(const Visualizer&) = delete;

        void derivate();

        Visualizer(const Visualizer&) = delete;

        void render();

        static void drawVertices(sf::RenderWindow& window, const std::vector<sf::Vertex>& axes);

        void zoomIn();

        void zoomOut();

};

#endif //VISUALIZATION_H
