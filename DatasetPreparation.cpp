#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std::filesystem;
using namespace std;

class FileNotFoundException : virtual public std::runtime_error {

protected:

    int error_number;               ///< Error number
    int error_offset;               ///< Error offset

public:

    /** Constructor (C++ STL string, int, int).
     *  @param msg The error message
     *  @param err_num Error number
     *  @param err_off Error offset
     */
    explicit
        FileNotFoundException(const std::string& msg, int err_num, int err_off) :
        std::runtime_error(msg)
    {
        error_number = err_num;
        error_offset = err_off;

    }

    /** Destructor.
     *  Virtual to allow for subclassing.
     */
    virtual ~FileNotFoundException() throw () {}

    /** Returns error number.
     *  @return #error_number
     */
    virtual int getErrorNumber() const throw() {
        return error_number;
    }

    /**Returns error offset.
     * @return #error_offset
     */
    virtual int getErrorOffset() const throw() {
        return error_offset;
    }

};


string img1_name, img2_name;
Mat img1, img2;
Mat mask;
Mat mask_inv;
bool save;
bool multiple;

inline bool test_exist(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

void askSave() {
    string answer;
    cout << "Do you want to save the result [Y/N]: ";
    cin >> answer;
    toLowerCase(answer);
    if (answer == "y" || answer == "yes") {
        save = true;
    }
    else if (answer == "n" || answer == "no") {
        save = false;
    }
}

void createMask() {
    Mat img2_gray;
    bool correct_thresh = false;
    int thresh = 127;
    string answer;
    string mask_name = img2_name.substr(0, img2_name.find_last_of('.'));
    mask_name += "_mask.png";
    cvtColor(img2, img2_gray, COLOR_BGR2GRAY);
    do {
        cout << "Trying with threshold of " << thresh << endl;
        threshold(img2_gray, mask, thresh, 255, THRESH_BINARY);
        destroyAllWindows();
        imshow("mask", mask);
        waitKey(1);
        cout << "Is the threshold correct? [Y/N]: ";
        cin >> answer;
        bitwise_not(mask, mask_inv);
        toLowerCase(answer);
        if (answer == "y" || answer == "yes") {
            correct_thresh = true;
            cout << endl << endl;
            break;
        }
        else if (answer == "n" || answer == "no") {
            cout << "Please type custom treshold (0-255): ";
            cin >> thresh;
            cout << endl;
        }
        
    }while (!correct_thresh);
    imwrite(("masks/" + mask_name), mask);
    destroyAllWindows();
    
}

void readMask() {
    string mask_path = "masks/";
    mask_path += img2_name.substr(0, img2_name.find_last_of('.'));
    mask_path += "_mask.png";
    if (!test_exist(mask_path)) {
        string msg = "File ";
        msg += mask_path.substr(mask_path.find_last_of('/'),mask_path.length());
        msg += " not found in ";
        msg += mask_path.substr(0,mask_path.find_last_of('/'));
        msg += " directory";
        throw FileNotFoundException(msg,-1,-1);
    }
    mask = imread(mask_path,IMREAD_GRAYSCALE);
    bitwise_not(mask, mask_inv);
    /*destroyAllWindows();
    imshow("mask", mask);
    imshow("mask_inv", mask_inv);
    waitKey(0);*/
    destroyAllWindows();
}

Rect centerImage(Mat img1, Mat img2) {
    int cx, cy;// center coordinates
    int x1, y1;//top coordinates of centered rectangle
    int w, h;// width and heigth of the centered rectangle
    cx = img1.cols / 2;
    cy = img1.rows / 2;

    x1 = cx - (img2.cols / 2);
    y1 = cy - (img2.rows / 2);
    w = img2.cols;
    h = img2.rows;
    Rect overlay(x1, y1, w, h);
    return overlay;
}

void addImages() {
    Mat roi;
    Mat img_fg, img_bg;
    Mat dst;
    Mat result;
    string result_filename;
    


    Rect overlay(centerImage(img1, img2));
    //imshow("messi", img1);
    //imshow("logo", img2);
    //waitKey(0);
    cout << "Adding images" << endl;
    if (!multiple) {
        askSave();
    }
    try {
        cout << "Trying to read the mask" << endl;
        readMask();
        cout << "Mask was read from memory" << endl;
    }
    catch (exception) {
        cout << "Creating the mask" << endl;
        createMask();
        cout << "Mask created" << endl;
    }
    //createMask();
    img1(overlay).copyTo(roi);
    /*imshow("roi", roi);
    waitKey(0);*/
    //return;
    bitwise_and(roi, roi, img_bg, mask);
    //imshow("img_bg", img_bg);
    //waitKey(0);
    bitwise_and(img2, img2, img_fg, mask_inv);
    //imshow("img_fg", img_fg);
    //waitKey(0);
    add(img_bg, img_fg, dst);
    img1.copyTo(result);
    dst.copyTo(result(overlay));
    if (save) {
        result_filename = img1_name.substr(0, img1_name.find_last_of('.'));
        result_filename += '_';
        result_filename += img2_name.substr(0, img2_name.find_last_of('.'));
        result_filename += ".png";
        imwrite("results/" + result_filename, result);
        cout << "Saved as " << result_filename << endl;
    }
    destroyAllWindows();
    putText(result, "Press ESC key to close the program", Point(10, 20), FONT_HERSHEY_COMPLEX, 0.5, CV_RGB(255,0,0));
    imshow("result",result);
    

    while (waitKey(0) != 27) {};
    destroyAllWindows();
}

void findHomography() {

}

void transformImage() {

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
            cout << i << ". " << files[i] << endl;
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
        return NULL;
    }

    return (files[selection]);
}

void selectFiles() {
    img1_name = selectFile("backgrounds/");
    img2_name = selectFile("money/");
    img1 = imread("backgrounds/" + img1_name);
    cout << "Reading " << img1_name << " from backgrounds directory as img1" << endl;
    img2 = imread("money/" + img2_name);
    cout << "Reading " << img2_name<<" from money directory as img2" << endl;
};




int main()
{
    selectFiles();
    addImages();
    return 0;
}
