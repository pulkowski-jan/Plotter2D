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
        bool drawAxes;
        bool drawGrid;
        ApproximationMode approximationMode;
        unsigned resolution;
        std::pair<double,double> plotRange;
        bool useCustomPlotRange;
        unsigned graphColor;

        Options();

        Options(bool drawUi, bool drawAxes, bool drawGrid, ApproximationMode approximationMode,
                unsigned resolution, bool useCustomPlotRange,
                const std::pair<double, double>& plotRange, unsigned graphColor);

    };

    class OptionsBuilder {
        bool drawUi_ = true;
        Options::ApproximationMode approximationMode_ = Options::Points;
        unsigned resolution_ = 5000;
        bool drawAxes_ = true;
        bool drawGrid_ = true;
        unsigned graphColor_ = 0x000000FF;
        std::pair<double, double> plotRange_{};
        bool useCustomPlotRange_ = false;

        public:
            OptionsBuilder& drawUi(bool value);

            OptionsBuilder& approximationMode(Options::ApproximationMode mode);

            OptionsBuilder& resolution(unsigned value);

            OptionsBuilder& drawAxes(bool value);

            OptionsBuilder& drawGrid(bool value);

            OptionsBuilder& graphColor(unsigned value);

            OptionsBuilder& plotRange(const std::pair<double, double>& range);

            OptionsBuilder& useCustomPlotRange(bool value);

            Options build() const;
    };


    void plot(const std::function<double(double)>& func, const std::pair<double, double>& domain,
              const Options& options = Options());

    void plotFromPolishNotation(const std::string& polishNotation,
                                const std::pair<double, double>& domain,
                                const Options& options = Options());
}

#endif //PLOTTER2D_H
