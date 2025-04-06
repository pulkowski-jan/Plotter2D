//
// Created by dzikimlecz on 14.03.25.
//

#ifndef FUNCTION_EVALUATOR_H
#define FUNCTION_EVALUATOR_H
#include "parser/function_parser.h"
#include "model/plot_model.h"


class FunctionEvaluator {
    const ParsedFunction* function;

    public:
        /**
         * @brief Evaluates the function for pointsCount of points in the given domain
         * @param fun The function to cache as evaluated
         * @param xMin The left bound of the domain
         * @param xMax The right bound of the domain
         * @param pointsCount The number of points to evaluate
         * @return Pointer to PlotData containing the results, or nullptr if xMin >= xMax
         */
        PlotData* evaluateGivenFunction(const ParsedFunction& fun, double xMin, double xMax,
                                        unsigned pointsCount);

        /**
         * @brief Evaluates cached function for pointsCount of points in the given domain
         * @param xMin The left bound of the domain
         * @param xMax The right bound of the domain
         * @param pointsCount The number of points to evaluate
         * @return Pointer to PlotData containing the results, or nullptr if xMin >= xMax
         */
        PlotData* evaluate(double xMin, double xMax, unsigned pointsCount) const;

        explicit FunctionEvaluator(const ParsedFunction& function);

        const ParsedFunction& parsedFunction() const;

        void setFunction(const ParsedFunction& fun);
};


#endif //FUNCTION_EVALUATOR_H
