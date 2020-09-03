# Dataset Preparation
Program for preparing dataset for YOLO training

## Files organization
* backgrouds/
  * contains pictures which will be used as backgrounds
* masks/
  * contains masks for object pictures created with program
* objects/
  * contains pictures of objects
* results/
  * contains generated results

## Selecting files
Function `selectFiles()` allows for selecting files containing objects or backgrounds.
If object or background directory contains more than one file user will be prompted to select file from printed list.
The object image will be stored in variable `img1` and the background image in variable `img2`.

## Masks
In the mask the object is marked as black and the background is marked as white.
### Reading masks
If mask for selected object file is already created program will read it from file stored in mask/ folder. 
### Creating masks
Else program will allow user to create a mask using a constant threshold.
The procedure is as follows:
1. The preview of the mask with default threshold of 127 is shown to the user.
2. User fill be promted to confirm that the threshold is correct answering Y or N.
    * If user confirms with Y mask is saved to masks/\<img1\>_mask.png where \<img1\> is the filename of the object file
    * If user declines with N they will be promped to type custom threshold (number from 0 to 255) and whole procedure repeats until user confirms threshold value
  
## Adding images
Function `addImages()` allows user to add images of the object and the background. Addition will be performed using appropriate mask which can be created or read from memory as described above. Object will be centered on the background image
User can choose whether they want to save the result or not. If they choose so the file will be saved as results/\<img1\>_\<img2\>.png where \<img1\> is the name of the object file and \<img2\> is the name of the background file.
  
## Finding homography
Not yet implemented.

## Transforming images
Not yet implemented.
