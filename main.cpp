#include <evaluation/function_evaluator.h>
#include <parser/function_parser.h>

#include "model/plot_model.h"
#include "visualization/visualization.h"

int main() {
    FunctionParser p;
    const ParsedFunction* parsedFunction = p.parsePolishNotation("* - x / 1 2 * - x 2 * - x 3 * - x 4 - x 5");
    constexpr int POINTS = 1000;
    FunctionEvaluator functionEvaluator;
    auto* plotData = functionEvaluator.evaluate(*parsedFunction, 0, 5.5, POINTS);
    delete parsedFunction;
    Visualizer v(plotData);
    v.render();
    delete plotData;
}
