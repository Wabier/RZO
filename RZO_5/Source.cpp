#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap("test.mkv");
    if (!cap.isOpened()) {
        std::cerr << "Ошибка открытия видео\n";
        return -1;
    }

    double fps = cap.get(cv::CAP_PROP_FPS);
    int width = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    cv::VideoWriter writer("output.mp4",
        cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
        fps,
        cv::Size(width, height));

    if (!writer.isOpened()) {
        std::cerr << "Ошибка создания выходного видео\n";
        return -1;
    }

    cv::Mat frame, gray, edges;

    while (cap.read(frame)) {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::Canny(gray, edges, 50, 100);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (auto& contour : contours) {
            double area = cv::contourArea(contour);
            if (area < 20) continue;

            std::vector<cv::Point> approx;
            double peri = cv::arcLength(contour, true);
            cv::approxPolyDP(contour, approx, 0.04 * peri, true);

            std::string shape;
            if (approx.size() == 3) {
                shape = "Triangle";
            }
            else if (approx.size() == 4) {
                shape = "Rectangle";
            }
            else {
                double circularity = 4 * CV_PI * area / (peri * peri);
                if (circularity > 0.75) {
                    shape = "Circle";
                }
                else {
                    continue;
                }
            }

            cv::Rect box = cv::boundingRect(contour);
            cv::rectangle(frame, box, cv::Scalar(0, 0, 255), 2);

            cv::Moments m = cv::moments(contour);
            if (m.m00 != 0) {
                cv::Point center(m.m10 / m.m00, m.m01 / m.m00);
                cv::putText(frame, shape, center,
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 2);
            }
        }

        writer.write(frame);
        cv::imshow("Фигуры", frame);
        cv::imshow("Canny", edges);

        if (cv::waitKey(30) == 27) break;
    }

    cap.release();
    writer.release();
    cv::destroyAllWindows();

    std::cout << "Сохранено в output.mp4" << std::endl;

    return 0;
}