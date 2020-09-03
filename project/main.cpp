#include "criteriaSquareSelector.hpp"

#include <iostream>
#include <algorithm>

#include <opencv2/viz/types.hpp>

void highlightSquare(cv::Mat& image, const std::vector<cv::Point>& contour) {
    for (size_t i = 1; i <= contour.size(); ++i) {
        cv::line(image, contour[i - 1], contour[i % contour.size()],cv::viz::Color::red(), 5, cv::LINE_AA);
    }
}

void drawToImage(cv::Mat& image, const std::vector<SquareWithCircles>& squaresWithCircles) {
    for (const auto& squareWithCircles : squaresWithCircles) {
    // NOTE: Sorry for 'Kebab' style 
    highlightSquare(image, squareWithCircles.first.getVertexCoords());
    cv::circle(image, squareWithCircles.first.getCenter(), 1, cv::viz::Color::red(), 6, cv::LINE_AA);
        for (const auto& circle : squareWithCircles.second) {
            cv::Point center = circle.getCenter();
            cv::circle(image, center, 1, cv::viz::Color::blue(), 4, cv::LINE_AA);
            cv::circle(image, center, circle.getRadius(), cv::viz::Color::blue(), 4, cv::LINE_AA);
        }
    }
}

int main(int argc, char* argv[]) {
   if (argc != 2) {
       std::cout << "usage: DisplayImage.out <Image_Path>" << std::endl;
       return EXIT_FAILURE;
   }

    cv::Mat image = cv::imread(argv[1]);
    if (image.empty()) {
        std::cout << "No image data" << std::endl;
        return EXIT_FAILURE;
    }

    SquaresDetecter squaresDetecter(image);
    auto squares = squaresDetecter.detect();

    CriteriaSquareSelector squareSelector(image);

    auto squaresWithCircles = squareSelector(squares);

    std::sort(squaresWithCircles.begin(), squaresWithCircles.end(),
    [](const SquareWithCircles& left, const SquareWithCircles& right){
        // NOTE: Sorry for 'Kebab' style
        const std::vector<cv::Point> leftCenters = {left.first.getCenter(), left.second[0].getCenter()};
        const auto leftDist = cv::arcLength(leftCenters, false);

        const std::vector<cv::Point> rightCenters = {right.first.getCenter(), right.second[0].getCenter()};
        const auto rightDist = cv::arcLength(rightCenters, false);

        const auto leftRatio = leftDist / (left.first.getSideLength() + 1e-10);
        const auto rightRatio = rightDist / (right.first.getSideLength() + 1e-10);

        return leftRatio < rightRatio;
    });

    for (const auto& squareWithCircles : squaresWithCircles) {
        for (const auto coord: squareWithCircles.first.getVertexCoords()) {
            std::cout << coord << " ";
        }
        std::cout << std::endl;
    }

    drawToImage(image, squaresWithCircles);

    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Image", image);
    cv::waitKey();

    return EXIT_SUCCESS;
}