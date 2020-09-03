#ifndef PROJECT_SHAPES_CIRCLE_HPP_
#define PROJECT_SHAPES_CIRCLE_HPP_

#include "types.hpp"

class Circle {
public:
    Circle(const cv::Vec3f& rawCircleData);

    const cv::Point& getCenter() const;

    const int getRadius() const;

private:
    cv::Point _center{};
    int _radius = 0;
};

#endif  // PROJECT_SHAPES_CIRCLE_HPP_