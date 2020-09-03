#include "circle.hpp"

Circle::Circle(const cv::Vec3f& rawCircleData) :
        _center{static_cast<int>(rawCircleData[0]), static_cast<int>(rawCircleData[1])}, _radius{rawCircleData[2]} {}

const cv::Point& Circle::getCenter() const {
    return _center;
}

const int Circle::getRadius() const {
    return _radius;
}
