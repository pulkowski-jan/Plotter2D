#ifndef PLOTTER2D_H
#define PLOTTER2D_H
#include <functional>
#include <string>
#include <cmath>


namespace plotter2d {
    struct Options {
        enum ApproximationMode {
            Lines, Points
        };

        bool drawUi;
        ApproximationMode approximationMode;
        unsigned resolution;
        std::pair<double,double> plotRange;
        bool useCustomPlotRange;

        Options() : drawUi(true), approximationMode(Points), resolution(5000),
                    plotRange(std::make_pair(-INFINITY, INFINITY)), useCustomPlotRange(false) { }


        Options(const bool drawUi, const ApproximationMode approximationMode,
                const unsigned resolution)
            : drawUi(drawUi), approximationMode(approximationMode), resolution(resolution),
        plotRange(std::make_pair(-INFINITY, INFINITY)), useCustomPlotRange(false) { }

        Options(const bool drawUi, const ApproximationMode approximationMode,
        const unsigned resolution, const std::pair<double, double>& plotRange):
        drawUi(drawUi), approximationMode(approximationMode), resolution(resolution),
      plotRange(plotRange), useCustomPlotRange(true) { }

    };

    void plot(const std::function<double(double)>& func, const std::pair<double, double>& domain,
              Options options = Options());

    void plotFromPolishNotation(const std::string& polishNotation,
                                const std::pair<double, double>& domain,
                                Options options = Options());
}

#endif //PLOTTER2D_H
