//Jose Joseph Thandapral
//CS5330
//Project 4 : Calibration and Augmented Reality 
//Tasks 4 to 7  

#include <cstdio>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/calib3d/calib3d_c.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/saturate.hpp>
#include <cstring>
#include <cstdlib>
#include <cmath>

using namespace cv;
using namespace std;
int main() {
	VideoCapture* capdev;
	vector<Point2f> harris_corners;
	Mat frame, grayImg;

	// open the video device
	capdev = new VideoCapture(0);

	if (!capdev->isOpened()) {

		std::cout << "Error opening camera stream" << std::endl;
		return -1;
	}

	while (true) {
		*capdev >> frame; // get a new frame from the camera, treat as a stream
		if (frame.empty()) {
			printf("frame is empty\n");
			break;
		}

		cvtColor(frame, grayImg, COLOR_BGR2GRAY);

		// Find N strongest feature points to track
		goodFeaturesToTrack(grayImg, harris_corners, 54, 0.1, 10); //54 to track the inner 9X6 checkerboard pattern
																  // Can be changed to any 'N'

		for (int idx = 0; idx < harris_corners.size(); idx++) {
			cv::circle(frame, harris_corners.at(idx), 2, cv::Scalar(0, 255, 0), 2);
		}
		imshow("Detected Harris corners", frame);
		for (int idx = 0; idx < harris_corners.size(); idx++) {
			line(frame, harris_corners[0], harris_corners[idx], Scalar(255, 0, 0), 4, LINE_8);
		}
		imshow("Pattern drawn using Harris corners", frame);
		//Print the number of strong features detected(<=54 here)
		cout << harris_corners.size() << endl;
		char key = waitKey(10);
		if (key == 'q')
			break;
	}

	return 0;
}