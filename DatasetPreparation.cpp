#include <iostream>

#include "ImageOperations.h"
#include "FileHandling.h"


int main()
{
    Mat img1, img2;
    string img1_name, img2_name;
    bool multiple=false;
    selectFiles(&img1, &img2, &img1_name, &img2_name, &multiple);
    addImages(&img1, &img2, &img1_name, &img2_name, &multiple);
    return 0;
}
