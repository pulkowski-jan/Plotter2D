#ifndef PLOTTER2D_H
#define PLOTTER2D_H
#include <functional>
#include <string>


namespace plotter2d {
    struct Options {
        enum ApproximationMode {
            LINES, POINTS
        };

        bool drawUi;
        bool drawAxes;
        bool drawGrid;
        ApproximationMode approximationMode;
        unsigned resolution;
        std::pair<double, double> plotRange;
        bool useCustomPlotRange;
        unsigned graphColor;
        bool cachingEnabled;

        Options();

        Options(bool drawUi, bool drawAxes, bool drawGrid, ApproximationMode approximationMode,
                unsigned resolution, bool useCustomPlotRange,
                const std::pair<double, double>& plotRange, unsigned graphColor,
                bool cachingEnabled);

    };

    class OptionsBuilder {
        bool drawUi_ = true;
        Options::ApproximationMode approximationMode_ = Options::POINTS;
        unsigned resolution_ = 5000;
        bool drawAxes_ = true;
        bool drawGrid_ = true;
        unsigned graphColor_ = 0x000000FF;
        std::pair<double, double> plotRange_{};
        bool useCustomPlotRange_ = false;
        bool cachingEnabled_ = true;

        public:
            OptionsBuilder& drawUi(bool value);

            OptionsBuilder& approximationMode(Options::ApproximationMode mode);

            OptionsBuilder& resolution(unsigned value);

            OptionsBuilder& drawAxes(bool value);

            OptionsBuilder& drawGrid(bool value);

            OptionsBuilder& graphColor(unsigned value);

            OptionsBuilder& plotRange(const std::pair<double, double>& range);

            OptionsBuilder& useCustomPlotRange(bool value);

            OptionsBuilder& cachingEnabled(bool value);

            Options build() const;
    };


    void plot(const std::function<double(double)>& func, const std::pair<double, double>& domain,
              const Options& options = Options());

    void plot(const std::vector<std::function<double(double)> >& funcs,
              const std::pair<double, double>& domain, const Options& options = Options());


    void plotFromPolishNotation(const std::string& polishNotation,
                                const std::pair<double, double>& domain,
                                const Options& options = Options());

    void plotFromPolishNotation(const std::vector<std::string>& polishNotations,
                                const std::pair<double, double>& domain,
                                const Options& options = Options());
}

#endif //PLOTTER2D_H
