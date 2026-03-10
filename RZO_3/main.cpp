#include <opencv2/opencv.hpp>
#include <iostream>

int main() {

	cv::Mat image = cv::imread("Markers.png");
	cv::Mat hsv;

	cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
	
	cv::Mat greenMask;
	cv::inRange(hsv, cv::Scalar(40, 150, 100), cv::Scalar(75, 255, 255), greenMask);

	cv::Mat result;
	image.copyTo(result, greenMask);

	cv::imshow("Исходное изображение", image);
	cv::imshow("Маска зелёного цвета", greenMask);
	cv::imshow("Зелёный маркер", result);

	cv::imwrite("GreenMask.png", greenMask);
	cv::imwrite("GreenMarker.png", result);
	
	cv::waitKey(0);
	return 0;
}