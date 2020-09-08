#include "ImageOperations.h"


static bool compareContourAreas(vector<Point> c1, vector<Point> c2) {
    return (contourArea(c1) < contourArea(c2));
}

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

void createMask(Mat *mask, Mat *mask_inv, Mat *img, string img_name) {
    Mat img_gray;
    Mat img_bin;
    Mat preview;
    bool correct_thresh = false;
    int thresh = 127;
    string answer;
    string mask_name = img_name.substr(0, img_name.find_last_of('.'));
    mask_name += "_mask.png";
    cout << "Creating the mask" << endl;
    cvtColor((*img), img_gray, COLOR_BGR2GRAY);
    blur(img_gray, img_gray, Size(20, 20), Point(-1,-1));
    do {
        cout << "Trying with threshold of " << thresh << endl;
        if (thresh < 0) {
            threshold(img_gray, img_bin, -thresh, 255, THRESH_BINARY_INV);
        }
        else {
            threshold(img_gray, img_bin, thresh, 255, THRESH_BINARY);
        }
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours(img_bin, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);
        vector<Point> contour(contours.at(distance(contours.begin(),max_element(contours.begin(), contours.end(), compareContourAreas))));
        *mask = Mat::zeros(img->size(), CV_8U);
        //int max_contour_id = distance(contours.begin(), max_element(contours.begin(), contours.end(), compareContourAreas));
        //cout << max_contour_id << endl;
        contours.clear();
        contours.push_back(contour);
        /*contours.erase(remove_if(contours.begin(), contours.end(), [](const vector<Point>& contour) {
            return contourArea(contour) < 1000 || contourArea(contour) > 30000; }),
            contours.end());*/
        drawContours(*mask, contours, -1, 255, -1);
        //drawContours(*mask, contours, max_contour_id, 255, -1);
        bitwise_not(*mask, *mask_inv);
        preview=Mat::zeros(img->size(), img->type());
        destroyAllWindows();
        bitwise_and(*img, *img, preview,*mask);
        //imshow("image", *img);
        imshow("preview", preview);
        //imshow("mask", *mask);
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
            cout << "Please type custom threshold (-255 - 255): ";
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

void readMask(Mat* mask, Mat* mask_inv, Mat* img, string img_name, bool show) {
    string mask_path = "masks/";
    mask_path += img_name.substr(0, img_name.find_last_of('.'));
    mask_path += "_mask.png";
    cout << "Trying to read the mask" << endl;
    try {
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
        cout << "Mask was read from memory" << endl;
        
    }
    catch (exception) {
        createMask(mask, mask_inv, img, img_name);
    }
    if (show) {
        destroyAllWindows();
        imshow("mask", *mask);
        imshow("mask_inv", *mask_inv);
        waitKey(0);
    }
    destroyAllWindows();
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

Rect randomPlace(Mat* img1, Mat* img2) {
    int x1, y1;//top coordinates of centered rectangle
    int w, h;// width and heigth of the centered rectangle

    if ((img1->cols > img2->cols) || (img1->rows > img2->rows)) {
        double k = min((img2->cols / (double)img1->cols), (img2->rows / (double)img1->rows));
        int width = k * (*img1).cols;
        int height = k * (*img1).rows;
        resize(*img1, *img1, Size(width, height));
    }
    w =img1->cols;
    h = img1->rows;
    x1 = rand()%(img2->cols-img1->cols);
    y1 = rand() % max(1,(img2->rows - img1->rows));

    Rect overlay(x1, y1, w, h);
    return overlay;
}
void randomRotateImage(Mat* image, Mat* mask, Mat* mask_inv, bool show) {
    double angle = (rand() % 720) / 2.0;
    rotateImage(image, mask, mask_inv, angle, show);
}

void rotateImage(Mat* image, Mat* mask, Mat* mask_inv, double angle, bool show) {
    Size img_size = image->size();
    Point c = Point((int)(img_size.width/2), (int)(img_size.height/2));
    Mat M = getRotationMatrix2D(c, -angle, 1.0);
    cout << "Rotating image" << endl;
    double cos = abs(M.at<double>(0, 0));
    double sin = abs(M.at<double>(0, 1));
    int nW = img_size.height * sin + img_size.width * cos;
    int nH = img_size.height * cos + img_size.width * sin;
    M.at<double>(0, 2) += (nW / 2.0 - c.x);
    M.at<double>(1, 2) += (nH / 2.0 - c.y);
    warpAffine(*image, *image, M, Size(nW, nH));
    warpAffine(*mask, *mask, M, Size(nW, nH));
    bitwise_not(*mask, *mask_inv);
    if (show) {
        imshow("image", *image);
        imshow("mask", *mask);
        imshow("mask inv", *mask_inv);
        waitKey(0);
    }
}

void resizeImage(Mat* image, Mat* mask, Mat* mask_inv, int width) {
    double scale = ((double)width) / (image->cols);
    Size size((image->cols) * scale, (image->rows) * scale);
    resize(*image, *image, size);
    resize(*mask, *mask, size);
    resize(*mask_inv, *mask_inv, size);
}
void randomResizeImage(Mat* image, Mat* mask, Mat* mask_inv, int minWidth, int maxWidth) {
    int width = (rand() % (maxWidth - minWidth)) + minWidth;
    resizeImage(image, mask, mask_inv, width);
}

YoloData randomAddImages(Mat* img1, Mat* img2, string img1_name, string img2_name, bool multiple) {
    Mat roi;
    Mat img_fg, img_bg;
    Mat dst;
    Mat result;
    Mat mask, mask_inv;
    string result_filename, filename;
    ofstream result_file, file;
    vector<string> classlist;
    bool save;
    Rect overlay;
    UnifiedCoordinates uc;
    YoloData data;
    int classid, split;
    classlist = readClasses();
    selectRoi(img1, img1_name);
    

    
    //imshow("img1", img1);
    //imshow("img2", img2);
    //waitKey(0);
    cout << "Randomly adding images" << endl;
    if (!(multiple)) {
        save = askSave();
    }
    else {
        save = false;
    }
    readMask(&mask, &mask_inv, img1, img1_name);
    randomResizeImage(img1, &mask, &mask_inv, 80, 420);
    randomRotateImage(img1, &mask, &mask_inv);
    overlay = Rect(randomPlace(img1, img2));
    (*img2)(overlay).copyTo(roi);
    /*imshow("roi", roi);
    waitKey(0);*/
    //return;
    bitwise_and(roi, roi, img_bg, mask_inv);
    /*imshow("img_bg", img_bg);
    waitKey(0);*/
    bitwise_and(*img1, *img1, img_fg, mask);
    /*imshow("img_fg", img_fg);
    waitKey(0);*/
    add(img_bg, img_fg, dst);
    
    (*img2).copyTo(result);
    dst.copyTo(result(overlay));
    uc = toUnified(&result, overlay);
    classid = readClass(img1_name, classlist);
    blur(result, result, Size(2, 2), Point(-1, -1));
    result.copyTo(*img2);
    data = {classid, uc};
    if (save) {
        result_filename = (img1_name).substr(0, (img1_name).find_last_of('.'));
        result_filename += '_';
        result_filename += (img2_name).substr(0, (img2_name).find_last_of('.'));
        imwrite("results/" + result_filename+ ".png", result);
        result_file.open("results/" + result_filename + ".txt", ios::out);
        result_file << classid << " ";
        result_file << uc.c.x << " ";
        result_file << uc.c.y << " ";
        result_file << uc.s.width << " ";
        result_file << uc.s.height << endl;
        result_file.close();
        split = rand() % 10;
        if (split < 7) {
            filename = "train.txt";
            
        }
        else if (split < 9) {
            filename = "valid.txt";
        }
        else {
            filename = "test.txt";
        }
        file.open(DATA_DIR + filename, ios::app);
        file << "results/" + result_filename + ".png" << endl;
        file.close();
        cout << "Result saved as " << result_filename << endl;
    }
    destroyAllWindows();
    if (!(multiple)) {
        cout << "Press ESC key to close the program" << endl;
        imshow("result", result);
        while (waitKey() != 27) {};
        destroyAllWindows();
    }
    cout << endl;
    return data;
}
Mat addImages(Mat *img1, Mat *img2, string img1_name, string img2_name, bool multiple) {
    Mat roi;
    Mat img_fg, img_bg;
    Mat dst;
    Mat result;
    Mat mask, mask_inv;
    string result_filename;
    bool save;
    Rect overlay;
    cout << "Adding images" << endl;
    selectRoi(img1, img1_name);

    overlay=Rect(centerImage(img1, img2));
    //imshow("img1", img1);
    //imshow("img2", img2);
    //waitKey(0);
    
    if (!(multiple)) {
        save=askSave();
    }
    else {
        save = false;
    }
    readMask(&mask, &mask_inv, img1, img1_name);
    (*img2)(overlay).copyTo(roi);
    /*imshow("roi", roi);
    waitKey(0);*/
    //return;
    bitwise_and(roi, roi, img_bg, mask_inv);
    /*imshow("img_bg", img_bg);
    waitKey(0);*/
    bitwise_and(*img1, *img1, img_fg, mask);
    //imshow("img_fg", img_fg);
    //waitKey(0);
    add(img_bg, img_fg, dst);
    (*img2).copyTo(result);
    dst.copyTo(result(overlay));
    if (save) {
        result_filename = (img1_name).substr(0, (img1_name).find_last_of('.'));
        result_filename += '_';
        result_filename += (img2_name).substr(0, (img2_name).find_last_of('.'));
        result_filename += ".png";
        imwrite("results/" + result_filename, result);
        cout << "Result saved as " << result_filename << endl;
    }
    destroyAllWindows();
    if (!(multiple)) {
        cout << "Press ESC key to close the program" << endl;
        imshow("result", result);
        while (waitKey() != 27) {};
        destroyAllWindows();
    }
    cout << endl;
    return result;
}

void mouse_click(int event, int x, int y, int, void *params) {
    if (event == EVENT_LBUTTONDOWN)
    {
        MouseClickArgs *args =static_cast<MouseClickArgs*>(params);
        Mat *image = args->image;
        vector<Point2f>* points_in_image = args->points_in_image;
        Point p(x, y);
        (*points_in_image).push_back(p);

        if (args->display) {
            cout << p << endl;
        }
        if (args->draw)
        {
            circle(*image, p, 5, CV_RGB(255, 0, 0));
            imshow("image", *image);
            waitKey(1);
        }
    }
}


VideoCapture initializeCamera() {
    VideoCapture camera;
    string answer;
    int number;
    bool correct_answer = false;
    cout << "Do you want to use a system camera? [Y/N]: ";
    cin >> answer;
    answer = toLowerCase(answer);
    while (!correct_answer) {
        if ((answer == "y") || (answer == "yes")) {
            correct_answer = true;
            cout << "Select camera number (numbering from 0): ";
            cin >> number;
            cout << endl;
            camera.open(number);
        }
        else if ((answer == "n") || (answer == "no")) {
            correct_answer = true;
            cout << "Please type the camera address." << endl;
            cout << "Format example: rstp://192.168.0.1:8080/h264_ulaw.sdp" << endl;
            cout << "Address: ";
            cin >> answer;
            cout << endl;
            camera.open(answer);
        }
        else {
            cout << "Please type a valid answer." << endl << endl;
        }
    }
    
    camera.set(CAP_PROP_FRAME_WIDTH, 1280);
    camera.set(CAP_PROP_FRAME_HEIGHT, 720);

    return camera;
}

pair<Point, Point> toPoints(Mat* image, UnifiedCoordinates uc) {
    Point2d c(uc.c);
    Size2d s(uc.s);
    Point p1 = Point((c.x - s.width / 2) * image->cols, (c.y - s.height / 2) * image->rows);
    Point p2 = Point((c.x + s.width / 2) * image->cols, (c.y + s.height / 2) * image->rows);
    return make_pair(p1, p2);
}

Rect toRect(Mat* image, UnifiedCoordinates uc) {
    pair<Point, Point> p = toPoints(image, uc);
    return Rect(p.first, p.second);
}

UnifiedCoordinates toUnified(Mat* image, Point p1, Point p2) {
    Point2d c = Point2d(((double)(p1.x + p2.x)) / (2 * image->cols), ((double)(p1.y + p2.y)) / (2 * image->rows));
    Size2d s = Size2d(((double)(p2.x - p1.x)) / image->cols, ((double)(p2.y - p1.y)) / image->rows);
    UnifiedCoordinates uc = { c, s };
    return uc;
}

UnifiedCoordinates toUnified(Mat* image, Rect r) {
    Point2d c = Point2d((r.x+r.width/2.0) / image->cols, (r.y+r.height/2.0) / image->rows);
    Size2d s = Size2d(((double)r.width) / image->cols, ((double)r.height) / image->rows);
    UnifiedCoordinates uc = { c, s };
    return uc;
}

Mat findHomographyMatrix(bool display) {
    Mat image, warped_image, H, H_inv;
    MouseClickArgs args;
    vector<Point2f> points_in_image;
    vector<Point2f>  points_on_object{ { 0, 0 },{ 3 * 210, 0 },{ 3 * 210, 3 * 297 },{ 0, 3 * 297 } };
    cout << "Finding homography matrix" << endl;
    bool save = askSave();
    VideoCapture camera(initializeCamera());    

    try {
        if (!camera.isOpened()) throw CameraNotAvailableException("Camera not available", -1, -1);
    }
    catch (const CameraNotAvailableException& e) {
        cerr << e.what() << endl;
        return Mat();
    }
    

    while (waitKey(1) != 27) { //the image is read from camera until user presses ESC key
    	camera >> image;
    	if (image.data == NULL) break; //protection - loop stops when there is no new image
    	putText(image, String("Press ESC in order to capture still image"), Point(0, 25), 0, 1, CV_RGB(255, 0, 0), 2);
    	imshow("image", image);
    }
    EVENT_MOUSEMOVE;
    camera >> image;
    imshow("image", image);
    waitKey(1);
    args = { &image, &points_in_image, false, true };
    setMouseCallback("image", mouse_click, (void*)&args); 
    waitKey(1);
    imshow("image", image);
    waitKey(500);
    putText(image, String("Mark 4 points on the surface"), Point(0, 55), 0, 1, CV_RGB(255, 0, 0), 2);
    imshow("image", image);
    waitKey(1);
    while (points_in_image.size() < 4) waitKey(1);

    for (int i = 0; i < 4; i++)
        line(image, points_in_image[i], points_in_image[(i + 1) % 4], CV_RGB(0, 255, 100), 2);


    putText(image, String("Object marked. Press any key..."), Point(0, 85), 0, 1, CV_RGB(255, 0, 0), 2);
    imshow("image", image);
    waitKey();
    H = findHomography(points_in_image, points_on_object);
    H_inv = H.inv();
    if (display) {
        cout << "Homography matrix:" << endl << H << endl;
        cout << endl;
        cout << "Inverted homography matrix:" << endl << H_inv << endl;
    }
    warpPerspective(image, warped_image, H, Size(3 * 210, 3 * 297));
    imshow("warped image", warped_image);
    waitKey(120);
    if (save) {
        saveMatrix(H_inv);
    }
    return H_inv;
}
void selectRoi(Mat *image, string img_name ) {
    Mat image_copy, roi_preview;
    MouseClickArgs args;
    Rect roi;
    vector<Point2f> points_in_image;
    bool roi_correct = false;
    string answer;
    try {
        if (!test_exist(ROI_DIR + img_name)) {
            string msg = "File ";
            msg += img_name;
            msg += " not found in ";
            msg += ROI_DIR;
            msg += ".";
            throw FileNotFoundException(msg, -1, -1);
        }
        else {
            (imread(ROI_DIR + img_name)).copyTo(*image);
        }
    }
    catch( FileNotFoundException e){
        cout << "Click on the top left and bottom right corner\nof rectangle bounding the object." << endl;
        while (!roi_correct) {
            (*image).copyTo(image_copy);
            (*image).copyTo(roi_preview);
            args = { &image_copy, &points_in_image, false, true };
            imshow("image", image_copy);
            waitKey(1);
            setMouseCallback("image", mouse_click, (void*)&args);
            while (points_in_image.size() < 2) waitKey(1);
            roi = Rect((points_in_image.at(0)), (points_in_image.at(1)));
            roi_preview = (roi_preview)(roi);
            imshow("roi preview", roi_preview);
            waitKey(1);
            cout << "Is the roi correct? [Y/N]: ";
            cin >> answer;
            cout << endl;
            toLowerCase(answer);
            if (answer == "y" || answer == "yes") {
                roi_correct = true;
                roi_preview.copyTo(*image);
            }
            else if (answer == "n" || answer == "no") {
                roi_correct = false;
            }
            points_in_image.clear();
        }
        imwrite(ROI_DIR + img_name, *image);
    }
    
}


void transformImage(Mat* img1, Mat* img2, Mat* H, string img1_name, string img2_name, string h_name, bool multiple) {
    bool save;
    string result_filename;
    Mat result(addImages(img1, img2, img1_name, img2_name, true));
    cout << "Transforming images" << endl;
    if (!(multiple)) {
        save = askSave();
    }
    else {
        save = true;
    }
    warpPerspective(result, result, *H, result.size());
    imshow("result", result);
    if (save) {
        string result_filename = img1_name.substr(0, img1_name.find_last_of('.'));
        result_filename += '_';
        result_filename += img2_name.substr(0, img2_name.find_last_of('.'));
        result_filename += '_';
        result_filename += h_name.substr(h_name.find_last_of('_') + 1, h_name.find_last_of('.') - h_name.find_last_of('_') - 1);
        result_filename += ".png";
        imwrite("results/" + result_filename, result);
        cout << "Result saved as " << result_filename << endl;
    }
    waitKey(0);
}

void transformImage(Mat* img, Mat* H, YoloData *data) {
    string result_filename, filename;
    ofstream result_file, file;
    int split;
    bool save;
    pair<Point, Point> p;
    Point3d p1, p2;
    save = askSave();
    p = toPoints(img, data->uc);
    try {
        warpPerspective(*img, *img, *H, img->size());
        p1 = Point3d((p.first).x, (p.first).y, 1);
        p2 = Point3d((p.second).x, (p.second).y, 1);
        p1 = Point3d(Mat((*H) * Mat(p1)));
        p1 /= p1.z;
        p2 = Point3d(Mat((*H) * Mat(p2)));
        p2 /= p2.z;
        cout << p1 << ' ' << p2 << endl;
        data->uc = toUnified(img, Point(p1.x, p1.y), Point(p2.x, p2.y));
    }
    catch (cv::Exception const& e) { std::cerr << "OpenCV exception: " << e.what() << std::endl; }

    imshow("result", *img);
    if (save) {
        result_filename = generateHash();
        imwrite(DATA_IMG_DIR + result_filename + ".png", *img);
        result_file.open(DATA_IMG_DIR + result_filename + ".txt", ios::out);
        result_file << data->classid << " ";
        result_file << data->uc.c.x << " ";
        result_file << data->uc.c.y << " ";
        result_file << data->uc.s.width << " ";
        result_file << data->uc.s.height << endl;
        result_file.close();
        split = rand() % 10;
        if (split < 7) {
            filename = "train.txt";

        }
        else if (split < 9) {
            filename = "valid.txt";
        }
        else {
            filename = "test.txt";
        }
        file.open(DATA_DIR + filename, ios::app);
        file << "results/" + result_filename + ".png" << endl;
        file.close();
        cout << "Result saved as " << result_filename << ".png" << endl;
    }
}