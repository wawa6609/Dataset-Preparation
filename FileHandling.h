#pragma once
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <random>
#include <ctime>
#include <fstream>

#include "opencv2/opencv.hpp"
#include "FileNotFoundException.h"

using namespace cv;
using namespace std::filesystem;
using namespace std;

namespace fh {
    constexpr auto BG_DIR = "backgrounds/";
    constexpr auto OBJ_DIR = "objects/";
    constexpr auto MAT_DIR = "homografies/";
}
using namespace fh;
	

bool test_exist(string name);
void listFiles(string directory, vector<string>* files);
string selectFile(string directory);

void selectImages(Mat *img1, Mat *img2, string *img1_name, string *img2_name);
string generateHash();
void saveMatrix(Mat m);
Mat readMatrix(string filename, bool display = false);


