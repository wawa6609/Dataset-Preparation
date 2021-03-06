#include "ImageOperations.h"

const int imgWidth = 1280;
const int imgHeight = 720;

typedef struct  MouseClickArgs {
	Mat* image;
	vector<Point2f>* points_in_image;
	bool display;
	bool draw;
};

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

void createMask(Mat& mask, Mat& mask_inv, Mat& img, string img_name) {
	Mat img_gray;
	bool correct_thresh = false;
	int thresh = 127;
	string answer;
	string mask_name = img_name.substr(0, img_name.find_last_of('.'));
	mask_name += "_mask.png";
	cout << "Creating the mask" << endl;
	selectRoi(img, img_name);
	cout << "White color represents the background and black color represents the object" << endl;
	cvtColor((img), img_gray, COLOR_BGR2GRAY);
	do {
		cout << "Trying with threshold of " << thresh << endl;
		if (thresh < 0) {
			threshold(img_gray, mask, -thresh, 255, THRESH_BINARY_INV);
		}
		else {
			threshold(img_gray, mask, thresh, 255, THRESH_BINARY);
		}

		bitwise_not(mask, mask_inv);
		destroyAllWindows();
		imshow("mask", mask);
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
	imwrite(("masks/" + mask_name), mask);
	cout << " and saved as " << mask_name << endl;
	cout << endl;
	destroyAllWindows();
}

void readMask(Mat& mask, Mat& mask_inv, Mat& img, string img_name, bool show) {
	string mask_path = "masks/";
	mask_path += img_name.substr(0, img_name.find_last_of('.'));
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
	mask = imread(mask_path, IMREAD_GRAYSCALE);
	bitwise_not(mask, mask_inv);
	if (show) {
		destroyAllWindows();
		imshow("mask", mask);
		imshow("mask_inv", mask_inv);
		waitKey(0);
	}
	destroyAllWindows();
	cout << "Mask was read from memory" << endl;
	cout << endl;
}

Rect centerImage(Mat& img1, Mat& img2) {
	int cx, cy;// center coordinates
	int x1, y1;//top coordinates of centered rectangle
	int w, h;// width and heigth of the centered rectangle

	if (((img1).cols > (img2).cols) || ((img1).rows > (img2).rows)) {
		double k = min(((img2).cols / (double)(img1).cols), ((img2).rows / (double)(img1).rows));
		int width = k * (img1).cols;
		int height = k * (img1).rows;
		resize(img1, img1, Size(width, height));
	}

	w = (img1).cols;
	h = (img1).rows;
	cx = (img2).cols / 2;
	cy = (img2).rows / 2;
	x1 = cx - (w / 2);
	y1 = cy - (h / 2);

	Rect overlay(x1, y1, w, h);
	return overlay;
}

Mat addImages(Mat& img1, Mat& img2, string img1_name, string img2_name, bool multiple) {
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
	if (!(multiple)) {
		save = askSave();
	}
	else {
		save = true;
	}
	try {
		readMask(mask, mask_inv, img1, img1_name);
	}
	catch (exception) {
		createMask(mask, mask_inv, img1, img1_name);
	}
	(img2)(overlay).copyTo(roi);
	/*imshow("roi", roi);
	waitKey(0);*/
	//return;
	bitwise_and(roi, roi, img_bg, mask);
	/*imshow("img_bg", img_bg);
	waitKey(0);*/
	bitwise_and(img1, img1, img_fg, mask_inv);
	//imshow("img_fg", img_fg);
	//waitKey(0);
	add(img_bg, img_fg, dst);
	(img2).copyTo(result);
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

void mouse_click(int event, int x, int y, int, void* params) {
	if (event == EVENT_LBUTTONDOWN)
	{
		MouseClickArgs* args = static_cast<MouseClickArgs*>(params);
		Mat* image = args->image;
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

	camera.set(CAP_PROP_FRAME_WIDTH, imgWidth);
	camera.set(CAP_PROP_FRAME_HEIGHT, imgHeight);

	return camera;
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

void selectRoi(Mat& image, string img_name) {
	Mat image_copy;
	MouseClickArgs args;
	vector<Point2f> points_in_image;
	cout << "SELECT ROI" << endl;
	(image).copyTo(image_copy);
	args = { &image_copy, &points_in_image, false, true };
	putText(image_copy, "Click on the top left and bottom right corner\nof rectangle bounding the object.", Point(10, 10), FONT_HERSHEY_COMPLEX, 0.5, CV_RGB(255, 0, 0));
	imshow("image", image_copy);
	waitKey(1);
	setMouseCallback("image", mouse_click, (void*)&args);
	while (points_in_image.size() < 2) waitKey(1);
	Rect roi((points_in_image.at(0)), (points_in_image.at(1)));
	image = (image)(roi);
	imshow("image", image);
	waitKey(0);
}

void transformImage(Mat& img1, Mat& img2, Mat& H, string img1_name, string img2_name, string h_name, bool multiple) {
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
	warpPerspective(result, result, H, result.size());
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

int frameWidth = 640;
int frameHeight = 480;

/*
 * This code illustrates bird's eye view perspective transformation using opencv
 * Paper: Distance Determination for an Automobile Environment using Inverse Perspective Mapping in OpenCV
 * Link to paper: https://www.researchgate.net/publication/224195999_Distance_determination_for_an_automobile_environment_using_Inverse_Perspective_Mapping_in_OpenCV
 * Code taken from: http://www.aizac.info/birds-eye-view-homography-using-opencv/
 */

Mat calculateParametrizedMatrix(int alpha_, int beta_, int gamma_, int f_, int dist_) {
	bool save = askSave();

	double focalLength, dist, alpha, beta, gamma;

	alpha = ((double)alpha_ - 90) * M_PI / 180;
	beta = ((double)beta_ - 90) * M_PI / 180;
	gamma = ((double)gamma_ - 90) * M_PI / 180;
	focalLength = (double)f_;
	dist = (double)dist_;

	Size image_size = Size(imgWidth, imgHeight);
	double w = (double)image_size.width, h = (double)image_size.height;

	// Projecion matrix 2D -> 3D
	Mat A1 = (Mat_<float>(4, 3) <<
		1, 0, -w / 2,
		0, 1, -h / 2,
		0, 0, 0,
		0, 0, 1);
	//cout << "A1:" << endl << A1 << endl;

	// Rotation matrices Rx, Ry, Rz

	Mat RX = (Mat_<float>(4, 4) <<
		1, 0, 0, 0,
		0, cos(alpha), -sin(alpha), 0,
		0, sin(alpha), cos(alpha), 0,
		0, 0, 0, 1);
	//cout << "RX:" << endl << RX << endl;

	Mat RY = (Mat_<float>(4, 4) <<
		cos(beta), 0, -sin(beta), 0,
		0, 1, 0, 0,
		sin(beta), 0, cos(beta), 0,
		0, 0, 0, 1);
	//cout << "RY:" << endl << RY << endl;

	Mat RZ = (Mat_<float>(4, 4) <<
		cos(gamma), -sin(gamma), 0, 0,
		sin(gamma), cos(gamma), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	//cout << "RZ:" << endl << RZ << endl;

	// R - rotation matrix
	Mat R = RX * RY * RZ;
	//cout << "R:" << endl << R << endl;

	// T - translation matrix
	Mat T = (Mat_<float>(4, 4) <<
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, dist,
		0, 0, 0, 1);
	//cout << "T:" << endl << T << endl;

	// K - intrinsic matrix
	Mat K = (Mat_<float>(3, 4) <<
		focalLength, 0, w / 2, 0,
		0, focalLength, h / 2, 0,
		0, 0, 1, 0
		);
	//cout << "K:" << endl << K << endl;

	Mat transformationMat = K * (T * (R * A1));
	//cout << "transformationMat:" << endl << transformationMat << endl;
	if (save) {
		saveMatrix(transformationMat);
	}
	return transformationMat;
}

void readParameters(int& alpha_, int& beta_, int& gamma_, int& f_, int& dist_) {
	cout << "alpha (0-180): ";
	cin >> alpha_;
	cout << "beta (0-180): ";
	cin >> beta_;
	cout << "gamma (0-180): ";
	cin >> gamma_;
	cout << "f [mm]: ";
	cin >> f_;
	cout << "dist [mm]: ";
	cin >> dist_;
}