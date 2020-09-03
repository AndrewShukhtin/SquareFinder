#include "criteriaSquareSelector.hpp"
#include "config.hpp"

static std::pair<cv::Point, cv::Mat> getSearchDomain(const cv::Mat& image, const Contour& square);

SquaresDetecter::SquaresDetecter(const cv::Mat& image) : _image{image.clone()} {}

std::vector<Square> SquaresDetecter::detect() {
    _preprocessInitialImage();

    std::vector<Contour> contours = _findContours();

    return _selectSquaresFromContours(contours);
}

CriteriaSquareSelector::CriteriaSquareSelector(const cv::Mat& image) : _image{image.clone()} {}

std::vector<SquareWithCircles> CriteriaSquareSelector::operator()(std::vector<Square>& squares) {
    _preprocessInitialImage();

    return _findSquares(squares);
}

std::vector<SquareWithCircles> CriteriaSquareSelector::_findSquares(std::vector<Square>& squares) {
    const double dp = 1.0;
    const double minDist = 10.0;
    const double param1 = 100.0;
    const double param2 = 100.0;
    const int minRadius = 10;

    std::vector<SquareWithCircles> squaresWithCircles;
    for (auto& square : squares) {
        const auto [origin, searchDomain] = getSearchDomain(_image, square.getVertexCoords());
        const auto maxRadius = square.getSideLength() * 0.45;

        std::vector<cv::Vec3f> rawCirclesData;
        cv::HoughCircles(searchDomain,
                            rawCirclesData,
                            cv::HOUGH_GRADIENT,
                            dp,
                            minDist,
                            param1,
                            param2,
                            minRadius,
                            maxRadius);
        if (!rawCirclesData.empty()) {
            std::vector<Circle> circles;
            for (auto& rawCircleData: rawCirclesData) {
                rawCircleData[0] += origin.x;
                rawCircleData[1] += origin.y;
                circles.emplace_back(rawCircleData);
            }
            Circle circle(std::move(circles.front()));

            squaresWithCircles.emplace_back(std::move(square), std::move(circles));
        }
    }

    return squaresWithCircles;
}

void CriteriaSquareSelector::_preprocessInitialImage() {
    cv::cvtColor(_image, _image, cv::COLOR_BGR2GRAY);
    cv::medianBlur(_image, _image, MEDIAN_BLUR_PARAM);
}

void SquaresDetecter::_preprocessInitialImage() {
    cv::cvtColor(_image, _image, cv::COLOR_BGR2GRAY);
    cv::medianBlur(_image, _image, MEDIAN_BLUR_PARAM);

    cv::threshold(_image, _image, THRESHOLD_PARAM, MAX_THRESHOLD_PARAM, cv::THRESH_BINARY_INV);
}

 std::vector<Contour> SquaresDetecter::_findContours() const {
    std::vector<Contour> contours;
    cv::findContours(_image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);

    return contours;
 }

 std::vector<Square> SquaresDetecter::_selectSquaresFromContours(std::vector<Contour>& contours) const {
     std::vector<Square> squares;
    for (auto& contour : contours) {
        double arcLength = cv::arcLength(contour, true);
        cv::approxPolyDP(contour, contour, arcLength * APPROX_POLY_DP_EPSILON_MULTIPLIER, true);
        if (_isSquare(contour)) {
            squares.emplace_back(std::move(contour));
        }
    }

    return squares;
 }

 bool SquaresDetecter::_isSquare(const Contour& contours) const {
    if (contours.size() != 4
        || std::fabs(cv::contourArea(contours)) < MIN_AREA
        || !cv::isContourConvex(contours)
        || !_isSidesLengthsValid(contours)) {
        return false;
    }

    return _isAnglesValid(contours);
 }

 bool SquaresDetecter::_isSidesLengthsValid(const Contour& quadrangle) const {
    std::vector<double> lenghts(4);
    for (size_t i = 1; i < 5; ++i) {
        std::vector<cv::Point> line = {quadrangle[i - 1], quadrangle[i % 4]};
        lenghts[i - 1] = cv::arcLength(line, false);
    }

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = i + 1; j < 4; ++j) {
            auto ratio = lenghts[i] / (lenghts[j] + EPSILON);
            if (ratio < MIN_SIDES_RATIO || ratio > MAX_SIDES_RATIO) {
                return false;
            }
        }
    }

    return true;
 }

 bool SquaresDetecter::_isAnglesValid(const Contour& quadrangle) const {
    const double pi = std::acos(-1);
    for (size_t i = 2; i < 6; ++i) {
        double cosine = _angleCosine(quadrangle[(i - 1) % 4],
                                     quadrangle[(i - 2) % 4],
                                     quadrangle[(i + 1) % 4]);
        if (cosine > std::cos(pi / 180 * MIN_ANGLES_DEGREE) && cosine < std::cos(pi / 180 * MAX_ANGLES_DEGREE)) {
            return false;
        }
    }

    return true;
 }

 double SquaresDetecter::_angleCosine(const cv::Point& pt1, const cv::Point& pt2, const cv::Point& pt3) const {
    double dx1 = pt2.x - pt1.x;
    double dy1 = pt2.y - pt1.y;
    double dx2 = pt2.x - pt3.x;
    double dy2 = pt2.y - pt3.y;
    return (dx1 * dx2 + dy1 * dy2) / std::sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + EPSILON);
 }

static std::pair<cv::Point, cv::Mat> getSearchDomain(const cv::Mat& image, const std::vector<cv::Point>& square) {
    const auto compareX = [](const auto& a, const auto& b) { return a.x < b.x; };
    const auto compareY = [](const auto& a, const auto& b) { return a.y < b.y; };

    auto minX  = std::min_element(square.begin(), square.end(), compareX)->x;
    auto minY  = std::min_element(square.begin(), square.end(), compareY)->y;
    auto maxX = std::max_element(square.begin(), square.end(), compareX)->x;
    auto maxY = std::max_element(square.begin(), square.end(), compareY)->y;

    cv::Range rows(minY, maxY);
    cv::Range cols(minX, maxX);

    return {{minX, minY}, image(rows, cols)};
}