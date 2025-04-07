#include "visualization.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

static constexpr unsigned DEFAULT_WINDOW_SIZE = 1000;
static constexpr double PADDING_SIZE[2] = {0.025, 0.01};
static constexpr double ZOOM_INCREMENT = 0.3;
static constexpr unsigned BUTTON_WIDTH = 50;
static constexpr unsigned BUTTON_HEIGHT = 30;
static constexpr unsigned BUTTON_PADDING = 10;
static constexpr double PAN_FACTOR = 0.15;

Visualizer::Visualizer(const ParsedFunction* function, const double xMin, const double xMax,
                       const unsigned pointsCount) : evaluator(*function), plotData(nullptr),
                                                     zoomFactor(1.0), xMin_(xMin), xMax_(xMax),
                                                     pointsCount_(pointsCount), yMin_(0), yMax_(0),
                                                     initialPlot_(true) {
    if (!font.loadFromFile("lato.ttf")) {
        std::cerr << "Warning: Failed to load font for buttons" << std::endl;
    }
}

void Visualizer::initializeButtons(const sf::Vector2u& windowSize) {
    zoomInButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    zoomInButton.setPosition(windowSize.x - BUTTON_WIDTH - BUTTON_PADDING, BUTTON_PADDING);
    zoomInButton.setFillColor(sf::Color(200, 200, 200));
    zoomInButton.setOutlineThickness(2);
    zoomInButton.setOutlineColor(sf::Color::Black);

    zoomOutButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    zoomOutButton.setPosition(windowSize.x - BUTTON_WIDTH - BUTTON_PADDING,
                              BUTTON_HEIGHT + 2 * BUTTON_PADDING);
    zoomOutButton.setFillColor(sf::Color(200, 200, 200));
    zoomOutButton.setOutlineThickness(2);
    zoomOutButton.setOutlineColor(sf::Color::Black);

    panLeftButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    panLeftButton.setPosition(windowSize.x - BUTTON_WIDTH - BUTTON_PADDING,
                              2 * BUTTON_HEIGHT + 3 * BUTTON_PADDING);
    panLeftButton.setFillColor(sf::Color(200, 200, 200));
    panLeftButton.setOutlineThickness(2);
    panLeftButton.setOutlineColor(sf::Color::Black);

    panRightButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    panRightButton.setPosition(windowSize.x - BUTTON_WIDTH - BUTTON_PADDING,
                               3 * BUTTON_HEIGHT + 4 * BUTTON_PADDING);
    panRightButton.setFillColor(sf::Color(200, 200, 200));
    panRightButton.setOutlineThickness(2);
    panRightButton.setOutlineColor(sf::Color::Black);

    if (!font.getInfo().family.empty()) {
        zoomInText.setFont(font);
        zoomInText.setString("+");
        zoomInText.setCharacterSize(24);
        zoomInText.setFillColor(sf::Color::Black);
        zoomInText.setPosition(
            zoomInButton.getPosition().x + (BUTTON_WIDTH - zoomInText.getLocalBounds().width) / 2,
            zoomInButton.getPosition().y + (BUTTON_HEIGHT - zoomInText.getLocalBounds().height) / 2
            - 5);

        zoomOutText.setFont(font);
        zoomOutText.setString("-");
        zoomOutText.setCharacterSize(24);
        zoomOutText.setFillColor(sf::Color::Black);
        zoomOutText.setPosition(
            zoomOutButton.getPosition().x + (BUTTON_WIDTH - zoomOutText.getLocalBounds().width) / 2,
            zoomOutButton.getPosition().y + (BUTTON_HEIGHT - zoomOutText.getLocalBounds().height) /
            2 - 5);

        panLeftText.setFont(font);
        panLeftText.setString("<-");
        panLeftText.setCharacterSize(24);
        panLeftText.setFillColor(sf::Color::Black);
        panLeftText.setPosition(
            panLeftButton.getPosition().x + (BUTTON_WIDTH - panLeftText.getLocalBounds().width) / 2,
            panLeftButton.getPosition().y + (BUTTON_HEIGHT - panLeftText.getLocalBounds().height) /
            2 - 5);

        panRightText.setFont(font);
        panRightText.setString("->");
        panRightText.setCharacterSize(24);
        panRightText.setFillColor(sf::Color::Black);
        panRightText.setPosition(
            panRightButton.getPosition().x + (BUTTON_WIDTH - panRightText.getLocalBounds().width) /
            2, panRightButton.getPosition().y + (
                   BUTTON_HEIGHT - panRightText.getLocalBounds().height) / 2 - 5);
    }
}

bool Visualizer::isPointInButton(const sf::Vector2f& point, const sf::RectangleShape& button) {
    const sf::FloatRect bounds = button.getGlobalBounds();
    return bounds.contains(point);
}

sf::Vector2f Visualizer::scalePoint(const Point& p, const unsigned effectiveSize[2],
                                    const unsigned offset[2]) const {
    const double effectiveWidth = xMax_ - xMin_;

    const double effectiveHeight = yMax_ - yMin_;

    double centerX = (xMin_ + xMax_) / 2;
    double centerY = (yMin_ + yMax_) / 2;

    const double effectiveAnchorX = centerX - effectiveWidth / 2;
    const double effectiveAnchorY = centerY - effectiveHeight / 2;

    auto x = (p.x() - effectiveAnchorX) / effectiveWidth * effectiveSize[0];
    auto y = (p.y() - effectiveAnchorY) / effectiveHeight * effectiveSize[1];

    y = effectiveSize[1] - y;

    x += offset[0];
    y += offset[1];

    return {static_cast<float>(x), static_cast<float>(y)};
}

sf::Vertex* Visualizer::renderGraph(const sf::Vector2u& windowSize) const {
    const unsigned offset[2] = {
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

void Visualizer::drawUI(sf::RenderWindow& window) const {
    window.draw(zoomInButton);
    window.draw(zoomOutButton);

    window.draw(panLeftButton);
    window.draw(panRightButton);

    if (!font.getInfo().family.empty()) {
        window.draw(zoomInText);
        window.draw(zoomOutText);
        window.draw(panLeftText);
        window.draw(panRightText);
    }
}

bool Visualizer::shouldReevaluatePlotData() const {
    if (plotData == nullptr) {
        return true;
    }
    const Rectangle& domain = plotData->domain();
    const double xAnchor = domain.anchor().x();
    return plotData->pointsCount() != pointsCount_ || xAnchor != xMin_ || xAnchor + domain.width()
           != xMax_;
}

void Visualizer::panLeft() {
    const double domainWidth = xMax_ - xMin_;
    double panAmount = domainWidth * PAN_FACTOR;
    xMin_ -= panAmount;
    xMax_ -= panAmount;
}

void Visualizer::panRight() {
    const double domainWidth = xMax_ - xMin_;
    double panAmount = domainWidth * PAN_FACTOR;
    xMin_ += panAmount;
    xMax_ += panAmount;
}

void Visualizer::updatePlotData() {
    delete plotData;

    plotData = evaluator.evaluate(xMin_, xMax_, pointsCount_);

    if (initialPlot_) {
        const Rectangle& domain = plotData->domain();
        yMin_ = domain.anchor().y();
        yMax_ = domain.anchor().y() + domain.height();
        initialPlot_ = false;
    }
}

void Visualizer::render() {
    sf::RenderWindow window(sf::VideoMode({DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE}), "Plotter2D");
    initializeButtons({DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE});
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button ==
                       sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                if (isPointInButton(mousePos, this->zoomInButton)) {
                    double growth = (xMax_ - xMin_) * ZOOM_INCREMENT;
                    xMin_ += 0.5 * growth;
                    xMax_ -= 0.5 * growth;
                }

                if (isPointInButton(mousePos, this->zoomOutButton)) {
                    double growth = (xMax_ - xMin_) * ZOOM_INCREMENT;
                    xMin_ -= 0.5 * growth;
                    xMax_ += 0.5 * growth;
                }

                if (isPointInButton(mousePos, this->panLeftButton)) {
                    panLeft();
                }

                if (isPointInButton(mousePos, this->panRightButton)) {
                    panRight();
                }
            }
            if (shouldReevaluatePlotData()) {
                updatePlotData();
            }
        }

        window.clear(sf::Color::White);

        auto lines = renderGraph({DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE});
        window.draw(lines, plotData->pointsCount(), sf::LineStrip);
        delete[] lines;

        drawUI(window);

        window.display();
    }
}
