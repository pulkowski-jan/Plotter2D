#include "function_evaluator.h"
#include <cmath>
#include <limits>

PlotData* FunctionEvaluator::evaluate(const ParsedFunction& function, const double xMin,
                                      const double xMax, const unsigned pointsCount) {
    if (xMin >= xMax) return nullptr;
    auto* points = new Point[pointsCount];
    const double step = (xMax - xMin) / pointsCount;
    double yMax = std::numeric_limits<double>::infinity();
    double yMin = -std::numeric_limits<double>::infinity();
    unsigned tail = 0;
    for (int i = 0; i < pointsCount; ++i) {
        const double x = xMin + i * step;
        const double y = function(x);
        if (std::isfinite(y)) {
            if (yMin > y) yMin = y;
            if (yMax < y) yMax = y;
            points[tail++] = Point(x, y);
        }
    }
    return new PlotData(Rectangle(xMax - xMin, yMax - yMin, Point(xMin, yMin)), points,
                        pointsCount);
}
