#include "visualization.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

static constexpr unsigned ABSOLUTE_WINDOW_SIZE = 700;
static constexpr double PADDING_SIZE[2] = {0.025, 0.01};
static constexpr double ZOOM_INCREMENT = 0.3;
static constexpr unsigned BUTTON_WIDTH = 50;
static constexpr unsigned BUTTON_HEIGHT = 30;
static constexpr unsigned BUTTON_PADDING = 10;
static constexpr double PAN_FACTOR = 0.15;
static constexpr unsigned GRID_SIZE = 10;

Visualizer::Visualizer(std::vector<const ParsedFunction*> functions, const double xMin,
                       const double xMax, const plotter2d::Options& options) : config(options),
    evaluator(functions), plotData(nullptr), zoomFactor(1.0), xMin_(xMin), xMax_(xMax),
    pointsCount_(options.resolution), yMin_(0), yMax_(0), rescaleY_(true),
    useCustomPlotRange_(options.useCustomPlotRange), validPointCount_(0),
    plotRange_(options.plotRange) {
    if (!font.loadFromFile("lato.ttf")) {
        std::cerr << "Warning: Failed to load font for buttons" << std::endl;
    }
}

Visualizer::Button::~Button() {
    delete action_;
}

Visualizer::Button::Button(): rectangle_({sf::Vector2f(0.f, 0.f)}), text_({}) { }

Visualizer::Button::Button(const sf::RectangleShape& rectangle,
                           const sf::Text& text): rectangle_(rectangle), text_(text) { }

sf::RectangleShape& Visualizer::Button::rectangle() {
    return rectangle_;
}

const sf::RectangleShape& Visualizer::Button::rectangle() const {
    return rectangle_;
}

sf::Text& Visualizer::Button::text() {
    return text_;
}

void Visualizer::Button::setAction(std::function<void()> action) {
    action_ = new std::function(action);
}

void Visualizer::Button::trigger() const {
    if (action_) {
        (*action_)();
    }
}

void Visualizer::initializeButtons(const sf::Vector2u& windowSize) {
    Button& zoomInButton = buttons[ZOOM_IN] = {};
    Button& zoomOutButton = buttons[ZOOM_OUT] = {};
    Button& panLeftButton = buttons[PAN_LEFT] = {};
    Button& panRightButton = buttons[PAN_RIGHT] = {};
    Button& rescaleButton = buttons[RESCALE] = {};
    unsigned i = 0;
    zoomInButton.setAction([this] {
        zoomIn();
    });
    zoomOutButton.setAction([this] {
        zoomOut();
    });
    panLeftButton.setAction([this] {
        panLeft();
    });
    panRightButton.setAction([this] {
        panRight();
    });
    rescaleButton.setAction([this] {
        rescaleY_ = true;
    });
    for (auto& entry : buttons) {
        auto& button = entry.second.rectangle();
        button.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
        button.setPosition(windowSize.x - BUTTON_WIDTH - BUTTON_PADDING,
                           i++ * (BUTTON_HEIGHT + BUTTON_PADDING) + BUTTON_PADDING);
        button.setFillColor(sf::Color(200, 200, 200));
        button.setOutlineThickness(2);
        button.setOutlineColor(sf::Color::Black);
    }

    if (!font.getInfo().family.empty()) {
        zoomInButton.text().setString("+");
        zoomOutButton.text().setString("-");
        panLeftButton.text().setString("<-");
        panRightButton.text().setString("->");
        rescaleButton.text().setString("-|-");

        for (auto& entry : buttons) {
            auto& text = entry.second.text();
            auto& button = entry.second.rectangle();
            text.setFont(font);
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::Black);
            text.setPosition(
                button.getPosition().x + (BUTTON_WIDTH - text.getLocalBounds().width) / 2,
                button.getPosition().y + (BUTTON_HEIGHT - text.getLocalBounds().height) / 2 - 5);
        }
    }
}

bool Visualizer::isMouseInButton(const sf::Vector2f& mousePosition,
                                 const sf::RectangleShape& button) {
    const sf::FloatRect bounds = button.getGlobalBounds();
    return bounds.contains(mousePosition);
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

    validPointCount_ = 0;

    for (int i = 0; i < plotData->pointsCount(); ++i) {
        const Point& p = plotData->points()[i];
        if (useCustomPlotRange_ && (p.y() < plotRange_.first || p.y() > plotRange_.second)) {
            continue;
        }
        sf::Vertex v(scalePoint(p, effectiveSize, offset));
        v.color = sf::Color(config.graphColor);
        line[validPointCount_++] = v;
    }

    if (validPointCount_ < plotData->pointsCount()) {
        auto* filteredLine = new sf::Vertex[validPointCount_];
        for (int i = 0; i < validPointCount_; ++i) {
            filteredLine[i] = line[i];
        }
        delete[] line;

        return filteredLine;
    }

    return line;
}

double Visualizer::calculateAxisPosition(const double min, const double max) {
    if (min <= 0 && 0 <= max) {
        return 0.0;
    }
    if (min >= 0) {
        return min;
    }
    return max;
}

std::vector<sf::Vertex> Visualizer::renderGrid(const sf::Vector2u& windowSize) const {
    const unsigned offset[2] = {
        static_cast<unsigned>(windowSize.x * PADDING_SIZE[0]),
        static_cast<unsigned>(windowSize.y * PADDING_SIZE[1])
    };
    const unsigned effectiveSize[2] = {windowSize.x - 2 * offset[0], windowSize.y - 2 * offset[1]};
    std::vector<sf::Vector2f> grid;
    grid.reserve(GRID_SIZE * 4);
    const double xStep = (xMax_ - xMin_) / (GRID_SIZE);
    const double xAxis = calculateAxisPosition(xMin_, xMax_);
    for (int i = 1; i <= GRID_SIZE; ++i) {
        const double x = xAxis == xMax_ ? xAxis - i * xStep : xAxis + i * xStep;
        grid.emplace_back(scalePoint(Point(x, yMin_), effectiveSize, offset));
        grid.emplace_back(scalePoint(Point(x, yMax_), effectiveSize, offset));
    }
    for (int i = 1; i <= GRID_SIZE; ++i) {
        const double x = xAxis == 0.
                             ? xAxis - i * xStep
                             : xAxis == xMin_
                                   ? xAxis + (GRID_SIZE + i) * xStep
                                   : xAxis - (GRID_SIZE + i) * xStep;
        grid.emplace_back(scalePoint(Point(x, yMin_), effectiveSize, offset));
        grid.emplace_back(scalePoint(Point(x, yMax_), effectiveSize, offset));
    }
    const double yStep = (yMax_ - yMin_) / (GRID_SIZE);
    const double yAxis = calculateAxisPosition(yMin_, yMax_);
    for (int i = 1; i <= GRID_SIZE; ++i) {
        const double y = yAxis == yMax_ ? yAxis - i * yStep : yAxis + i * yStep;
        grid.emplace_back(scalePoint(Point(xMin_, y), effectiveSize, offset));
        grid.emplace_back(scalePoint(Point(xMax_, y), effectiveSize, offset));
    }
    for (int i = 1; i <= GRID_SIZE; ++i) {
        const double y = yAxis == 0.
                             ? yAxis - i * yStep
                             : yAxis == yMin_
                                   ? yAxis + (GRID_SIZE + i) * yStep
                                   : yAxis - (GRID_SIZE + i) * yStep;
        grid.emplace_back(scalePoint(Point(xMin_, y), effectiveSize, offset));
        grid.emplace_back(scalePoint(Point(xMax_, y), effectiveSize, offset));
    }
    std::vector<sf::Vertex> result;
    result.reserve(grid.size());
    std::transform(grid.begin(), grid.end(), std::back_inserter(result),
                   [](const sf::Vector2f& pos) {
                       return sf::Vertex(pos, sf::Color(0xAAAAAAFF));
                   });
    return result;
}

std::vector<sf::Vertex> Visualizer::renderAxes(const sf::Vector2u& windowSize) const {
    const unsigned offset[2] = {
        static_cast<unsigned>(windowSize.x * PADDING_SIZE[0]),
        static_cast<unsigned>(windowSize.y * PADDING_SIZE[1])
    };
    const unsigned effectiveSize[2] = {windowSize.x - 2 * offset[0], windowSize.y - 2 * offset[1]};
    const double xAxisPosY = calculateAxisPosition(yMin_, yMax_);
    const double yAxisPosX = calculateAxisPosition(xMin_, xMax_);
    std::vector<sf::Vector2f> vectors;
    vectors.push_back(scalePoint(Point(xMin_, xAxisPosY), effectiveSize, offset));
    vectors.push_back(scalePoint(Point(xMax_, xAxisPosY), effectiveSize, offset));
    vectors.push_back(scalePoint(Point(yAxisPosX, yMin_), effectiveSize, offset));
    vectors.push_back(scalePoint(Point(yAxisPosX, yMax_), effectiveSize, offset));
    vectors.emplace_back(vectors[1]);
    vectors.emplace_back(vectors[1].x - effectiveSize[0] * 0.01,
                         vectors[1].y + effectiveSize[1] * 0.005);
    vectors.emplace_back(vectors[1]);
    vectors.emplace_back(vectors[1].x - effectiveSize[0] * 0.01,
                         vectors[1].y - effectiveSize[1] * 0.005);
    vectors.emplace_back(vectors[3]);
    vectors.emplace_back(vectors[3].x - effectiveSize[0] * 0.005,
                         vectors[3].y + effectiveSize[1] * 0.01);
    vectors.emplace_back(vectors[3]);
    vectors.emplace_back(vectors[3].x + effectiveSize[0] * 0.005,
                         vectors[3].y + effectiveSize[1] * 0.01);

    std::vector<sf::Vertex> result;
    result.reserve(vectors.size());
    std::transform(vectors.begin(), vectors.end(), std::back_inserter(result),
                   [](const sf::Vector2f& pos) {
                       return sf::Vertex(pos, sf::Color::Black);
                   });
    return result;
}

void Visualizer::drawUI(sf::RenderWindow& window) {
    for (auto& entry : buttons) {
        window.draw(entry.second.rectangle());
        if (!font.getInfo().family.empty()) {
            window.draw(entry.second.text());
        }
    }
}

bool Visualizer::doublesSignificantlyDiffer(const double a, const double b) {
    return std::abs(a - b) >= 1e-14;
}

bool Visualizer::shouldReevaluatePlotData() const {
    if (plotData == nullptr || rescaleY_) {
        return true;
    }
    const Rectangle& domain = plotData->domain();
    const double xAnchor = domain.anchor().x();
    return plotData->pointsCount() != pointsCount_ || doublesSignificantlyDiffer(xAnchor, xMin_) ||
           doublesSignificantlyDiffer(xAnchor + domain.width(), xMax_);
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
        if (useCustomPlotRange_) {
            yMin_ = plotRange_.first;
            yMax_ = plotRange_.second;
        } else {
            yMin_ = domain.anchor().y();
            yMax_ = domain.anchor().y() + domain.height();
        }
        rescaleY_ = false;
    }
}

void Visualizer::drawGraph(sf::RenderWindow& window, const sf::Vertex* lines) const {
    window.draw(lines, validPointCount_, config.approximationMode == plotter2d::Options::Points
                                             ? sf::Points
                                             : sf::LineStrip);
}

void Visualizer::drawVertices(sf::RenderWindow& window, const std::vector<sf::Vertex>& axes) {
    window.draw(axes.data(), axes.size(), sf::Lines);
}

void Visualizer::zoomIn() {
    double xGrowth = (xMax_ - xMin_) * ZOOM_INCREMENT;
    double yGrowth = (yMax_ - yMin_) * ZOOM_INCREMENT;
    xMin_ += 0.5 * xGrowth;
    xMax_ -= 0.5 * xGrowth;
    yMin_ += 0.5 * yGrowth;
    yMax_ -= 0.5 * yGrowth;
}

void Visualizer::zoomOut() {
    double growth = (xMax_ - xMin_) * ZOOM_INCREMENT;
    double yGrowth = (yMax_ - yMin_) * ZOOM_INCREMENT;
    xMin_ -= 0.5 * growth;
    xMax_ += 0.5 * growth;
    yMin_ -= 0.5 * yGrowth;
    yMax_ += 0.5 * yGrowth;
}

sf::Vector2f scaleMousePositionToAbsolute(int x, int y, sf::Vector2u windowSize) {
    return {
        static_cast<float>(x) * (static_cast<float>(ABSOLUTE_WINDOW_SIZE) / static_cast<float>(
                                     windowSize.x)),
        static_cast<float>(y) * (static_cast<float>(ABSOLUTE_WINDOW_SIZE) / static_cast<float>(
                                     windowSize.y))
    };
}

void Visualizer::render() {
    sf::RenderWindow window(sf::VideoMode({ABSOLUTE_WINDOW_SIZE, ABSOLUTE_WINDOW_SIZE}),
                            "Plotter2D");
    if (config.drawUi) {
        initializeButtons({ABSOLUTE_WINDOW_SIZE, ABSOLUTE_WINDOW_SIZE});
    }
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (config.drawUi && event.type == sf::Event::MouseButtonPressed && event.
                       mouseButton.button == sf::Mouse::Left) {
                const auto mousePos = scaleMousePositionToAbsolute(
                    event.mouseButton.x, event.mouseButton.y, window.getSize());
                std::cout << "Mouse Event received at: (" << event.mouseButton.x << ", " << event.
                        mouseButton.y << "), scaled to (" << mousePos.x << ", " << mousePos.y <<
                        ")\n";

                auto triggered = std::find_if(buttons.cbegin(), buttons.cend(),
                                              [&mousePos](auto& entry) {
                                                  return isMouseInButton(
                                                      mousePos, entry.second.rectangle());
                                              });
                if (triggered != buttons.cend()) {
                    std::cout << "Button " << triggered->first << " triggered\n";
                    triggered->second.trigger();
                }
            }
        }
        if (shouldReevaluatePlotData()) {
            updatePlotData();
        }

        window.clear(sf::Color::White);

        if (config.drawAxes) {
            auto grid = renderGrid({ABSOLUTE_WINDOW_SIZE, ABSOLUTE_WINDOW_SIZE});
            drawVertices(window, grid);
        }
        if (config.drawAxes) {
            auto axes = renderAxes({ABSOLUTE_WINDOW_SIZE, ABSOLUTE_WINDOW_SIZE});
            drawVertices(window, axes);
        }
        auto graph = renderGraph({ABSOLUTE_WINDOW_SIZE, ABSOLUTE_WINDOW_SIZE});
        drawGraph(window, graph);
        delete[] graph;
        if (config.drawUi) {
            drawUI(window);
        }

        window.display();
    }
}

Visualizer::~Visualizer() {
    delete plotData;
}
