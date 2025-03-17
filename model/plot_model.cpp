#include "plot_model.h"

#include <algorithm>
#include <cmath>
#include <cstring>

Point::Point() : x_(0), y_(0) { }

Point::Point(const double x, const double y) : x_(x), y_(y) { }

double Point::x() const {
    return x_;
}

double Point::y() const {
    return y_;
}

Rectangle::Rectangle(const double width, const double height, const Point& p)
    : width_(width), height_(height), anchor_(p) { }

Rectangle::Rectangle(const Rectangle& r)
    : width_(r.width()), height_(r.height()), anchor_(r.anchor()) { }

Rectangle::Rectangle(const Point& anchor, const Point& counterAnchor)
    : width_(std::abs(counterAnchor.x() - anchor.x())),
      height_(std::abs(counterAnchor.y() - anchor.y())), anchor_(anchor) { }

double Rectangle::width() const {
    return width_;
}

double Rectangle::height() const {
    return height_;
}

const Point& Rectangle::anchor() const {
    return anchor_;
}

PlotData::PlotData(const Rectangle& r, Point* points, const size_t pointsCount)
    : domain_(r), points_(points), pointsCount_(pointsCount) {
    std::sort(points_, points_ + pointsCount, [](const auto& a, const auto& b) {
        return a.x() < b.x();
    });
}

PlotData::~PlotData() {
    delete[] points_;
}

const Rectangle& PlotData::domain() const {
    return domain_;
}

const Point* PlotData::points() const {
    return points_;
}

size_t PlotData::pointsCount() const {
    return pointsCount_;
}
