#pragma once
#include <iostream>
#include <filesystem>
#include <stdexcept>
//#include <boost/log/trivial.hpp>

#define _USE_MATH_DEFINES
#include <cmath>

#include "opencv2/opencv.hpp"
#include "FileHandling.h"

#include "FileNotFoundException.h"
#include "CameraNotAvailableException.h"

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

Mat calculateParametrizedMatrix(int alpha_ = 90, int beta_ = 90, int gamma_ = 90,
	int f_ = 500, int dist_ = 500);
Mat findHomographyMatrix(bool display = false);

void transformImage(Mat* img1, Mat* img2, Mat* H, string img1_name, string img2_name, string h_name, bool multiple = false);
void selectRoi(Mat* image, string img_name);
void readParameters(int& alpha_, int& beta_, int& gamma_, int& f_, int& dist_);
