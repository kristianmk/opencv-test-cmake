// Example from opencv https://docs.opencv.org/4.10.0/de/d70/samples_2cpp_2kalman_8cpp-example.html
// ..with a few changes by K. M. Knausgård.
#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <cmath>
#include <iostream>


inline cv::Point2f calcPoint(const cv::Point2f& center, double radius, double angle)
{
    return center + cv::Point2f(static_cast<float>(std::cos(angle)), static_cast<float>(-std::sin(angle))) * static_cast<float>(radius);
}

void printVersion()
{
    std::cout   << "OpenCV version: " 
                << cv::getVersionString()
                << std::endl;
}

void printHelp()
{
    std::cout <<
        "\nExample of c calls to OpenCV's Kalman filter.\n"
        "   Tracking of rotating point.\n"
        "   Point moves in a circle and is characterized by a 1D state.\n"
        "   state_k+1 = state_k + speed + process_noise N(0, 1e-5)\n"
        "   The speed is constant.\n"
        "   Both state and measurements vectors are 1D (a point angle),\n"
        "   Measurement is the real state + gaussian noise N(0, 1e-1).\n"
        "   The real and the measured points are connected with red line segment,\n"
        "   the real and the estimated points are connected with yellow line segment,\n"
        "   the real and the corrected estimated points are connected with green line segment.\n"
        "   (if Kalman filter works correctly,\n"
        "    the yellow segment should be shorter than the red one and\n"
        "    the green segment should be shorter than the yellow one).\n"
        "   Pressing any key (except ESC) will reset the tracking.\n"
        "   Pressing ESC will stop the program.\n";
}

int main()
{

    printVersion();
    printHelp();
    
    constexpr int imageSize = 800;
    cv::Mat img(imageSize, imageSize, CV_8UC3);
    cv::KalmanFilter kalmanFilter(2, 1, 0);
    cv::Mat state(2, 1, CV_32F); // (phi, delta_phi)
    cv::Mat processNoise(2, 1, CV_32F);
    cv::Mat measurement = cv::Mat::zeros(1, 1, CV_32F);
    char code = static_cast<char>(-1);

    for (;;) {
        img = cv::Scalar::all(0);
        state.at<float>(0) = 0.0f;
        state.at<float>(1) = 2.f * static_cast<float>(CV_PI) / 6;
        kalmanFilter.transitionMatrix = (cv::Mat_<float>(2, 2) << 1, 1, 0, 1);

        cv::setIdentity(kalmanFilter.measurementMatrix);
        cv::setIdentity(kalmanFilter.processNoiseCov, cv::Scalar::all(1e-5));
        cv::setIdentity(kalmanFilter.measurementNoiseCov, cv::Scalar::all(1e-1));
        cv::setIdentity(kalmanFilter.errorCovPost, cv::Scalar::all(1));

        cv::randn(kalmanFilter.statePost, cv::Scalar::all(0), cv::Scalar::all(0.1));

        for (;;) {
            const cv::Point2f center(img.cols * 0.5f, img.rows * 0.5f);
            constexpr float radius = imageSize / 3.f;
            const double stateAngle = state.at<float>(0);
            const cv::Point statePoint = calcPoint(center, radius, stateAngle);

            const cv::Mat prediction = kalmanFilter.predict();
            const double predictAngle = prediction.at<float>(0);
            const cv::Point predictPoint = calcPoint(center, radius, predictAngle);

            // Generate measurement
            cv::randn(measurement, cv::Scalar::all(0), cv::Scalar::all(kalmanFilter.measurementNoiseCov.at<float>(0)));
            measurement += kalmanFilter.measurementMatrix * state;

            const double measuredAngle = measurement.at<float>(0);
            const cv::Point measuredPoint = calcPoint(center, radius, measuredAngle);

            // Correct the state estimates based on measurements
            kalmanFilter.correct(measurement);
            const double improvedAngle = kalmanFilter.statePost.at<float>(0);
            const cv::Point improvedPoint = calcPoint(center, radius, improvedAngle);

            // Plot points
            img *= 0.2;
            cv::drawMarker(img, measuredPoint, cv::Scalar(0, 0, 255), cv::MARKER_SQUARE, 5, 2);
            cv::drawMarker(img, predictPoint, cv::Scalar(0, 255, 255), cv::MARKER_SQUARE, 5, 2);
            cv::drawMarker(img, improvedPoint, cv::Scalar(0, 255, 0), cv::MARKER_SQUARE, 5, 2);
            cv::drawMarker(img, statePoint, cv::Scalar(255, 255, 255), cv::MARKER_STAR, 10, 1);

            cv::line(img, statePoint, measuredPoint, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
            cv::line(img, statePoint, predictPoint, cv::Scalar(0, 255, 255), 1, cv::LINE_AA);
            cv::line(img, statePoint, improvedPoint, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);

            cv::randn(processNoise, cv::Scalar(0), cv::Scalar::all(std::sqrt(kalmanFilter.processNoiseCov.at<float>(0, 0))));
            state = kalmanFilter.transitionMatrix * state + processNoise;

            cv::imshow("Kalman", img);
            code = static_cast<char>(cv::waitKey(1000));

            if (code > 0) {
                break;
            }
        }

        if (code == 27 || code == 'q' || code == 'Q') {
            break;
        }
    }

    return 0;
}
