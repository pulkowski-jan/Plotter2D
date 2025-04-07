#ifndef PLOTTER2D_H
#define PLOTTER2D_H
#include <functional>
#include <string>

namespace plotter2d {
    struct Options {
        enum ApproximationMode {
            Lines, Points
        };

        bool drawUi;
        ApproximationMode approximationMode;
        unsigned resolution;

        Options() : drawUi(true), approximationMode(Points), resolution(5000) { }

        Options(const bool drawUi, const ApproximationMode approximationMode,
                const unsigned resolution)
            : drawUi(drawUi), approximationMode(approximationMode), resolution(resolution) { }
    };

    void plot(const std::function<double(double)>& func, const std::pair<double, double>& domain,
              Options options = Options());

    void plotFromPolishNotation(const std::string& polishNotation,
                                const std::pair<double, double>& domain,
                                Options options = Options());
}

#endif //PLOTTER2D_H
