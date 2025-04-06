#include "function_evaluator.h"
#include <cfloat>
#include <cmath>
#include <limits>

PlotData* FunctionEvaluator::evaluateGivenFunction(const ParsedFunction& fun, const double xMin,
                                                   const double xMax, const unsigned pointsCount) {
    setFunction(fun);
    return evaluate(xMin, xMax, pointsCount);
}

FunctionEvaluator::FunctionEvaluator(const ParsedFunction& function) : function(&function) { }

const ParsedFunction& FunctionEvaluator::parsedFunction() const {
    return *function;
}

void FunctionEvaluator::setFunction(const ParsedFunction& fun) {
    this->function = &fun;
}

PlotData* FunctionEvaluator::evaluate(const double xMin, const double xMax,
                                      const unsigned pointsCount) const {
    if (xMin >= xMax) return nullptr;
    auto* points = new Point[pointsCount];
    const double step = (xMax - xMin) / pointsCount;
    double yMax = -std::numeric_limits<double>::infinity();
    double yMin = std::numeric_limits<double>::infinity();
    unsigned tail = 0;
    for (int i = 0; i < pointsCount; ++i) {
        const double x = xMin + i * step;
        const double y = (*function)(x);
        if (std::isfinite(y)) {
            if (yMin > y) yMin = y;
            if (yMax < y) yMax = y;
            points[tail++] = Point(x, y);
        }
    }
    return new PlotData(
        Rectangle(xMax - xMin, std::max(yMax - yMin, DBL_TRUE_MIN), Point(xMin, yMin)), points,
        tail);
}
