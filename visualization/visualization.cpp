#include "visualization.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

static constexpr unsigned DEFAULT_WINDOW_SIZE = 1000;
static constexpr double PADDING_SIZE[2] = {0.025, 0.01};
static constexpr double ZOOM_INCREMENT = 0.2; // 20% zoom in/out per click
static constexpr double MIN_ZOOM = 0.2;
static constexpr double MAX_ZOOM = 5.0;
static constexpr unsigned BUTTON_WIDTH = 50;
static constexpr unsigned BUTTON_HEIGHT = 30;
static constexpr unsigned BUTTON_PADDING = 10;

Visualizer::Visualizer(PlotData* plotData) : plotData(plotData), zoomFactor(1.0) {
    // Set initial zoom center to the center of the domain
    const Rectangle& domain = plotData->domain();
    zoomCenter = Point(
        domain.anchor().x() + domain.width() / 2,
        domain.anchor().y() + domain.height() / 2
    );

    // Try to load font - not necessary but improves button appearance
    if (!font.loadFromFile("arial.ttf")) {
        // Font loading failed - could provide a fallback or just continue without text
        std::cerr << "Warning: Failed to load font for zoom buttons" << std::endl;
    }
}

void Visualizer::initializeButtons(const sf::Vector2u& windowSize) {
    // Create zoom in button
    zoomInButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    zoomInButton.setPosition(windowSize.x - BUTTON_WIDTH - BUTTON_PADDING, BUTTON_PADDING);
    zoomInButton.setFillColor(sf::Color(200, 200, 200));
    zoomInButton.setOutlineThickness(2);
    zoomInButton.setOutlineColor(sf::Color::Black);

    // Create zoom out button
    zoomOutButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    zoomOutButton.setPosition(windowSize.x - BUTTON_WIDTH - BUTTON_PADDING,
                             BUTTON_HEIGHT + 2 * BUTTON_PADDING);
    zoomOutButton.setFillColor(sf::Color(200, 200, 200));
    zoomOutButton.setOutlineThickness(2);
    zoomOutButton.setOutlineColor(sf::Color::Black);

    // Set up button texts if font was loaded
    if (font.getInfo().family != "") {
        zoomInText.setFont(font);
        zoomInText.setString("+");
        zoomInText.setCharacterSize(24);
        zoomInText.setFillColor(sf::Color::Black);
        zoomInText.setPosition(
            zoomInButton.getPosition().x + (BUTTON_WIDTH - zoomInText.getLocalBounds().width) / 2,
            zoomInButton.getPosition().y + (BUTTON_HEIGHT - zoomInText.getLocalBounds().height) / 2 - 5
        );

        zoomOutText.setFont(font);
        zoomOutText.setString("-");
        zoomOutText.setCharacterSize(24);
        zoomOutText.setFillColor(sf::Color::Black);
        zoomOutText.setPosition(
            zoomOutButton.getPosition().x + (BUTTON_WIDTH - zoomOutText.getLocalBounds().width) / 2,
            zoomOutButton.getPosition().y + (BUTTON_HEIGHT - zoomOutText.getLocalBounds().height) / 2 - 5
        );
    }
}

bool Visualizer::isPointInButton(const sf::Vector2f& point, const sf::RectangleShape& button) const {
    sf::FloatRect bounds = button.getGlobalBounds();
    return bounds.contains(point);
}

sf::Vector2f Visualizer::scalePoint(const Point& p, const unsigned effectiveSize[2],
                                  const unsigned offset[2]) {
    const Rectangle& domain = plotData->domain();

    // Calculate the effective domain dimensions after zooming
    double effectiveWidth = domain.width() / zoomFactor;
    double effectiveHeight = domain.height() / zoomFactor;

    // Calculate the effective domain anchor point that keeps zoomCenter centered
    double effectiveAnchorX = zoomCenter.x() - effectiveWidth / 2;
    double effectiveAnchorY = zoomCenter.y() - effectiveHeight / 2;

    // Scale the point to the window size
    auto x = (p.x() - effectiveAnchorX) / effectiveWidth * effectiveSize[0];
    auto y = (p.y() - effectiveAnchorY) / effectiveHeight * effectiveSize[1];

    // Flip the y axis
    y = effectiveSize[1] - y;

    // Apply the offset
    x += offset[0];
    y += offset[1];

    return {static_cast<float>(x), static_cast<float>(y)};
}

sf::Vertex* Visualizer::renderGraph(const sf::Vector2u& windowSize)  {
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

void Visualizer::drawUI(sf::RenderWindow& window) const {
    // Draw the buttons
    window.draw(zoomInButton);
    window.draw(zoomOutButton);

    // Draw the button texts if font was loaded
    if (font.getInfo().family != "") {
        window.draw(zoomInText);
        window.draw(zoomOutText);
    }
}

void Visualizer::render() {
    sf::RenderWindow window(sf::VideoMode({DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE}), "Plotter2D");

    // Create a non-const copy of this for modifying zoom

    // Initialize the buttons
    initializeButtons({DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE});

    std::cout << "Zoom in button at: " << this->zoomInButton.getPosition().x << ", "
          << this->zoomInButton.getPosition().y << " with size: "
          << this->zoomInButton.getSize().x << "x" << this->zoomInButton.getSize().y << std::endl;

    std::cout << "Zoom out button at: " << this->zoomOutButton.getPosition().x << ", "
              << this->zoomOutButton.getPosition().y << " with size: "
              << this->zoomOutButton.getSize().x << "x" << this->zoomOutButton.getSize().y << std::endl;


    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed &&
                     event.mouseButton.button == sf::Mouse::Left) {
                std::cout << "Mouse button pressed event detected" << std::endl;
                // Get mouse position
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                std::cout << "Mouse clicked at: " << mousePos.x << ", " << mousePos.y << std::endl;

                bool inZoomIn = this->isPointInButton(mousePos, this->zoomInButton);
                std::cout << "Is in zoom in button: " << (inZoomIn ? "yes" : "no") << std::endl;
                std::cout << "Current zoom factor: " << this->zoomFactor << ", MAX_ZOOM: " << MAX_ZOOM << std::endl;


                // Check if zoom in button was clicked
                if (this->isPointInButton(mousePos, this->zoomInButton)) {
                    if (this->zoomFactor < MAX_ZOOM) {
                        this->zoomFactor += ZOOM_INCREMENT;
                        std::cout << "New zoom factor: " << this->zoomFactor << std::endl;
                    }
                }

                // Check if zoom out button was clicked
                if (this->isPointInButton(mousePos, this->zoomOutButton)) {
                    if (this->zoomFactor > MIN_ZOOM + ZOOM_INCREMENT) {
                        this->zoomFactor -= ZOOM_INCREMENT;
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        // Draw the graph
        auto lines = renderGraph({DEFAULT_WINDOW_SIZE, DEFAULT_WINDOW_SIZE});
        window.draw(lines, plotData->pointsCount(), sf::LineStrip);
        delete[] lines;

        // Draw the UI elements
        drawUI(window);

        window.display();
    }
}