# Dataset Preparation
> **Program for preparing dataset for YOLO training**

## Files organization
### Directories
* backgrounds/
  * contains pictures which will be used as backgrounds
* masks/
  * contains masks for object pictures created with program
* objects/
  * contains pictures of objects
* results/
  * contains generated results
### Source files
* DatasetPreparation.cpp
  * contains the main program
* FileHandling.h
  * contains functions allowing for file handling
  * FileHandling.cpp contains corresponding implementations
* ImageOperations.h
  * contains functions allowing for image operations
  * ImageOperations.cpp contains corresponding implementations
* FileNotFoundException.h
  * contains custom FileNotFoundException exception class
  * FileNotFoundException.cpp contains corresponding implementations

## Selecting files
```c++
void selectFiles(Mat *img1, Mat *img2, string *img1_name, string *img2_name, bool *multiple)
```

Function `selectFiles()` allows for selecting files containing objects and backgrounds.
If object or background directory contains more than one file user will be prompted to select file from printed list.
The object image will be stored in variable `img1` and the background image in variable `img2`.

## Masks
In the mask the object is marked as black and the background is marked as white.
### Reading masks
```c++
void readMask(Mat* mask, Mat* mask_inv, Mat* img, string* img_name, bool show)
```

If mask for selected object file is already created program will read it from file stored in mask/ folder. 
### Creating masks
```c++
void createMask(Mat* mask, Mat* mask_inv, Mat* img, string* img_name)
```

Else program will allow user to create a mask using a constant threshold.
The procedure is as follows:
1. The preview of the mask with default threshold of 127 is shown to the user.
2. User fill be promted to confirm that the threshold is correct answering Y or N.
    * If user confirms with Y mask is saved to masks/\<img1\>_mask.png where \<img1\> is the name of the object file
    * If user declines with N they will be promped to type custom threshold (number from 0 to 255) and whole procedure repeats until user confirms threshold value
  
## Adding images
```c++
Mat addImages(Mat* img1, Mat* img2, string* img1_name, string* img2_name, bool* multiple)
```

Function `addImages()` allows user to add images of the object and the background. Addition will be performed using appropriate mask which can be created or read from memory as described above. Object will be centered on the background image.
User can choose whether they want to save the result or not. If they choose so the file will be saved as results/\<img1\>_\<img2\>.png where \<img1\> is the name of the object file and \<img2\> is the name of the background file.
The function returns result image as its output.
The `multiple` parameter will be used in case of performing addition operation on all files.
  
## Finding homography
Not yet implemented.

## Transforming images
Not yet implemented.
