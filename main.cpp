#include <cmath>
#include <interface/plotter2d.h>

int main() {
    plotter2d::plot([](const double x) {
        return x * std::cos(x);
    }, std::make_pair(-10.0, 10.0));
    auto f = [](const double x) {
        return x / std::cos(x);
    };
    constexpr auto domain = std::make_pair(-3.0, 3.0);
    plotter2d::plot(f, domain,
                    plotter2d::OptionsBuilder().approximationMode(plotter2d::Options::Lines).
                    resolution(1000).drawAxes(false).build());
    plotter2d::plot(f, domain,
                    plotter2d::OptionsBuilder().approximationMode(plotter2d::Options::Lines).
                    resolution(20).graphColor(0xFF0000FF).build());
    plotter2d::plotFromPolishNotation("* - x / 1 2 * - x 2 * - x 3 * - x 4 - x 5",
                                      std::make_pair(0, 6));
}
