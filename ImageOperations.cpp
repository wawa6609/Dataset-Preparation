#include "ImageOperations.h"


Mat mask;
Mat mask_inv;

bool multiple;


bool askSave() {
    string answer;
    cout << "Do you want to save the result [Y/N]: ";
    cin >> answer;
    cout << endl;
    toLowerCase(answer);
    if (answer == "y" || answer == "yes") {
        return true;
    }
    else if (answer == "n" || answer == "no") {
        return false;
    }
}

void createMask(Mat *mask, Mat *mask_inv, Mat *img, string *img_name) {
    Mat img_gray;
    bool correct_thresh = false;
    int thresh = 127;
    string answer;
    string mask_name = (*img_name).substr(0, (*img_name).find_last_of('.'));
    mask_name += "_mask.png";
    cout << "Creating the mask" << endl;
    cout << "White color represents the background and black color represents the object" << endl;
    cvtColor((*img), img_gray, COLOR_BGR2GRAY);
    do {
        cout << "Trying with threshold of " << thresh << endl;
        threshold(img_gray, *mask, thresh, 255, THRESH_BINARY);
        bitwise_not(*mask, *mask_inv);
        destroyAllWindows();
        imshow("mask", *mask);
        waitKey(1);
        cout << "Is the threshold correct? [Y/N]: ";
        cin >> answer;
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
        else {
            cout << "Type valid answer" << endl;
        }

    } while (!correct_thresh);
    cout << "Mask created";
    imwrite(("masks/" + mask_name), *mask);
    cout << " and saved as " << mask_name << endl;
    cout << endl;
    destroyAllWindows();


}

void readMask(Mat* mask, Mat* mask_inv, Mat* img, string* img_name, bool show=false) {
    string mask_path = "masks/";
    mask_path += (*img_name).substr(0, (*img_name).find_last_of('.'));
    mask_path += "_mask.png";
    cout << "Trying to read the mask" << endl;
    if (!test_exist(mask_path)) {
        string msg = "File ";
        msg += mask_path.substr(mask_path.find_last_of('/'), mask_path.length());
        msg += " not found in ";
        msg += mask_path.substr(0, mask_path.find_last_of('/'));
        msg += " directory";
        throw FileNotFoundException(msg, -1, -1);
    }
    *mask = imread(mask_path, IMREAD_GRAYSCALE);
    bitwise_not(*mask, *mask_inv);
    if (show) {
        destroyAllWindows();
        imshow("mask", *mask);
        imshow("mask_inv", *mask_inv);
        waitKey(0);
    }
    destroyAllWindows();
    cout << "Mask was read from memory" << endl;
    cout << endl;
}

Rect centerImage(Mat *img1, Mat *img2) {
    int cx, cy;// center coordinates
    int x1, y1;//top coordinates of centered rectangle
    int w, h;// width and heigth of the centered rectangle
    
    if (((*img1).cols > (*img2).cols) || ((*img1).rows > (*img2).rows)) {
        double k = min(((*img2).cols / (double)(*img1).cols), ((*img2).rows / (double)(*img1).rows));
        int width = k * (*img1).cols;
        int height = k * (*img1).rows;
        resize(*img1, *img1,Size(width,height));
    }

    w = (*img1).cols;
    h = (*img1).rows;
    cx = (*img2).cols / 2;
    cy = (*img2).rows / 2;
    x1 = cx - (w / 2);
    y1 = cy - (h / 2);
    
    Rect overlay(x1, y1, w, h);
    return overlay;
}

Mat addImages(Mat *img1, Mat *img2, string *img1_name, string *img2_name, bool *multiple) {
    Mat roi;
    Mat img_fg, img_bg;
    Mat dst;
    Mat result;
    Mat mask, mask_inv;
    string result_filename;
    bool save;

    Rect overlay(centerImage(img1, img2));
    //imshow("img1", img1);
    //imshow("img2", img2);
    //waitKey(0);
    cout << "Adding images" << endl;
    if (!(*multiple)) {
        save=askSave();
    }
    else {
        save = true;
    }
    try {
        readMask(&mask, &mask_inv, img1, img1_name);
    }
    catch (exception) {
        createMask(&mask, &mask_inv, img1, img1_name);
    }
    (*img2)(overlay).copyTo(roi);
    /*imshow("roi", roi);
    waitKey(0);*/
    //return;
    bitwise_and(roi, roi, img_bg, mask);
    /*imshow("img_bg", img_bg);
    waitKey(0);*/
    bitwise_and(*img1, *img1, img_fg, mask_inv);
    //imshow("img_fg", img_fg);
    //waitKey(0);
    add(img_bg, img_fg, dst);
    (*img2).copyTo(result);
    dst.copyTo(result(overlay));
    if (save) {
        result_filename = (*img1_name).substr(0, (*img1_name).find_last_of('.'));
        result_filename += '_';
        result_filename += (*img2_name).substr(0, (*img2_name).find_last_of('.'));
        result_filename += ".png";
        imwrite("results/" + result_filename, result);
        cout << "Result saved as " << result_filename << endl;
    }
    destroyAllWindows();
    if (!(*multiple)) {
        putText(result, "Press ESC key to close the program", Point(10, 20), FONT_HERSHEY_COMPLEX, 0.5, CV_RGB(255, 0, 0));
        imshow("result", result);
        while (waitKey(0) != 27) {};
        destroyAllWindows();
    }
    cout << endl;
    return result;
}

void findHomography() {

}

void transformImage() {

}