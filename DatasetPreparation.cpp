#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;

void createMask() {

}

void addImages() {

}

void findHomography() {

}

void transformImage() {

}

void test(){
    Mat image;
    image = imread("glasses.png");

    imshow("res",image);
    waitKey(0);
}

int main()
{
    test();
    return 0;
}
