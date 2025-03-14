#ifndef PLOT_INTERFACE_H
#define PLOT_INTERFACE_H
#include <string>

using std::size_t;

class Point {
    double x_;
    double y_;

    public:
        Point();

        Point(double, double);

        double x() const;

        double y() const;
};

/**
 * A rectangle with given dimensions and anchor point
 * width - The width of the rectangle
 * height - The height of the rectangle
 * anchor - The anchor point (bottom-left corner) of the rectangle
 */
class Rectangle {
    double width_;
    double height_;
    Point anchor_;

    public:
        Rectangle(double, double, const Point&);

        Rectangle(const Rectangle&);

        Rectangle(const Point&, const Point&);

        double width() const;

        double height() const;

        const Point& anchor() const;
};

/**
 * A set of points to be plotted on a 2D plane
 * domain - The domain of the plot (a rectangle containing all points)
 * points - The array of points to plot
 * pointsCount - The number of points in the array
 */
class PlotData {
    Rectangle domain_;
    Point* points_;
    size_t pointsCount_;

    public:
        PlotData(const Rectangle&, const Point*, size_t);

        PlotData(const PlotData&) = delete;

        PlotData& operator=(const PlotData&) = delete;

        ~PlotData();

        const Rectangle& domain() const;

        const Point* points() const;

        size_t pointsCount() const;
};


#endif //PLOT_INTERFACE_H
