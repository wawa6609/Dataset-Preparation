#include <iostream>

#include "ImageOperations.h"
#include "FileHandling.h"


int main()
{
    Mat img1, img2;
    Mat mask, mask_inv;
    string img1_name, img2_name;
    bool multiple=false;
    //selectFiles(&img1, &img2, &img1_name, &img2_name, &multiple);
    //createMask(&mask, &mask_inv, &img1, &img1_name);
    //addImages(&img1, &img2, &img1_name, &img2_name, &multiple);
    findHomographyMatrix();
    return 0;
}
