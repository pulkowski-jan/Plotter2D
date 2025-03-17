#include <cmath>
#include <evaluation/function_evaluator.h>
#include <parser/function_parser.h>

#include "model/plot_model.h"
#include "visualization/visualization.h"

class Funct final : public ParsedFunction {
    public:
        Funct() = default;

        ~Funct() override = default;

        double operator()(const double x) const override {
            return std::sin(x * M_PI_4);
        }
};

int main() {
    constexpr int POINTS = 1000;
    FunctionEvaluator functionEvaluator;
    auto* plotData = functionEvaluator.evaluate(Funct(), 0, 10 * M_PI, POINTS);
    const Visualizer v(plotData);
    v.render();
    delete plotData;
}
