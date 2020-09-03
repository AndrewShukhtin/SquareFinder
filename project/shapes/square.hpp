#ifndef PROJECT_SHAPES_SQUARE_HPP_
#define PROJECT_SHAPES_SQUARE_HPP_

#include "types.hpp"

class Square {
public:
    Square(const Contour& _ontour);
    Square(Contour&& contour);

    Square(const Square& square) = default;
    Square(Square&& square) = default;

    Square& operator=(const Square& square) = default;
    Square& operator=(Square&& square) = default;

    const cv::Point& getCenter() const;

    const double& getSideLength() const;

    const Contour& getVertexCoords() const;

private:
    void _initializeParams();

private:
    Contour _contour;
    cv::Point _center;
    double _sideLength;
};

#endif  // PROJECT_SHAPES_SQUARE_HPP_