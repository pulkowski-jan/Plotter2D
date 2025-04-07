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
                       const plotter2d::Options& options) : config(options), evaluator(*function),
                                                            plotData(nullptr), zoomFactor(1.0),
                                                            xMin_(xMin), xMax_(xMax),
                                                            pointsCount_(options.resolution),
                                                            yMin_(0), yMax_(0), rescaleY_(true) {
    if (!font.loadFromFile("lato.ttf")) {
        std::cerr << "Warning: Failed to load font for buttons" << std::endl;
    }
}

void Visualizer::initializeButtons(const sf::Vector2u& windowSize) {
    constexpr unsigned BUTTONS_COUNT = 5;
    sf::RectangleShape* buttons[BUTTONS_COUNT] = {
        &zoomInButton, &zoomOutButton, &panLeftButton, &panRightButton, &rescaleButton
    };
    for (int i = 0; i < BUTTONS_COUNT; ++i) {
        buttons[i]->setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
        buttons[i]->setPosition(windowSize.x - BUTTON_WIDTH - BUTTON_PADDING,
                                i * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_PADDING);
        buttons[i]->setFillColor(sf::Color(200, 200, 200));
        buttons[i]->setOutlineThickness(2);
        buttons[i]->setOutlineColor(sf::Color::Black);
    }

    if (!font.getInfo().family.empty()) {
        zoomInText.setString("+");
        zoomOutText.setString("-");
        panLeftText.setString("<-");
        panRightText.setString("->");
        rescaleText.setString("-|-");

        sf::Text* texts[BUTTONS_COUNT] = {
            &zoomInText, &zoomOutText, &panLeftText, &panRightText, &rescaleText
        };

        for (int i = 0; i < BUTTONS_COUNT; ++i) {
            texts[i]->setFont(font);
            texts[i]->setCharacterSize(24);
            texts[i]->setFillColor(sf::Color::Black);
            texts[i]->setPosition(
                buttons[i]->getPosition().x + (BUTTON_WIDTH - texts[i]->getLocalBounds().width) / 2,
                buttons[i]->getPosition().y + (BUTTON_HEIGHT - texts[i]->getLocalBounds().height) /
                2 - 5);
        }
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
    window.draw(rescaleButton);

    if (!font.getInfo().family.empty()) {
        window.draw(zoomInText);
        window.draw(zoomOutText);
        window.draw(panLeftText);
        window.draw(panRightText);
        window.draw(rescaleText);
    }
}

bool Visualizer::shouldReevaluatePlotData() const {
    if (plotData == nullptr || rescaleY_) {
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

    std::cout << "Evaluating for: [" << xMin_ << "; " << xMax_ << "], with resolution " <<
    pointsCount_ << "\n";
    plotData = evaluator.evaluate(xMin_, xMax_, pointsCount_);

    if (rescaleY_) {
        std::cout << std::flush;
        const Rectangle& domain = plotData->domain();
        yMin_ = domain.anchor().y();
        yMax_ = domain.anchor().y() + domain.height();
        rescaleY_ = false;
    }
}

void Visualizer::render() {
    sf::RenderWindow window(sf::VideoMode({DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE}), "Plotter2D");
    if (config.drawUi) {
        initializeButtons({DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE});
    }
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (config.drawUi && event.type == sf::Event::MouseButtonPressed && event.
                       mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                if (isPointInButton(mousePos, this->zoomInButton)) {
                    double xGrowth = (xMax_ - xMin_) * ZOOM_INCREMENT;
                    double yGrowth = (yMax_ - yMin_) * ZOOM_INCREMENT;
                    xMin_ += 0.5 * xGrowth;
                    xMax_ -= 0.5 * xGrowth;
                    yMin_ += 0.5 * yGrowth;
                    yMax_ -= 0.5 * yGrowth;
                }

                if (isPointInButton(mousePos, this->zoomOutButton)) {
                    double growth = (xMax_ - xMin_) * ZOOM_INCREMENT;
                    double yGrowth = (yMax_ - yMin_) * ZOOM_INCREMENT;
                    xMin_ -= 0.5 * growth;
                    xMax_ += 0.5 * growth;
                    yMin_ -= 0.5 * yGrowth;
                    yMax_ += 0.5 * yGrowth;
                }

                if (isPointInButton(mousePos, this->panLeftButton)) {
                    panLeft();
                }

                if (isPointInButton(mousePos, this->panRightButton)) {
                    panRight();
                }

                if (isPointInButton(mousePos, this->rescaleButton)) {
                    rescaleY_ = true;
                }
            }
        }
        if (shouldReevaluatePlotData()) {
            updatePlotData();
        }

        window.clear(sf::Color::White);

        auto lines = renderGraph({DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE});
        window.draw(lines, plotData->pointsCount(), config.approximationMode ==
        plotter2d::Options::Points ? sf::Points : sf::LineStrip);
        delete[] lines;
        if (config.drawUi) {
            drawUI(window);
        }

        window.display();
    }
}

Visualizer::~Visualizer() {
    delete plotData;
}
