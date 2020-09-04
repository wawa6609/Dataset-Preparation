#include "FileHandling.h"

constexpr auto BG_DIR = "backgrounds/";
constexpr auto OBJ_DIR = "objects/";
constexpr auto MAT_DIR = "homografies/";

bool test_exist(string name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

string selectFile(string directory) {
    map<int, string> files;
    int i = 1;
    int selection = 0;
    path pathToDisplay(directory);

    for (const auto& entry : directory_iterator(pathToDisplay)) {
        const auto filename = entry.path().filename().string();
        if (entry.is_regular_file()) {
            files.insert({ i++,filename });
        }
    }
    if (i > 2) {
        for (int j = 1; j < i; j++) {
            cout << j << ". " << files[j] << endl;
        }
        cout << endl;
        while (selection < 1 || selection >= i) {
            cout << "Type file number: ";
            cin >> selection;
            if (selection >= 1 && selection < i) {
                break;
            }
            cout << "Number out of range." << endl;
            cout << "Type number from 1 to " << i - 1 << endl;
            cout << endl << endl;
        }
    }
    else if (i == 2) {
        selection = 1;
    }
    else {
        cout << "There are no files in " << directory << endl;
        return NULL;
    }

    return (files[selection]);
}

void selectFiles(Mat *img1, Mat *img2, string* img1_name, string* img2_name, bool* multiple) {
    *img1_name = selectFile(OBJ_DIR);
    *img2_name = selectFile(BG_DIR);
    *img1 = imread(OBJ_DIR + *img1_name);
    cout << "Reading "<< OBJ_DIR << *img1_name << " as img1" << endl;
    *img2 = imread(BG_DIR + *img2_name);
    cout << "Reading "<< BG_DIR << *img2_name << " as img2" << endl;
    cout << endl;
}


string generateHash() {
    string hash= "";
    char c;
    for (int i = 0; i < 10; i++) {
        c = rand() % 25+97;
        hash += c;
    }
    return hash;
}

void saveMatrix(Mat m) {
    string filename;
    ofstream file;
    do {
        filename = generateHash() + ".txt";
    } while (test_exist(MAT_DIR + filename));
    file.open(MAT_DIR + filename, 'w');
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            if (j != 0) {
                file << " ";
            }
            file << round((m.at<double>(i,j)*100))/100;
        }
        file << endl;
    }
    file.close();
    cout << "File saved as " << filename << endl;
}

Mat readMatrix(string filename, bool display) {
    Mat m=Mat::zeros(3,3,CV_32F);
    ifstream file;
    float num;
    int start_pos=0, end_pos=0;
    file.open(MAT_DIR + filename);
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            file >> num;
            m.at<float>(i, j) = num;
        }
    }
    if (display) {
        cout << m << endl;
    }
    return m;
}
