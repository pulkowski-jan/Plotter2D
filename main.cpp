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
    plotter2d::Options polyOptions(true, plotter2d::Options::Lines, 5000, std::make_pair(-100.0, 100.0));
    plotter2d::plotFromPolishNotation("^ x 3", std::make_pair(-4, 4), polyOptions);
    plotter2d::plotFromPolishNotation("^ x 2", std::make_pair(-4, 4), polyOptions);
}
