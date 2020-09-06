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

typedef struct  MouseClickArgs;

bool askSave();

void createMask(Mat* mask, Mat* mask_inv, Mat* img, string img_name);

void readMask(Mat* mask, Mat* mask_inv, Mat* img, string img_name, bool show = false);

Rect centerImage(Mat* img1, Mat* img2);

Mat addImages(Mat* img1, Mat* img2, string img1_name, string img2_name, bool multiple = false);

void mouse_click(int event, int x, int y, int, void* params);

VideoCapture initializeCamera();

Mat findHomographyMatrix(bool display = false);

void transformImage(Mat* img1, Mat* img2, Mat* H, string img1_name, string img2_name, string h_name, bool multiple = false);
void selectRoi(Mat* image, string img_name);
