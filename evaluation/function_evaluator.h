//
// Created by dzikimlecz on 14.03.25.
//

#ifndef FUNCTION_EVALUATOR_H
#define FUNCTION_EVALUATOR_H
#include "parser/function_parser.h"
#include "model/plot_model.h"


class FunctionEvaluator {
   public:
/**
 * @brief Evaluates the function for pointsCount of points in the given domain
 * @param function The function to evaluate
 * @param xMin The left bound of the domain
 * @param xMax The right bound of the domain
 * @param pointsCount The number of points to evaluate
 * @return Pointer to PlotData containing the results, or nullptr if xMin >= xMax
 */
     PlotData* evaluate(const ParsedFunction& function, double xMin, double xMax, unsigned pointsCount);
};



#endif //FUNCTION_EVALUATOR_H
