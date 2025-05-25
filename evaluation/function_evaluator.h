//
// Created by dzikimlecz on 14.03.25.
//

#ifndef FUNCTION_EVALUATOR_H
#define FUNCTION_EVALUATOR_H
#include "parser/function_parser.h"
#include "model/plot_model.h"


class FunctionEvaluator {
    std::vector<const ParsedFunction*> functions;
    Point* buffer = nullptr;
    unsigned pointsPerFunction = 0;
    bool cachingEnabled;

    public:
        bool outOfBounds(double xMin, double xMax) const;

        size_t bufferSize() const;

        static Rectangle calculateBounds(const Point* windowStart, const Point* windowEnd);

        bool closeToBufferEnd(const Point* windowEnd) const;

        bool mangify(double xMin, double xMax) const;

        /**
         * @brief Evaluates cached function for pointsCount of points in the given domain
         * @param xMin The left bound of the domain
         * @param xMax The right bound of the domain
         * @param pointsCount The number of points to evaluate
         * @return Pointer to PlotData containing the results, or nullptr if xMin >= xMax
         */
        PlotData* evaluate(double xMin, double xMax, unsigned pointsCount);

        void pushFunction(const ParsedFunction* derivative);

        explicit FunctionEvaluator(const std::vector<const ParsedFunction*>& functions,
                                   bool cachingEnabled = false);

        const std::vector<const ParsedFunction*>& parsedFunctions() const;

        const std::vector<const ParsedFunction*>& parsedFunctions();

        void setFunctions(const std::vector<const ParsedFunction*>& functions);

        unsigned bufferSizeCoefficient() const;

        static Point* evaluateFunctionPoints(unsigned resolution, double xMin, double xMax, const ParsedFunction& function, Point* bufferCursor) ;

        void calculateFunctionPoints(double xMin, double xMax, unsigned resolution) const;


        static ParsedFunction* computeDerivative(const ParsedFunction& function, double xMin,
                                                 double xMax, unsigned pointsCount);
};


#endif //FUNCTION_EVALUATOR_H
