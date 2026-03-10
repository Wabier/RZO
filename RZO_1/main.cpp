#include <opencv2/opencv.hpp>
#include <iostream>

int main() {

    setlocale(LC_ALL, "Russian");

    cv::Mat original = cv::imread("Kobe_Bryant.png");

    // Исходное изображение
    cv::imshow("Оригинал", original);

    // Перевод в градации серого
    cv::Mat gray;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::imshow("В градациях серого", gray);

    // Изменение размера
    cv::Mat resized;
    cv::resize(original, resized, cv::Size(original.cols / 2, original.rows / 2));
    cv::imshow("Вдвое меньше", resized);

    // Поворот
    cv::Mat rotated;
    cv::Point2f center(original.cols / 2, original.rows / 2);
    cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, 30, 1); // на 30 градусов
    cv::warpAffine(original, rotated, rotationMatrix, original.size());
    cv::imshow("Поворот на 30 градусов", rotated);

    // Размытие
    cv::Mat blurred;
    cv::GaussianBlur(original, blurred, cv::Size(15, 15), 0);
    cv::imshow("Размытие", blurred);

    // Выделение границ
    cv::Mat edges, grayForEdges;
    cv::cvtColor(original, grayForEdges, cv::COLOR_BGR2GRAY);
    cv::Canny(grayForEdges, edges, 50, 150);
    cv::imshow("Выделение границ", edges);

    // Сохранение на диск
    cv::imwrite("Gray.png", gray);
    cv::imwrite("Resized.png", resized);
    cv::imwrite("Rotated.png", rotated);
    cv::imwrite("Blurred.png", blurred);
    cv::imwrite("Canny.png", edges);

    cv::waitKey(0);
    return 0;
}