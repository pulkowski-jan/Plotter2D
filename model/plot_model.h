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

class PlotData {
    Rectangle domain_;
    Point* points_;
    size_t pointsCount_;

    public:
        PlotData(const Rectangle&, const Point*, size_t);

        PlotData(const PlotData&) = delete;

        ~PlotData();

        const Rectangle& domain() const;

        const Point* points() const;

        size_t pointsCount() const;
};


#endif //PLOT_INTERFACE_H
