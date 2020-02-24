#include "ColorReducer.h"

int ColorReducer::generateLUT() {
    if (lookupTable.size() != 256) {
        cout << "Cannot generate look up table. The input size is: " << lookupTable.size() << endl;
        return 0;
    }

    for (int i = 0; i < 256; i++) {
        lookupTable[i] = (uchar)(reductionNumber * (i / reductionNumber));
    }
    return 1;
}

void ColorReducer::reduceColorSpace(Mat& inputImage, Mat& outputImage, vector<uchar>& lut) {
    Mat _lookUpTable(1, 256, CV_8U);
    uchar* p = _lookUpTable.ptr();
    for (int i = 0; i < 256; ++i) {
        p[i] = lut[i];
    }
    LUT(inputImage, _lookUpTable, outputImage);
}

void ColorReducer::reduceColorSpaceFast(Mat& image, Mat& reducedImage, vector<uchar>& table) {
    CV_Assert(image.depth() == CV_8U);

    int channels = image.channels();
    int numRows = image.rows;
    int numCols = image.cols * channels;

    if (image.isContinuous()) {
        numCols *= numRows;
        numRows = 1;
    }
    int i, j;
    uchar* p;
    reducedImage = image.clone();
    for (i = 0; i < numRows; ++i) {
        p = reducedImage.ptr<uchar>(i);
        for (j = 0; j < numCols; ++j) {
            p[j] = table[p[j]];
        }
    }
}

void ColorReducer::saveImageData(string filename) {
    if (!originalImage.empty() && !reducedImage.empty()) {
        ofstream imageFile;
        imageFile.open(filename);
        imageFile << "Image one: " << endl << originalImage << endl;
        imageFile << "Reduced Image: " << endl << reducedImage << endl;
        imageFile.close();
    }
    else {
        cout << "Cannot save images, one of images is null :/" << endl;
    }
}

void ColorReducer::setReductionNumber(int number) {
    reductionNumber = number;
}

int ColorReducer::loadImage(int argc, char** argv) {
    string imageName("goldenGate.jpg");
    if (argc != 2)
    {
        cout << " Usage: " << argv[0] << " ImageToLoadAndDisplay" << endl;
        return -1;
    }
    
    originalImage = imread("images/goldenGate.jpg", IMREAD_COLOR); // Read the file
    if (originalImage.empty()) // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    if (generateLUT()) {
        //reduceColorSpace(image, reducedImage, lookupTable);
        reduceColorSpaceFast(originalImage, reducedImage, lookupTable);
    }
    Size size(1000, 1000);
    resize(originalImage, originalImage, size);
    resize(reducedImage, reducedImage, size);
}

void ColorReducer::displayImage() {
    if (!originalImage.empty() && !reducedImage.empty()) {
        namedWindow("Reduced Image", WINDOW_AUTOSIZE); // Create a window for display.
        namedWindow("Normal Image", WINDOW_AUTOSIZE);
        imshow("Normal Image", originalImage);
        imshow("Reduced Image", reducedImage); // Show our image inside it.
        waitKey(0); // Wait for a keystroke in the window
    }
    else {
        cout << "One of the images was null cannot display images." << endl;
    }
}
