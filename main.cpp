#include <cmath>
#include "model/plot_model.h"
#include "visualization/visualization.h"

int main() {
    const int POINTS = 1000;
    Point* points = new Point[POINTS];
    for (size_t i = 0; i < POINTS; i++) {
        points[i] = Point(i, std::sin(0.1 * i * M_PI_4));
    }
    PlotData plotData(Rectangle(POINTS - 1, 2., Point(0., -1.)), points, POINTS);
    Visualizer v(&plotData);
    v.render();
    delete[] points;
}
