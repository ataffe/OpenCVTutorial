#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

class ColorReducer {
private:
	Mat originalImage;
	Mat reducedImage;
	vector<uchar> lookupTable;
	int reductionNumber = 10;
	int generateLUT();
	void reduceColorSpace(Mat& inputImage, Mat& outputImage, vector<uchar>& lut);
	void reduceColorSpaceFast(Mat& image, Mat& reducedImage, vector<uchar>& table);

public:
	ColorReducer() {
		lookupTable = vector<uchar>(256, 0);
	}
	int loadImage(int argc, char** argv);
	void displayImage();
	void saveImageData(string filename);
	void setReductionNumber(int number);
	static int convertToGray(int argc, char** argv) {
		string imageName = argv[1];
		Mat image;
		image = imread(imageName, IMREAD_COLOR);

		if (argc != 2 || !image.data) {
			cout << "No image data" << endl;
			return -1;
		}

		Mat gray_image;
		cvtColor(image, gray_image, COLOR_BGR2GRAY);

		imwrite("Gray_Image.jpg", gray_image);

		namedWindow(imageName, WINDOW_AUTOSIZE);
		namedWindow("Gray image", WINDOW_AUTOSIZE);

		imshow(imageName, image);
		imshow("Gray image", gray_image);

		waitKey(0);
		return 0;
	}
};


