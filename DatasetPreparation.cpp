#include <iostream>

#include "ImageOperations.h"
#include "FileHandling.h"

using namespace std;
using namespace cv;
using namespace fh;
using namespace imgstruct;

int main()
{
    Mat img1, img2;
    Mat mask, mask_inv;
    Mat H;
    string img1_name, img2_name;
    string h_name;
    
    bool multiple=false;
    srand(time(NULL));
    //vector<string> classlist = readClasses();
    //readClasses();
    selectImages(&img1, &img2, &img1_name, &img2_name);
    h_name = selectFile(MAT_DIR);
    H = readMatrix(h_name);
    //createMask(&mask, &mask_inv, &img1, img1_name);
    //transformImage(&img1, &img2, &H, img1_name, img2_name, h_name);
    //findHomographyMatrix();
    //createMask(&mask, &mask_inv, &img1, img1_name);
    YoloData data=randomAddImages(&img1, &img2, img1_name, img2_name);
    transformImage(&img2, &H, &data);
 /*   readMask(&mask, &mask_inv, &img1, img1_name);
    rotateImage(&img1, &mask, &mask_inv, 30, true);
    imshow("image", img1);
    imshow("mask", mask);
    waitKey(0);*/
    //addImages(&img1, &img2, img1_name, img2_name);
    //createMask(&mask, &mask_inv, &img1, img1_name);
    
    //waitKey(1);
    return 0;
}
