//
// Created by dzikimlecz on 14.03.25.
//

#ifndef FUNCTION_EVALUATOR_H
#define FUNCTION_EVALUATOR_H
#include "parser/function_parser.h"
#include "model/plot_model.h"


class FunctionEvaluator {
    std::vector<const ParsedFunction*> functions;

    public:
        /**
         * @brief Evaluates cached function for pointsCount of points in the given domain
         * @param xMin The left bound of the domain
         * @param xMax The right bound of the domain
         * @param pointsCount The number of points to evaluate
         * @return Pointer to PlotData containing the results, or nullptr if xMin >= xMax
         */
        PlotData* evaluate(double xMin, double xMax, unsigned pointsCount) const;

        explicit FunctionEvaluator(std::vector<const ParsedFunction*>& functions);

        const std::vector<const ParsedFunction*>& parsedFunctions() const;

        void setFunctions(const std::vector<const ParsedFunction*>& functions);
};


#endif //FUNCTION_EVALUATOR_H
