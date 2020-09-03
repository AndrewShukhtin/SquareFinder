#include "square.hpp"

#include <numeric>

Square::Square(const Contour& contour) : _contour{contour} {}

Square::Square(Contour&& contour) : _contour{contour} {}

const cv::Point& Square::getCenter() {
    if (_isCenterComputed) {
        return _center;
    }

    cv::Moments m = cv::moments(_contour, true);
    _center = cv::Point{static_cast<int>(m.m10/m.m00), static_cast<int>(m.m01/m.m00)};

    return _center;
}

const double& Square::getSideLength() {
    if (_isLengthComputed) {
        return _sideLength;
    }

    std::vector<double> lenghts(4);
    for (size_t i = 1; i < 5; ++i) {
        std::vector<cv::Point> line = {_contour[i - 1], _contour[i % 4]};
        lenghts[i - 1] = cv::arcLength(line, false);
    }

    _sideLength = std::accumulate(lenghts.begin(), lenghts.end(), 0.0) / 4.0;
    return _sideLength;
}
