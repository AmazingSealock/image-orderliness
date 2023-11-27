#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace std;

int main()
{
      // 读取图像
    cv::Mat srcimage = cv::imread("../image/dirty.jpg");
    cv::Mat image;
    cv::resize(srcimage, image, cv::Size(640, 480));

    // 转换为灰度图像
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::imshow("gray", gray);

    // 图像分割
    cv::Mat binary, filter;
    // cv::threshold(gray, binary, 127, 255, cv::THRESH_BINARY_INV);
    cv::adaptiveThreshold(gray, binary, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 3, -4);
    // medianBlur(binary, binary, 3);
    cv::imshow("binary", binary);

    // 连通区域分析
    cv::Mat labels, stats, centroids;
    int numLabels = cv::connectedComponentsWithStats(binary, labels, stats, centroids);
    cout << "numLabels: " << numLabels << endl;

    // 计算物体密度
    int totalArea = binary.rows * binary.cols;
    int objectArea = sum(stats.col(4))(0);
    float objectDensity = static_cast<float>(objectArea) / totalArea;
    cout << "totalArea: " << totalArea << "   objectArea: " << objectArea << endl;
    // 打印物体密度
    cout << "Object Density: " << objectDensity << endl;

    // 杂乱程度评估
    if (objectDensity < 0.1) {
        cout << "Kitchen is well-organized." << endl;
    } else if (objectDensity < 0.2) {
        cout << "Kitchen is moderately organized." << endl;
    } else {
        cout << "Kitchen is messy." << endl;
    }

    // 显示图像
    cv::imshow("Kitchen Image", image);
    cv::waitKey(0);
    cv::destroyAllWindows();
  

  return 0;
}