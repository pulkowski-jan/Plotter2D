#include <cmath>
#include "model/plot_model.h"
#include "visualization/visualization.h"

int main() {
    const int POINTS = 100;
    Point* points = new Point[POINTS];
    for (size_t i = 0; i < POINTS; i++) {
        points[i] = Point(i, std::sin(i * M_PI_4));
    }
    PlotData plotData(Rectangle(11., 11., Point(0., 0.)), points, POINTS);
    Visualizer v(&plotData);
    v.render();
    delete[] points;
}
