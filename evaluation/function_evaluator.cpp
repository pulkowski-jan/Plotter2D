#include "function_evaluator.h"
#include <limits>
#include <cmath>
PlotData* FunctionEvaluator::evaluate(const ParsedFunction& function, double xMin, double xMax, unsigned pointsCount){
    if (xMin >= xMax) return nullptr;
    Point* points = new Point[pointsCount];
    double step = (xMax - xMin) / pointsCount;
    double yMax = std::numeric_limits<double>::infinity();
    double yMin = -std::numeric_limits<double>::infinity();
    unsigned tail = 0;
    for (int i = 0; i < pointsCount; ++i) {
      double x = xMin + i*step;
      double y = function(x);
      if (isfinite(y)) {
          if (yMin > y) yMin = y;
          if (yMax < y) yMax = y;
        points[tail++] = Point(x, y);}
    }
    PlotData* plotData = new PlotData(Rectangle(xMax - xMin, yMax - yMin, Point(xMin, yMin)), points, pointsCount);
    return plotData;
  }
