//
// Created by dzikimlecz on 14.03.25.
//

#ifndef FUNCTION_EVALUATOR_H
#define FUNCTION_EVALUATOR_H
#include "../parser/function_parser.h"
#include "../model/plot_model.h"


class FunctionEvaluator {
   public:
     PlotData* evaluate(const ParsedFunction& function, double xMin, double xMax, unsigned pointsCount);
};



#endif //FUNCTION_EVALUATOR_H
