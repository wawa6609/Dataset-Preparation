#include "FileHandling.h"


bool test_exist(string name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

void listFiles(string directory, vector<string> *files) {
    
    int i = 1;
    path pathToDisplay(directory);

    for (const auto& entry : directory_iterator(pathToDisplay)) {
        const auto filename = entry.path().filename().string();
        if (entry.is_regular_file()) {
            (*files).push_back(filename);
        }
    }
}

string selectFile(string directory) {
    vector<string> files;
    listFiles(directory, &files);
    int selection = 0;
    int len = files.size();
    
    if (len > 1) {
        for (int i = 0; i < len; i++) {
            cout << i+1 << ". " << files[i] << endl;
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
    else if (len==1) {
        selection = 1;
    }
    else {
        cout << "There are no files in " << directory << endl;
        return "";
    }
    selection -= 1;
    return (files[selection]);
}

void selectImages(Mat *img1, Mat *img2, string* img1_name, string* img2_name) {
    cout << "Selecting object" << endl;
    *img1_name = selectFile(OBJ_DIR);
    cout << "Selecting background" << endl;
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
        filename = "matrix_" + generateHash() + ".txt";
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
    Mat m=Mat::zeros(3,3, CV_32F);
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

vector<string> createConfig() {
    int num_of_classes;
    ofstream f1, f2;
    string classname;
    string filename;
    vector<string> classlist;
    filename = "obj.data";
    f1.open(DATA_DIR+filename, ios::out);
    cout << "Type number of classes: ";
    cin >> num_of_classes;
    cout << endl;
    f1 << "classes = " << num_of_classes << endl;
    f1 << "train = data/train.txt" << endl;
    f1 << "valid = data/valid.txt" << endl;
    f1 << "names = data/obj.names" << endl;
    f1 << "backup = backup/" << endl;
    f1.close();

    filename = "obj.names";
    f2.open(DATA_DIR + filename, ios::out);
    cout << "Type classnames line by line" << endl;
    cout << "(use '_' instead of space)" << endl;
    for (int i = 0; i < num_of_classes; i++) {
        cin >> classname;
        f2 << classname << endl;
        classlist.push_back(classname);
    }
    f2.close();
    return classlist;
}

vector<string> readClasses() {
    string line;
    ifstream f1,f2;
    string filename;
    string classname;
    vector<string> classlist;
    int num_of_classes;
    int start_pos, end_pos, count;
    filename = "obj.data";
    try {
        if (!test_exist(DATA_DIR+filename)) {
            string msg = "File ";
            msg += filename;
            msg += " not found in ";
            msg += DATA_DIR;
            throw FileNotFoundException(msg, -1, -1);
        }
        else {
            f1.open(DATA_DIR + filename, ios::in);
            getline(f1, line);
            f1.close();
            start_pos = line.find('=') + 2;
            end_pos = line.length();
            count = end_pos - start_pos;
            num_of_classes = stoi((line.substr(start_pos, count)));
            filename = "obj.names";
            f2.open(DATA_DIR + filename, ios::in);
            for (int i = 0; i < num_of_classes; i++) {
                f2 >> classname;
                classlist.push_back(classname);
            }
            f2.close();
        }
    }
    catch (FileNotFoundException e) {
        classlist=createConfig();
    }
    return classlist;
}

int readClass(string img_filename, vector<string> classlist) {
    string filename;
    ifstream f;
    int classid;
    try {
        filename = img_filename.substr(0, img_filename.find_last_of('.'));
        filename += ".txt";
        if (!test_exist(CLASS_DIR + filename)) {
            string msg = "File ";
            msg += filename;
            msg += " not found in ";
            msg += CLASS_DIR;
            throw FileNotFoundException(msg, -1, -1);
        }
        else {
            f.open(CLASS_DIR + filename, ios::in);
            f >> classid;
            f.close();
        }
    }
    catch (FileNotFoundException e) {
        classid=askClass(img_filename, classlist);
    }
    return classid;
}

int askClass(string img_filename, vector<string> classlist) {
    int classid;
    ofstream f;
    string filename;
    cout << "Select class number" << endl;
    for (int i = 0; i < classlist.size(); i++) {
        cout << i+1 << ". " << classlist.at(i) << endl;
    }
    cout << "Class number: ";
    cin >> classid;
    cout << endl;
    classid -= 1;
    filename = img_filename.substr(0, img_filename.find_last_of('.'));
    filename += ".txt";
    f.open(CLASS_DIR + filename, ios::out);
    f << classid;
    f.close();
    return classid;
}