#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <string>
#include <windows.h>



using namespace cv;
using namespace std;

int findLargestArea(std::vector<std::vector<cv::Point>> contours){
	double largest = 0;
	int ind = 0;
	for (int i = 0; i < contours.size(); i++) {
		if (cv::contourArea(contours[i]) > largest) {
			ind = i;
			largest = cv::contourArea(contours[i]);
		}
	}
	return ind;
}

int main()
{
	VideoCapture cap(0);
	Mat frame;

	int count = 0;
	int count2 = 0;
	bool blinked = false;

	stringstream text;

	cv::Mat threshed;

	int x1 = 0;
	int y1 = 0;

	while (1) {

		cap >> frame;

		if (frame.empty()) {
			break;
		}

		cvtColor(frame, frame, cv::COLOR_RGB2GRAY);

		GaussianBlur(frame, frame, Size(17, 17), 0);

		threshold(frame, threshed, 50, 255, THRESH_BINARY_INV);

		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(threshed, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
		// draw contours on the original image
		Mat image_copy = frame.clone();
		
		namedWindow("Contour: ", WINDOW_NORMAL);

		int x = GetSystemMetrics(SM_CXSCREEN);
		int y = GetSystemMetrics(SM_CYSCREEN);


		drawContours(image_copy, contours, findLargestArea(contours), Scalar(255, 0, 0), 3);

		cv::Moments M = cv::moments(contours[findLargestArea(contours)]);
		cv::Point center(M.m10 / M.m00, M.m01 / M.m00);

		circle(image_copy, center, 25, (255, 255, 255), -1);

		cv::Point origin(0, 10);

		x1 = center.x;
		y1 = center.y;

		text << "x: " << center.x << "y: " << center.y;

		putText(image_copy, text.str(), origin, FONT_HERSHEY_PLAIN, 1, (0, 255, 0), 1);

		SetCursorPos(center.x*3, center.y*3);

		count = 0;
		


		imshow("Contour: ", image_copy);
		resizeWindow("Contour: ", 600, 600);
		

		char c = (char)waitKey(25);

		if (c == 27) {
			break;
		}
		waitKey(50);
	}
	return 0;
}