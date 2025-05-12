#include "plotter2d.h"
#include "parser/parsed_function.h"
#include "visualization/visualization.h"

void plotter2d::plot(const std::function<double(double)>& func,
                     const std::pair<double, double>& domain, const Options& options) {
    const FunctionWrapper function(func);
    Visualizer visualizer({&function}, domain.first, domain.second, options);
    visualizer.render();
}

void plotter2d::plot(const std::vector<std::function<double(double)>>& funcs,
                     const std::pair<double, double>& domain, const Options& options) {
    std::vector<const ParsedFunction*> functions;
    std::vector<FunctionWrapper> functionObjects;

    for (const auto& func : funcs) {
        functionObjects.emplace_back(func);
    }

    for (const auto& func : functionObjects) {
        functions.push_back(&func);
    }

    Visualizer visualizer(functions, domain.first, domain.second, options);
    visualizer.render();
}


void plotter2d::plotFromPolishNotation(const std::string& polishNotation,
                                       const std::pair<double, double>& domain,
                                       const Options& options) {
    FunctionParser parser;
    const ParsedFunction* parsedFunction = parser.parsePolishNotation(polishNotation);
    Visualizer visualizer({parsedFunction}, domain.first, domain.second, options);
    visualizer.render();
    delete parsedFunction;
}

void plotter2d::plotFromPolishNotation(const std::vector<std::string>& polishNotations,
                                      const std::pair<double, double>& domain,
                                      const Options& options) {
    FunctionParser parser;
    std::vector<const ParsedFunction*> parsedFunctions;

    for (const auto& notation : polishNotations) {
        const ParsedFunction* parsedFunction = parser.parsePolishNotation(notation);
        parsedFunctions.push_back(parsedFunction);
    }

    Visualizer visualizer(parsedFunctions, domain.first, domain.second, options);
    visualizer.render();

    for (const auto* func : parsedFunctions) {
        delete func;
    }
}



plotter2d::Options::Options(): drawUi(true), drawAxes(true), drawGrid(true),
                               approximationMode(POINTS), resolution(5000), plotRange({}),
                               useCustomPlotRange(false), graphColor(0x000000FF) { }

plotter2d::Options::Options(const bool drawUi, const bool drawAxes, const bool drawGrid,
                            const ApproximationMode approximationMode, const unsigned resolution,
                            const bool useCustomPlotRange,
                            const std::pair<double, double>& plotRange,
                            const unsigned graphColor) : drawUi(drawUi), drawAxes(drawAxes),
                                                         drawGrid(drawGrid),
                                                         approximationMode(approximationMode),
                                                         resolution(resolution),
                                                         plotRange(plotRange),
                                                         useCustomPlotRange(useCustomPlotRange),
                                                         graphColor(graphColor) { }

plotter2d::OptionsBuilder& plotter2d::OptionsBuilder::drawUi(const bool value) {
    drawUi_ = value;
    return *this;
}

plotter2d::OptionsBuilder& plotter2d::OptionsBuilder::approximationMode(
    const Options::ApproximationMode mode) {
    approximationMode_ = mode;
    return *this;
}

plotter2d::OptionsBuilder& plotter2d::OptionsBuilder::resolution(const unsigned value) {
    resolution_ = value;
    return *this;
}

plotter2d::OptionsBuilder& plotter2d::OptionsBuilder::drawAxes(const bool value) {
    drawAxes_ = value;
    return *this;
}

plotter2d::OptionsBuilder& plotter2d::OptionsBuilder::graphColor(const unsigned value) {
    graphColor_ = value;
    return *this;
}

plotter2d::OptionsBuilder& plotter2d::OptionsBuilder::plotRange(
    const std::pair<double, double>& range) {
    plotRange_ = range;
    useCustomPlotRange_ = true;
    return *this;
}

plotter2d::OptionsBuilder& plotter2d::OptionsBuilder::drawGrid(bool value) {
    drawGrid_ = value;
    return *this;
}

plotter2d::OptionsBuilder& plotter2d::OptionsBuilder::useCustomPlotRange(bool value) {
    useCustomPlotRange_ = value;
    if (!value) {
        plotRange_ = {};
    }
    return *this;
}

plotter2d::Options plotter2d::OptionsBuilder::build() const {
    bool customPlotRange = useCustomPlotRange_;
    if (useCustomPlotRange_ && plotRange_ == std::pair<double, double>()) {
        customPlotRange = false;
    }
    return {
        drawUi_, drawAxes_, drawGrid_, approximationMode_, resolution_, customPlotRange, plotRange_,
        graphColor_
    };
}
