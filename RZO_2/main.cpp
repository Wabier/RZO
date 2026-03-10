#include <opencv2/opencv.hpp>
#include <iostream>

int main() {

    std::string text = "Bernyukov Oleg 22-AM-1";
    cv::Point textPos(350, 300);
    int direction = 1;
    int speed = 5;

    int baseline = 0;
    cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 1.0, 2, &baseline);

    while (true) {
        cv::Mat canvas(600, 800, CV_8UC3, cv::Scalar(0, 0, 0));

        cv::line(canvas, cv::Point(50, 50), cv::Point(750, 50), cv::Scalar(255, 255, 255), 5);
        cv::rectangle(canvas, cv::Point(50, 100), cv::Point(250, 250), cv::Scalar(255, 120, 0), -1);
        cv::circle(canvas, cv::Point(400, 300), 100, cv::Scalar(100, 170, 75), 2);
        cv::ellipse(canvas, cv::Point(150, 450), cv::Size(150, 100), 135, 0, 360, cv::Scalar(0, 140, 255), 3);

        std::vector<cv::Point> pts = {
            cv::Point(650, 100),
            cv::Point(750, 150),
            cv::Point(750, 250),
            cv::Point(650, 300),
            cv::Point(550, 250),
            cv::Point(550, 150)
        };

        cv::polylines(canvas, pts, true, cv::Scalar(0, 0, 255), 3);

        textPos.y += direction * speed;

        if (textPos.y < textSize.height + 10) {
            textPos.y = textSize.height + 10;
            direction = 1;
        }
        if (textPos.y > 600 - 20) {
            textPos.y = 600 - 20;
            direction = -1;
        }

        cv::putText(canvas, text, textPos, cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 1.0, cv::Scalar(0, 165, 255), 2);

        cv::imshow("Canvas", canvas);
        cv::imwrite("Canvas.png", canvas);
        cv::waitKey(30);
    }

    return 0;
}