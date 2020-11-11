#include "FileHandling.h"

bool test_exist(string name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void listFiles(string directory, vector<string>& files) {
	int i = 1;
	path pathToDisplay(directory);

	for (const auto& entry : directory_iterator(pathToDisplay)) {
		const auto filename = entry.path().filename().string();
		if (entry.is_regular_file()) {
			(files).push_back(filename);
		}
	}
}

string selectFile(string directory) {
	vector<string> files;
	listFiles(directory, files);
	int selection = 0;
	int len = files.size();

	if (len > 1) {
		cout << "SELECT FILE" << endl;
		for (int i = 0; i < len; i++) {
			cout << i + 1 << ". " << files[i] << endl;
		}
		cout << endl;
		while (selection < 1 || selection > len) {
			cout << "Type file number: ";
			cin >> selection;
			if (selection >= 1 && selection <= len) {
				cout << endl << endl;
				break;
			}
			cout << "Number out of range." << endl;
			cout << "Type number from 1 to " << len << "." << endl;
			cout << endl << endl;
		}
	}
	else if (len == 1) {
		selection = 1;
	}
	else {
		cout << "There are no files in " << directory << endl;
		return "";
	}
	selection -= 1;
	return (files[selection]);
}

void selectImages(Mat& img1, Mat& img2, string& img1_name, string& img2_name) {
	cout << "SELECT IMAGES" << endl;
	cout << "Selecting object" << endl;
	img1_name = selectFile(OBJ_DIR);
	cout << "Selecting background" << endl;
	img2_name = selectFile(BG_DIR);
	img1 = imread(OBJ_DIR + img1_name);
	cout << "Reading " << OBJ_DIR << img1_name << " as img1" << endl;
	img2 = imread(BG_DIR + img2_name);
	cout << "Reading " << BG_DIR << img2_name << " as img2" << endl;
	cout << endl;
}

string generateHash() {
	string hash = "";
	char c;
	for (int i = 0; i < 10; i++) {
		c = rand() % 25 + 97;
		hash += c;
	}
	return hash;
}

void saveMatrix(Mat& m) {
	string filename;
	ofstream file;
	cout << m << endl;
	do {
		filename = "matrix_" + generateHash() + ".txt";
	} while (test_exist(MAT_DIR + filename));
	file.open(MAT_DIR + filename, 'w');
	for (int i = 0; i < m.rows; i++) {
		for (int j = 0; j < m.cols; j++) {
			if (j != 0) {
				cout << " ";
				file << " ";
			}
			file << round((m.at<float>(i, j) * 100)) / 100;
			//cout << round((m.at<float>(i, j) * 100)) / 100;
		}
		file << endl;
		//cout << endl;
	}
	file.close();
	cout << "File saved as " << filename << endl;
}

Mat readMatrix(string filename, bool display) {
	Mat m = Mat::zeros(3, 3, CV_32F);
	ifstream file;
	float num;
	int start_pos = 0, end_pos = 0;
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