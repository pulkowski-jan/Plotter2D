#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "model/plot_model.h"

class Visualizer {
    PlotData* plotData;

    sf::Vector2f scalePoint(const Point& p, const unsigned effectiveSize[2],
                                    const unsigned offset[2]) const;

    sf::Vertex* renderGraph(const sf::Vector2u& windowSize) const;

    public:
        explicit Visualizer(PlotData*);


        void render() const;
};

#endif //VISUALIZATION_H
