#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::CascadeClassifier faceCascade, eyeCascade, smileCascade;
    faceCascade.load("haarcascade_frontalface_default.xml");
    eyeCascade.load("haarcascade_eye.xml");
    smileCascade.load("haarcascade_smile.xml");

    cv::VideoCapture cap("ZUA.mp4");
    cv::Mat frame, gray;

    int frameWidth = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frameHeight = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(cv::CAP_PROP_FPS);

    cv::VideoWriter writer("output.mp4", cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps, cv::Size(frameWidth, frameHeight));

    while (cap.read(frame) && !frame.empty()) {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(gray, faces, 2.0, 5, 0, cv::Size(100, 100));

        for (int i = 0; i < std::min(2, (int)faces.size()); i++) {
            cv::rectangle(frame, faces[i], cv::Scalar(0, 255, 0), 2);

            cv::Mat ROI = gray(faces[i]);
            std::vector<cv::Rect> eyes, smiles;

            eyeCascade.detectMultiScale(ROI, eyes, 1.3, 5, 0, cv::Size(40, 40));
            for (auto& e : eyes)
                cv::circle(frame, cv::Point(faces[i].x + e.x + e.width / 2, faces[i].y + e.y + e.height / 2), e.width / 2, cv::Scalar(0, 0, 255), 2);

            smileCascade.detectMultiScale(ROI, smiles, 1.5, 20, 0, cv::Size(25, 25));
            for (auto& s : smiles)
                cv::rectangle(frame, cv::Rect(faces[i].x + s.x, faces[i].y + s.y, s.width, s.height), cv::Scalar(255, 0, 0), 2);
        }
        writer.write(frame);
    }

    cap.release();
    writer.release();

    return 0;
}