#include "plotter2d.h"
#include "parser/parsed_function.h"
#include "visualization/visualization.h"

class FunctionFromLambda final : public ParsedFunction {
    std::function<double(double)> func_;

    public:
        explicit FunctionFromLambda(const std::function<double(double)>& func) : func_(func) { }

        double operator()(const double x) const override {
            return func_(x);
        }

};

void plotter2d::plot(const std::function<double(double)>& func,
                     const std::pair<double, double>& domain, const Options options) {
    const FunctionFromLambda function(func);
    Visualizer visualizer(&function, domain.first, domain.second, options);
    visualizer.render();
}

void plotter2d::plotFromPolishNotation(const std::string& polishNotation,
                                       const std::pair<double, double>& domain,
                                       const Options options) {
    FunctionParser parser;
    const ParsedFunction* parsedFunction = parser.parsePolishNotation(polishNotation);
    Visualizer visualizer(parsedFunction, domain.first, domain.second, options);
    visualizer.render();
    delete parsedFunction;
}
