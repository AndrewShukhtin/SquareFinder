#include "square.hpp"

#include <numeric>

Square::Square(const Contour& contour) : _contour{contour} {
    _initializeParams();
}

Square::Square(Contour&& contour) : _contour{contour} {
    _initializeParams();
}

const cv::Point& Square::getCenter() const {
    return _center;
}

const double& Square::getSideLength() const {
    return _sideLength;
}

const Contour& Square::getVertexCoords() const {
    return _contour;
}

void Square::_initializeParams() {
    cv::Moments m = cv::moments(_contour, true);
    _center = cv::Point{static_cast<int>(m.m10/m.m00), static_cast<int>(m.m01/m.m00)};

    std::vector<double> lenghts(4);
    for (size_t i = 1; i < 5; ++i) {
        std::vector<cv::Point> line = {_contour[i - 1], _contour[i % 4]};
        lenghts[i - 1] = cv::arcLength(line, false);
    }

    _sideLength = std::accumulate(lenghts.begin(), lenghts.end(), 0.0) / 4.0;
}
