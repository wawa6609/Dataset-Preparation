#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;

using std::cout;
using std::cin;
using std::endl;
using std::string;

Mat img1, img2;
Mat mask;
Mat mask_inv;

void createMask() {
    Mat img2_gray;
    bool correct_thresh = false;
    int thresh = 127;
    string answer;
    cvtColor(img2, img2_gray, COLOR_BGR2GRAY);
    do {
        threshold(img2_gray, mask, thresh, 255, THRESH_BINARY);
        destroyAllWindows();
        imshow("mask", mask);
        waitKey(1);
        cout << "Is the threshold correct? [Y/N]: ";
        cin >> answer;
        bitwise_not(mask, mask_inv);
        toLowerCase(answer);
        if (answer == "y" || answer == "yes") {
            correct_thresh = true;
            cout << endl << endl;
            break;
        }
        else if (answer == "n" || answer == "no") {
            cout << "Please type custom treshold (0-255): ";
            cin >> thresh;
            cout << endl;
        }
        
    }while (!correct_thresh);
    
}

Rect centerImage(Mat img1, Mat img2) {
    int cx, cy;// center coordinates
    int x1, y1;//top coordinates of centered rectangle
    int w, h;// width and heigth of the centered rectangle
    cx = img1.cols / 2;
    cy = img1.rows / 2;

    x1 = cx - (img2.cols / 2);
    y1 = cy - (img2.rows / 2);
    w = img2.cols;
    h = img2.rows;
    Rect overlay(x1, y1, w, h);
    return overlay;
}

void addImages() {
    Mat roi;
    Mat img_fg, img_bg;
    Mat dst;
    Mat result;
    img1 = imread("messi.jpg");
    img2 = imread("logo.png");
    imshow("messi", img1);
    Rect overlay(centerImage(img1, img2));
    imshow("logo", img2);
    waitKey(0);
    createMask();
    img1(overlay).copyTo(roi);
    //imshow("roi", roi);
    //waitKey(0);
    //return;
    bitwise_and(roi, roi, img_bg, mask);
    //imshow("img_bg", img_bg);
    //waitKey(0);
    bitwise_and(img2, img2, img_fg, mask_inv);
    //imshow("img_fg", img_fg);
    //waitKey(0);
    add(img_bg, img_fg, dst);
    img1.copyTo(result);
    dst.copyTo(result(overlay));
    destroyAllWindows();
    imshow("result",result);
    waitKey(0);
}

void findHomography() {

}

void transformImage() {

}


int main()
{
    //test();
    addImages();
    return 0;
}
