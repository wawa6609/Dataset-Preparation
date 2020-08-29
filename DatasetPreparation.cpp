#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;

using std::cout;
using std::cin;

Mat img1, img2;
Mat mask;
Mat mask_inv;

void createMask() {
    Mat img2_gray;
    cvtColor(img2, img2_gray, COLOR_BGR2GRAY);
    threshold(img2_gray, mask, 127, 255, THRESH_BINARY);
    bitwise_not(mask, mask_inv);
}

void addImages() {
    Mat roi;
    Mat img_fg, img_bg;
    Mat dst;
    Mat result;
    img1 = imread("messi.jpg");
    img2 = imread("logo.png");
    //imshow("messi", img1);
    Rect overlay(0, 0, img2.cols, img2.rows);
    //imshow("logo", img2);
    createMask();
    //imshow("mask", mask);
    img1(overlay).copyTo(roi);
    //imshow("roi", roi);
    bitwise_and(roi, roi, img_bg, mask);
    bitwise_and(img2, img2, img_fg, mask_inv);
    //imshow("bg", img_bg);
    //imshow("fg", img_fg);
    add(img_bg, img_fg, dst);
    img1.copyTo(result);
    dst.copyTo(result(overlay));
    imshow("result",result);
    waitKey(0);
}

void findHomography() {

}

void transformImage() {

}

void test(){
    Mat image;
    image = imread("logo.png");

    imshow("res",image);
    waitKey(0);
}

int main()
{
    //test();
    addImages();
    return 0;
}
