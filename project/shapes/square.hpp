#ifndef PROJECT_SHAPES_SQUARE_HPP_
#define PROJECT_SHAPES_SQUARE_HPP_

#include "types.hpp"

class Square {
public:
    Square(const Contour& shape);
    Square(Contour&& shape);

    const cv::Point& getCenter();

    const double& getSideLength();

private:
    Contour _contour;
    cv::Point _center;
    double _sideLength;
    bool _isCenterComputed = false;
    bool _isLengthComputed = false;
};

#endif  // PROJECT_SHAPES_SQUARE_HPP_