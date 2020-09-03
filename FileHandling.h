#pragma once
#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "opencv2/opencv.hpp"
#include "FileNotFoundException.h"

using namespace cv;
using namespace std::filesystem;
using namespace std;

bool test_exist(string name);
string selectFile(string directory);

void selectFiles(Mat *img1, Mat *img2, string *img1_name, string *img2_name, bool *multiple);


