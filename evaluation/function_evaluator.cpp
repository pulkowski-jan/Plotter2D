#include "function_evaluator.h"
#include <cfloat>
#include <cmath>
#include <limits>

FunctionEvaluator::FunctionEvaluator(std::vector<const ParsedFunction*>& functions) : functions(
    functions) { }

const std::vector<const ParsedFunction*>& FunctionEvaluator::parsedFunctions() const {
    return functions;
}

std::vector<const ParsedFunction*>& FunctionEvaluator::parsedFunctions() {
    return functions;
}

void FunctionEvaluator::setFunctions(const std::vector<const ParsedFunction*>& functions) {
    this->functions = functions;
}

PlotData* FunctionEvaluator::evaluate(const double xMin, const double xMax,
                                      const unsigned pointsCount) const {
    if (xMin >= xMax) return nullptr;
    auto* points = new Point[pointsCount * functions.size()];
    const double step = (xMax - xMin) / pointsCount;
    double yMax = -std::numeric_limits<double>::infinity();
    double yMin = std::numeric_limits<double>::infinity();
    unsigned tail = 0;
    for (const auto functionPtr : functions) {
        const auto& function = *functionPtr;
        for (int i = 0; i < pointsCount; ++i) {
            const double x = xMin + i * step;
            const double y = function(x);
            if (std::isfinite(y)) {
                if (yMin > y) {
                    yMin = y;
                }
                if (yMax < y) {
                    yMax = y;
                }
                points[tail++] = Point(x, y);
            }
        }
    }
    return new PlotData(
        Rectangle(xMax - xMin, std::max(yMax - yMin, DBL_TRUE_MIN), Point(xMin, yMin)), points,
        tail);
}

ParsedFunction* FunctionEvaluator::derivate(const ParsedFunction& function, const double xMin,
                                            const double xMax, const unsigned pointsCount) {
    return new FunctionWrapper([xMin, xMax, &function, pointsCount](const double x) {
        const auto dx = (xMax - xMin) / pointsCount;
        const auto left = std::max(x - dx, xMin);
        const auto right = std::min(x + dx, xMax);
        return (function(right) - function(left)) / (right - left);
    });
}
