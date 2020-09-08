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
namespace imgstruct {
    struct MouseClickArgs {
        Mat* image;
        vector<Point2f>* points_in_image;
        bool display;
        bool draw;
    };
    struct UnifiedCoordinates {
        Point2d c;
        Size2d s;
    };
    struct YoloData {
        int classid;
        UnifiedCoordinates uc;
    };
}
using namespace imgstruct;


static bool compareContourAreas(vector<Point> c1, vector<Point> c2);

bool askSave();

void createMask(Mat* mask, Mat* mask_inv, Mat* img, string img_name);

void readMask(Mat* mask, Mat* mask_inv, Mat* img, string img_name, bool show = false);

Rect centerImage(Mat* img1, Mat* img2);

void randomRotateImage(Mat* image, Mat* mask, Mat* mask_inv, bool show = false);

void rotateImage(Mat* image, Mat* mask, Mat* mask_inv, double angle, bool show=false);

YoloData randomAddImages(Mat* img1, Mat* img2, string img1_name, string img2_name, bool multiple = false);

Mat addImages(Mat* img1, Mat* img2, string img1_name, string img2_name, bool multiple = false);

void mouse_click(int event, int x, int y, int, void* params);

VideoCapture initializeCamera();

pair<Point, Point> toPoints(Mat* image, UnifiedCoordinates uc);

Rect toRect(Mat* image, UnifiedCoordinates uc);

UnifiedCoordinates toUnified(Mat* image, Point p1, Point p2);

UnifiedCoordinates toUnified(Mat* image, Rect r);

Mat findHomographyMatrix(bool display = false);

void transformImage(Mat* img1, Mat* img2, Mat* H, string img1_name, string img2_name, string h_name, bool multiple = false);
void transformImage(Mat* img, Mat* H, YoloData* data);
void selectRoi(Mat* image, string img_name);
