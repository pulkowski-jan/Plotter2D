#include "function_evaluator.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <thread>

static constexpr unsigned BUFFER_SIZE_COEFFICIENT = 2;
static constexpr double MIN_CACHE_REEVALUATION_MARGIN = 0.1;

FunctionEvaluator::FunctionEvaluator(const std::vector<const ParsedFunction*>& functions,
                                     const bool cachingEnabled): functions(functions),
                                                                 cachingEnabled(cachingEnabled) { }

const std::vector<const ParsedFunction*>& FunctionEvaluator::parsedFunctions() const {
    return functions;
}

PlotData* FunctionEvaluator::evaluate(const double xMin, const double xMax,
                                      const unsigned pointsCount) {
    if (xMin >= xMax) {
        return nullptr;
    }
    semaphore.lock();
    if (pointsCount != pointsPerFunction || outOfBounds(xMin, xMax) || rangeSmaller(xMin, xMax)) {
        if (pointsCount != pointsPerFunction) {
            delete buffer;
            pointsPerFunction = pointsCount;
            buffer = new Point[bufferSize()];
        }
        calculateFunctionPoints(xMin, xMax, pointsPerFunction);
        std::cout << std::flush;
    }
    const Point* windowStart = std::lower_bound(buffer, buffer + bufferSize(), xMin,
                                                [](const Point& p, const double x) {
                                                    return p.x() < x;
                                                });
    const Point* windowEnd = std::upper_bound(buffer, buffer + bufferSize(), xMax,
                                              [](const double x, const Point& p) {
                                                  return x < p.x();
                                              });
    auto* plotData = new PlotData(calculateBounds(windowStart, windowEnd), windowStart,
                                  windowEnd - windowStart);
    semaphore.unlock();
    if (cachingEnabled && (closeToBufferStart(windowStart) || closeToBufferEnd(windowEnd))) {
        std::thread backgroundThread([this, xMin, xMax] {
            std::lock_guard lock(semaphore);
            std::cout << "background ";
            calculateFunctionPoints(xMin, xMax, pointsPerFunction);
            std::cout << "fin" << std::endl;
        });
        backgroundThread.detach();
    }
    return plotData;
}

void FunctionEvaluator::pushFunction(const ParsedFunction* derivative) {
    std::lock_guard lock(semaphore);
    const double xMin = buffer->x();
    const double xMax = buffer[bufferSize() - 1].x();
    auto* newBuffer = new Point[bufferSize() + bufferSizeCoefficient() * pointsPerFunction];
    std::copy_n(buffer, bufferSize(), newBuffer);
    delete buffer;
    buffer = newBuffer;
    auto bufferCursor = buffer + bufferSize();
    functions.push_back(derivative);
    evaluateSingleFunctionPoints(bufferSizeCoefficient() * pointsPerFunction, xMin, xMax,
                                 *derivative, bufferCursor);
    std::sort(buffer, buffer + bufferSize(), [](const Point& a, const Point& b) {
        return a.x() < b.x();
    });
}


ParsedFunction* FunctionEvaluator::computeDerivative(const ParsedFunction* function,
                                                     const double dx) {
    return new FunctionWrapper([function, dx](const double x) {
        return ((*function)(x + dx) - (*function)(x - dx)) / (2 * dx);
    });
}

FunctionEvaluator::~FunctionEvaluator() {
    delete[] buffer;
}

void FunctionEvaluator::calculateFunctionPoints(const double xMin, const double xMax,
                                                const unsigned resolution) const {
    std::cout << "cache reevaluation\n";
    const double width = xMax - xMin;
    const double margin = width * (bufferSizeCoefficient() - 1) / 2;
    Point* bufferCursor = buffer;
    for (const auto functionPtr : functions) {
        bufferCursor = evaluateSingleFunctionPoints(bufferSizeCoefficient() * resolution,
                                                    xMin - margin, xMax + margin, *functionPtr,
                                                    bufferCursor);
    }
    std::sort(buffer, buffer + bufferSize(), [](const Point& a, const Point& b) {
        return a.x() < b.x();
    });
}

Point* FunctionEvaluator::evaluateSingleFunctionPoints(const unsigned resolution, const double xMin,
                                                       const double xMax,
                                                       const ParsedFunction& function,
                                                       Point* bufferCursor) {
    unsigned tail = 0;
    const double width = xMax - xMin;
    const double step = width / (resolution - 1);
    for (int i = 0; tail < resolution; ++i) {
        const double x = xMin + i * step;
        const double y = function(x);
        if (std::isfinite(y)) {
            bufferCursor[tail++] = Point(x, y);
        }
    }
    return bufferCursor + tail;
}

size_t FunctionEvaluator::bufferSize() const {
    return bufferSizeCoefficient() * pointsPerFunction * functions.size();
}

unsigned FunctionEvaluator::bufferSizeCoefficient() const {
    return cachingEnabled ? BUFFER_SIZE_COEFFICIENT : 1;
}

bool FunctionEvaluator::outOfBounds(const double xMin, const double xMax) const {
    return xMin < buffer->x() || buffer[bufferSize() - 1].x() < xMax;
}

bool FunctionEvaluator::rangeSmaller(const double xMin, const double xMax) const {
    return (buffer[bufferSize() - 1].x() - buffer->x()) / bufferSizeCoefficient() - (xMax - xMin) >
           1e-13;
}

bool FunctionEvaluator::closeToBufferEnd(const Point* windowEnd) const {
    return bufferSize() - (windowEnd - buffer) <= static_cast<unsigned>(std::round(
               static_cast<double>(functions.size() * pointsPerFunction) *
               MIN_CACHE_REEVALUATION_MARGIN));
}

bool FunctionEvaluator::closeToBufferStart(const Point* windowStart) const {
    return windowStart - buffer <= static_cast<unsigned>(std::round(
               static_cast<double>(functions.size() * pointsPerFunction) *
               MIN_CACHE_REEVALUATION_MARGIN));
}

Rectangle FunctionEvaluator::calculateBounds(const Point* windowStart, const Point* windowEnd) {
    double xMin = windowStart->x();
    double xMax = windowStart->x();
    double yMin = windowStart->y();
    double yMax = windowStart->y();
    for (const Point* p = windowStart + 1; p < windowEnd; ++p) {
        if (p->x() < xMin) {
            xMin = p->x();
        } else if (p->x() > xMax) {
            xMax = p->x();
        }
        if (p->y() < yMin) {
            yMin = p->y();
        } else if (p->y() > yMax) {
            yMax = p->y();
        }
    }
    return {xMax - xMin, yMax - yMin, Point(xMin, yMin)};
}
