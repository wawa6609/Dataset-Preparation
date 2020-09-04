#pragma once
#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "opencv2/opencv.hpp"
#include "FileNotFoundException.h"
#include "CameraNotAvailableException.h"
#include "FileHandling.h"
#include <cmath>

using namespace cv;
using namespace std::filesystem;
using namespace std;

typedef tuple <Mat*, vector<Point2f>*, bool, bool> MouseClickArgs;

bool askSave();

void createMask(Mat* mask, Mat* mask_inv, Mat* img, string* img_name);

void readMask(Mat* mask, Mat* mask_inv, Mat* img, string* img_name, bool show);

Rect centerImage(Mat* img1, Mat* img2);

Mat addImages(Mat* img1, Mat* img2, string* img1_name, string* img2_name, bool* multiple);

Mat findHomographyMatrix(bool display=false);

void transformImage();