#include <evaluation/function_evaluator.h>
#include <parser/function_parser.h>

#include "model/plot_model.h"
#include "visualization/visualization.h"

int main() {
    FunctionParser p;
    const ParsedFunction* parsedFunction = p.parsePolishNotation("* - x / 1 2 * - x 2 * - x 3 * - x 4 - x 5");
    constexpr unsigned POINTS = 1000;
    Visualizer v(parsedFunction, 0, 6, POINTS);
    v.render();
    delete parsedFunction;
}
