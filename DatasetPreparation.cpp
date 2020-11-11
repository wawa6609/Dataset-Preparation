#include <iostream>

#include "ImageOperations.h"
#include "FileHandling.h"

using namespace fh;
using namespace std;
using namespace cv;

int main()
{
    Mat img1, img2;
    Mat mask, mask_inv;
    Mat H;
    string img1_name, img2_name;
    string h_name;
    bool multiple=false;
    int alpha, beta, gamma, f, dist;
    srand(time(NULL));
    
    
    
    selectImages(img1, img2, img1_name, img2_name);
    //h_name = selectFile(MAT_DIR);
    //H = readMatrix(h_name);
    
    
    createMask(mask, mask_inv, img1, img1_name);
    //readParameters(alpha, beta, gamma, f, dist);
    //H = calculateParametrizedMatrix(alpha, beta, gamma, f, dist);
    H = calculateParametrizedMatrix(85, 90, 90, 500, 500);
    transformImage(img1, img2, H, img1_name, img2_name, h_name);
    //findHomographyMatrix();

    waitKey(1);
    return 0;
}
