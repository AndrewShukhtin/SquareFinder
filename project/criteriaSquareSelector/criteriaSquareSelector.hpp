#ifndef PROJECT_CRITERIA_SQUARE_SELECTOR_CRITERIA_SQUARE_SELECTOR_HPP_
#define PROJECT_CRITERIA_SQUARE_SELECTOR_CRITERIA_SQUARE_SELECTOR_HPP_

#include "square.hpp"
#include "circle.hpp"

struct SquareWithCircles {
    SquareWithCircles(Square &&square, Circle&& circles) : square{square}, circles{circles} {}
    Square square;
    std::vector<Circle> circles;
};

class SquaresDetecter {
public:
    SquaresDetecter(const cv::Mat& image);

    std::vector<Square> detect();

private:
    void _preprocessInitialImage();
    std::vector<Contour> _findContours() const;
    std::vector<Square> _selectSquaresFromContours(std::vector<Contour>& contours) const;

    bool _isSquare(const Contour& contour) const;

    bool _isSidesLengthsValid(const Contour& quadrangle) const;
    bool _isAnglesValid(const Contour& quadrangle) const;


    double _angleCosine(const cv::Point& pt1, const cv::Point& pt2, const cv::Point& pt3) const;
private:
    cv::Mat _image;
};

class CriteriaSquareSelector {
public:
    CriteriaSquareSelector(const cv::Mat& image);

    std::vector<SquareWithCircles> operator()(std::vector<Square>& squares);

private:
    void _preprocessInitialImage();
    std::vector<SquareWithCircles> _findSquares(std::vector<Square>& squares);

private:
    cv::Mat _image;
};

#endif  // PROJECT_CRITERIA_SQUARE_SELECTOR_CRITERIA_SQUARE_SELECTOR_HPP_