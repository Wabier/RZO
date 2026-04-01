#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

string getShapeName(const vector<Point>& contour) {
    double peri = arcLength(contour, true);
    vector<Point> approx;
    approxPolyDP(contour, approx, 0.02 * peri, true);

    int vertices = approx.size();
    double area = contourArea(contour);
    double circularity = 4 * CV_PI * area / (peri * peri);

    // Для фигур с большим количеством вершин (круг, эллипс)
    if (vertices > 6) {
        if (contour.size() >= 5) {
            RotatedRect ellipse = fitEllipse(contour);

            double ellipseArea = CV_PI * (ellipse.size.width / 2) * (ellipse.size.height / 2);
            double areaRatio = area / ellipseArea;

            double aspectRatio = ellipse.size.width / ellipse.size.height;

            if (aspectRatio >= 0.9 && aspectRatio <= 1.1 && areaRatio > 0.85) {
                return "Circle";
            }
            else if (areaRatio > 0.8) {
                return "Ellipse";
            }
        }

        if (circularity > 0.85) {
            return "Circle";
        }
        else {
            return "Ellipse";
        }
    }

    // Для треугольников
    if (vertices == 3) {
        return "Triangle";
    }

    // Для четырехугольников
    if (vertices == 4) {
        // Вычисляем длины сторон
        vector<double> sides;
        for (int i = 0; i < 4; i++) {
            double len = norm(approx[i] - approx[(i + 1) % 4]);
            sides.push_back(len);
        }

        // Вычисляем углы
        vector<double> angles;
        for (int i = 0; i < 4; i++) {
            Point p1 = approx[i];
            Point p2 = approx[(i + 1) % 4];
            Point p3 = approx[(i + 2) % 4];

            Vec2d v1(p2.x - p1.x, p2.y - p1.y);
            Vec2d v2(p3.x - p2.x, p3.y - p2.y);

            double angle = abs(atan2(v1[1], v1[0]) - atan2(v2[1], v2[0]));
            angle = angle * 180 / CV_PI;
            angles.push_back(angle);
        }

        bool hasRightAngle = false;
        for (double angle : angles) {
            if (abs(angle - 90) < 10) {
                hasRightAngle = true;
                break;
            }
        }

        double avgSide = (sides[0] + sides[1] + sides[2] + sides[3]) / 4;
        int true_counter = 0;
        bool allSidesEqual = false;

        for (double side : sides) {
            if (abs(side - avgSide) < avgSide * 0.05) {
                true_counter++;
            }
        }

        if (true_counter == 4) {
            allSidesEqual = true;
        }

        // Определяем тип четырехугольника
        if (hasRightAngle && allSidesEqual) {
            return "Square";
        }
        else if (hasRightAngle && !allSidesEqual) {
            return "Rectangle";
        }
        else if (!hasRightAngle && allSidesEqual) {
            return "Rhombus";
        }
    }

    // Для пятиугольников и шестиугольников
    if (vertices == 5) return "Pentagon";
    if (vertices == 6) return "Hexagon";

    return "Unknown";
}

int main() {
    Mat img = imread("figures.png");

    Mat gray, edges;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, gray, Size(5, 5), 1);
    Canny(gray, edges, 50, 150);

    vector<vector<Point>> contours;
    findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Mat result = img.clone();

    vector<Point2f> centersOfMass;
    vector<string> shapeNames;

    for (const auto& contour : contours) {
        double area = contourArea(contour);
        if (area < 500) continue;

        double perimeter = arcLength(contour, true);

        Moments m = moments(contour);

        Point2f centerOfMass(m.m10 / m.m00, m.m01 / m.m00);

        centersOfMass.push_back(centerOfMass);

        string name = getShapeName(contour);
        shapeNames.push_back(name);

        string label = name + " P:" + to_string((int)perimeter);

        drawContours(result, vector<vector<Point>>{contour}, 0, Scalar(255, 0, 25), 3);

        circle(result, centerOfMass, 5, Scalar(0, 0, 255), -1);

        int baseline;
        Size textSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 2, &baseline);

        Point textPos(centerOfMass.x - textSize.width / 2, centerOfMass.y - textSize.height / 2 - 10);
        rectangle(result, Point(textPos.x - 2, textPos.y - baseline - 2), Point(textPos.x + textSize.width + 2, textPos.y + textSize.height + 2), Scalar(255, 255, 255), FILLED);
        putText(result, label, textPos,FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);
    }

    imshow("Result", result);
    imwrite("output.png", result);

    waitKey(0);
    return 0;
}