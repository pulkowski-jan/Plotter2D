#include <cmath>
#include <interface/plotter2d.h>

int main() {
    plotter2d::plot([](const double x) {
        return x * std::cos(x);
    }, std::make_pair(-10.0, 10.0));
    plotter2d::plot([](const double x) {
        return x / std::cos(x);
    }, std::make_pair(-3.0, 3.0), plotter2d::Options(true, plotter2d::Options::Lines, 1000));
    plotter2d::plot([](const double x) {
        return x / std::cos(x);
    }, std::make_pair(-3.0, 3.0), plotter2d::Options(true, plotter2d::Options::Points, 10000));
    plotter2d::plotFromPolishNotation("* - x / 1 2 * - x 2 * - x 3 * - x 4 - x 5",
                                      std::make_pair(0, 6));
}
