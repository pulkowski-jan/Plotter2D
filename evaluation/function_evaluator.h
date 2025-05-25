//
// Created by dzikimlecz on 14.03.25.
//

#ifndef FUNCTION_EVALUATOR_H
#define FUNCTION_EVALUATOR_H
#include <mutex>

#include "parser/function_parser.h"
#include "model/plot_model.h"


class FunctionEvaluator {
    std::vector<const ParsedFunction*> functions;
    Point* buffer = nullptr;
    unsigned pointsPerFunction = 0;
    bool cachingEnabled;
    std::mutex semaphore;

    bool outOfBounds(double xMin, double xMax) const;

    size_t bufferSize() const;

    static Rectangle calculateBounds(const Point* windowStart, const Point* windowEnd);

    bool closeToBufferEnd(const Point* windowEnd) const;

    bool rangeSmaller(double xMin, double xMax) const;

    bool closeToBufferStart(const Point* windowStart) const;

    unsigned bufferSizeCoefficient() const;

    static Point* evaluateSingleFunctionPoints(unsigned resolution, double xMin, double xMax,
                                               const ParsedFunction& function, Point* bufferCursor);

    void calculateFunctionPoints(double xMin, double xMax, unsigned resolution) const;

    public:
        /**
         * @brief Evaluates cached function for pointsCount of points in the given domain
         * @param xMin The left bound of the domain
         * @param xMax The right bound of the domain
         * @param pointsCount The number of points to evaluate
         * @return Pointer to PlotData containing the results, or nullptr if xMin >= xMax
         */
        PlotData* evaluate(double xMin, double xMax, unsigned pointsCount);

        /**
         * @brief Adds a function to the evaluator.
         * @param functionPtr function to add
         */
        void pushFunction(const ParsedFunction* functionPtr);

        /**
         * @brief Constructs a FunctionEvaluator with the given functions
         * @param functions evaluated functions
         * @param cachingEnabled whether it should cache interval a bit bigger than requested for evaluation
         */
        explicit FunctionEvaluator(const std::vector<const ParsedFunction*>& functions,
                                   bool cachingEnabled = false);

        /**
         * @return functions that are currently evaluated
         */
        const std::vector<const ParsedFunction*>& parsedFunctions() const;

        /**
         * @brief Computes the derivative of a function using finite differences.
         * @param function function to compute the derivative of
         * @param dx finite difference step size
         * @return ParsedFunction* representing the derivative of the function
         */
        static ParsedFunction* computeDerivative(const ParsedFunction* function, double dx);

        ~FunctionEvaluator();
};


#endif //FUNCTION_EVALUATOR_H
